// tAIWeaponOfficer (RTTI): an AI officer of a ship's captain.

#include "tAIOfficer.h"
#include "tPriorityPowerRequest.h"
#include "tEntity.h"
#include "tObject_0096eca8.h"

class tObject_00429103;

// HYPOTHESIS: a weapon of the ship; only the value compared with the officer's range at
// 0xf8 and slot 134 (offset 0x214), a test returning a bool, are known.
class tObject_0042496f_weapon
{
public:
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();
	virtual void vfunction41();
	virtual void vfunction42();
	virtual void vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual void vfunction55();
	virtual void vfunction56();
	virtual void vfunction57();
	virtual void vfunction58();
	virtual void vfunction59();
	virtual void vfunction60();
	virtual void vfunction61();
	virtual void vfunction62();
	virtual void vfunction63();
	virtual void vfunction64();
	// HYPOTHESIS: slot 65 (offset 0x100) takes a byte of one of the officer's objects at 0x128.
	virtual void vfunction65(int8u param_1);
	virtual void vfunction66();
	virtual void vfunction67();
	virtual void vfunction68();
	virtual void vfunction69();
	virtual void vfunction70();
	virtual void vfunction71();
	virtual void vfunction72();
	virtual void vfunction73();
	virtual void vfunction74();
	virtual void vfunction75();
	virtual void vfunction76();
	virtual void vfunction77();
	virtual void vfunction78();
	virtual void vfunction79();
	virtual void vfunction80();
	virtual void vfunction81();
	virtual void vfunction82();
	virtual void vfunction83();
	virtual void vfunction84();
	virtual void vfunction85();
	virtual void vfunction86();
	virtual void vfunction87();
	virtual void vfunction88();
	virtual void vfunction89();
	virtual void vfunction90();
	virtual void vfunction91();
	virtual void vfunction92();
	virtual void vfunction93();
	virtual void vfunction94();
	virtual void vfunction95();
	virtual void vfunction96();
	virtual void vfunction97();
	virtual void vfunction98();
	virtual void vfunction99();
	virtual void vfunction100();
	virtual void vfunction101();
	virtual void vfunction102();
	virtual void vfunction103();
	virtual void vfunction104();
	virtual void vfunction105();
	// HYPOTHESIS: a signed byte 0x005143d2 scales each weapon's value by; meaning not known.
	virtual char vfunction106();
	virtual void vfunction107();
	virtual void vfunction108();
	virtual void vfunction109();
	virtual void vfunction110();
	virtual void vfunction111();
	virtual void vfunction112();
	virtual void vfunction113();
	virtual void vfunction114();
	virtual void vfunction115();
	virtual void vfunction116();
	virtual void vfunction117();
	virtual void vfunction118();
	virtual void vfunction119();
	virtual void vfunction120();
	virtual void vfunction121();
	virtual void vfunction122();
	virtual void vfunction123();
	virtual void vfunction124();
	virtual void vfunction125();
	virtual void vfunction126();
	virtual void vfunction127();
	virtual void vfunction128();
	virtual void vfunction129();
	virtual void vfunction130();
	virtual void vfunction131();
	virtual void vfunction132();
	virtual void vfunction133();
	virtual bool vfunction134();
	// HYPOTHESIS: slot 135 (offset 0x218) returns a float for an integer argument.
	virtual float vfunction135(int param_1);
	// HYPOTHESIS: slot 136 (offset 0x21c) takes an integer and returns a test as an int.
	virtual int vfunction136(int param_1);
	virtual void vfunction137();
	// HYPOTHESIS: slot 138 (offset 0x224) is another test returning a bool.
	virtual bool vfunction138();

	char field_0x4[0xf4];
	int field_0xf8;
};

// HYPOTHESIS: one of the officer's 15 weapon entries at 0x38: the weapon and its index in
// the ship's weapon list, with two fields not yet understood.
class tObject_00513eb6_entry
{
public:
	tObject_00513eb6_entry()
	{
		fWeapon = 0;
		fIndex = 0;
		field_0x8 = 0;
		field_0xc = 0;
	}

	tObject_0042496f_weapon *fWeapon;     // +0x0
	int8u fIndex;                         // +0x4
	int field_0x8;                        // +0x8
	int8u field_0xc;                      // +0xc
};

