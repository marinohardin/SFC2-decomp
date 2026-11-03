// P0-3 step 4: translation-unit mapping from source-path strings (__FILE__ in assertions/logging).
// Args: <out tsv>
//
// First adds missing operand references from instructions to defined strings (see
// addScalarReferences). Then finds every string that contains a backslash and ends in a C/C++ source extension (defining
// string data where analysis left raw bytes), follows references to it (directly from code, or one
// hop through a data pointer) to the referencing functions, and tags each function with
// "TU:<path>" for .c/.cpp files and "HDR:<path>" for headers. The tsv lists every
// (string, referencing function) pair.
//@category SFCOP
import java.io.*;
import java.util.*;
import java.util.regex.Pattern;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.StringDataType;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.*;

public class SfcMapTU extends GhidraScript {
	static final Pattern PATH =
		Pattern.compile("(?i)^[ -~]*\\\\[ -~]*\\.(cpp|cxx|cc|c|h|hpp|hxx|inl)$");
	static final Pattern SOURCE = Pattern.compile("(?i).*\\.(cpp|cxx|cc|c)$");

	@Override
	protected void run() throws Exception {
		File outFile = new File(getScriptArgs()[0]);
		Map<Address, String> paths = findPathStrings();
		ReferenceManager rm = currentProgram.getReferenceManager();
		Listing listing = currentProgram.getListing();
		Set<Address> strings = new HashSet<>(paths.keySet());
		for (Data d : listing.getDefinedData(true)) {
			if (d.hasStringValue()) {
				strings.add(d.getAddress());
			}
		}
		int added = addScalarReferences(strings);
		Set<Function> tuFunctions = new HashSet<>();
		Set<String> referenced = new HashSet<>();
		int pairs = 0;
		try (PrintWriter out = new PrintWriter(new FileWriter(outFile))) {
			out.println("string_address\tpath\tkind\tvia\tref_address\tfunction_address\tfunction");
			for (Map.Entry<Address, String> e : paths.entrySet()) {
				String path = e.getValue();
				boolean isSource = SOURCE.matcher(path).matches();
				List<Address[]> refs = new ArrayList<>(); // {from, via}
				for (Reference r : rm.getReferencesTo(e.getKey())) {
					refs.add(new Address[] { r.getFromAddress(), null });
					Data d = listing.getDataContaining(r.getFromAddress());
					if (d != null && d.isPointer()) {
						for (Reference r2 : rm.getReferencesTo(d.getAddress())) {
							refs.add(new Address[] { r2.getFromAddress(), d.getAddress() });
						}
					}
				}
				for (Address[] ref : refs) {
					Function f = getFunctionContaining(ref[0]);
					if (f == null) {
						continue;
					}
					pairs++;
					referenced.add(path);
					f.addTag((isSource ? "TU:" : "HDR:") + path);
					if (isSource) {
						tuFunctions.add(f);
					}
					out.println(e.getKey() + "\t" + path + "\t" + (isSource ? "source" : "header") +
						"\t" + (ref[1] == null ? "direct" : "ptr@" + ref[1]) + "\t" + ref[0] +
						"\t" + f.getEntryPoint() + "\t" + f.getName(true));
				}
			}
		}
		long multi = tuFunctions.stream()
				.filter(f -> f.getTags().stream().filter(t -> t.getName().startsWith("TU:")).count() > 1)
				.count();
		println(String.format(
			"TU: path strings=%d referenced=%d pairs=%d functions_with_TU=%d (of which >1 TU: %d) " +
				"operand_refs_added=%d",
			paths.size(), referenced.size(), pairs, tuFunctions.size(), multi, added));
	}

	/**
	 * Analysis ran before many strings were defined, so "push offset str" instructions often carry
	 * no reference. Add a DATA reference for every scalar operand equal to a defined string's
	 * address (all strings, so strings_to_functions.csv benefits too).
	 */
	private int addScalarReferences(Set<Address> targets) throws Exception {
		Set<Long> offsets = new HashSet<>();
		targets.forEach(a -> offsets.add(a.getOffset()));
		ReferenceManager rm = currentProgram.getReferenceManager();
		int added = 0;
		for (Instruction ins : currentProgram.getListing().getInstructions(true)) {
			monitor.checkCancelled();
			for (int op = 0; op < ins.getNumOperands(); op++) {
				for (Object o : ins.getOpObjects(op)) {
					if (o instanceof Scalar sc && offsets.contains(sc.getUnsignedValue())) {
						Address to = toAddr(sc.getUnsignedValue());
						boolean has = false;
						for (Reference r : rm.getReferencesFrom(ins.getAddress(), op)) {
							has |= r.getToAddress().equals(to);
						}
						if (!has) {
							ins.addOperandReference(op, to, RefType.DATA, SourceType.ANALYSIS);
							added++;
						}
					}
				}
			}
		}
		return added;
	}

	/** Defined strings plus raw NUL-terminated runs in initialised non-code blocks. */
	private Map<Address, String> findPathStrings() throws Exception {
		TreeMap<Address, String> found = new TreeMap<>();
		for (Data d : currentProgram.getListing().getDefinedData(true)) {
			if (d.hasStringValue() && d.getValue() instanceof String s && PATH.matcher(s).matches()) {
				found.put(d.getAddress(), s);
			}
		}
		for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
			if (!b.isInitialized() || b.isExecute()) {
				continue;
			}
			byte[] bytes = new byte[(int) b.getSize()];
			b.getBytes(b.getStart(), bytes);
			int start = 0;
			for (int i = 0; i < bytes.length; i++) {
				int c = bytes[i] & 0xff;
				if (c >= 0x20 && c < 0x7f) {
					continue;
				}
				if (c == 0 && i - start >= 6) {
					String s = new String(bytes, start, i - start, "US-ASCII");
					Address a = b.getStart().add(start);
					if (PATH.matcher(s).matches() && !found.containsKey(a) &&
						currentProgram.getListing().getDefinedDataContaining(a) == null) {
						try {
							createData(a, new StringDataType());
							found.put(a, s);
						}
						catch (Exception ex) {
							println("could not define string at " + a + ": " + ex.getMessage());
						}
					}
				}
				start = i + 1;
			}
		}
		return found;
	}
}
