// P0-3 step 1b (part 2): disassemble and create functions in undefined .text gaps.
// Arg 0: out tsv (one row per pass).
//
// After auto-analysis about 1.1 MB of .text is still undefined, in ~15,800 gaps that mostly begin
// right where the previous function ends, on prologue bytes (push ecx, sub esp, mov eax,<EH handler>):
// VC6 packed the functions without alignment, and their only callers are themselves undiscovered
// or reach them through tables. Each pass takes every undefined range, skips padding
// (CC/90/00), rejects starts that look like a jump-table entry (a dword pointing into .text) or a
// lone ret, disassembles by flow and creates a function; then auto-analysis runs on the changes
// (switch tables get defined, references created) before the next pass. Stops when a pass adds
// nothing or after 8 passes.
//@category SFCOP
import java.io.*;

import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.MemoryBlock;

public class SfcFillGaps extends GhidraScript {
	@Override
	protected void run() throws Exception {
		MemoryBlock t = currentProgram.getMemory().getBlock(".text");
		AddressSet text = new AddressSet(t.getStart(), t.getEnd());
		long lo = t.getStart().getOffset(), hi = t.getEnd().getOffset();
		Listing listing = currentProgram.getListing();
		try (PrintWriter out = new PrintWriter(new FileWriter(getScriptArgs()[0]))) {
			out.println("pass\tgaps\tundefined_bytes_before\tfunctions_created\trejected\tbad_instruction_bookmarks");
			for (int pass = 1; pass <= 8; pass++) {
				AddressSetView undef = listing.getUndefinedRanges(text, true, monitor);
				long before = undef.getNumAddresses();
				int gaps = 0, created = 0, rejected = 0;
				for (AddressRange r : undef) {
					monitor.checkCancelled();
					gaps++;
					Address a = r.getMinAddress();
					while (a.compareTo(r.getMaxAddress()) <= 0) {
						int b = getByte(a) & 0xff;
						if (b != 0xcc && b != 0x90 && b != 0x00) {
							break;
						}
						a = a.add(1);
					}
					if (a.compareTo(r.getMaxAddress()) > 0 || listing.getInstructionAt(a) != null) {
						continue;
					}
					int first = getByte(a) & 0xff;
					long dword = r.getMaxAddress().subtract(a) >= 3 ? getInt(a) & 0xffffffffL : 0;
					if (first == 0xc3 || first == 0xc2 || (dword >= lo && dword <= hi)) {
						rejected++;
						continue;
					}
					if (!disassemble(a) || listing.getInstructionAt(a) == null) {
						rejected++;
						continue;
					}
					if (getFunctionContaining(a) == null &&
						new CreateFunctionCmd(a).applyTo(currentProgram, monitor)) {
						created++;
					}
				}
				analyzeChanges(currentProgram);
				int bad = currentProgram.getBookmarkManager().getBookmarkCount("Error");
				out.println(pass + "\t" + gaps + "\t" + before + "\t" + created + "\t" + rejected +
					"\t" + bad);
				out.flush();
				println(String.format("gap pass %d: gaps=%d undefined=%d created=%d rejected=%d " +
					"error_bookmarks=%d", pass, gaps, before, created, rejected, bad));
				if (created == 0) {
					break;
				}
			}
			long after = listing.getUndefinedRanges(text, true, monitor).getNumAddresses();
			out.println("#final\t\t" + after + "\t\t\t");
			println("gap fill: undefined .text bytes left " + after);
		}
	}
}
