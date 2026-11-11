// Decompiled pseudo-C for the grind harness's context pack (harness/lib/pack.py).
//   Arg 0: output directory; one file per function, <0xaddress>.c.
//   Arg 1: a file of function addresses, one per line (hex, with or without 0x).
//   Arg 2: optional per-function decompiler timeout in seconds (default 30).
// A function whose file already exists is skipped, so an interrupted run resumes where it
// stopped. A function the decompiler cannot finish gets a file holding a one-line comment, so it
// is not retried on every run. Read-only: nothing is saved to the program.
//@category SFCOP
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class SfcExportDecomp extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		File out = new File(args[0]);
		out.mkdirs();
		List<String> wanted = Files.readAllLines(Paths.get(args[1]), StandardCharsets.UTF_8);
		int timeout = args.length > 2 ? Integer.parseInt(args[2]) : 30;

		DecompInterface ifc = new DecompInterface();
		ifc.setOptions(new DecompileOptions());
		ifc.toggleCCode(true);
		ifc.toggleSyntaxTree(false);
		if (!ifc.openProgram(currentProgram)) {
			printerr("SfcExportDecomp: decompiler did not open: " + ifc.getLastMessage());
			return;
		}
		int done = 0, skipped = 0, failed = 0, missing = 0;
		long t0 = System.currentTimeMillis();
		for (String line : wanted) {
			String s = line.trim().toLowerCase();
			if (s.isEmpty() || s.startsWith("#")) continue;
			if (s.startsWith("0x")) s = s.substring(2);
			long a = Long.parseLong(s, 16);
			String id = String.format("0x%08x", a);
			File f = new File(out, id + ".c");
			if (f.exists()) { skipped++; continue; }
			if (monitor.isCancelled()) break;
			Address addr = toAddr(a);
			Function fn = getFunctionAt(addr);
			if (fn == null) { missing++; continue; }
			String text;
			DecompileResults r = ifc.decompileFunction(fn, timeout, monitor);
			if (r != null && r.decompileCompleted() && r.getDecompiledFunction() != null) {
				text = r.getDecompiledFunction().getC();
				done++;
			} else {
				text = "/* decompiler gave up: " + (r == null ? "no result" : r.getErrorMessage()) + " */\n";
				failed++;
			}
			File tmp = new File(out, id + ".c.tmp");
			try (Writer w = new OutputStreamWriter(new FileOutputStream(tmp), StandardCharsets.UTF_8)) {
				w.write(text);
			}
			Files.move(tmp.toPath(), f.toPath(), StandardCopyOption.REPLACE_EXISTING);
			if ((done + failed) % 500 == 0) {
				println(String.format("SfcExportDecomp: %d decompiled, %d failed, %d skipped, %d s",
					done, failed, skipped, (System.currentTimeMillis() - t0) / 1000));
			}
		}
		ifc.dispose();
		println(String.format("SfcExportDecomp: %d decompiled, %d failed, %d already there, %d not functions, %d s",
			done, failed, skipped, missing, (System.currentTimeMillis() - t0) / 1000));
	}
}
