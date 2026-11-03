// P0-3 step 2a: import every COFF object member of the SDK .lib archives into the project,
// one folder per archive: /sdklibs/<archive>/<member basename>. Adapted from Ghidra's
// ImportMSLibs.java. Headless "-import -recursive" cannot be used: the members are named with
// relative paths (".\Release\x.obj", "..\..\y.obj") and the headless importer skips them.
// Args: <project folder> <out tsv> <lib>...   (no analysis here; step 2a runs it afterwards)
//@category SFCOP
import java.io.*;
import java.nio.file.AccessMode;
import java.util.*;

import ghidra.app.script.GhidraScript;
import ghidra.app.util.bin.*;
import ghidra.app.util.bin.format.coff.*;
import ghidra.app.util.bin.format.coff.archive.CoffArchiveHeader;
import ghidra.app.util.bin.format.coff.archive.CoffArchiveMemberHeader;
import ghidra.app.util.importer.MessageLog;
import ghidra.app.util.importer.ProgramLoader;
import ghidra.app.util.opinion.*;
import ghidra.framework.model.DomainFile;
import ghidra.framework.model.DomainFolder;
import ghidra.framework.store.local.LocalFileSystem;
import ghidra.program.model.listing.Program;
import ghidra.util.task.CancelOnlyWrappingTaskMonitor;
import ghidra.util.task.TaskMonitor;

public class SfcImportLibs extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		String rootPath = args[0];
		File report = new File(args[1]);
		DomainFolder top = state.getProject().getProjectData().getRootFolder();
		DomainFolder root = top.getFolder(rootPath.replaceAll("^/", ""));
		if (root != null) {
			deleteContents(root);
		}
		else {
			root = top.createFolder(rootPath.replaceAll("^/", ""));
		}
		try (PrintWriter out = new PrintWriter(new FileWriter(report))) {
			out.println("archive\tmember\tproject_file\tstatus");
			for (int i = 2; i < args.length; i++) {
				File lib = new File(args[i]);
				DomainFolder folder = root.createFolder(lib.getName());
				importLibrary(folder, lib, out);
			}
		}
	}

	private void importLibrary(DomainFolder folder, File file, PrintWriter out) throws Exception {
		MessageLog log = new MessageLog();
		Set<String> used = new HashSet<>();
		int ok = 0, skipped = 0, failed = 0;
		try (ByteProvider provider = new FileByteProvider(file, null, AccessMode.READ)) {
			CoffArchiveHeader hdr = CoffArchiveHeader.read(provider, TaskMonitor.DUMMY);
			Set<Long> seen = new HashSet<>();
			for (CoffArchiveMemberHeader m : hdr.getArchiveMemberHeaders()) {
				monitor.checkCancelled();
				if (!seen.add(m.getPayloadOffset())) {
					continue;
				}
				String member = m.getName();
				if (!m.isCOFF()) {
					skipped++;
					out.println(file.getName() + "\t" + member + "\t\tnot-coff");
					continue;
				}
				String[] parts = member.split("[/\\\\]");
				String name = safe(parts[parts.length - 1]);
				String unique = name;
				for (int n = 2; !used.add(unique); n++) {
					unique = name + "_" + n;
				}
				try (ByteProvider coff = new ByteProviderWrapper(provider, m.getPayloadOffset(),
					m.getSize())) {
					CoffFileHeader ch = new CoffFileHeader(coff);
					if (!CoffMachineType.isMachineTypeDefined(ch.getMagic())) {
						skipped++;
						out.println(file.getName() + "\t" + member + "\t\tunknown-machine");
						continue;
					}
					try (LoadResults<Program> results = ProgramLoader.builder()
							.source(coff)
							.project(state.getProject())
							.projectFolderPath(folder.getPathname())
							.loaders(MSCoffLoader.class)
							.compiler("windows")
							.name(unique)
							.log(log)
							.monitor(new CancelOnlyWrappingTaskMonitor(monitor))
							.load()) {
						for (Loaded<Program> loaded : results) {
							loaded.save(monitor);
						}
					}
					ok++;
					out.println(file.getName() + "\t" + member + "\t" + folder.getPathname() + "/" +
						unique + "\timported");
				}
				catch (CoffException e) {
					// short import descriptors (e.g. BugslayerUtil.dll stubs) carry no code
					skipped++;
					out.println(file.getName() + "\t" + member + "\t\tnot-object: " + e.getMessage());
				}
				catch (Exception e) {
					failed++;
					out.println(file.getName() + "\t" + member + "\t\tfailed: " + e.getMessage());
				}
			}
		}
		println(String.format("%s: imported=%d skipped=%d failed=%d", file.getName(), ok, skipped,
			failed));
	}

	private void deleteContents(DomainFolder f) throws IOException {
		for (DomainFolder sub : f.getFolders()) {
			deleteContents(sub);
			sub.delete();
		}
		for (DomainFile df : f.getFiles()) {
			df.delete();
		}
	}

	private static String safe(String name) {
		if (name == null || name.isEmpty()) {
			return "(EMPTY)";
		}
		StringBuilder sb = new StringBuilder();
		for (char c : name.toCharArray()) {
			sb.append(LocalFileSystem.isValidNameCharacter(c) ? c : '_');
		}
		return sb.toString();
	}
}
