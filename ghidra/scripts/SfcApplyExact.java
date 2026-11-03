// P0-3 step 3: apply analysis/results/StarFleetOP_2552_sdklib_matches.tsv (address, size, mangled
// name; byte-exact matches of SDK .lib COMDATs, relocations masked).
// Args: <matches tsv> <out log tsv>
//
// - Duplicate rows are collapsed; an address with several names is identical-code folding:
//   the shortest mangled name becomes primary, the others stay as labels (tag "sdklib-exact-icf").
// - A match starting inside the previous match is skipped and logged (one case: a 16-byte
//   ?mDispose@tBasicFile@@ hit at 0062bf4f inside ?mRelease@tINIFileSimple@... at 0062bf40).
// - Missing functions are created with the body [address, address+size); a function that wrongly
//   swallowed the range is trimmed first.
// - Names are USER_DEFINED (highest confidence) and demangled; tag "sdklib-exact".
// - A function previously named by SDK FID with a name not in the TSV set is logged as a conflict;
//   the exact match wins.
// - Rows whose "name" is an object section id (no function symbol in the .lib) do not rename the
//   function; they add tag "sdklib-exact-section" and a comment with the section id.
//@category SFCOP
import java.io.*;
import java.nio.file.Files;
import java.util.*;

import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.cmd.label.DemanglerCmd;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.*;

