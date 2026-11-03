// P0-3: save the analysed program as a Ghidra Zip File (.gzf) that any Ghidra 12.2+ can import
// without re-analysis. Arg 0: output .gzf path. Run it with -readOnly after the last saving step.
//@category SFCOP
import java.io.File;

import ghidra.app.script.GhidraScript;
import ghidra.app.util.exporter.GzfExporter;

public class SfcSaveGzf extends GhidraScript {
	@Override
	protected void run() throws Exception {
		File out = new File(getScriptArgs()[0]);
		if (out.exists()) {
			out.delete();
		}
		GzfExporter exporter = new GzfExporter();
		// export the saved domain file: the open program is locked by the script's transaction
		if (!exporter.export(out, currentProgram.getDomainFile(), monitor)) {
			throw new RuntimeException("gzf export failed: " + exporter.getMessageLog());
		}
		println("saved " + out + " (" + out.length() + " bytes)");
	}
}
