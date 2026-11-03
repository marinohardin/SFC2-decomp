// P0-3 step 1b (part 1): MSVC 6 RTTI markup that Ghidra 12.2's RTTI analyzer misses.
// Arg 0: out tsv (one row per vftable).
//
// The analyzer finds the 2,105 type descriptors (RTTI0) but creates no complete object locators,
// hierarchy descriptors or vftables: VC6 base class descriptors are 0x18 bytes (no
// pClassDescriptor field, added in VC7), so its RTTI1/RTTI4 validation fails. This script
//   - defines VC6-layout structures under the names RecoverClassesFromRTTIScript looks up
//     (RTTICompleteObjectLocator 0x14, RTTIClassHierarchyDescriptor 0x10,
//     RTTIBaseClassDescriptor 0x18) in the root category;
//   - finds every COL in .rdata (signature 0, pTypeDescriptor = known RTTI0, CHD/BCA consistent,
//     BCA[0] describes the same type) and labels COL/CHD/BCA/BCDs in the class namespace;
//   - finds every vftable (dword pointing at a COL = vftable_meta_ptr, vftable follows), sizes it
//     (pointers into executable memory, stopping at the next referenced address or meta pointer),
//     defines it as a pointer array and creates missing functions at its slots.
//@category SFCOP
import java.io.*;
import java.util.*;

import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.symbol.*;

public class SfcVc6Rtti extends GhidraScript {
	Memory mem;
	SymbolTable st;
	Map<Long, Namespace> tdNs = new HashMap<>();
	DataType colDt, chdDt, bcdDt;
	PointerDataType ptr;

	@Override
	protected void run() throws Exception {
		mem = currentProgram.getMemory();
		st = currentProgram.getSymbolTable();
		ptr = new PointerDataType(currentProgram.getDataTypeManager());
		for (Symbol s : st.getSymbols("RTTI_Type_Descriptor")) {
			tdNs.put(s.getAddress().getOffset(), s.getParentNamespace());
		}
		createTypes();
		MemoryBlock rdata = mem.getBlock(".rdata");
		long lo = rdata.getStart().getOffset(), hi = rdata.getEnd().getOffset();

		// 1. complete object locators
		TreeMap<Long, Namespace> cols = new TreeMap<>();
		Set<Long> chdsDone = new HashSet<>();
		int bcds = 0;
		for (long a = lo; a + 0x14 <= hi; a += 4) {
			monitor.checkCancelled();
			if (u32(a) != 0) {
				continue;
			}
			long td = u32(a + 12), chd = u32(a + 16);
			Namespace ns = tdNs.get(td);
			if (ns == null || chd < lo || chd + 16 > hi || !validChd(chd, td, lo, hi)) {
				continue;
			}
			cols.put(a, ns);
			put(a, colDt, "RTTI_Complete_Object_Locator", ns);
			if (chdsDone.add(chd)) {
				int n = (int) u32(chd + 8);
				long bca = u32(chd + 12);
				put(chd, chdDt, "RTTI_Class_Hierarchy_Descriptor", ns);
				put(bca, new ArrayDataType(ptr, n, 4), "RTTI_Base_Class_Array", ns);
				for (int i = 0; i < n; i++) {
					long bcd = u32(bca + 4L * i);
					Namespace bns = tdNs.getOrDefault(u32(bcd), ns);
					String label = String.format("RTTI_Base_Class_Descriptor_at_(%d,%d,%d,%d)",
						(int) u32(bcd + 8), (int) u32(bcd + 12), (int) u32(bcd + 16),
						(int) u32(bcd + 20));
					if (getDataAt(addr(bcd)) == null ||
						!getDataAt(addr(bcd)).getDataType().getName().equals(bcdDt.getName())) {
						put(bcd, bcdDt, label, bns);
						bcds++;
					}
				}
			}
		}

		// 2. vftables: meta pointer -> COL, table follows
		ReferenceManager rm = currentProgram.getReferenceManager();
		int vftables = 0, slots = 0, newFuncs = 0;
		try (PrintWriter out = new PrintWriter(new FileWriter(getScriptArgs()[0]))) {
			out.println("meta_ptr\tvftable\tclass\tcol_offset\tslots");
			for (long p = lo; p + 8 <= hi; p += 4) {
				monitor.checkCancelled();
				Namespace ns = cols.get(u32(p));
				if (ns == null) {
					continue;
				}
				long vt = p + 4;
				int n = 0;
				while (vt + 4L * (n + 1) <= hi + 1) {
					long q = vt + 4L * n;
					if (n > 0 && (cols.containsKey(u32(q)) || rm.hasReferencesTo(addr(q)))) {
						break;
					}
					MemoryBlock b = mem.getBlock(addr(u32(q)));
					if (b == null || !b.isExecute()) {
						break;
					}
					n++;
				}
				if (n == 0) {
					continue;
				}
				put(p, ptr, "vftable_meta_ptr", ns);
				put(vt, new ArrayDataType(ptr, n, 4), "vftable", ns);
				vftables++;
				slots += n;
				for (int i = 0; i < n; i++) {
					Address target = addr(u32(vt + 4L * i));
					if (getFunctionAt(target) == null && getFunctionContaining(target) == null) {
						if (getInstructionAt(target) == null) {
							disassemble(target);
						}
						if (new CreateFunctionCmd(target).applyTo(currentProgram, monitor)) {
							newFuncs++;
						}
					}
				}
				out.println(addr(p) + "\t" + addr(vt) + "\t" + ns.getName(true) + "\t" +
					(int) u32(u32(p) + 4) + "\t" + n);
			}
		}
		println(String.format("VC6 RTTI: COLs=%d CHDs=%d BCDs=%d vftables=%d slots=%d " +
			"functions_created=%d", cols.size(), chdsDone.size(), bcds, vftables, slots, newFuncs));
	}

