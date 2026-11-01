// ApplySymbols (T11, phase 1.3): applies config/retail/symbols.txt names to the analysed program before ExportProgram.
// Args: one or more symbol tables, read in order (2.1 T4: symbols.txt then symbols_pin.txt, as gate.py). Grammar per non-# line: <va 0x%08X> <func|data> <size|-> <name> <source> <status>.
// Function at VA -> Function.setName(name, USER_DEFINED); else createLabel(USER_DEFINED) + setPrimary.
// A same-name label already at a function entry is deleted first (the function takes the name).
// A `func` line whose VA has no function and lies in no function body gets one (T12, phase 1.5): disassemble if no
// instruction, createFunction(VA, name) (USER_DEFINED), body clipped to [VA, VA+size). Why: the delinker synthesizes
// code relocations only for instructions inside a function; 698 of 4794 func lines had none (vtable-only callees such
// as SetState 0x000A871D: 12 relocs, all jump table, vs 144 with a function; C0034). Inside another body or
// createFunction null -> label as before (counted). analyzeChanges then adds the new code's operand references.
// Hole fill (2.2 T9): a `func` line whose existing function's body leaves holes inside [VA, VA+size) below the
// body's max address (code reachable only through data, e.g. an SEH filter/handler via its .rdata scope table: _main
// 0x000E0BB0 body 35 of 0x3C B, no relocs at the filter) gets each undefined hole start disassembled (repeated until
// the hole is covered) and those instructions added to the body. Defined data and bytes in another function are
// skipped; bytes past the body's max stay out (tails are C0070's). Counted "holes H instrs I".
// Body grow (3.1 T8): a `func` line (renamed or created) whose body ends before VA+size-1 is queued; after the line
// loop, in line order, R = [bodyMax+1, VA+size-1] is refused if another function overlaps R, R leaves f's memory
// block, or setBody throws (counted, "ApplySymbols: growRefused <name> at <va> (<why>)"); else R's undefined starts
// are disassembled and f.setBody(body + R) ("ApplySymbols: grown <name> at <va> +0x<n>"). Grows only to the pin's
// size (D78). Counted "grown G growRefused Q".
// Absorb (3.2 T2): before refusing, an overlapping function o with a DEFAULT-source name, no func line at its entry
// and body inside [VA, VA+size-1] is removed (removeFunction) when f's line is sized with an accepted status; the
// grow then proceeds ("ApplySymbols: absorbed <o> into <f>", counted "absorbed A"). Evidence: the 7 refused in 3.1
// (heap tails, head stubs, ___FrameUnwindToState tail) are one prototype proc of the pin's length, masked-equal
// retail over the range, no PDB proc/public at o's entry (phase-ends logs T2.c1).
// Split (3.2 T2): a sized, accepted `func` line at VA inside function C, where C's entry has a sized, accepted func
// line and C.entry+C.size <= VA, clips C to body ∩ [C.entry, C.entry+C.size) and creates VA's function (counted in
// "created" and "split S"; "ApplySymbols: split <C> at <entry> for <name> at <va>"); else `inside` as before.
// Split below (3.2 T4): same pins, VA < C.entry (Ghidra folded a tail-jump target into C's body): C is clipped to
// body ∩ [C.entry, C.entry+C.size) and VA's function created (counted in "created" and "splitBelow B";
// "ApplySymbols: splitBelow <C> at <entry> for <name> at <va>").
// Accepted = candidates.py:324 set (units_seed.STATUSES + PIN_STATUSES: unverified, verified, disasm-verified);
// site-corr, boundary and size `-` never qualify.
// Every createFn clip prints "ApplySymbols: clipped <name> at <va> body <min>..<max> kept ..<end>".
// Also disambiguates duplicate function names and turns the delinker's thunk redirect off (see run()).
// First (T11, phase 1.8, C0053): Function ID names vary between fresh runs, so at every FID bookmark (ANALYSIS type,
// category "Function ID Analyzer"/"Function ID Conflict") global ANALYSIS-source labels are deleted and a global
// ANALYSIS-source function name is reset to DEFAULT (no FID candidate chosen, D78). Prints
// "ApplySymbols: FID canonicalised addresses A labels L functions F".
// Prints "ApplySymbols: lines N renamed R labelled L failed F (same-name labels removed D, duplicate names reset K)
// created C clipped K inside I createFailed X holes H instrs I grown G growRefused Q split S splitBelow B absorbed A
// scalarRefs data D func F"
// Scalar references (3.2 T4): after the duplicate reset, an imm32/disp32 operand equal to a line's exact VA gets a
// DATA memory reference when the instruction has none to it (see run()); the delinker then emits DIR32 there.
// (tools/delink.py checks it) and each failure by name; any failure aborts later scripts and throws.
// Firewall D46: names, addresses and counts only.
//@category VoodooVince

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

