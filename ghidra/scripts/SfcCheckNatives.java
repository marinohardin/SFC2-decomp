// P0-3: fail fast if Ghidra cannot find the native decompiler.
//@category SFCOP
import ghidra.app.script.GhidraScript;
import ghidra.framework.Application;

public class SfcCheckNatives extends GhidraScript {
	@Override
	protected void run() throws Exception {
		println("decompile native: " + Application.getOSFile("decompile"));
	}
}