// HYPOTHESIS: the six 0x10-byte objects at 0x128, constructed by 0x00513d44.
class tObject_00513d44
{
public:
	tObject_00513d44();

	char field_0x0;            // +0x0
	// HYPOTHESIS: from 0x005146f0 only: a byte passed to slot 65 of a weapon.
	int8u field_0x1;           // +0x1
	char field_0x2[0x2];       // +0x2
	// HYPOTHESIS: from 0x005147ab only: two floats and the index of the next object, -1 for
	// none, which chains the objects; their meaning is not known.
	float field_0x4;           // +0x4
	float field_0x8;           // +0x8
	char field_0xc;            // +0xc
	char field_0xd[0x3];       // +0xd
};

// HYPOTHESIS: the four 0x9c-byte objects at 0x188, constructed by 0x00513d0f.
class tObject_00513d0f
{
public:
	tObject_00513d0f();
	// HYPOTHESIS: called by 0x005142b3 before it refills the object; probably a reset.
	void fn_00513d1b();

	// HYPOTHESIS: from 0x00514141 only: a float, a flag per weapon entry from +0x4, and a
	// value per weapon entry from +0x60. From 0x005146f0: per weapon entry, an index into
	// the objects at 0x128 from +0x13 and a float from +0x24.
	float field_0x0;           // +0x0
	char field_0x4[15];        // +0x4
	char field_0x13[15];       // +0x13
	float field_0x24[15];      // +0x24
	int field_0x60[15];        // +0x60
};

// HYPOTHESIS: one 0x14-byte entry of the object below, as 0x005143d2 reads it: a byte at
// +0x4 that is 0xff for an unused entry, and two floats; their meaning is not known.
class tObject_005142b3_entry
{
public:
	int field_0x0;          // +0x0
	int8u field_0x4;        // +0x4
	char field_0x5[7];      // +0x5
	float field_0xc;        // +0xc
	float field_0x10;       // +0x10
};

// HYPOTHESIS: the object 0x005142b3 takes as its third argument: 32 entries and their count
// at 0x280, which must be positive; 0x005143d2 reads at most 15 of them.
class tObject_005142b3
{
public:
	tObject_005142b3_entry field_0x0[32];   // +0x0
	int field_0x280;                        // +0x280
};

// HYPOTHESIS: the object 0x005142b3 takes from the ship's 0x004156e3 and passes on to slot 15;
// only its test at 0x00429103 is known.
class tObject_00429103
{
public:
	bool fn_00429103(float param_1, int param_2, int param_3);
	// HYPOTHESIS: from 0x00514599 only: a weapon entry's index and two fields of an entry of
	// tObject_005142b3; the meaning of the value is not known.
	float fn_004291a7(int param_1, int param_2, int8u param_3);
};

// HYPOTHESIS: one 0x80-byte row of the global at 0x0096da08, filled by slot 15 for one
// entry of the object above: the entry's value, two values per weapon entry, and a flag.
class tObject_0096da08_row
{
public:
	float field_0x0;           // +0x0
	float field_0x4[15];       // +0x4
	float field_0x40[15];      // +0x40
	bool field_0x7c;           // +0x7c
};

// HYPOTHESIS: the global at 0x0096da08, passed by address to slots 15 to 17 of the officer;
// slot 15 fills one row per entry, at most 15, and slot 17 picks the row.
class tObject_0096da08
{
public:
	tObject_0096da08_row fRows[15];
};

extern tObject_0096da08 gObject_0096da08;

// HYPOTHESIS: the bases and the fields are recovered from the constructor at 0x00513eb6 only.
class tAIWeaponOfficer : public tAIOfficer, public tPriorityPowerRequest
{
public:
	tAIWeaponOfficer(int aPowerParam, int aOfficerParam, int aMinType, int aMaxType);
	virtual ~tAIWeaponOfficer();
	tAIWeaponOfficer *scalar_deleting_destructor(unsigned char flags);

	int fMinType;                             // +0x24
	int fMaxType;                             // +0x28
	int field_0x2c;                           // +0x2c
	int field_0x30;                           // +0x30
	int8u fNumEntries;                        // +0x34
	tObject_00513eb6_entry fEntries[15];      // +0x38
	tObject_00513d44 field_0x128[6];          // +0x128
	tObject_00513d0f field_0x188[4];          // +0x188
	int field_0x3f8;                          // +0x3f8
	int8u field_0x3fc;                        // +0x3fc

