// P0-3 step 5: CSV exports. Arg 0: export directory.
//   functions.csv            one row per function (name, provenance, TU, library flag)
//   classes.csv              RTTI classes: parents from the RTTI base-class arrays, vftables, size
//   vtables.csv              one row per vftable slot
//   strings_to_functions.csv every defined string referenced from a function (direct or 1 hop)
//   unnamed_regions.csv      runs of consecutive functions that still have default names
//@category SFCOP
import java.io.*;
import java.util.*;
import java.util.regex.Pattern;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.*;

public class SfcExport extends GhidraScript {
	static final Pattern GAMESPY =
		Pattern.compile("(?i).*(gamespy|\\\\gs[a-z]*\\\\|goaceng|\\bqr2?_|gstats|\\bgpi?[A-Z]).*");
	static final Pattern WON = Pattern.compile("^(WONAPI|WONMsg|WONCrypt|WONAuth|WONCommon|WONMisc)::.*");

	Memory mem;
	SymbolTable st;
	BookmarkManager bm;

	@Override
	protected void run() throws Exception {
		File dir = new File(getScriptArgs()[0]);
		dir.mkdirs();
		mem = currentProgram.getMemory();
		st = currentProgram.getSymbolTable();
		bm = currentProgram.getBookmarkManager();
		exportFunctions(new File(dir, "functions.csv"), new File(dir, "unnamed_regions.csv"));
		exportClasses(new File(dir, "classes.csv"), new File(dir, "vtables.csv"));
		exportStrings(new File(dir, "strings_to_functions.csv"));
	}

	// ---------------------------------------------------------------- functions

	private void exportFunctions(File file, File regionsFile) throws Exception {
		List<Function> funcs = new ArrayList<>();
		currentProgram.getFunctionManager().getFunctions(true).forEach(funcs::add);
		// direct TU per function (exactly one .c/.cpp referenced) for neighbour inference
		String[] direct = new String[funcs.size()];
		boolean[] anyTU = new boolean[funcs.size()];
		for (int i = 0; i < funcs.size(); i++) {
			List<String> tus = tagValues(funcs.get(i), "TU:");
			direct[i] = tus.size() == 1 ? tus.get(0) : null;
			anyTU[i] = !tus.isEmpty();
		}
		String[] neighbour = neighbourTUs(direct, anyTU);
		Map<String, Integer> sourceCounts = new TreeMap<>();
		Map<String, Integer> libCounts = new TreeMap<>();
		int withTU = 0, withTUneighbor = 0;
		List<long[]> regions = new ArrayList<>(); // start, end, count, bytes
		long[] cur = null;
		try (PrintWriter out = new PrintWriter(new FileWriter(file))) {
			out.println("address,size,name,mangled,demangled,name_source,translation_unit," +
				"tu_evidence,headers,is_library,library,kind,tags");
			for (int i = 0; i < funcs.size(); i++) {
				monitor.checkCancelled();
				Function f = funcs.get(i);
				Address a = f.getEntryPoint();
				long size = f.getBody().getNumAddresses();
				String name = f.getName(true);
				String mangled = "";
				for (Symbol s : st.getSymbols(a)) {
					if (s.getName().startsWith("?")) {
						mangled = s.getName();
						break;
					}
				}
				String kind = f.isThunk() ? "thunk"
						: (f.getName().startsWith("Unwind@") || f.getName().startsWith("Catch@"))
								? "eh-funclet"
								: "function";
				String src = nameSource(f, kind);
				List<String> tus = tagValues(f, "TU:");
				String tu = String.join(";", tus);
				String evidence = tus.isEmpty() ? "" : "assert-path";
				if (tus.isEmpty() && !kind.equals("eh-funclet")) {
					String n = neighbour[i];
					if (n != null) {
						tu = n;
						evidence = "neighbour";
						withTUneighbor++;
					}
				}
				else if (!tus.isEmpty()) {
					withTU++;
				}
				String lib = library(f, name, mangled, src, tu);
				String demangled = src.equals("default") ? ""
						: f.getSignature().getPrototypeString(true);
				List<String> tags = new ArrayList<>();
				for (FunctionTag t : f.getTags()) {
					if (!t.getName().startsWith("TU:") && !t.getName().startsWith("HDR:")) {
						tags.add(t.getName());
					}
				}
				Collections.sort(tags);
				out.println(csv(a.toString(), Long.toString(size), name, mangled, demangled, src, tu,
					evidence, String.join(";", tagValues(f, "HDR:")), lib.isEmpty() ? "0" : "1",
					lib, kind, String.join(";", tags)));
				sourceCounts.merge(kind + "/" + src, 1, Integer::sum);
				libCounts.merge(lib.isEmpty() ? "(game)" : lib, 1, Integer::sum);

				boolean unnamed = src.equals("default") && kind.equals("function");
				if (unnamed) {
					if (cur == null) {
						cur = new long[] { a.getOffset(), 0, 0, 0 };
					}
					cur[1] = f.getBody().getMaxAddress().getOffset();
					cur[2]++;
					cur[3] += size;
				}
				else if (!kind.equals("eh-funclet") && cur != null) {
					regions.add(cur);
					cur = null;
				}
			}
		}
		if (cur != null) {
			regions.add(cur);
		}
		regions.sort((x, y) -> Long.compare(y[3], x[3]));
		try (PrintWriter out = new PrintWriter(new FileWriter(regionsFile))) {
			out.println("start,end,functions,bytes,named_before,named_after");
			for (long[] r : regions) {
				out.println(csv(String.format("%08x", r[0]), String.format("%08x", r[1]),
					Long.toString(r[2]), Long.toString(r[3]), namedNeighbour(r[0], false),
					namedNeighbour(r[1], true)));
			}
		}
		println("functions by kind/name_source: " + sourceCounts);
		println("functions by library: " + libCounts);
		println("functions with TU: assert-path=" + withTU + " neighbour=" + withTUneighbor);
	}

