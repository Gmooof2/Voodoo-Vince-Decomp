// ExportProgram (T5, phase 1.3): text export of an analysed program for tools/delink.py ingest.
// Arg 0: output directory. Writes functions.jsonl, symbols.jsonl, then summary.json LAST.
// Firewall D46: addresses, names, counts and hashes only; never bytes or disassembly.
// xbsymdb: ghidra-xbe XbeXbSymbolDatabaseAnalyzer creates each recovered label with
//   createLabel(addr, name, <global namespace named after the library, created IMPORTED>, SourceType.ANALYSIS);
//   when a function is later created there the label becomes its FUNCTION symbol (same name, namespace, source);
//   XbeXtlidAnalyzer uses the same pattern for .XTLID table entries, so those (va, namespace, name) are excluded
//   (xtlid = true). kernel: XbeLoader labels each kernel thunk slot (IMPORTED) and adds an external reference
//   from it to library "xboxkrnl.exe".
//@category VoodooVince

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import ghidra.app.script.GhidraScript;
import ghidra.framework.Application;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.ExternalReference;
import ghidra.program.model.symbol.Namespace;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolType;

public class ExportProgram extends GhidraScript {

	private static String q(String s) {
		if (s == null) {
			return "null";
		}
		StringBuilder b = new StringBuilder("\"");
		for (char c : s.toCharArray()) {
			if (c == '"' || c == '\\') {
				b.append('\\').append(c);
			}
			else if (c < 0x20) {
				b.append(String.format("\\u%04x", (int) c));
			}
			else {
				b.append(c);
			}
		}
		return b.append('"').toString();
	}

	private static String va(Address a) {
		return String.format("\"0x%08X\"", a.getOffset());
	}

	private PrintWriter open(File f) throws Exception {
		return new PrintWriter(new OutputStreamWriter(new FileOutputStream(f), StandardCharsets.UTF_8));
	}

	/** "va|namespace|name" of every resolvable .XTLID table entry (XbeXtlidAnalyzer's own lookup). */
	private Set<String> xtlidKeys() throws Exception {
		Set<String> keys = new HashSet<>();
		MemoryBlock sec = currentProgram.getMemory().getBlock(".XTLID");
		if (sec == null) {
			return keys;
		}
		for (Address a = sec.getStart(); a.add(7).compareTo(sec.getEnd()) <= 0; a = a.add(8)) {
			long id = getInt(a) & 0xFFFFFFFFL;
			if (id == 0) {
				continue;
			}
			long target = getInt(a.add(4)) & 0xFFFFFFFFL;
			String[] nn = xbeloader.XbeXtlidDb.xtlids.get(id);
			if (nn != null) {
				keys.add(String.format("%08X|%s|%s", target, nn[0], nn[1]));
			}
		}
		return keys;
	}

	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length != 1) {
			throw new IllegalArgumentException("usage: ExportProgram.java <outdir>");
		}
		File out = new File(args[0]);
		out.mkdirs();
		Namespace global = currentProgram.getGlobalNamespace();

		int nFunc = 0;
		try (PrintWriter w = open(new File(out, "functions.jsonl"))) {
			FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
			while (it.hasNext()) {
				Function f = it.next();
				if (f.isExternal()) {
					continue;
				}
				AddressSetView body = f.getBody();
				w.println("{\"entry\": " + va(f.getEntryPoint()) + ", \"size\": " + body.getNumAddresses() +
					", \"bodyMin\": " + va(body.getMinAddress()) + ", \"bodyMax\": " + va(body.getMaxAddress()) +
					", \"name\": " + q(f.getName()) + ", \"nameSource\": " + q(f.getSymbol().getSource().toString()) +
					", \"thunk\": " + f.isThunk() + "}");
				nFunc++;
			}
		}

		Set<String> xtlid = xtlidKeys();
		int nSym = 0, nXbs = 0, nXtlid = 0, nKern = 0;
		// rows sorted as text: fixed-width va first, then name: a fixed tie-break for symbols sharing a va
		// (Ghidra's own same-address order and primary choice vary between runs at shared switch-case targets)
		List<String> rows = new ArrayList<>();
		{
			SymbolIterator it = currentProgram.getSymbolTable().getAllSymbols(false);
			while (it.hasNext()) {
				Symbol s = it.next();
				Address a = s.getAddress();
				if (s.isExternal() || a == null || !a.isMemoryAddress()) {
					continue;
				}
				Namespace ns = s.getParentNamespace();
				Symbol nsSym = ns.getSymbol();
				boolean libNs = !ns.isGlobal() && ns.getParentNamespace() != null &&
					ns.getParentNamespace().isGlobal() && nsSym.getSymbolType() == SymbolType.NAMESPACE &&
					nsSym.getSource() == SourceType.IMPORTED;
				// a label later becomes the FUNCTION symbol when analysis creates a function at its address
				boolean mech = (s.getSymbolType() == SymbolType.LABEL || s.getSymbolType() == SymbolType.FUNCTION) &&
					s.getSource() == SourceType.ANALYSIS && libNs;
				boolean isX = mech && xtlid.contains(String.format("%08X|%s|%s", a.getOffset(), ns.getName(), s.getName()));
				boolean xbs = mech && !isX;
				boolean kern = false;
				if (s.getSource() == SourceType.IMPORTED) {
					for (Reference r : currentProgram.getReferenceManager().getReferencesFrom(a)) {
						if (r instanceof ExternalReference &&
							"xboxkrnl.exe".equalsIgnoreCase(((ExternalReference) r).getLibraryName())) {
							kern = true;
						}
					}
				}
				rows.add("{\"va\": " + va(a) + ", \"name\": " + q(s.getName()) +
					", \"namespace\": " + q(ns.isGlobal() ? "" : ns.getName(true)) +
					", \"symbolType\": " + q(s.getSymbolType().toString()) + ", \"source\": " + q(s.getSource().toString()) +
					", \"primary\": " + s.isPrimary() + ", \"xbsymdb\": " + xbs + ", \"xtlid\": " + isX +
					", \"kernel\": " + kern + "}");
				nSym++;
				nXbs += xbs ? 1 : 0;
				nXtlid += isX ? 1 : 0;
				nKern += kern ? 1 : 0;
			}
		}
		Collections.sort(rows);
		try (PrintWriter w = open(new File(out, "symbols.jsonl"))) {
			for (String r : rows) {
				w.println(r);
			}
		}

		try (PrintWriter w = open(new File(out, "summary.json"))) {
			w.println("{\"functions\": " + nFunc + ", \"symbols\": " + nSym + ", \"xbsymdb\": " + nXbs +
				", \"xtlid\": " + nXtlid + ", \"kernel\": " + nKern +
				", \"xtlidTableEntries\": " + xtlid.size() +
				", \"loader\": " + q(currentProgram.getExecutableFormat()) +
				", \"ghidraVersion\": " + q(Application.getApplicationVersion()) +
				", \"executableSHA256\": " + q(currentProgram.getExecutableSHA256()) +
				", \"executableMD5\": " + q(currentProgram.getExecutableMD5()) + "}");
		}
		println("ExportProgram: functions " + nFunc + " symbols " + nSym + " xbsymdb " + nXbs + " kernel " + nKern);
	}
}
