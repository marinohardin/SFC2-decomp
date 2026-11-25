#include "Common/BaseTypes.h"

// HYPOTHESIS: this is the tAIUnitData constructor (no header declares the class yet);
// only the members this function writes are declared.
class tAIUnitData
{
public:
	int field_0;
	int field_4;
	float field_8;
	float field_c;
	float field_10;
	float field_14;
	float field_18;
	int field_1c;
	float field_20;
	float field_24;
	int field_28;
	float field_2c;
	float field_30;
	char field_34;
	int field_38;
	int field_3c;

	tAIUnitData();
	tAIUnitData(int a, int b, int c, float d, float e, char f);
};

// HYPOTHESIS: the two scale factors are constants at fixed exe addresses.
extern float g_007b01e0;
extern float g_007b01e4;

// FUNCTION: STARFLEETOP 0x00520ab2
tAIUnitData::tAIUnitData(int a, int b, int c, float d, float e, char f)
{
	field_c = d;
	field_10 = d;
	field_14 = d;
	field_0 = a;
	field_4 = b;
	field_1c = 0;
	field_28 = 0;
	field_3c = -1;
	field_8 = e;
	field_20 = 0.0f;
	field_34 = f;
	field_24 = 0.0f;
	field_38 = c;
	field_18 = d * g_007b01e0;
	field_2c = d * g_007b01e4;
	field_30 = e / d;
}

// FUNCTION: STARFLEETOP 0x00520b1a
// HYPOTHESIS: the default constructor of tAIUnitData (same -1 sentinels as the other one).
tAIUnitData::tAIUnitData()
{
	field_0 = -1;
	field_4 = -1;
	field_c = 0;
	field_3c = -1;
	field_10 = 0;
	field_1c = 0;
	field_28 = 0;
	field_14 = 0;
	field_34 = 0;
	field_38 = 0;
	field_18 = 0;
	field_20 = 0;
	field_24 = 0;
	field_2c = 0;
	field_30 = 0;
}

// HYPOTHESIS: owner of the 0x32-entry tAIUnitData array at +8 (count at +0xc88); real class unknown.
class tAIUnitDataList
{
public:
	char pad_0[8];
	// HYPOTHESIS: the array and its count form one member at +8 (the count is addressed from the array base).
	struct tList
	{
		tAIUnitData field_0[0x32];
		int field_c80;

		tList();

		// MATCH-ONLY: inlined append helper; the original's name is unknown.
		void Append(const tAIUnitData& d)
		{
			if (field_c80 < 0x32) {
				field_0[field_c80] = d;
				field_c80++;
			}
		}
	} field_8;
	// HYPOTHESIS: per-index id lists (0x32 ids plus a count, 0xcc bytes each); entry count unknown.
	struct IdList
	{
		int field_0[0x32];
		int field_c8;
	} field_c8c[2];

	// HYPOTHESIS: per-index command lists (0x324 bytes each); entry count unknown.
	struct CommandEntry
	{
		int field_0;
		int field_4;
		int field_8;
		int field_c;
	};
	struct CommandList
	{
		CommandEntry field_0[0x32];
		int field_320;

		// MATCH-ONLY: inlined append helper; the original's name is unknown.
		void Append(const CommandEntry& e)
		{
			if (field_320 < 0x32) {
				field_0[field_320] = e;
				field_320++;
			}
		}
	} field_e24[2];

	int fn_00520cba(int a, int b, float c, char d);
	tAIUnitData* fn_00520d38(int a);
	void fn_00520d69(int a, int b);
	void fn_00520dad(int a, int b, int c, int d);
	void fn_00520e30();
	char fn_00520e5d();
};

// FUNCTION: STARFLEETOP 0x00520cba
int tAIUnitDataList::fn_00520cba(int a, int b, float c, char d)
{
	if (field_8.field_c80 < 0x32) {
		tAIUnitData data(field_8.field_c80, a, b, c, c, d);
		tAIUnitData* p = fn_00520d38(a);
		if (p == 0) {
			field_8.Append(data);
		} else {
			*p = data;
		}
		return data.field_0;
	}
	return -1;
}

