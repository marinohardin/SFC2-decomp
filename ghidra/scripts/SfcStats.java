// P0-3: function/symbol counts after a pipeline step. Arg 0: output file (overwritten).
//@category SFCOP
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Map;
import java.util.TreeMap;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.SourceType;

public class SfcStats extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		Map<String, Integer> counts = new TreeMap<>();
		long bytes = 0, namedBytes = 0;
		int total = 0, thunks = 0, external = 0;
		for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
			total++;
			if (f.isThunk()) {
				thunks++;
			}
			long size = f.getBody().getNumAddresses();
			bytes += size;
			SourceType src = f.getSymbol().getSource();
			counts.merge("source." + src, 1, Integer::sum);
			if (src != SourceType.DEFAULT) {
				namedBytes += size;
			}
		}
		for (Function f : currentProgram.getFunctionManager().getExternalFunctions()) {
			external++;
		}
		for (Bookmark b : (Iterable<Bookmark>) () -> currentProgram.getBookmarkManager()
				.getBookmarksIterator()) {
			counts.merge("bookmark." + b.getTypeString() + "." + b.getCategory(), 1,
				Integer::sum);
		}
		try (PrintWriter out =
			new PrintWriter(new FileWriter(args.length > 0 ? args[0] : "/dev/stdout"))) {
			out.println("# " + currentProgram.getName() + " " + new java.util.Date());
			out.println("functions\t" + total);
			out.println("thunks\t" + thunks);
			out.println("external_functions\t" + external);
			out.println("function_bytes\t" + bytes);
			out.println("named_function_bytes\t" + namedBytes);
			out.println("classes\t" + count(currentProgram.getSymbolTable().getClassNamespaces()));
			out.println("data_types\t" + currentProgram.getDataTypeManager().getDataTypeCount(true));
			for (Map.Entry<String, Integer> e : counts.entrySet()) {
				out.println(e.getKey() + "\t" + e.getValue());
			}
		}
	}

	private static int count(java.util.Iterator<?> it) {
		int n = 0;
		while (it.hasNext()) {
			it.next();
			n++;
		}
		return n;
	}
}