	// HYPOTHESIS: the getter of the entry count; the name and return type are not known.
	int8u fn_00513fd4();
	// HYPOTHESIS: counts the entries whose weapon passes the test in its slot 134; the name
	// is the analysis's.
	short vfunction7_for_tAIOfficer();
	// HYPOTHESIS: counts the entries whose weapon passes the test in slot 134 or whose value
	// from slot 135 is at most the argument; the meaning of the value is not known.
	short fn_00514011(float param_1);
	// HYPOTHESIS: as fn_00514011, counting only weapons whose value at 0xf8 is below 0x25;
	// what that value and the bound mean is not known.
	short fn_00514067(float param_1);
	// HYPOTHESIS: counts the entries whose weapon has a value at 0xf8 below 0x25 and passes
	// the test in slot 138; the name is the analysis's.
	short vfunction9_for_tAIOfficer();
	// HYPOTHESIS: counts the entries whose weapon passes the test in slot 138; the name is
	// the analysis's.
	short vfunction8_for_tAIOfficer();
	// HYPOTHESIS: the name is the analysis's.
	int vfunction6_for_tAIOfficer(int *param_1, char param_2, int param_3);
	// HYPOTHESIS: tests whether any weapon entry is flagged in the object 0x00514141 selects
	// with the same argument; the name is the analysis's.
	bool vfunction10_for_tAIOfficer(int *param_1);
	// HYPOTHESIS: the setter of the byte at 0x3fc; the name and meaning are not known.
	void fn_005142a6(int8u param_1);
	// HYPOTHESIS: the name is the analysis's.
	float vfunction11_for_tAIOfficer(int *param_1, int param_2, tObject_005142b3 *param_3, int param_4);

	// HYPOTHESIS: slots 15 to 17 (offsets 0x38 to 0x40), the first after tAIOfficer's; their
	// arguments are read from 0x005142b3's calls only.
	virtual int8u vfunction15(int *param_1, tObject_00429103 *param_2, tObject_005142b3 *param_3, tObject_0096da08 *param_4);

	virtual bool vfunction16(int param_1, float param_2, int8u param_3, tObject_0096da08 *param_4, int param_5, int param_6);

	virtual int8u vfunction17(int param_1, int8u param_2, tObject_0096da08 *param_3);
	// HYPOTHESIS: slots 18 and 19 (offsets 0x44 and 0x48); their arguments are read from
	// 0x005143d2's calls only.
	virtual float vfunction18(int8u param_1, tObject_00429103 *param_2, tObject_005142b3_entry *param_3);

	virtual float vfunction19(float param_1);
	// HYPOTHESIS: the name is the analysis's.
	void vfunction2_for_tAIOfficer(int *param_1, int param_2);
	// HYPOTHESIS: the name is the analysis's slot number; 0x005142b3 calls it after slot 12.
	virtual void vfunction13(int *param_1, int param_2);
	// HYPOTHESIS: the override of the power request's slot 3; the name is the analysis's.
	virtual float vfunction3_for_tPriorityPowerRequest(int *param_1, int param_2, float param_3);

};

// FUNCTION: STARFLEETOP 0x00513fd4
int8u tAIWeaponOfficer::fn_00513fd4()
{
	return fNumEntries;
}