// FUNCTION: STARFLEETOP 0x00520d38
tAIUnitData* tAIUnitDataList::fn_00520d38(int a)
{
	for (int i = 0; i < field_8.field_c80; i++) {
		if (field_8.field_0[i].field_4 == a) {
			return &field_8.field_0[i];
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00520d69
void tAIUnitDataList::fn_00520d69(int a, int b)
{
	tAIUnitData* p = fn_00520d38(a);
	if (p != 0 && p->field_0 != -1) {
		IdList* list = &field_c8c[b];
		if (list->field_c8 < 0x32) {
			list->field_0[list->field_c8] = p->field_0;
			list->field_c8++;
		}
	}
}

// FUNCTION: STARFLEETOP 0x00520dad
void tAIUnitDataList::fn_00520dad(int a, int b, int c, int d)
{
	for (int i = 0; i < field_e24[a].field_320; i++) {
		if (field_e24[a].field_0[i].field_4 == b) {
			return;
		}
	}
	tAIUnitData* p = fn_00520d38(b);
	if (p != 0 && p->field_0 != -1) {
		CommandEntry e;
		e.field_0 = p->field_0;
		e.field_8 = c;
		e.field_c = d;
		e.field_4 = b;
		field_e24[a].Append(e);
	}
}

// FUNCTION: STARFLEETOP 0x00520e30
void tAIUnitDataList::fn_00520e30()
{
	for (int i = 0; i < 2; i++) {
		field_c8c[i].field_c8 = 0;
		field_e24[i].field_320 = 0;
		field_8.field_c80 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00520e5d
char tAIUnitDataList::fn_00520e5d()
{
	char result = 1;
	for (int i = 0; i < 2; i++) {
		if (field_c8c[i].field_c8 == 0 || field_e24[i].field_320 == 0) {
			result = 0;
			break;
		}
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x0052238b
// HYPOTHESIS: the constructor of the array holder; it default-constructs the 0x32 entries and clears the count.
tAIUnitDataList::tList::tList()
{
	field_c80 = 0;
}

// HYPOTHESIS: the 0x38-byte list value; its one-argument constructor is at 0x005223dc.
struct fn_005223dc_value
{
	char field_0[0x38];
	fn_005223dc_value(int arg);
};

inline void* operator new(unsigned int, void* p) { return p; }
inline void operator delete(void*, void*) {}

// FUNCTION: STARFLEETOP 0x005223af
void fn_005223af(int* dest, int value)
{
	new (dest) fn_005223dc_value(value);
}

// HYPOTHESIS: copy constructor of a vector of 8-byte elements (allocator flag byte, first, last, end).
struct fn_00522441_elem
{
	int field_0;
	int field_4;
};

struct fn_00522441_vector
{
	char field_0;
	fn_00522441_elem* field_4;
	fn_00522441_elem* field_8;
	fn_00522441_elem* field_c;

	fn_00522441_vector(const fn_00522441_vector& other);

	// MATCH-ONLY: inlined helper; the original's name is unknown.
	int size() const
	{
		if (field_4 == 0) {
			return 0;
		}
		return field_8 - field_4;
	}
};

// MATCH-ONLY: inlined helper; the original's name is unknown.
inline fn_00522441_elem* fn_00522441_allocate(int count)
{
	if (count < 0) {
		count = 0;
	}
	return (fn_00522441_elem*)operator new(count * sizeof(fn_00522441_elem));
}

// MATCH-ONLY: inlined helper; the original's name is unknown.
inline fn_00522441_elem* fn_00522441_copy(fn_00522441_elem* first, fn_00522441_elem* last, fn_00522441_elem* dst)
{
	for (; first != last; first++) {
		if (dst != 0) {
			new (dst) fn_00522441_elem(*first);
		}
		dst++;
	}
	return dst;
}

// FUNCTION: STARFLEETOP 0x00522441
fn_00522441_vector::fn_00522441_vector(const fn_00522441_vector& other)
{
	field_0 = other.field_0;
	field_4 = fn_00522441_allocate(other.size());
	fn_00522441_elem* dst = fn_00522441_copy(other.field_4, other.field_8, field_4);
	field_8 = dst;
	field_c = dst;
}

// HYPOTHESIS: constructor of a holder: a value, an unused slot, and a cleared 8-byte pair.
struct fn_005224d8_holder
{
	int field_0;
	int field_4;
	double field_8;

	fn_005224d8_holder(int value);
};

// FUNCTION: STARFLEETOP 0x005224d8
fn_005224d8_holder::fn_005224d8_holder(int value)
{
	field_0 = value;
	field_8 = 0.0;
}