	private boolean validChd(long chd, long td, long lo, long hi) throws Exception {
		if (u32(chd) != 0) {
			return false;
		}
		long n = u32(chd + 8), bca = u32(chd + 12);
		if (n < 1 || n > 64 || bca < lo || bca + 4 * n > hi) {
			return false;
		}
		for (int i = 0; i < n; i++) {
			long bcd = u32(bca + 4L * i);
			if (bcd < lo || bcd + 0x18 > hi || !tdNs.containsKey(u32(bcd))) {
				return false;
			}
		}
		return u32(u32(bca)) == td;
	}

	private void createTypes() {
		DataTypeManager dtm = currentProgram.getDataTypeManager();
		StructureDataType col = new StructureDataType(CategoryPath.ROOT, "RTTICompleteObjectLocator", 0, dtm);
		col.add(DWordDataType.dataType, "signature", null);
		col.add(DWordDataType.dataType, "offset", "offset of this vftable in the class");
		col.add(DWordDataType.dataType, "cdOffset", "constructor displacement offset");
		col.add(ptr, "pTypeDescriptor", null);
		col.add(ptr, "pClassDescriptor", null);
		StructureDataType chd = new StructureDataType(CategoryPath.ROOT, "RTTIClassHierarchyDescriptor", 0, dtm);
		chd.add(DWordDataType.dataType, "signature", null);
		chd.add(DWordDataType.dataType, "attributes", "1 multiple inheritance, 2 virtual inheritance");
		chd.add(DWordDataType.dataType, "numBaseClasses", null);
		chd.add(ptr, "pBaseClassArray", null);
		StructureDataType bcd = new StructureDataType(CategoryPath.ROOT, "RTTIBaseClassDescriptor", 0, dtm);
		bcd.add(ptr, "pTypeDescriptor", null);
		bcd.add(DWordDataType.dataType, "numContainedBases", null);
		bcd.add(IntegerDataType.dataType, "mdisp", null);
		bcd.add(IntegerDataType.dataType, "pdisp", null);
		bcd.add(IntegerDataType.dataType, "vdisp", null);
		bcd.add(DWordDataType.dataType, "attributes", "VC6 layout: no pClassDescriptor field");
		colDt = dtm.addDataType(col, DataTypeConflictHandler.REPLACE_HANDLER);
		chdDt = dtm.addDataType(chd, DataTypeConflictHandler.REPLACE_HANDLER);
		bcdDt = dtm.addDataType(bcd, DataTypeConflictHandler.REPLACE_HANDLER);
	}

	private void put(long a, DataType dt, String label, Namespace ns) throws Exception {
		Address at = addr(a);
		Data d = getDataAt(at);
		if (d == null || !d.getDataType().isEquivalent(dt)) {
			clearListing(at, at.add(dt.getLength() - 1));
			createData(at, dt);
		}
		for (Symbol s : st.getSymbols(at)) {
			if (s.getName().equals(label) && s.getParentNamespace().equals(ns)) {
				return;
			}
		}
		st.createLabel(at, label, ns, SourceType.ANALYSIS).setPrimary();
	}

	private long u32(long a) throws MemoryAccessException {
		return mem.getInt(addr(a)) & 0xffffffffL;
	}

	private Address addr(long a) {
		return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(a);
	}
}