	private String nameSource(Function f, String kind) {
		Set<String> tags = new HashSet<>();
		f.getTags().forEach(t -> tags.add(t.getName()));
		if (tags.contains("sdklib-exact")) {
			return "sdklib-exact";
		}
		if (tags.contains("sdk-fid")) {
			return "sdk-fid";
		}
		SourceType s = f.getSymbol().getSource();
		if (s == SourceType.DEFAULT) {
			return "default";
		}
		if (s == SourceType.IMPORTED) {
			return "imported";
		}
		if (kind.equals("eh-funclet")) {
			return "eh-analyzer";
		}
		Address a = f.getEntryPoint();
		for (Bookmark b : bm.getBookmarks(a)) {
			if (b.getCategory().equals("Function ID Analyzer")) {
				return "ghidra-fid-vs";
			}
		}
		if (f.getParentNamespace() instanceof GhidraClass) {
			return "rtti-class-recovery";
		}
		return s == SourceType.USER_DEFINED ? "user" : "ghidra-analysis";
	}

	private String library(Function f, String name, String mangled, String src, String tu) {
		if (f.isThunk() && f.getThunkedFunction(true) != null &&
			f.getThunkedFunction(true).isExternal()) {
			return "import-thunk";
		}
		if (name.startsWith("std::") || mangled.contains("@std@@")) {
			return "stl";
		}
		if (name.startsWith("CryptoPP::") || mangled.contains("@CryptoPP@@") ||
			tu.toLowerCase().contains("crypto")) {
			return "cryptopp";
		}
		if (WON.matcher(name).matches() || mangled.matches(".*@WON(API|Msg|Crypt|Auth|Common|Misc)@@.*")) {
			return "wonapi";
		}
		if (GAMESPY.matcher(tu).matches() || GAMESPY.matcher(name).matches()) {
			return "gamespy";
		}
		if (src.equals("ghidra-fid-vs")) {
			return "crt";
		}
		return "";
	}

	/** TU of the nearest TU-tagged functions before and after, if both have the same single TU. */
	private static String[] neighbourTUs(String[] direct, boolean[] anyTU) {
		int n = direct.length;
		String[] res = new String[n];
		int[] prev = new int[n];
		int last = -1;
		for (int i = 0; i < n; i++) {
			prev[i] = last;
			if (anyTU[i]) {
				last = i;
			}
		}
		last = -1;
		for (int i = n - 1; i >= 0; i--) {
			if (!anyTU[i] && prev[i] >= 0 && last >= 0 && direct[prev[i]] != null &&
				direct[prev[i]].equals(direct[last])) {
				res[i] = direct[last];
			}
			if (anyTU[i]) {
				last = i;
			}
		}
		return res;
	}

