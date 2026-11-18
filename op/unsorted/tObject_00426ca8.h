//	tObject_00426ca8.h: the team object of Source/entity/team.cpp.
// Evidence:  the unit's assertion strings; the class name is not known

#ifndef tObject_00426ca8_h
#define tObject_00426ca8_h 1

// HYPOTHESIS: one (a, b) pair of the list below; the meaning of either word is unknown.
struct tObject_00426ca8_entry
{
	int field_0x0;
	int field_0x4;
};

// HYPOTHESIS: a team object; the growable pair list at +0xc4 (array, count, capacity) and the
// relation per team at +0x10 that 0x00427581 reads are known. 0x00426da7 is called on it
// before the list is read.
class tObject_00426ca8
{
public:
	void fn_00426da7();
	void fn_00426ca8(int a, int b);
	void fn_00426e40(int aArg1, int aArg2);
	void fn_00426e61(int aArg1, int aArg2);
	bool fn_00426c23(int aArg);
	void fn_00426c5e(int aArg);
	int fn_00426e7a(int aArg);

	char pad_0x0[0x10];
	int field_0x10[20];
	char pad_0x60[0xc4 - 0x60];
	tObject_00426ca8_entry *field_0xc4;
	int field_0xc8;
	int field_0xcc;
};

#endif // tObject_00426ca8_h
