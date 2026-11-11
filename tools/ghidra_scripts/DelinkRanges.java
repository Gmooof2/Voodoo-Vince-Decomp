// DelinkRanges (phase 5.4 T3.c2): run by tools/relink.py cut and tools/delink.py --fresh after the project opens.
// Its one script argument is an args file (UTF-8, one DelinkProgram argument per line, blank lines skipped); it calls
// DelinkProgram.java once with those lines, so the extension's exporter, options and argument parsing run unchanged.
// Why: a range list on the command line hits cmd.exe's 8191-char cap, which forced one analyzeHeadless launch (one
// JVM start and project open) per ~5500 chars of ranges; the file carries any number of ranges in one launch.
// A DelinkProgram failure propagates as this script's error ("REPORT SCRIPT ERROR"). Firewall D46: paths only.
//@category VoodooVince

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.stream.Collectors;

import ghidra.app.util.headless.HeadlessScript;

public class DelinkRanges extends HeadlessScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length != 1) {
			throw new IllegalArgumentException("DelinkRanges: want one argument (the args file), got " + args.length);
		}
		List<String> lines = Files.readAllLines(Paths.get(args[0]), StandardCharsets.UTF_8).stream()
				.filter(s -> !s.isEmpty()).collect(Collectors.toList());
		println("DelinkRanges: " + lines.size() + " DelinkProgram argument(s) from " + args[0]);
		runScript("DelinkProgram.java", lines.toArray(new String[0]));
	}
}