	private String namedNeighbour(long off, boolean forward) {
		Address a = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(off);
		FunctionIterator it = currentProgram.getFunctionManager().getFunctions(a, forward);
		while (it.hasNext()) {
			Function f = it.next();
			if (f.getSymbol().getSource() != SourceType.DEFAULT &&
				!f.getName().startsWith("Unwind@") && !f.getName().startsWith("Catch@")) {
				return f.getEntryPoint() + " " + f.getName(true);
			}
		}
		return "";
	}

	private static List<String> tagValues(Function f, String prefix) {
		List<String> v = new ArrayList<>();
		for (FunctionTag t : f.getTags()) {
			if (t.getName().startsWith(prefix)) {
				v.add(t.getName().substring(prefix.length()));
			}
		}
		Collections.sort(v);
		return v;
	}

	// ---------------------------------------------------------------- classes, vtables

	private void exportClasses(File classFile, File vtFile) throws Exception {
		// class structure sizes from RecoverClassesFromRTTIScript (/ClassDataTypes/...)
		Map<String, Integer> sizes = new HashMap<>();
		Iterator<Structure> si = currentProgram.getDataTypeManager().getAllStructures();
		while (si.hasNext()) {
			Structure s = si.next();
			String cat = s.getCategoryPath().getPath();
			if (cat.startsWith("/ClassDataTypes/")) {
				String ns = cat.substring("/ClassDataTypes/".length()).replace("/", "::");
				if (ns.endsWith(s.getName())) {
					sizes.put(ns, s.getLength());
				}
			}
		}
		// type descriptor address -> class name
		Map<Address, String> tdName = new HashMap<>();
		for (Symbol s : st.getSymbols("RTTI_Type_Descriptor")) {
			tdName.put(s.getAddress(), s.getParentNamespace().getName(true));
		}
		int classes = 0, vtables = 0, slots = 0;
		try (PrintWriter cOut = new PrintWriter(new FileWriter(classFile));
				PrintWriter vOut = new PrintWriter(new FileWriter(vtFile))) {
			cOut.println("class,parents,all_bases,vtables,size,size_source,type_descriptor," +
				"is_library");
			vOut.println("vtable_address,class,vtable_name,slot,function_address,function");
			TreeMap<String, List<Symbol>> vftByClass = new TreeMap<>();
			for (Symbol s : st.getAllSymbols(true)) {
				if (s.getName().startsWith("vftable") && !s.getName().contains("meta_ptr")) {
					vftByClass.computeIfAbsent(s.getParentNamespace().getName(true),
						k -> new ArrayList<>()).add(s);
				}
			}
			TreeMap<String, Address> chds = new TreeMap<>();
			for (Symbol s : st.getSymbols("RTTI_Class_Hierarchy_Descriptor")) {
				chds.put(s.getParentNamespace().getName(true), s.getAddress());
			}
			// every RTTI type: polymorphic ones (hierarchy descriptor, vftables) and the rest
			// (type descriptor only, e.g. exception and typeid-only types)
			Set<String> all = new TreeSet<>(chds.keySet());
			all.addAll(vftByClass.keySet());
			all.addAll(tdName.values());
			all.remove("Global");
			for (String cls : all) {
				List<String> parents = new ArrayList<>(), bases = new ArrayList<>();
				Address chd = chds.get(cls);
				String td = "";
				if (chd != null) {
					readHierarchy(chd, tdName, parents, bases);
				}
				for (Symbol s : st.getSymbols("RTTI_Type_Descriptor")) {
					if (s.getParentNamespace().getName(true).equals(cls)) {
						td = s.getAddress().toString();
					}
				}
				List<String> vts = new ArrayList<>();
				for (Symbol v : vftByClass.getOrDefault(cls, List.of())) {
					vts.add(v.getAddress().toString());
					vtables++;
					int n = 0;
					for (Address fa : vtableSlots(v.getAddress())) {
						Function f = getFunctionAt(fa);
						vOut.println(csv(v.getAddress().toString(), cls, v.getName(),
							Integer.toString(n++), fa.toString(), f == null ? "" : f.getName(true)));
						slots++;
					}
				}
				Integer size = sizes.get(cls);
				String lib = cls.startsWith("std::") ? "stl"
						: cls.startsWith("CryptoPP::") ? "cryptopp"
								: WON.matcher(cls + "::").matches() ? "wonapi" : "";
				cOut.println(csv(cls, String.join(";", parents), String.join(";", bases),
					String.join(";", vts), size == null ? "" : size.toString(),
					size == null ? "" : "class-recovery-estimate", td, lib.isEmpty() ? "0" : "1"));
				classes++;
			}
		}
		println("classes.csv rows=" + classes + " vtables=" + vtables + " slots=" + slots);
	}

