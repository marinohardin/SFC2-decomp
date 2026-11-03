// P0-3 step 2b: apply the SDK Function ID database (and only it) to StarFleetOP.exe.
// Args: <fidb file> <out tsv>
//
// Rules (kept simple so the counts are easy to audit):
//  - a function is a candidate if FID returns matches scoring >= 14.6 (Ghidra's default);
//  - exactly one distinct symbol name -> name it (mangled name as label, demangled by Ghidra),
//    source ANALYSIS, function tag "sdk-fid";
//  - several distinct names -> do not name; tag "sdk-fid-multi" and list the candidates;
//  - a function already carrying an IMPORTED or USER_DEFINED name is never renamed.
// Every candidate is written to the tsv with its previous name, so the conflict check against the
// exact-match table (step 3) and against class recovery can be audited.
//@category SFCOP
import java.io.*;
import java.util.*;

import ghidra.app.cmd.label.DemanglerCmd;
import ghidra.app.script.GhidraScript;
import ghidra.feature.fid.db.*;
import ghidra.feature.fid.service.*;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.*;

public class SfcApplyFid extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		File fidbFile = new File(args[0]);
		File outFile = new File(args[1]);

		FidFileManager mgr = FidFileManager.getInstance();
		FidFile ours = mgr.addUserFidFile(fidbFile);
		Map<FidFile, Boolean> saved = new HashMap<>();
		for (FidFile f : mgr.getFidFiles()) {
			saved.put(f, f.isActive());
			f.setActive(f.equals(ours));
		}
		FidService service = new FidService();
		int single = 0, multi = 0, renamed = 0, keptStrong = 0;
		try (FidQueryService q =
			service.openFidQueryService(new FidProgramID(currentProgram, true), false);
				PrintWriter out = new PrintWriter(new FileWriter(outFile))) {
			out.println("address\tdisposition\tscore\tprevious_name\tprevious_source\tfid_names");
			List<FidSearchResult> results = service.processProgram(currentProgram, q,
				service.getDefaultScoreThreshold(), monitor);
			results.sort(Comparator.comparing(r -> r.function.getEntryPoint()));
			for (FidSearchResult r : results) {
				monitor.checkCancelled();
				if (r.matches == null || r.matches.isEmpty()) {
					continue;
				}
				TreeSet<String> names = new TreeSet<>();
				float best = 0;
				for (FidMatch m : r.matches) {
					names.add(m.getFunctionRecord().getName());
					best = Math.max(best, m.getOverallScore());
				}
				Function f = r.function;
				Symbol sym = f.getSymbol();
				String prev = f.getName(true);
				SourceType prevSrc = sym.getSource();
				String disp;
				if (names.size() == 1) {
					single++;
					if (prevSrc == SourceType.IMPORTED || prevSrc == SourceType.USER_DEFINED) {
						disp = "single-kept-existing";
						keptStrong++;
					}
					else {
						applyName(f, names.first());
						f.addTag("sdk-fid");
						disp = prevSrc == SourceType.DEFAULT ? "single-named" : "single-renamed";
						renamed++;
					}
				}
				else {
					multi++;
					f.addTag("sdk-fid-multi");
					disp = "multi";
				}
				out.println(f.getEntryPoint() + "\t" + disp + "\t" + best + "\t" + prev + "\t" +
					prevSrc + "\t" + String.join(" | ", names));
			}
		}
		finally {
			for (Map.Entry<FidFile, Boolean> e : saved.entrySet()) {
				e.getKey().setActive(e.getValue());
			}
			mgr.removeUserFile(ours);
		}
		println(String.format("SDK FID: single=%d (applied=%d, kept existing=%d) multi=%d",
			single, renamed, keptStrong, multi));
	}

	private void applyName(Function f, String name) throws Exception {
		SymbolTable st = currentProgram.getSymbolTable();
		// demangled name on the function, mangled name kept as a label
		if (name.startsWith("?") &&
			new DemanglerCmd(f.getEntryPoint(), name).applyTo(currentProgram, monitor)) {
			if (st.getGlobalSymbol(name, f.getEntryPoint()) == null) {
				st.createLabel(f.getEntryPoint(), name, SourceType.ANALYSIS);
			}
		}
		else {
			f.setName(name, SourceType.ANALYSIS);
		}
	}
}