// FUNCTION: STARFLEETOP 0x00513fd8
short tAIWeaponOfficer::vfunction7_for_tAIOfficer()
{
	short count = 0;
	for (int i = 0; i < fNumEntries; i++)
	{
		if (fEntries[i].fWeapon && fEntries[i].fWeapon->vfunction134())
			count++;
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x00514011
short tAIWeaponOfficer::fn_00514011(float param_1)
{
	short count = 0;
	for (int i = 0; i < fNumEntries; i++)
	{
		if (fEntries[i].fWeapon &&
			(fEntries[i].fWeapon->vfunction134() || fEntries[i].fWeapon->vfunction135(0) <= param_1))
			count++;
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x00514067
short tAIWeaponOfficer::fn_00514067(float param_1)
{
	short count = 0;
	for (int i = 0; i < fNumEntries; i++)
	{
		if (fEntries[i].fWeapon && fEntries[i].fWeapon->field_0xf8 < 0x25 &&
			(fEntries[i].fWeapon->vfunction134() || fEntries[i].fWeapon->vfunction135(0) <= param_1))
			count++;
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x00514108
short tAIWeaponOfficer::vfunction8_for_tAIOfficer()
{
	short count = 0;
	for (int i = 0; i < fNumEntries; i++)
	{
		if (fEntries[i].fWeapon && fEntries[i].fWeapon->vfunction138())
			count++;
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x00514141
int tAIWeaponOfficer::vfunction6_for_tAIOfficer(int *param_1, char param_2, int param_3)
{
	tObject_00513d0f *obj = &field_0x188[param_1[1] * 4 + param_1[2]];
	int result = (int)obj->field_0x0;
	int total = 0;
	int local = 0;
	if (!param_2)
	{
		if (fField10 && !fField10->fn_00525270(fShipID))
			return 0;
		if (fField10 && !fField10->fn_005259b2(fShipID, field_0x30, result, &local, vfunction14()))
			return 0;
	}
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(fShipID));
	if (!ship)
		return 0;
	ship->vfunction24();
	for (int i = 0; i < fNumEntries && i < param_3; i++)
	{
		if (obj->field_0x4[i])
		{
			obj->field_0x4[i] = 0;
			if (!fEntries[i].fWeapon->vfunction136(1))
			{
				ship->vfunction61();
				tObject_0043c0f2 *member = ship->field_0xad4->vfunction80();
				if (member)
					member->fn_0043c0f2();
				if (!param_2)
					total += obj->field_0x60[i];
			}
		}
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x005140c6
short tAIWeaponOfficer::vfunction9_for_tAIOfficer()
{
	short count = 0;
	for (int i = 0; i < fNumEntries; i++)
	{
		if (fEntries[i].fWeapon && fEntries[i].fWeapon->field_0xf8 < 0x25 &&
			fEntries[i].fWeapon->vfunction138())
			count++;
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x0051426b
bool tAIWeaponOfficer::vfunction10_for_tAIOfficer(int *param_1)
{
	tObject_00513d0f *obj = &field_0x188[param_1[1] * 4 + param_1[2]];
	for (int i = 0; i < fNumEntries; i++)
	{
		if (obj->field_0x4[i])
			return true;
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x005142a6
void tAIWeaponOfficer::fn_005142a6(int8u param_1)
{
	field_0x3fc = param_1;
}

// FUNCTION: STARFLEETOP 0x005142b3
float tAIWeaponOfficer::vfunction11_for_tAIOfficer(int *param_1, int param_2,
	tObject_005142b3 *param_3, int param_4)
{
	if (param_3->field_0x280 <= 0)
		return 0.0f;
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(fShipID));
	if (!ship)
		return 0.0f;
	tObject_00429103 *value = ship->fn_004156e3();
	field_0x2c = (int)param_1;
	field_0x30 = param_2;
	int8u row = vfunction15(param_1, value, param_3, &gObject_0096da08);
	tObject_00513d0f *obj = &field_0x188[param_1[1] * 4 + param_1[2]];
	obj->fn_00513d1b();
	vfunction12(param_1, param_2);
	vfunction13(param_1, param_2);
	for (int i = 0; i < fNumEntries; i++)
	{
		int8u pick = vfunction17(i, row, &gObject_0096da08);
		float value = gObject_0096da08.fRows[pick].field_0x4[i];
		obj->field_0x4[i] = vfunction16(i, value, row,
			&gObject_0096da08, param_4, field_0x30);
		if (obj->field_0x4[i])
		{
			obj->field_0x0 += gObject_0096da08.fRows[0].field_0x4[i];
			obj->field_0x60[i] = (int)gObject_0096da08.fRows[0].field_0x4[i];
		}
		else
			obj->field_0x60[i] = 0;
	}
	return obj->field_0x0;
}

// FUNCTION: STARFLEETOP 0x005143d2
// HYPOTHESIS: the meaning of the rows; the name is the analysis's slot number. Fills one
// row of param_4 per entry of param_3, at most 15: the entry's value, then for each weapon
// entry a value from slot 18 and that value weighted by slot 19 and the entry's range.
int8u tAIWeaponOfficer::vfunction15(int *param_1, tObject_00429103 *param_2,
	tObject_005142b3 *param_3, tObject_0096da08 *param_4)
{
	int8u count = param_3->field_0x280;
	if (count > 15)
		count = 15;
	for (int i = 0; i < count; i++)
	{
		tObject_005142b3_entry *entry = &param_3->field_0x0[i];
		// HYPOTHESIS: the original read the range through its own pointer; the loop's entry
		// pointer in the original points at +0xc, which only this local reproduces.
		float *range = &entry->field_0xc;
		float next = i + 1 < count ? entry[1].field_0x10
			: gObject_0096eca8.fn_00528a88();
		if (entry->field_0x4 != 0xff)
		{
			param_4->fRows[i].field_0x0 = entry->field_0x10;
			float weight = vfunction19(entry->field_0x10);
			param_4->fRows[i].field_0x7c = false;
			float factor = 1.0f;
			if (*range <= 0.0f)
			{
				param_4->fRows[i].field_0x7c = true;
				factor = 2.0f;
			}
			else if (*range < 10.0f)
				factor = 1.5f;
			for (int j = 0; j < fNumEntries; j++)
			{
				float value = vfunction18(j, param_2, entry);
				if (value != 0.0f)
				{
					value = fEntries[j].fWeapon->vfunction106() * value;
					if (param_2->fn_00429103(next, fEntries[j].fIndex, 0))
						param_4->fRows[i].field_0x40[j] = value;
					else
						param_4->fRows[i].field_0x40[j] = value * 0.1f;
					param_4->fRows[i].field_0x4[j] =
						param_4->fRows[i].field_0x40[j] * (factor * weight);
				}
				else
				{
					param_4->fRows[i].field_0x40[j] = 0.0f;
					param_4->fRows[i].field_0x4[j] = 0.0f;
				}
			}
		}
		else
		{
			param_4->fRows[i].field_0x0 = entry->field_0x10;
			for (int j = 0; j < fNumEntries; j++)
			{
				param_4->fRows[i].field_0x40[j] = 0.0f;
				param_4->fRows[i].field_0x4[j] = 0.0f;
			}
		}
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x0051460b
// HYPOTHESIS: the name is the analysis's slot number; decides whether weapon entry param_1
// is flagged, from its value in the first row of param_4, the distance from this ship to the
// ship at 0x30, and the sum of slot 63 of this ship and ship param_6. param_3 and param_5 are
// not read.
bool tAIWeaponOfficer::vfunction16(int param_1, float param_2, int8u param_3,
	tObject_0096da08 *param_4, int param_5, int param_6)
{
	// HYPOTHESIS: as in 0x005145c6, the original reads only the signed low byte of the entry.
	char entry = param_1;
	float mine = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(fShipID))->vfunction63();
	float other = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(param_6))->vfunction63();
	float sum = mine + other;
	// HYPOTHESIS: 0x004246e4 is declared for two entities; the ships are passed as such.
	float distance = gObject_008e1060.fn_004246e4(
		(tEntity *)((tObject_0042496f *)gObject_008e1060.fn_0042496f(fShipID)),
		(tEntity *)((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x30)));
	float *value = &param_4->fRows[0].field_0x4[entry];
	if (*value > 0.0f && !field_0x3fc && param_4->fRows[0].field_0x7c == true)
		return true;
	// HYPOTHESIS: the original's exit follows the false result and its true results share
	// one block; a chain of returns builds the same instructions with the exit after the
	// first true result, so the rest is written as one expression, which leaves 5 bytes
	// of difference in how the result is made.
	return distance < 4.0 || sum < 2.0f ||
		(*value > 0.0f && (param_2 - 1.0f <= *value || sum < 30.0f) &&
		(!field_0x3fc || !param_4->fRows[0].field_0x7c));
}

// FUNCTION: STARFLEETOP 0x005146f0
// HYPOTHESIS: the name is the analysis's. Unless param_2 is -1: passes the selector and
// param_2 to slot 3 of the power request with its value, then for each weapon entry with a
// nonzero float in the object the selector picks whose index there differs from the entry's
// own, gives the weapon the byte of that object at 0x128.
void tAIWeaponOfficer::vfunction2_for_tAIOfficer(int *param_1, int param_2)
{
	if (param_2 == -1)
		return;
	float power = fn_00524109();
	vfunction3_for_tPriorityPowerRequest(param_1, param_2, power);
	gObject_0096eca8.fn_0052944d(0.0f, param_1);
	tObject_00513d0f *obj = &field_0x188[param_1[1] * 4 + param_1[2]];
	for (int i = 0; i < fNumEntries; i++)
	{
		if (obj->field_0x24[i] != 0.0f && fEntries[i].field_0xc != obj->field_0x13[i])
			fEntries[i].fWeapon->vfunction65(field_0x128[obj->field_0x13[i]].field_0x1);
	}
}

// FUNCTION: STARFLEETOP 0x005147ab
// HYPOTHESIS: the meaning; the name is the analysis's slot number. For each weapon entry,
// until one with no object in the selected object at 0x188, walks the chain of objects at
// 0x128 from the entry's object and passes each one's amount to 0x005238d7.
void tAIWeaponOfficer::vfunction13(int *param_1, int param_2)
{
	tObject_00513d0f *obj = &field_0x188[param_1[1] * 4 + param_1[2]];
	for (int i = 0; i < fNumEntries; i++)
	{
		char sel = obj->field_0x13[i];
		if (sel == -1)
			return;
		char count = fEntries[i].fWeapon->vfunction106();
		float amount = field_0x128[sel].field_0x4;
		float first = field_0x128[sel].field_0x8;
		bool same = !(field_0x128[sel].field_0x4 > field_0x128[fEntries[i].field_0xc].field_0x4);
		for (int j = 0; j < count; j++)
		{
			if (fEntries[i].fWeapon->vfunction135(j) == 0.0f && same)
			{
				field_0x8->fn_005238d7(1, first, 1, field_0xc, param_1);
			}
			else
			{
				float value = amount;
				char cur = sel;
				char next;
				char part;
				do
				{
					int mode = field_0x128[cur].field_0x0 ? 2 : 1;
					next = field_0x128[cur].field_0xc;
					if (next != -1)
					{
						part = (char)field_0x128[next].field_0x4;
						value -= part;
					}
					field_0x8->fn_005238d7(mode, value, 1, field_0xc, param_1);
					value = part;
					cur = next;
				} while (next != -1);
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x0051492b
// HYPOTHESIS: the meaning; the name is the analysis's. Unless param_2 is -1, spends param_3
// over the weapon entries of the object the selector picks: for each entry, walks the chain
// of objects at 0x128 from its own until a whole number of the weapon's shots is paid for,
// records that share, and returns what is left.
float tAIWeaponOfficer::vfunction3_for_tPriorityPowerRequest(int *param_1, int param_2,
	float param_3)
{
	if (param_2 != -1)
	{
		tObject_00513d0f *obj = &field_0x188[param_1[1] * 4 + param_1[2]];
		for (int i = 0; i < fNumEntries; i++)
		{
			// HYPOTHESIS: the original tests the selector again after the test of the share.
			char sel;
			while ((sel = obj->field_0x13[i]) != -1)
			{
				char count = fEntries[i].fWeapon->vfunction106();
				float amount = field_0x128[sel].field_0x4;
				float first = field_0x128[sel].field_0x8;
				bool same = !(field_0x128[sel].field_0x4 > field_0x128[fEntries[i].field_0xc].field_0x4);
				int j;
				for (j = 0; j < count && param_3 > 0.0f; j++)
				{
					float cost = fEntries[i].fWeapon->vfunction135(j) == 0.0f && same
						? first : amount;
					if (cost > param_3)
						break;
					param_3 -= cost;
				}
				if (j > 0)
					obj->field_0x24[i] = j / count;
				else
				{
					obj->field_0x24[i] = 0.0f;
					obj->field_0x13[i] = field_0x128[sel].field_0xc;
				}
				if (obj->field_0x24[i] != 0.0f || obj->field_0x13[i] == -1)
					break;
			}
		}
	}
	return param_3;
}

// FUNCTION: STARFLEETOP 0x00514599
// HYPOTHESIS: the name is the analysis's slot number; it asks param_2 for a value for the
// weapon entry's index and the fields of param_3.
float tAIWeaponOfficer::vfunction18(int8u param_1, tObject_00429103 *param_2,
	tObject_005142b3_entry *param_3)
{
	return param_2->fn_004291a7(fEntries[param_1].fIndex, param_3->field_0x0,
		param_3->field_0x4);
}

// FUNCTION: STARFLEETOP 0x005145bd
// HYPOTHESIS: the weight 0x005143d2 applies to an entry's value; this class weighs every
// entry the same and ignores the argument.
float tAIWeaponOfficer::vfunction19(float param_1)
{
	return 1.0f;
}

// FUNCTION: STARFLEETOP 0x005145c6
// HYPOTHESIS: the name is the analysis's slot number; returns the row of param_3, among the
// first param_2, with the highest value for weapon entry param_1 (the last one on a tie).
int8u tAIWeaponOfficer::vfunction17(int param_1, int8u param_2, tObject_0096da08 *param_3)
{
	// HYPOTHESIS: the original reads only the signed low byte of the entry index. Declaring
	// the parameter char builds this function the same but changes the code of its caller
	// 0x005142b3; an int parameter narrowed here matches both.
	char entry = param_1;
	float best = param_3->fRows[0].field_0x4[entry];
	int row = 0;
	for (int i = 1; i < param_2; i++)
	{
		if (param_3->fRows[i].field_0x4[entry] >= best)
		{
			best = param_3->fRows[i].field_0x4[entry];
			row = i;
		}
	}
	return row;
}

// FUNCTION: STARFLEETOP 0x00513fc2
tAIWeaponOfficer::~tAIWeaponOfficer()
{
}

// FUNCTION: STARFLEETOP 0x00513e90
// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no source line,
// so the harness cannot claim it directly; it is written out as a member that calls the
// destructor, then deletes when bit 0 of the flag is set.
tAIWeaponOfficer *tAIWeaponOfficer::scalar_deleting_destructor(unsigned char flags)
{
	tAIWeaponOfficer::~tAIWeaponOfficer();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: not a class of the original. 0x00513eac is not a function of its own: the
// original is `mov eax, <ehhandler>; call __EH_prolog`, the /O1 frame setup of the
// constructor whose body the analysis records separately at 0x00513eb6; its unwind funclet
// 0x00768300 names this address as its parent.
// MATCH-ONLY: the address was held by an empty tAIWeaponOfficer(), which no longer compiles
// to a stub now that the class has its bases and members; this class with a virtual table
// keeps the same 9-byte stub (mov eax, ecx; set the virtual table; ret), as 0x00413dd6 does.
class tObject_00513eac
{
public:
	tObject_00513eac();
	virtual void vfunction1();
};

// FUNCTION: STARFLEETOP 0x00513eac
tObject_00513eac::tObject_00513eac()
{
}

// HYPOTHESIS: the parameter names and the meaning of the range. The body after the frame
// setup at 0x00513eac. Collects the ship's weapons
// whose value at 0xf8 lies in the officer's range, with their index in the weapon list.
// The compiler emits the frame setup and this body as one function, so scored against
// 0x00513eb6 alone everything after the frame setup is offset by 10 bytes.
// FUNCTION: STARFLEETOP 0x00513eb6
tAIWeaponOfficer::tAIWeaponOfficer(int aPowerParam, int aOfficerParam, int aMinType, int aMaxType)
	: tAIOfficer(aOfficerParam), tPriorityPowerRequest(aPowerParam),
	  fMinType(aMinType), fMaxType(aMaxType), field_0x30(-1), fNumEntries(0), field_0x3fc(0)
{
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(fShipID));
	if (ship)
	{
		tObject_0042496f_weapon **weapons = ship->field_0xad4->vfunction58();
		for (int i = 0; i < 25; i++)
		{
			tObject_0042496f_weapon *weapon = weapons[i];
			if (weapon && weapon->field_0xf8 >= fMinType && weapon->field_0xf8 <= fMaxType)
			{
				fEntries[fNumEntries].fWeapon = weapon;
				fEntries[fNumEntries].fIndex = i;
				fNumEntries++;
			}
		}
	}
}