public class SfcApplyExact extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		TreeMap<Long, TreeSet<String>> names = new TreeMap<>();
		Map<Long, Long> sizes = new HashMap<>();
		int rows = 0;
		for (String line : Files.readAllLines(new File(args[0]).toPath())) {
			String[] c = line.split("\t");
			if (c.length < 3) {
				continue;
			}
			rows++;
			long a = Long.parseLong(c[0], 16);
			names.computeIfAbsent(a, k -> new TreeSet<>(
				Comparator.comparingInt(String::length).thenComparing(Comparator.naturalOrder())))
					.add(c[2]);
			sizes.put(a, Long.parseLong(c[1]));
		}

		AddressSpace space = currentProgram.getAddressFactory().getDefaultAddressSpace();
		SymbolTable st = currentProgram.getSymbolTable();
		int created = 0, trimmed = 0, failed = 0, renamed = 0, fidAgree = 0, fidConflict = 0,
				icf = 0, demangleFail = 0, labels = 0, anon = 0, overlaps = 0;
		long prevEnd = 0;
		try (PrintWriter out = new PrintWriter(new FileWriter(args[1]))) {
			out.println("address\tevent\tdetail");
			for (Map.Entry<Long, TreeSet<String>> e : names.entrySet()) {
				monitor.checkCancelled();
				Address addr = space.getAddress(e.getKey());
				long size = sizes.get(e.getKey());
				if (e.getKey() < prevEnd) {
					// two byte-exact matches cannot both be functions; keep the first
					overlaps++;
					out.println(addr + "\toverlaps-previous-match\t" + String.join("; ", e.getValue()));
					continue;
				}
				prevEnd = e.getKey() + size;
				AddressSet body = new AddressSet(addr, addr.add(size - 1));
				Function f = getFunctionAt(addr);
				if (f == null) {
					Function outer = getFunctionContaining(addr);
					if (outer != null) {
						AddressSetView nb = outer.getBody().subtract(body);
						if (!nb.isEmpty() && nb.contains(outer.getEntryPoint())) {
							outer.setBody(nb);
							trimmed++;
							out.println(addr + "\ttrimmed\t" + outer.getEntryPoint() + " " +
								outer.getName());
						}
					}
					if (getInstructionAt(addr) == null) {
						disassemble(addr);
					}
					CreateFunctionCmd cmd = new CreateFunctionCmd(null, addr, body,
						SourceType.DEFAULT, false, true);
					if (!cmd.applyTo(currentProgram, monitor) || getFunctionAt(addr) == null) {
						cmd = new CreateFunctionCmd(addr);
						cmd.applyTo(currentProgram, monitor);
					}
					f = getFunctionAt(addr);
					if (f == null) {
						failed++;
						out.println(addr + "\tcreate-failed\t" + cmd.getStatusMsg());
						continue;
					}
					created++;
					out.println(addr + "\tcreated\t" + size);
				}
				// rows naming an object section (".\\x Release\\y.obj:.text#12") carry provenance
				// only: the matcher found no function symbol for that COMDAT
				TreeSet<String> set = new TreeSet<>(e.getValue().comparator());
				List<String> sections = new ArrayList<>();
				for (String n : e.getValue()) {
					if (n.contains(".obj:") || n.chars().anyMatch(Character::isWhitespace)) {
						sections.add(n);
					}
					else {
						set.add(n);
					}
				}
				if (set.isEmpty()) {
					anon++;
					f.addTag("sdklib-exact-section");
					String c = f.getComment();
					f.setComment((c == null ? "" : c + "\n") + "SDK byte-exact match: " +
						String.join("; ", sections));
					out.println(addr + "	section-only	" + f.getName(true) + " <- " +
						String.join("; ", sections));
					continue;
				}
				String prev = f.getName(true);
				SourceType prevSrc = f.getSymbol().getSource();
				if (f.getTags().stream().anyMatch(t -> t.getName().equals("sdk-fid"))) {
					boolean agree = false;
					for (Symbol s : st.getSymbols(addr)) {
						agree |= set.contains(s.getName());
					}
					if (agree) {
						fidAgree++;
					}
					else {
						fidConflict++;
						out.println(addr + "\tfid-conflict\t" + prev + " -> " + set.first());
					}
				}
				else if (prevSrc != SourceType.DEFAULT) {
					renamed++;
					out.println(addr + "\treplaced\t" + prevSrc + " " + prev);
				}
				// demangled primary name on the function, every mangled name kept as a label
				String primary = set.first();
				DemanglerCmd dc = new DemanglerCmd(addr, primary);
				if (!dc.applyTo(currentProgram, monitor)) {
					demangleFail++;
					out.println(addr + "\tdemangle-failed\t" + primary + " " + dc.getStatusMsg());
					Symbol old = st.getGlobalSymbol(primary, addr);
					if (old != null && !old.equals(f.getSymbol())) {
						old.delete();
					}
					f.setParentNamespace(currentProgram.getGlobalNamespace());
					f.setName(primary, SourceType.USER_DEFINED);
				}
				f = getFunctionAt(addr);
				for (String n : set) {
					if (st.getGlobalSymbol(n, addr) == null && !n.equals(f.getName())) {
						st.createLabel(addr, n, SourceType.USER_DEFINED);
						labels++;
					}
				}
				Symbol fs = f.getSymbol();
				if (fs.getSource() != SourceType.USER_DEFINED) {
					fs.setNameAndNamespace(fs.getName(), fs.getParentNamespace(),
						SourceType.USER_DEFINED);
				}
				f.addTag("sdklib-exact");
				if (set.size() > 1) {
					icf++;
					f.addTag("sdklib-exact-icf");
				}
			}
			out.printf("#summary\trows=%d addresses=%d names=%d created=%d trimmed=%d " +
				"create_failed=%d replaced_other_names=%d fid_agree=%d fid_conflict=%d " +
				"icf_addresses=%d labels_added=%d demangle_failed=%d section_only=%d " +
				"skipped_overlap=%d%n", rows, names.size(),
				names.values().stream().mapToInt(Set::size).sum(), created, trimmed, failed, renamed,
				fidAgree, fidConflict, icf, labels, demangleFail, anon, overlaps);
		}
		println(String.format("exact: addresses=%d created=%d failed=%d fid_agree=%d " +
			"fid_conflict=%d icf=%d demangle_failed=%d", names.size(), created, failed, fidAgree,
			fidConflict, icf, demangleFail));
	}
}
