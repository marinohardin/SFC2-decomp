// P0-3 pre-analysis script for StarFleetOP.exe: makes sure the MSVC RTTI analyzer and the
// built-in Function ID analyzer are on, and prints the analyzer switches actually used.
//@category SFCOP
import java.util.Map;
import java.util.TreeMap;

import ghidra.app.script.GhidraScript;

public class SfcAnalysisOptions extends GhidraScript {
	private static final String[] ENABLE = {
		"Windows x86 PE RTTI Analyzer",
		"Function ID",
		"Demangler Microsoft",
	};

	@Override
	protected void run() throws Exception {
		Map<String, String> options = getCurrentAnalysisOptionsAndValues(currentProgram);
		for (String name : ENABLE) {
			if (!options.containsKey(name)) {
				printerr("analyzer option not found: " + name);
				continue;
			}
			setAnalysisOption(currentProgram, name, "true");
		}
		options = new TreeMap<>(getCurrentAnalysisOptionsAndValues(currentProgram));
		for (Map.Entry<String, String> e : options.entrySet()) {
			if (!e.getKey().contains(".")) {
				println("ANALYZER\t" + e.getKey() + "\t" + e.getValue());
			}
		}
	}
}
