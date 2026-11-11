// TrimAnalyzers (phase 5.4 T3.c4): -preScript of tools/delink.py ingest (plain and --fresh), run after import and
// before auto-analysis. Disables the "Decompiler Parameter ID", "Call Convention ID" and "Stack" analyzers to shorten
// the ingest. Each option name must exist in the program's analysis options and read back false after the set, else
// the script throws (analyzeHeadless then fails the ingest): a renamed analyzer never passes silently. Prints
// "TrimAnalyzers: disabled <names>". Firewall D46: option names only.
//@category VoodooVince

import java.util.List;

import ghidra.app.script.GhidraScript;
import ghidra.framework.options.Options;
import ghidra.program.model.listing.Program;

public class TrimAnalyzers extends GhidraScript {
	private static final List<String> OFF = List.of("Decompiler Parameter ID", "Call Convention ID", "Stack");

	@Override
	protected void run() throws Exception {
		Options opts = currentProgram.getOptions(Program.ANALYSIS_PROPERTIES);
		for (String name : OFF) {
			if (!opts.contains(name)) {
				throw new IllegalStateException("TrimAnalyzers: no analysis option '" + name + "'");
			}
			setAnalysisOption(currentProgram, name, "false");
			if (opts.getBoolean(name, true)) {
				throw new IllegalStateException("TrimAnalyzers: option '" + name + "' still enabled");
			}
		}
		println("TrimAnalyzers: disabled " + String.join(", ", OFF));
	}
}