import ghidra.app.util.headless.HeadlessScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Bookmark;
import ghidra.program.model.listing.BookmarkType;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolType;

public class ApplySymbols extends HeadlessScript {

	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 1) {  // was != 1 (one table) until 2.1 T4
			throw new IllegalArgumentException("usage: ApplySymbols.java <symbols.txt> [<symbols_pin.txt>..]");
		}
		List<String> lines = new ArrayList<>();
		for (String arg : args) {
			lines.addAll(Files.readAllLines(new File(arg).toPath(), StandardCharsets.UTF_8));
		}
		// FID canonicalisation (header): categories are ApplyFidEntriesCommand's bookmark constants.
		TreeSet<Address> fidAt = new TreeSet<>();
		Iterator<Bookmark> bit = currentProgram.getBookmarkManager().getBookmarksIterator(BookmarkType.ANALYSIS);
		while (bit.hasNext()) {
			Bookmark b = bit.next();
			if ("Function ID Analyzer".equals(b.getCategory()) || "Function ID Conflict".equals(b.getCategory())) {
				fidAt.add(b.getAddress());
			}
		}
		int fidLabels = 0, fidFuncs = 0;
		for (Address a : fidAt) {
			for (Symbol s : currentProgram.getSymbolTable().getSymbols(a)) {
				if (s.getSymbolType() == SymbolType.LABEL && s.getSource() == SourceType.ANALYSIS
						&& s.getParentNamespace().isGlobal()) {
					s.delete();
					fidLabels++;
				}
			}
			Function f = currentProgram.getFunctionManager().getFunctionAt(a);
			if (f != null && f.getSymbol().getSource() == SourceType.ANALYSIS && f.getParentNamespace().isGlobal()) {
				f.setName(null, SourceType.DEFAULT);
				fidFuncs++;
			}
		}
		println("ApplySymbols: FID canonicalised addresses " + fidAt.size() + " labels " + fidLabels
				+ " functions " + fidFuncs);
		int n = 0, renamed = 0, labelled = 0, dropped = 0, created = 0, clipped = 0, inside = 0;
		List<String> failed = new ArrayList<>();
		List<String> createFailed = new ArrayList<>();
		// 3.2 T2 (header): pre-parse func lines -> funcAt (every VA), goodSize (sized + accepted status)
		for (String raw : lines) {
			String[] p = raw.trim().split("\\s+");
			if (p.length == 6 && p[0].startsWith("0x") && (p[1].equals("func") || p[1].equals("data"))) {
				// 3.2 T4: scalar-reference targets (exact line VAs); a func line wins over a data line at one VA
				scalarTargets.merge(Long.parseLong(p[0].substring(2), 16), p[1].equals("func"), Boolean::logicalOr);
			}
			if (p.length != 6 || !p[0].startsWith("0x") || !p[1].equals("func")) {
				continue;
			}
			long va = Long.parseLong(p[0].substring(2), 16);
			funcAt.add(va);
			if (p[2].startsWith("0x") && ACCEPTED.contains(p[5])) {
				long sz = Long.parseLong(p[2].substring(2), 16);
				if (sz > 0) {
					goodSize.put(va, sz);
				}
			}
		}
		for (String raw : lines) {
			String t = raw.trim();
			if (t.isEmpty() || t.startsWith("#")) {
				continue;
			}
			n++;
			String[] p = t.split("\\s+");
			if (p.length != 6 || !p[0].startsWith("0x")) {
				failed.add("bad line " + n);
				continue;
			}
			String name = p[3];
			try {
				Address a = toAddr(Long.parseLong(p[0].substring(2), 16));
				Function f = currentProgram.getFunctionManager().getFunctionAt(a);
				if (f != null) {
					// a same-name label at the entry in the function's namespace makes setName throw
					// DuplicateNameException (SymbolManager.checkDuplicateSymbolName); the name moves to the function
					Symbol dup = currentProgram.getSymbolTable().getSymbol(name, a, f.getParentNamespace());
					if (dup != null && !dup.equals(f.getSymbol())) {
						dup.delete();
						dropped++;
					}
					f.setName(name, SourceType.USER_DEFINED);
					renamed++;
					if (p[1].equals("func")) {
						fillHoles(f, a, p[2]);
						queueGrow(f, a, p[2]);
					}
				}
				else if (p[1].equals("func") && currentProgram.getFunctionManager().getFunctionContaining(a) == null &&
					createFn(a, name, p[2])) {
					created++;
					if (lastClipped) {
						clipped++;
					}
					// 3.2 T10: a created body can hold a hole too (flow stopped on a misaligned analysis instruction;
					// ?Update@TrolleyScreen@@UAEXXZ 0x1302E3..0x130341); outside every function the delinker's
					// per-function relocation synthesizer emits no relocation for the hole's referencing instructions
					fillHoles(currentProgram.getFunctionManager().getFunctionAt(a), a, p[2]);
					queueGrow(currentProgram.getFunctionManager().getFunctionAt(a), a, p[2]);
				}
				else if (p[1].equals("func") && goodSize.containsKey(a.getOffset()) && splitAt(a, name, p[2])) {
					created++;
					if (lastSplitBelow) {
						splitBelow++;
					}
					else {
						split++;
					}
					if (lastClipped) {
						clipped++;
					}
					fillHoles(currentProgram.getFunctionManager().getFunctionAt(a), a, p[2]);  // 3.2 T10
					queueGrow(currentProgram.getFunctionManager().getFunctionAt(a), a, p[2]);
				}
				else {
					if (p[1].equals("func")) {
						if (currentProgram.getFunctionManager().getFunctionContaining(a) != null) {
							inside++;
						}
						else {
							createFailed.add(name + " at " + p[0]);
						}
					}
					Symbol s = currentProgram.getSymbolTable().createLabel(a, name, SourceType.USER_DEFINED);
					s.setPrimary();
					labelled++;
				}
			}
			catch (Exception e) {
				failed.add(name + " at " + p[0] + " (" + e.getClass().getSimpleName() + ")");
			}
		}
		growAll();
		// new functions' code gets operand references (the delinker's relocation source) and the applied/ export
		// lists them (T12, phase 1.5)
		analyzeChanges(currentProgram);
		// Duplicate function names (T11): the delinker's COFF exporter refuses an object holding two global symbols
		// of one name (ProgramUtil.checkDuplicateSymbols -> "Failed to export"). Seen: a DEFAULT-source thunk showing
		// its USER_DEFINED target's name, and one ANALYSIS name on several functions. Every function in a duplicate
		// (namespace, name) group whose name is not USER_DEFINED (not from symbols.txt) is renamed: a thunk to
		// "thunk_<name>" (Ghidra's thunk prefix), any other to its default FUN_<va>. Address order; deterministic.
		Map<String, List<Function>> groups = new TreeMap<>();
		for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
			groups.computeIfAbsent(f.getParentNamespace().getID() + "|" + f.getName(), k -> new ArrayList<>()).add(f);
		}
		int reset = 0;
		for (List<Function> g : groups.values()) {
			if (g.size() < 2) {
				continue;
			}
			for (Function f : g) {
				if (f.getSymbol().getSource() == SourceType.USER_DEFINED) {
					continue;
				}
				String old = f.getName();
				try {
					if (f.isThunk()) {
						f.setName("thunk_" + old, SourceType.ANALYSIS);
					}
					else {
						f.setName(null, SourceType.DEFAULT);
					}
					reset++;
					println("ApplySymbols: duplicate " + old + " at " + f.getEntryPoint() + " -> " + f.getName());
				}
				catch (Exception e) {
					failed.add("duplicate " + old + " at " + f.getEntryPoint() + " (" + e.getClass().getSimpleName() + ")");
				}
			}
		}
		// Scalar references (3.2 T4): `mov r32, offset X` whose register is only pushed/passed, and
		// `lea r,[r*4+disp32]`, get no reference from constant propagation, so the delinker emitted no DIR32 there (75
		// pinned sites). Every instruction in an executable block, address order: a scalar operand whose value is the
		// exact VA of a table line gets a DATA memory reference (USER_DEFINED, that operand) unless the instruction
		// already references that address. A 32-bit VA >= the image base fits only an imm32/disp32 field (imm8/imm16
		// sign-extend to other values), so the value match is the 4-byte test. Counted "scalarRefs data D func F".
		int scalarData = 0, scalarFunc = 0;
		ReferenceManager rm = currentProgram.getReferenceManager();
		for (MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
			if (!blk.isExecute()) {
				continue;
			}
			InstructionIterator it = currentProgram.getListing().getInstructions(new AddressSet(blk.getStart(), blk.getEnd()), true);
			while (it.hasNext()) {
				Instruction ins = it.next();
				for (int op = 0; op < ins.getNumOperands(); op++) {
					for (Object o : ins.getOpObjects(op)) {
						if (!(o instanceof Scalar)) {
							continue;
						}
						long v = ((Scalar) o).getUnsignedValue() & 0xFFFFFFFFL;
						Boolean isFunc = scalarTargets.get(v);
						if (isFunc == null) {
							continue;
						}
						Address to = toAddr(v);
						boolean has = false;
						for (Reference r : ins.getReferencesFrom()) {
							if (r.getToAddress().equals(to)) {
								has = true;
								break;
							}
						}
						if (has) {
							continue;
						}
						rm.addMemoryReference(ins.getAddress(), to, RefType.DATA, SourceType.USER_DEFINED, op);
						if (isFunc) {
							scalarFunc++;
						}
						else {
							scalarData++;
						}
					}
				}
			}
		}
		// Delinker option (T11): the "Relocation table synthesizer" analyzer of ghidra-delinker-extension defaults
		// "Redirect thunks to targets" to true, so a call to a thunk stub is relocated against the thunk's target
		// while the retail field addresses the stub (12 byte_compare mismatches + 1 kernel-thunk provisional in the
		// first T11 self-check). Stored false in the program's analysis options; the project is saved after the
		// postScripts and DelinkProgram's one-time analyzer run in the read-only export reads it.
		currentProgram.getOptions(Program.ANALYSIS_PROPERTIES)
				.setBoolean("Relocation table synthesizer.Redirect thunks to targets", false);
		println("ApplySymbols: option Relocation table synthesizer.Redirect thunks to targets = false");
		for (String f : createFailed) {
			println("ApplySymbols: createFailed " + f);
		}
		for (String f : failed) {
			println("ApplySymbols: failed " + f);
		}
		println("ApplySymbols: lines " + n + " renamed " + renamed + " labelled " + labelled + " failed " +
			failed.size() + " (same-name labels removed " + dropped + ", duplicate names reset " + reset + ")" +
			" created " + created + " clipped " + clipped + " inside " + inside + " createFailed " + createFailed.size() + " holes " + holeFns + " instrs " + holeInstrs +
			" grown " + grown + " growRefused " + growRefused + " split " + split + " splitBelow " + splitBelow + " absorbed " + absorbed + " scalarRefs data " + scalarData + " func " + scalarFunc);
		if (!failed.isEmpty()) {
			if (isRunningHeadless()) {
				setHeadlessContinuationOption(HeadlessContinuationOption.ABORT);
			}
			throw new RuntimeException("ApplySymbols: " + failed.size() + " failed");
		}
	}

	private boolean lastClipped;
	private int holeFns, holeInstrs;
	private int grown, growRefused;
	private int split, absorbed;  // 3.2 T2
	private int splitBelow;  // 3.2 T4
	private boolean lastSplitBelow;
	private final List<Object[]> grows = new ArrayList<>();  // {Function, Address va, Address end}, line order
	// 3.2 T2: candidates.py:324 accepted statuses; func-line VAs; sized accepted func lines va -> size
	private static final java.util.Set<String> ACCEPTED = java.util.Set.of("unverified", "verified", "disasm-verified");
	private final TreeSet<Long> funcAt = new TreeSet<>();
	private final Map<Long, Long> goodSize = new TreeMap<>();
	private final Map<Long, Boolean> scalarTargets = new TreeMap<>();  // 3.2 T4: line VA -> is a func line

	// 3.2 T2 split (header): a (sized, accepted line) lies in C whose entry line is sized and accepted and ends at or
	// before a, or (3.2 T4 splitBelow) starts after a -> clip C to its pin range, create a's function.
	// -> false (C untouched) otherwise.
	private boolean splitAt(Address a, String name, String size) throws Exception {
		FunctionManager fm = currentProgram.getFunctionManager();
		Function c = fm.getFunctionContaining(a);
		if (c == null) {
			return false;
		}
		Address ce = c.getEntryPoint();
		lastSplitBelow = false;
		Long csz = goodSize.get(ce.getOffset());
		if (csz == null) {
			return false;
		}
		if (a.compareTo(ce) < 0) {
			// 3.2 T4 splitBelow: a below C's entry (tail-jump target folded into C's body)
			lastSplitBelow = true;
		}
		else if (ce.add(csz).compareTo(a) > 0) {
			return false;
		}
		ghidra.program.model.address.AddressSetView old = c.getBody();
		c.setBody(old.intersect(new AddressSet(ce, ce.add(csz - 1))));
		if (!createFn(a, name, size)) {
			c.setBody(old);
			lastSplitBelow = false;
			return false;
		}
		println("ApplySymbols: split" + (lastSplitBelow ? "Below " : " ") + c.getName() + " at " + ce + " for " +
			name + " at " + a);
		return true;
	}

	// 3.1 T8 (header): queue f for growth when its body ends before a+size-1.
	private void queueGrow(Function f, Address a, String size) throws Exception {
		if (f == null || !size.startsWith("0x")) {
			return;
		}
		long sz = Long.parseLong(size.substring(2), 16);
		if (sz <= 0) {
			return;
		}
		Address end = a.add(sz - 1);
		if (f.getBody().getMaxAddress().compareTo(end) < 0) {
			grows.add(new Object[] { f, a, end });
		}
	}

	// 3.1 T8 (header): grow each queued body to [.., end] unless R overlaps another function, leaves the block or
	// setBody throws.
	private void growAll() {
		Listing L = currentProgram.getListing();
		FunctionManager fm = currentProgram.getFunctionManager();
		for (Object[] g : grows) {
			Function f = (Function) g[0];
			Address a = (Address) g[1], end = (Address) g[2];
			Address max = f.getBody().getMaxAddress();
			if (max.compareTo(end) >= 0) {
				continue;
			}
			Address lo = max.next();
			AddressSet r = new AddressSet(lo, end);
			String why = null;
			MemoryBlock blk = currentProgram.getMemory().getBlock(max);
			if (blk == null || !blk.contains(end)) {
				why = "leaves block " + (blk == null ? "?" : blk.getName());
			}
			if (why == null) {
				// 3.2 T2 absorb (header): every overlapping o must be absorbable, else refuse as before
				List<Function> absorb = new ArrayList<>();
				AddressSet pin = new AddressSet(a, end);
				boolean ok = goodSize.containsKey(a.getOffset());
				Iterator<Function> it = fm.getFunctionsOverlapping(r);
				while (it.hasNext()) {
					Function o = it.next();
					if (o.equals(f)) {
						continue;
					}
					if (ok && o.getSymbol().getSource() == SourceType.DEFAULT &&
						!funcAt.contains(o.getEntryPoint().getOffset()) && pin.contains(o.getBody())) {
						absorb.add(o);
					}
					else {
						why = "overlaps " + o.getName() + " at " + o.getEntryPoint();
						break;
					}
				}
				if (why == null) {
					for (Function o : absorb) {
						String on = o.getName();
						fm.removeFunction(o.getEntryPoint());
						absorbed++;
						println("ApplySymbols: absorbed " + on + " into " + f.getName());
					}
				}
			}
			if (why == null) {
				try {
					Address cur = lo;
					while (cur != null && cur.compareTo(end) <= 0) {
						Data d = L.getDefinedDataContaining(cur);
						Instruction c = L.getInstructionContaining(cur);
						if (d == null && c == null) {
							disassemble(cur);
							c = L.getInstructionContaining(cur);
						}
						Address last = d != null ? d.getMaxAddress() : c != null ? c.getMaxAddress() : cur;
						cur = last.next();
					}
					f.setBody(f.getBody().union(r));
				}
				catch (Exception e) {
					why = "setBody " + e.getClass().getSimpleName();
				}
			}
			if (why != null) {
				growRefused++;
				println("ApplySymbols: growRefused " + f.getName() + " at " + a + " (" + why + ")");
			}
			else {
				grown++;
				println("ApplySymbols: grown " + f.getName() + " at " + a + " +0x" +
					Long.toHexString(end.subtract(max)).toUpperCase());
			}
		}
	}

	// 2.2 T9 hole fill (header): holes = [a, min(bodyMax, a+size-1)] minus f's body; walk them in address order,
	// disassembling each undefined start; an instruction lying wholly in a hole and in no other function joins f.
	private void fillHoles(Function f, Address a, String size) throws Exception {
		if (!size.startsWith("0x")) {
			return;
		}
		long sz = Long.parseLong(size.substring(2), 16);
		if (sz <= 0) {
			return;
		}
		Address max = f.getBody().getMaxAddress();
		Address end = a.add(sz - 1);
		if (max.compareTo(end) > 0) {
			max = end;
		}
		if (max.compareTo(a) <= 0) {
			return;
		}
		AddressSet hole = new AddressSet(a, max).subtract(f.getBody());
		if (hole.isEmpty()) {
			return;
		}
		Listing L = currentProgram.getListing();
		FunctionManager fm = currentProgram.getFunctionManager();
		AddressSet add = new AddressSet();
		int added = 0;
		Address cur = hole.getMinAddress();
		while (cur != null) {
			Address next;
			Function other = fm.getFunctionContaining(cur);
			Data d = L.getDefinedDataContaining(cur);
			if (other != null && !other.equals(f)) {
				next = cur.next();
			}
			else if (d != null) {
				next = d.getMaxAddress().next();
			}
			else {
				Instruction ins = L.getInstructionAt(cur);
				if (ins == null && L.getInstructionContaining(cur) == null) {
					disassemble(cur);
					ins = L.getInstructionAt(cur);
				}
				if (ins != null) {
					Function o2 = fm.getFunctionContaining(ins.getMaxAddress());
					if (hole.contains(ins.getMinAddress(), ins.getMaxAddress()) && (o2 == null || o2.equals(f))) {
						add.add(ins.getMinAddress(), ins.getMaxAddress());
						added++;
					}
					next = ins.getMaxAddress().next();
				}
				else {
					Instruction c = L.getInstructionContaining(cur);
					next = c != null ? c.getMaxAddress().next() : cur.next();
				}
			}
			if (next == null || next.compareTo(hole.getMaxAddress()) > 0) {
				break;
			}
			AddressSet rest = hole.intersectRange(next, hole.getMaxAddress());
			cur = rest.isEmpty() ? null : rest.getMinAddress();
		}
		if (added > 0) {
			f.setBody(f.getBody().union(add));
			holeFns++;
			holeInstrs += added;
			println("ApplySymbols: hole " + f.getName() + " at " + a + " instrs " + added);
		}
	}

	// T12 (1.5): disassemble at a if needed, create a USER_DEFINED function named name there and clip a body that runs
	// past a+size (size "0x.." from symbols.txt) to [a, a+size). -> false if no function was created.
	private boolean createFn(Address a, String name, String size) throws Exception {
		lastClipped = false;
		if (currentProgram.getListing().getInstructionAt(a) == null) {
			disassemble(a);
		}
		Function f = createFunction(a, name);
		if (f == null) {
			return false;
		}
		if (size.startsWith("0x")) {
			long sz = Long.parseLong(size.substring(2), 16);
			if (sz > 0) {
				AddressSet keep = new AddressSet(a, a.add(sz - 1));
				if (!keep.contains(f.getBody())) {
					Address bmin = f.getBody().getMinAddress(), bmax = f.getBody().getMaxAddress();
					f.setBody(f.getBody().intersect(keep));
					lastClipped = true;
					// 3.2 T2: name each clip so its remainder can be identified
					println("ApplySymbols: clipped " + name + " at " + a + " body " + bmin + ".." + bmax + " kept .." +
						keep.getMaxAddress());
				}
			}
		}
		return true;
	}
}
