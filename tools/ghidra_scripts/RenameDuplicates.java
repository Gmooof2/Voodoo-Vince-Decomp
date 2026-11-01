// RenameDuplicates (phase 2.3 T4.c2): run by tools/relink.py cut before DelinkProgram.java on the read-only project.
// DelinkProgram fails a whole object on a duplicate symbol name (T4.c1: `xboxkrnl.exe::thunk_XcSHAUpdate` on thunks
// 0x000172B8 and 0x0001786C; ApplySymbols renames duplicate thunks to `thunk_<name>`, which stays duplicate for two
// thunks to one import, C0021). Every function whose fully qualified name another function also has, except the
// first in address order, is renamed in memory to `<name>_<va8>` (upper hex), same namespace and source type.
// The project is opened -readOnly: nothing is saved. Prints "RenameDuplicates: groups G renamed R" and each rename;
// a failed rename throws (DelinkProgram does not run). Firewall D46: names and addresses only.
//@category VoodooVince

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;

public class RenameDuplicates extends GhidraScript {
	@Override
	protected void run() throws Exception {
		Map<String, List<Function>> groups = new TreeMap<>();
		for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
			groups.computeIfAbsent(f.getName(true), k -> new ArrayList<>()).add(f);
		}
		int ng = 0, renamed = 0;
		for (List<Function> g : groups.values()) {
			if (g.size() < 2) {
				continue;
			}
			ng++;
			for (Function f : g.subList(1, g.size())) {
				String old = f.getName(true);
				String nu = String.format("%s_%08X", f.getName(), f.getEntryPoint().getOffset());
				f.setName(nu, f.getSymbol().getSource());
				renamed++;
				println("RenameDuplicates: " + old + " at " + f.getEntryPoint() + " -> " + f.getName(true));
			}
		}
		println("RenameDuplicates: groups " + ng + " renamed " + renamed);
	}
}
