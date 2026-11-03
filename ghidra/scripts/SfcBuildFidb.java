// P0-3 step 2a: build a Function ID database from the SDK .lib members imported (and analysed)
// under one project folder per archive, e.g. /sdklibs/SharedR.lib/...
// Args: <fidb file> <project folder holding one subfolder per .lib> <report tsv> [common symbols]
// One FID library per archive: family "SFCOP-SDK", version = archive name, variant "msvc6sp4-release".
//@category SFCOP
import java.io.*;
import java.nio.file.Files;
import java.util.*;

import ghidra.app.script.GhidraScript;
import ghidra.feature.fid.db.*;
import ghidra.feature.fid.service.*;
import ghidra.framework.model.DomainFile;
import ghidra.framework.model.DomainFolder;
import ghidra.program.database.ProgramContentHandler;

public class SfcBuildFidb extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		File fidbFile = new File(args[0]);
		String rootPath = args[1];
		File report = new File(args[2]);
		List<String> commonSymbols = null;
		if (args.length > 3) {
			commonSymbols = new ArrayList<>();
			for (String line : Files.readAllLines(new File(args[3]).toPath())) {
				if (!line.isBlank()) {
					commonSymbols.add(line.trim());
				}
			}
		}

		DomainFolder root = state.getProject().getProjectData().getFolder(rootPath);
		if (root == null) {
			throw new IllegalArgumentException("no project folder " + rootPath);
		}
		if (fidbFile.exists()) {
			fidbFile.delete();
		}
		FidFileManager mgr = FidFileManager.getInstance();
		mgr.createNewFidDatabase(fidbFile);
		FidFile fidFile = mgr.addUserFidFile(fidbFile);
		FidService service = new FidService();
		FidDB fidDb = fidFile.getFidDB(true);
		try (PrintWriter out = new PrintWriter(new FileWriter(report))) {
			out.println("library\tprograms\tattempted\tadded\texcluded\texclusion_breakdown");
			for (DomainFolder lib : root.getFolders()) {
				List<DomainFile> programs = new ArrayList<>();
				findPrograms(programs, lib);
				FidFilter filter = new FidFilter("x86:LE:32:default", "", "");
				FidPopulateResult result = service.createNewLibraryFromPrograms(fidDb, "SFCOP-SDK",
					lib.getName(), "msvc6sp4-release", programs, null, filter, null, commonSymbols,
					monitor);
				StringBuilder why = new StringBuilder();
				if (result != null) {
					for (Map.Entry<FidPopulateResult.Disposition, Integer> e : result.getFailures()
							.entrySet()) {
						if (e.getKey() != FidPopulateResult.Disposition.INCLUDED) {
							why.append(e.getKey()).append('=').append(e.getValue()).append(' ');
						}
					}
				}
				String line = lib.getName() + "\t" + programs.size() + "\t" +
					(result == null ? "-\t-\t-" : result.getTotalAttempted() + "\t" +
						result.getTotalAdded() + "\t" + result.getTotalExcluded()) +
					"\t" + why.toString().trim();
				out.println(line);
				println(line);
			}
			fidDb.saveDatabase("Saving", monitor);
		}
		finally {
			fidDb.close();
		}
	}

	private void findPrograms(List<DomainFile> programs, DomainFolder folder) {
		for (DomainFile f : folder.getFiles()) {
			if (f.getContentType().equals(ProgramContentHandler.PROGRAM_CONTENT_TYPE)) {
				programs.add(f);
			}
		}
		for (DomainFolder sub : folder.getFolders()) {
			findPrograms(programs, sub);
		}
	}
}