	/** MSVC RTTI: CHD -> base class array; element 0 is the class itself. */
	private void readHierarchy(Address chd, Map<Address, String> tdName, List<String> parents,
			List<String> bases) {
		try {
			int n = mem.getInt(chd.add(8));
			Address arr = toAddr(mem.getInt(chd.add(12)) & 0xffffffffL);
			if (n <= 0 || n > 256) {
				return;
			}
			String[] names = new String[n];
			int[] contained = new int[n];
			for (int i = 0; i < n; i++) {
				Address bcd = toAddr(mem.getInt(arr.add(4L * i)) & 0xffffffffL);
				Address td = toAddr(mem.getInt(bcd) & 0xffffffffL);
				contained[i] = mem.getInt(bcd.add(4));
				names[i] = tdName.getOrDefault(td, "td@" + td);
			}
			for (int i = 1; i < n; i++) {
				bases.add(names[i]);
			}
			for (int i = 1; i < n; i += 1 + Math.max(0, contained[i])) {
				parents.add(names[i]);
			}
		}
		catch (Exception e) {
			parents.add("?error:" + e.getMessage());
		}
	}

	/** Slots: the defined vftable data if the RTTI analyzer sized it, else pointers into code. */
	private List<Address> vtableSlots(Address vt) throws Exception {
		List<Address> out = new ArrayList<>();
		Data d = getDataAt(vt);
		int max = d != null && d.getLength() >= 4 ? d.getLength() / 4 : 1024;
		for (int i = 0; i < max; i++) {
			Address p = vt.add(4L * i);
			if (i > 0 && (d == null || d.getLength() < 4) && st.getPrimarySymbol(p) != null) {
				break;
			}
			long v = mem.getInt(p) & 0xffffffffL;
			Address t = toAddr(v);
			if (mem.getBlock(t) == null || !mem.getBlock(t).isExecute()) {
				break;
			}
			out.add(t);
		}
		return out;
	}

	// ---------------------------------------------------------------- strings

	private void exportStrings(File file) throws Exception {
		ReferenceManager rm = currentProgram.getReferenceManager();
		Listing listing = currentProgram.getListing();
		int rows = 0;
		try (PrintWriter out = new PrintWriter(new FileWriter(file))) {
			out.println("string_address,function_address,function,via,string");
			for (Data d : listing.getDefinedData(true)) {
				if (!d.hasStringValue() || !(d.getValue() instanceof String s) || s.length() < 4) {
					continue;
				}
				TreeMap<Address, String> fns = new TreeMap<>();
				for (Reference r : rm.getReferencesTo(d.getAddress())) {
					addRef(fns, r.getFromAddress(), "direct");
					Data p = listing.getDataContaining(r.getFromAddress());
					if (p != null && p.isPointer()) {
						for (Reference r2 : rm.getReferencesTo(p.getAddress())) {
							addRef(fns, r2.getFromAddress(), "ptr@" + p.getAddress());
						}
					}
				}
				for (Map.Entry<Address, String> e : fns.entrySet()) {
					Function f = getFunctionAt(e.getKey());
					out.println(csv(d.getAddress().toString(), e.getKey().toString(),
						f.getName(true), e.getValue(), s));
					rows++;
				}
			}
		}
		println("strings_to_functions rows=" + rows);
	}

	private void addRef(Map<Address, String> fns, Address from, String via) {
		Function f = getFunctionContaining(from);
		if (f != null) {
			fns.putIfAbsent(f.getEntryPoint(), via);
		}
	}

	private static String csv(String... cols) {
		StringBuilder sb = new StringBuilder();
		for (int i = 0; i < cols.length; i++) {
			if (i > 0) {
				sb.append(',');
			}
			String c = cols[i] == null ? "" : cols[i];
			if (c.indexOf(',') >= 0 || c.indexOf('"') >= 0 || c.indexOf('\n') >= 0 ||
				c.indexOf('\r') >= 0) {
				sb.append('"').append(c.replace("\"", "\"\"")).append('"');
			}
			else {
				sb.append(c);
			}
		}
		return sb.toString();
	}
}
