//	tAICaptain.cpp: tAICaptain, the AI that commands one ship.
// Evidence:  class (RTTI; original file unknown)
#include <string.h>
#include <list>
#include <vector>
#include "tEntity.h"
#include "tAIOfficer.h"
#include "tAIHelmOfficer.h"
#include "tAIOffensiveOfficer.h"
#include "tAIDefensiveOfficer.h"
#include "unsorted/tObject_0092deb4.h"
#include "tObject_0096eca8.h"
#include "fn_0090be9c_type.h"
#include "tShipCommand.h"
// HYPOTHESIS: the second base of the officers that ask for extra energy (RTTI); only its position in each officer is known, from the pointers tAICaptain's constructor hands to its power object.
class tExtraEnergyRequest
{
public:
	virtual void vfunction1();
};
class tObject_00528a11;
#include "unsorted/tObject_005237dc.h"











// HYPOTHESIS: the object at 0x35c8 of tAICaptain, 0x24 bytes, called the selector where the
// officers read it; its constructor (0x00528a11) takes nothing and it has no destructor. A
// second constructor (0x00528a23) takes five values; 0x00514f8b copies all 0x24 bytes.
class tObject_00528a11
{
public:
	tObject_00528a11();
	tObject_00528a11(int param_1, int param_2, int param_3, int param_4, char param_5);
	int field_0x0;                          // +0x0
	// HYPOTHESIS: the two values 0x0051e454 passes to the helm officer's 0x005169e0.
	int field_0x4;                          // +0x4
	int field_0x8;                          // +0x8
	int field_0xc;                          // +0xc
	int field_0x10;                         // +0x10
	char field_0x14[0xc];                   // +0x14
	char field_0x20;                        // +0x20
	char field_0x21[0x3];                   // +0x21
};
class tObject_004b94dc;
class tObject_00524875;
// HYPOTHESIS: the object the pointer at 0x0090bed4 names; tSfcApp3d.cpp declares the same
// pointer with only its method 0x00522681. Its method 0x00522708 returns an object tAICaptain
// keeps at 0x35fc.
class tObject_00522708
{
public:
	tObject_00524875 *fn_00522708();
	tObject_004b94dc *fn_00523574(int aID);
	// HYPOTHESIS: takes the object 0x00522708 returned; tAICaptain's destructor hands it back.
	void fn_00522681(tObject_00524875 *aObject);
};
class tObject_0051e0c4_list;
class tObject_0051e0c4_35b8;
class tObject_0051e0c4_3a34;
extern tObject_00522708 *g_0090bed4;
// HYPOTHESIS: a double tAICaptain's constructor copies to 0xb0.
extern double g_0090bef0;
// HYPOTHESIS: a string tAICaptain's constructor passes with its second argument to 0x0054814e.
extern const char g_0085912c[];
// HYPOTHESIS: the officer classes below are declared only as far as tAICaptain's constructor uses
// them: their constructors take the ship ID, and those that also ask for extra energy have that
// base after their tAIOfficer part (0x14 bytes), or after tAITransportOfficer's (0x24 bytes).
class tAITransportOfficer : public tAIOfficer
{
public:
	tAITransportOfficer(int aShipID);
	char field_0x14[0x10];
};
class tAIShieldReinforcementOfficer : public tAIOfficer, public tExtraEnergyRequest
{
public:
	tAIShieldReinforcementOfficer(int aShipID);
};
class tAIECMOfficer : public tAIOfficer, public tExtraEnergyRequest
{
public:
	tAIECMOfficer(int aShipID);
};
class tAIECCMOfficer : public tAIOfficer, public tExtraEnergyRequest
{
public:
	tAIECCMOfficer(int aShipID);
};
class tAITractorOfficer : public tAIOfficer, public tExtraEnergyRequest
{
public:
	tAITractorOfficer(int aShipID);
};
class tAIRepairOfficer : public tAIOfficer
{
public:
	tAIRepairOfficer(int aShipID);
};
class tAISeekingWeaponDefenseOfficer : public tAIOfficer, public tExtraEnergyRequest
{
public:
	tAISeekingWeaponDefenseOfficer(int aShipID);
};
class tAIHitAndRunOfficer : public tAITransportOfficer, public tExtraEnergyRequest
{
public:
	tAIHitAndRunOfficer(int aShipID);
};
class tAITBombOfficer : public tAITransportOfficer, public tExtraEnergyRequest
{
public:
	tAITBombOfficer(int aShipID);
};
class tAIHETOfficer : public tAIOfficer
{
public:
	tAIHETOfficer(int aShipID);
	// HYPOTHESIS: whether the officer takes over; 0x0051e613 then skips the selector search.
	bool fn_0051819f();
};
class tAICloakOfficer : public tAIOfficer
{
public:
	tAICloakOfficer(int aShipID, tAIHelmOfficer *aHelm, tObject_005237dc *aPower);
	void fn_00514bea(int aValue); // HYPOTHESIS: takes one value; tAICaptain's 0x0051f16e forwards its argument.
};
class tAIFighterOfficer : public tAIOfficer
{
public:
	tAIFighterOfficer(int aShipID);
};
class tAIPlanetKillerOfficer : public tAIOfficer
{
public:
	tAIPlanetKillerOfficer(int aShipID);
};
class tAIShuttleOfficer : public tAIOfficer
{
public:
	tAIShuttleOfficer(int aShipID);
};
// HYPOTHESIS: layout unknown beyond the constructor; filled from the disassembly. The four arguments: a float kept at 0x3a30 and given to every officer, a value passed on to 0x0054814e, one the constructor does not use, and the ship ID.
class tAICaptain : public tEntity
{
public:
	tAICaptain(float aParam1, int aParam2, int aParam3, int aShipID);
	virtual ~tAICaptain();
	virtual void vfunction7(tObject_0041018e_arg *param_1, int param_2);
	virtual int vfunction18();
	virtual void vfunction3();
	virtual void vfunction4(tObject_00413ac3_arg *aObject);
	void fn_0051e275(int aValue);
	void fn_0051f559();
	tObject_0051e0c4_35b8 *fn_0051e32e();
	void fn_0051e33f(tShipCommand *param_1); // HYPOTHESIS: takes a command for the ship; 0x0051f0e1 passes a tCommandDisengage or a tCommandClearCurrentOrders.
	tPoint3 fn_0051e37d();
	void fn_0051e3a1();
	bool fn_0051e3ac(int aID, int aValue);
	tObject_0051e0c4_3a34 *fn_0051e44d();
	void fn_0051fa75();
	void fn_0051fa3c();
	void fn_0051fce4();
	tPoint3 fn_0051feca(tObject_0042496f *aShip); // HYPOTHESIS: returns by value a point for the ship given; 0x0051fa3c sends the ship there.
	void fn_0051ff8b(int aTargetID, char aFlag); // HYPOTHESIS: sets the target ID at 0xc4 and the flag at 0xd0, and hands the ID to the helm officer.
	void fn_00520004(float aValue, unsigned char aCount, char *aData); // HYPOTHESIS: a float, a count and that many bytes, as order 2 carries them.
	void fn_0051f1ed();
	void fn_0052008f();
	void fn_0051f602();
	void fn_0051e613();
	void fn_0051f0c3(char aValue);
	char fn_0051f0d0();
	void fn_0051f0e1(char aValue);
	void fn_0051f153(int aIndex, int aValue);
	void fn_0051f16e(int aValue);
	void fn_0051f184(tObject_00525081 *aFormation);
	void fn_0051ffae(tPoint3 aPoint);
	void fn_0051ff8b(int aID, int aValue);
	void fn_0051f958(int aID, char aFlag);
	void fn_0051f9d1(int aID);
	void fn_0051f9e9(int aID);
	void fn_0051ffa0(int aID, char aFlag); // HYPOTHESIS: argument types; 0x0051f9d1 passes its argument and 1.
	tAICaptain *scalar_deleting_destructor(unsigned char flags);
	void fn_0054814e(int param_1, const char *param_2); // HYPOTHESIS: 0x0054814e is three bytes, so an empty function that takes two arguments.
	int field_0x88;                  // +0x88
	int field_0x8c;                         // +0x8c
	char field_0x90;                        // +0x90
	// HYPOTHESIS: a list (the constructor sets allocator, head node, count); element type unknown.
	std::list<int> field_0x94;              // +0x94
	double field_0xa0;                      // +0xa0
	double field_0xa8;                      // +0xa8
	double field_0xb0;                      // +0xb0
	char field_0xb8[0xc];                   // +0xb8
	int field_0xc4;                         // +0xc4
	int field_0xc8;                         // +0xc8
	int field_0xcc;                         // +0xcc HYPOTHESIS: the ID 0x0051f9e9 hands the tractor officer
	char field_0xd0;                        // +0xd0
	char field_0xd1[0x3];                   // +0xd1
	tObject_005237dc field_0xd4;            // +0xd4
	int field_0x35b4;                       // +0x35b4
	tObject_0051e0c4_35b8 *field_0x35b8;    // +0x35b8
	tObject_0051e0c4_35b8 *field_0x35bc;    // +0x35bc
	int field_0x35c0;                       // +0x35c0
	int field_0x35c4;                       // +0x35c4
	tObject_00528a11 field_0x35c8;          // +0x35c8
	float field_0x35ec;                     // +0x35ec
	float field_0x35f0;                     // +0x35f0
	float field_0x35f4;                     // +0x35f4
	char field_0x35f8;                      // +0x35f8
	tObject_00524875 *field_0x35fc;         // +0x35fc
	char field_0x3600;                      // +0x3600
	short field_0x3602;                     // +0x3602
	float field_0x3604;                     // +0x3604
	tAIOffensiveOfficer field_0x3608;       // +0x3608
	tAIDefensiveOfficer field_0x37a8;       // +0x37a8
	tAIHelmOfficer field_0x3810;            // +0x3810
	tAIOfficer *field_0x396c[13];           // +0x396c
	float field_0x39a0;                     // +0x39a0 HYPOTHESIS: a range 0x0051f602 searches in
	unsigned char field_0x39a4;             // +0x39a4
	unsigned char field_0x39a5;             // +0x39a5
	char field_0x39a6[0x78];                // +0x39a6
	char field_0x3a1e;                      // +0x3a1e
	char field_0x3a1f[0x11];                // +0x3a1f
	float field_0x3a30;                     // +0x3a30
	tObject_0051e0c4_3a34 *field_0x3a34;    // +0x3a34
	tObject_00525081 *field_0x3a38;         // +0x3a38 HYPOTHESIS: the formation, as the helm's 0x140
	char field_0x3a3c;                      // +0x3a3c
	char field_0x3a3d;                      // +0x3a3d
	char field_0x3a3e;                      // +0x3a3e
	unsigned int field_0x3a40;              // +0x3a40 HYPOTHESIS: a time; 0x0051e454 reads it unsigned
	char field_0x3a44;                      // +0x3a44
	char field_0x3a45;                      // +0x3a45
};
// FUNCTION: STARFLEETOP 0x0051dae7
tAICaptain::tAICaptain(float aParam1, int aParam2, int aParam3, int aShipID)
	: field_0x88(aShipID),
	  field_0x8c(-1),
	  field_0x90(0),
	  field_0xa0(0.0),
	  field_0xa8(0.0),
	  field_0xc4(-1),
	  field_0xc8(-1),
	  field_0xd0(0),
	  field_0xd4(aShipID),
	  field_0x35b4(-1),
	  field_0x35b8(0),
	  field_0x35bc(0),
	  field_0x35c0(0),
	  field_0x35c4(0),
	  field_0x35ec(0.5f),
	  field_0x35f0(1.0f),
	  field_0x35f4(1.0f),
	  field_0x35f8(0),
	  field_0x3602(-1),
	  field_0x3604(1.0f),
	  field_0x3608(aShipID, &field_0xd4),
	  field_0x37a8(aShipID),
	  field_0x3810(aShipID, &field_0xd4),
	  field_0x3a1e(5),
	  field_0x3a30(aParam1),
	  field_0x3a38(0),
	  field_0x3a3c(0),
	  field_0x3a3d(0),
	  field_0x3a3e(0)
{
	field_0x35fc = g_0090bed4->fn_00522708();
	fClassification |= 0x1000;
	fProperties |= 5;
	memset(field_0x396c, 0, sizeof(field_0x396c));
	field_0x3a34 = 0;

	tAIShieldReinforcementOfficer *shield = new tAIShieldReinforcementOfficer(field_0x88);
	tAIECMOfficer *ecm = new tAIECMOfficer(field_0x88);
	tAIECCMOfficer *eccm = new tAIECCMOfficer(field_0x88);
	tAITractorOfficer *tractor = new tAITractorOfficer(field_0x88);
	tAIRepairOfficer *repair = new tAIRepairOfficer(field_0x88);
	tAISeekingWeaponDefenseOfficer *seekingDefense =
		new tAISeekingWeaponDefenseOfficer(field_0x88);
	tAIHitAndRunOfficer *hitAndRun = new tAIHitAndRunOfficer(field_0x88);
	tAITBombOfficer *tBomb = new tAITBombOfficer(field_0x88);
	tAIHETOfficer *het = new tAIHETOfficer(field_0x88);
	tAICloakOfficer *cloak = new tAICloakOfficer(field_0x88, &field_0x3810, &field_0xd4);
	tAIFighterOfficer *fighter = new tAIFighterOfficer(field_0x88);
	tAIPlanetKillerOfficer *planetKiller = new tAIPlanetKillerOfficer(field_0x88);
	tAIShuttleOfficer *shuttle = new tAIShuttleOfficer(field_0x88);

	field_0x396c[0] = ecm;
	field_0x396c[1] = eccm;
	field_0x396c[2] = tractor;
	field_0x396c[3] = repair;
	field_0x396c[4] = seekingDefense;
	field_0x396c[5] = hitAndRun;
	field_0x396c[6] = tBomb;
	field_0x396c[7] = het;
	field_0x396c[8] = cloak;
	field_0x396c[9] = fighter;
	field_0x396c[10] = planetKiller;
	field_0x396c[11] = shield;
	field_0x396c[12] = shuttle;

	for (int i = 0; i < 13; i++)
	{
		if (field_0x396c[i])
		{
			if (field_0x396c[i]->vfunction3())
			{
				field_0x396c[i]->fn_005c7fc6(field_0x3a30);
			}
			else
			{
				delete field_0x396c[i];
				field_0x396c[i] = 0;
			}
		}
	}

	if (field_0x396c[1])
		field_0xd4.fn_00523a86(eccm);
	if (field_0x396c[0])
		field_0xd4.fn_00523a86(ecm);
	if (field_0x396c[2])
		field_0xd4.fn_00523a86(tractor);
	if (field_0x396c[5])
		field_0xd4.fn_00523a86(hitAndRun);
	if (field_0x396c[6])
		field_0xd4.fn_00523a86(tBomb);
	if (field_0x396c[11])
		field_0xd4.fn_00523a86(shield);

	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x88));
	if (ship && ship->field_0xa40 == 7)
	{
		field_0x3600 = 1;
		field_0x3810.fn_00517e85();
	}
	else
	{
		field_0x3600 = 0;
	}

	fn_0054814e(aParam2, g_0085912c);
	field_0xb0 = g_0090bef0;
	field_0x3a40 = 0;
	field_0x3a44 = 0;
	field_0x3a45 = 0;
}

// FUNCTION: STARFLEETOP 0x0051e09a
int tAICaptain::vfunction18()
{
	return 19;
}

// FUNCTION: STARFLEETOP 0x0051e09e
// MATCH-ONLY: the scalar deleting destructor is compiler-generated from the virtual
// destructor and has no source line, so the harness cannot claim it directly; it is
// written out as a member that runs the destructor, then deletes when bit 0 of the
// flag is set. The destructor's own body is not recovered yet.
tAICaptain *tAICaptain::scalar_deleting_destructor(unsigned char flags)
{
	tAICaptain::~tAICaptain();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the objects tAICaptain's list at 0x94 points to; its destructor deletes each
// through the virtual destructor in vtable slot 0. Nothing else is known.
class tObject_0051e0c4_list
{
public:
	virtual ~tObject_0051e0c4_list();
};

// HYPOTHESIS: the objects at 0x35b8 and 0x35bc of tAICaptain, deleted through a virtual
// destructor in vtable slot 0.
class tObject_0051e0c4_35b8
{
public:
	virtual ~tObject_0051e0c4_35b8();

	// HYPOTHESIS: the order 0x0051fa3c copies to 0x35b4, a priority it compares with 0x35c0,
	// and two flags: 0xc set for an order carried out at once, 0xd for one that waits.
	int field_0x4;                          // +0x4
	int field_0x8;                          // +0x8
	char field_0xc;                         // +0xc
	char field_0xd;                         // +0xd
	char field_0xe[0x2];                    // +0xe
	// HYPOTHESIS: the ID of the ship to dock with and the point to leave for, as 0x0051f1ed
	// reads them from the object at 0x35bc.
	int field_0x10;                         // +0x10
	tPoint3 field_0x14;                     // +0x14
};

// HYPOTHESIS: an entry of the vector at 0x3a34 of tAICaptain, 0x10 bytes, as 0x0051e3ac builds
// it: an ID, the value 0x0042750a returns for it (-1 until then), a value, and a flag.
struct tObject_0051e3ac_entry
{
	tObject_0051e3ac_entry(int aID, int aValue);

	int fID;                                // +0x0
	int fRelation;                          // +0x4
	int fValue;                             // +0x8
	bool fFlag;                             // +0xc
};

inline tObject_0051e3ac_entry::tObject_0051e3ac_entry(int aID, int aValue)
{
	fRelation = -1;
	fFlag = false;
	fID = aID;
	fValue = aValue;
}
typedef std::vector<tObject_0051e3ac_entry> tObject_0051e3ac_entry_vector;
// HYPOTHESIS: the object at 0x3a34 of tAICaptain is a vector of these entries: 0x0051e3ac news
// it (allocator, three null pointers) and reserves five.
class tObject_0051e0c4_3a34 : public tObject_0051e3ac_entry_vector
{
};

// HYPOTHESIS: the analysis starts this function 10 bytes into the destructor, after its
// exception-frame prolog (0x0051e0ba); the source is the whole destructor.
// FUNCTION: STARFLEETOP 0x0051e0ba
tAICaptain::~tAICaptain()
{
	g_0090bed4->fn_00522681(field_0x35fc);

	delete field_0x3a34;

	std::list<int>::iterator it = field_0x94.begin();
	while (it != field_0x94.end())
	{
		// HYPOTHESIS: the list holds pointers; its element type is not known, so it stays
		// list<int> as the constructor declares it.
		delete (tObject_0051e0c4_list *)*it;
		it = field_0x94.erase(it);
	}

	if (field_0x35b8)
	{
		delete field_0x35b8;
		field_0x35b8 = 0;
	}
	if (field_0x35bc)
	{
		delete field_0x35bc;
		field_0x35bc = 0;
	}

	for (int i = 0; i < 13; i++)
	{
		if (field_0x396c[i])
		{
			delete field_0x396c[i];
			field_0x396c[i] = 0;
		}
	}
}

// An 8-byte object with the vtable at 0x00796d10, built on the stack in 0x0051e1f7 and filled
// by 0x00434d0e; the entity ID at +0x04 is read.
// HYPOTHESIS: the layout and the virtual are inferred from 0x0051e1f7 only.
class tObject_00796d10
{
public:
	virtual void vfunction1();
	void fn_00434d0e(int aValue);

	tID fField04;                 // +0x04
};

// FUNCTION: STARFLEETOP 0x0051e1f7
void tAICaptain::vfunction7(tObject_0041018e_arg *param_1, int param_2)
{
	if (param_1->fType == 0x33)
	{
		tObject_00796d10 local;
		local.fn_00434d0e(param_2);
		tEntity *entity = gObject_008e1060.fn_lookup(local.fField04);
		if (entity)
		{
			int relation = gObject_0090be60->fn_0042748f(field_0x88);
			int team = entity->fTeam;
			if (gObject_0090be60->fn_00427581(relation, team) == 5)
				gObject_0090be60->fn_004275fe(relation, team, 6);
		}
	}
}

// HYPOTHESIS: the 16 bytes 0x0051c81f returns by value (it copies four dwords from a local
// through its hidden result pointer and returns with ret 0x14); 0x0051f602 reads only the first,
// as a target ID.
struct tObject_0051c81f_result
{
	int field_0x0;                          // +0x0
	int field_0x4;                          // +0x4
	int field_0x8;                          // +0x8
	int field_0xc;                          // +0xc
};

// HYPOTHESIS: the object 0x00523574 (a method of the object at 0x0090bed4) returns for an ID;
// its method 0x004b94dc (7 bytes) returns an integer that 0x0051e275 compares with slot 0x84
// of the object at 0x0092deb4.
class tObject_004b94dc
{
public:
	int fn_004b94dc();
	// HYPOTHESIS: a truth value for a ship and a flag; 0x0051f1ed passes 0 for orders 30 and 31,
	// 1 for 35 and 45.
	bool fn_0051cb40(tObject_0042496f *aShip, int aValue);
	// HYPOTHESIS: takes the values 0x0051f559 fills by address.
	void fn_0051c280(struct tObject_0051c280_arg *aArg);
	// HYPOTHESIS: chooses a target for the ship with the ID and returns it, its ID first;
	// 0x0051f602 passes 1, 0 and whether the ship's kind at 0x870 is 0x16.
	tObject_0051c81f_result fn_0051c81f(int aID, int param_3, int param_4, bool param_5);

	char field_0x0[0x114];                  // +0x0
	// HYPOTHESIS: a formation; 0x0051f184 sets it when it is still empty.
	tObject_00525081 *field_0x114;          // +0x114
};

// HYPOTHESIS: an inline setter of the offensive officer with no address of its own, seen only
// inlined in 0x0051e275; it takes both values by reference and stores them at 0xa0 and 0xa4.
inline void tAIOffensiveOfficer::fn_0051e275_inline(const int &param_1, const int &param_2)
{
	field_0xa0 = param_1;
	field_0xa4 = param_2;
}

// HYPOTHESIS: sets the value at 0x8c and passes it with the ship's own ID to the offensive
// officer; when the object for that value answers like the object at 0x0092deb4, every officer
// gets 1.0 instead of the float at 0x3a30, and those the ship cannot use are deleted as in the
// constructor. Ends in 0x0051f559.
// FUNCTION: STARFLEETOP 0x0051e275
void tAICaptain::fn_0051e275(int aValue)
{
	field_0x8c = aValue;
	field_0x3608.fn_0051e275_inline(field_0x8c, fID);
	float value;
	tObject_004b94dc *obj = g_0090bed4->fn_00523574(field_0x8c);
	if (obj && obj->fn_004b94dc() == gObject_0092deb4->vfunction33())
	{
		field_0x90 = 1;
		value = 1.0f;
	}
	else
	{
		field_0x90 = 0;
		value = field_0x3a30;
	}
	for (int i = 0; i < 13; i++)
	{
		if (field_0x396c[i])
		{
			if (field_0x396c[i]->vfunction3())
			{
				field_0x396c[i]->fn_005c7fc6(value);
			}
			else
			{
				delete field_0x396c[i];
				field_0x396c[i] = 0;
			}
		}
	}
	fn_0051f559();
}

// HYPOTHESIS: returns the object at 0x35b8 when there is one, else the one at 0x35bc.
// FUNCTION: STARFLEETOP 0x0051e32e
tObject_0051e0c4_35b8 *tAICaptain::fn_0051e32e()
{
	if (field_0x35b8)
		return field_0x35b8;
	return field_0x35bc;
}

// HYPOTHESIS: appends the value the object yields to the list at 0x94, then calls 0x0051fa75
// unless the flag at 0x35f8 is set.
// FUNCTION: STARFLEETOP 0x0051e33f
void tAICaptain::fn_0051e33f(tShipCommand *param_1)
{
	field_0x94.push_back(param_1->vfunction1());
	if (!field_0x35f8)
	{
		fn_0051fa75();
	}
}

// HYPOTHESIS: returns by value the point the helm officer gives.
// FUNCTION: STARFLEETOP 0x0051e37d
tPoint3 tAICaptain::fn_0051e37d()
{
	return field_0x3810.fn_005462d0();
}

// FUNCTION: STARFLEETOP 0x0051e3a1
void tAICaptain::fn_0051e3a1()
{
	field_0x3810.fn_00516dc1();
}

// HYPOTHESIS: creates the vector at 0x3a34 on first use; sets the value of the entry for the
// ID, or appends a new entry with the value 0x0042750a gives for the ID. Always returns true.
// FUNCTION: STARFLEETOP 0x0051e3ac
bool tAICaptain::fn_0051e3ac(int aID, int aValue)
{
	if (!field_0x3a34)
	{
		field_0x3a34 = new tObject_0051e0c4_3a34;
		field_0x3a34->reserve(5);
	}
	for (tObject_0051e0c4_3a34::iterator it = field_0x3a34->begin(); it != field_0x3a34->end();
		 ++it)
	{
		if (it->fID == aID)
		{
			it->fValue = aValue;
			return true;
		}
	}
	tObject_0051e3ac_entry entry(aID, aValue);
	entry.fRelation = gObject_0090be60->fn_0042748f(aID);
	entry.fFlag = true;
	field_0x3a34->push_back(entry);
	return true;
}

// HYPOTHESIS: returns the vector at 0x3a34 that 0x0051e3ac creates and fills.
// FUNCTION: STARFLEETOP 0x0051e44d
tObject_0051e0c4_3a34 *tAICaptain::fn_0051e44d()
{
	return field_0x3a34;
}

// HYPOTHESIS: the object at 0x35fc of tAICaptain, from 0x00522708; its method 0x00524875
// returns a truth value that lets 0x0051e454 run its update.
class tObject_00524875
{
public:
	bool fn_00524875();
};

// HYPOTHESIS: the object at 0x0090bf18; its method 0x005da4a0 returns a truth value that
// 0x0051e454 needs before it updates anything.
class tObject_005da4a0
{
public:
	bool fn_005da4a0();
};
extern tObject_005da4a0 g_0090bf18;
// HYPOTHESIS: a double global at 0x00c7c148 holding the current time.
extern double g_00c7c148;
// HYPOTHESIS: a byte flag at 0x0096ea14 that 0x0051e454 clears.
extern bool g_0096ea14;

// HYPOTHESIS: the per-frame update; it waits until the time at 0x3a40 is old enough, then
// updates the power object, the officers and the helm officer.
// FUNCTION: STARFLEETOP 0x0051e454
void tAICaptain::vfunction3()
{
	double now = g_00c7c148;
	if (!g_0090bf18.fn_005da4a0())
	{
		if (field_0x3a40)
			field_0x3a40 = g_00c7c148;
		return;
	}
	if (now - field_0x3a40 < 5000000.0)
		return;
	if (!fIsAlive)
		return;
	if (field_0x88 == gObject_008e1060.fn_00525081())
		return;
	if (field_0x35f8)
		return;
	if (field_0x3a38 && field_0x3a38->fn_00525228(field_0x88))
		field_0xd4.fn_00523c41(2);
	else
		field_0xd4.fn_00523c41(0);
	fn_0051fa3c();
	double next = g_0090bef0;
	if (!field_0x35fc->fn_00524875())
		return;
	if (g_0096ea14)
		g_0096ea14 = false;
	field_0xa8 = next;
	fn_0051f1ed();
	fn_0052008f();
	fn_0051f602();
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x88));
	if (ship)
	{
		if (ship->vfunction63() == 0.0 &&
			!(field_0x3a38 && field_0x3a38->fn_00525228(field_0x88) &&
			  field_0x3a38->fn_005251e5(field_0x88) > 1))
			field_0x3608.field_0x178 = 3;
		else
			field_0x3608.field_0x178 = 0;
	}
	fn_0051e613();
	bool result = false;
	if (ship)
	{
		tObject_0043c0f2 *object = ship->field_0xad4->vfunction80();
		if (object)
			result = object->fn_0043bfc1();
	}
	if (!result)
		field_0x3810.fn_005169e0(field_0x35c8.field_0x4, field_0x35c8.field_0x8);
}

// HYPOTHESIS: what the officers in slots 0 and 1 of tAICaptain (ECM and ECCM) share, as
// 0x0051e613 uses it: a flag at 0x4c, a short at 0x4e, and the method 0x005159be.
class tObject_005159be : public tAIOfficer
{
public:
	void fn_005159be();

	char field_0x14[0x38];
	char field_0x4c;                        // +0x4c
	char field_0x4d;                        // +0x4d
	short field_0x4e;                       // +0x4e
};

// HYPOTHESIS: a byte flag at 0x0096e92c that 0x0051e613 sets once.
extern bool g_0096e92c;
// HYPOTHESIS: a byte at 0x0085922a; 0x0051e613 drives the other officers only while it is 1.
extern char g_0085922a;

inline void tAIHelmOfficer::fn_0051e613_inline(int aMode)
{
	field_0x5c = aMode;
	field_0x60 = -1;
	field_0x64 = -1;
	field_0x69 = false;
	field_0x7c = 0;
}

// HYPOTHESIS: the part of the per-frame update that 0x0051e454 calls after its officers. It
// builds a selector for the target ship at 0xc8, sets the helm officer's mode from the
// distance and the defensive officer's mode, then rates the four headings with the helm,
// offensive and defensive officers, keeps the best in the selector at 0x35c8 and hands it to
// every officer.
// FUNCTION: STARFLEETOP 0x0051e613
void tAICaptain::fn_0051e613()
{
	tObject_00528a11 selector(field_0x88, 0, 0, field_0xc8, 0);
	bool done = false;
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x88));
	if (ship)
	{
		tObject_0043c0f2 *object = ship->field_0xad4->vfunction80();
		if (object)
			done = object->fn_0043bfc1();
	}
	tObject_0042496f *target = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0xc8));
	field_0xd4.fn_00523ad3();
	if (!done && field_0x396c[7])
	{
		// HYPOTHESIS: the selector is typed int * where the officers read it.
		field_0x396c[7]->vfunction2((int *)&selector, field_0xc8);
		if (((tAIHETOfficer *)field_0x396c[7])->fn_0051819f())
		{
			field_0x3810.fn_0051692e();
			done = true;
		}
	}
	if (!g_0096e92c)
	{
		if (field_0x3608.field_0x19c)
			field_0x3a44 = 1;
		g_0096e92c = true;
	}
	field_0x3810.field_0x13c = field_0x35b4;
	field_0x3608.fn_00420f2f(field_0xc8);
	field_0x37a8.field_0x28 = field_0xc8;
	float distance = 0.0f;
	if (target)
		distance = gObject_0096eca8.fn_0052944d(0.0f, (int *)&selector);
	int mode = field_0x37a8.fn_00515288();
	if (!ship->fn_00419c87() && ship->field_0x874 != 1)
	{
		if (distance > 200.0f)
		{
			field_0x3810.fn_0051e613_inline(1);
			field_0xb0 = field_0xa8;
		}
		else if (field_0xa8 - field_0xb0 > 1.5)
		{
			double ratio = (double)g_0090be9c->vfunction7() / (double)g_0090be9c->field_20;
			field_0x3a3e = 0;
			if (g_0090be9c->fn_004105b4() < field_0x3a30)
			{
				if (ratio < 0.3333)
					field_0x3810.fn_0051e613_inline(0);
				else if (ratio < 0.6666)
					field_0x3810.fn_0051e613_inline(1);
				else
					field_0x3810.fn_0051e613_inline(2);
			}
			field_0xb0 = field_0xa8;
		}
		else if (mode == 2 || mode == 4 || mode == 3)
		{
			if (g_0090be9c->fn_004105b4() < field_0x3a30)
			{
				field_0x3a3e = 0;
				if (field_0x3810.field_0x5c == 0)
				{
					field_0x3810.fn_0051e613_inline(
						(double)g_0090be9c->vfunction7() / (double)g_0090be9c->field_20 < 0.5
							? 2
							: 1);
					field_0xb0 = field_0xa8;
				}
				else if (field_0xa8 - field_0xb0 > 1.0 && field_0x3810.field_0x5c == 2)
				{
					field_0x3810.fn_0051e613_inline(1);
					field_0xb0 = field_0xa8;
				}
			}
		}
		else if (mode == 0 && !field_0x3a3e)
		{
			field_0x3a3e = g_0090be9c->fn_004105b4() < field_0x3a30;
		}
	}
	else
	{
		field_0x3810.field_0x5c = 0;
		field_0x3810.field_0x69 = false;
		field_0x3810.field_0x7c = 0;
		field_0x3810.field_0x60 = -1;
		field_0x3810.field_0x64 = -1;
	}
	int value = field_0x37a8.fn_005152ae();
	field_0x3810.field_0x6c = mode;
	field_0x3810.field_0x70 = value;
	field_0x3810.fn_00516879();

	tObject_00528a11 best = selector;
	float bestScore = -1.0f;
	int bestHeading = -1;
	field_0x3a3d = 0;
	if (done)
	{
		field_0x3810.fn_00516e2a();
		field_0x3608.fn_00518f89();
		best.field_0x4 = 0;
		best.field_0x8 = 2;
	}
	else if (field_0x3600)
	{
		field_0x3810.fn_00516e2a();
		field_0x3608.fn_00518f89();
		best.field_0x4 = 0;
		best.field_0x8 = 3;
	}
	else if (field_0x35b4 == 11 && (ship->fn_00419c87() || ship->field_0x874 == 1))
	{
		float most = 0.0f;
		for (int heading = 0; heading < 4; heading++)
		{
			selector.field_0x4 = 0;
			selector.field_0x8 = heading;
			int kind;
			float score = field_0x3810.fn_00516dc6(0, heading, &kind);
			if (score >= most)
			{
				best = selector;
				bestHeading = heading;
				most = score;
			}
		}
	}
	else
	{
		float closeRatio = 0.0f;
		field_0x3608.fn_00518f89();
		float closeCount = field_0x3608.fn_00519a88();
		float nearCount = field_0x3608.fn_00519ac3(0.3f);
		float total = field_0x3608.fn_00519b08();
		if (total > 0.0)
			closeRatio = closeCount / total;
		float shipSpeed = ship->vfunction24();
		float targetSpeed = 0.0f;
		if (target)
			targetSpeed = target->vfunction24();
		bool faster = false;
		bool slower = false;
		if (field_0x3a30 > 0.4)
		{
			if (shipSpeed > 0.9 && targetSpeed > 0.9)
			{
				float speedRatio = (ship->field_0x79c * shipSpeed) /
					(target->field_0x79c * targetSpeed);
				faster = speedRatio > 1.25;
				slower = speedRatio < 0.75;
			}
			else
			{
				faster = shipSpeed - targetSpeed >= 0.25;
				slower = targetSpeed - shipSpeed >= 0.25;
			}
		}
		if (!ship->fn_00419c87())
		{
			float limit = 0.0f;
			float level = 0.0f;
			float part = field_0x3608.fn_00519b43(0.3f);
			float whole = field_0x3608.fn_00519b88();
			float share = 0.0f;
			if (ship->field_0xad4->vfunction90())
			{
				limit = ship->field_0xad4->vfunction90()->vfunction87();
				level = ship->field_0xad4->vfunction90()->vfunction86();
			}
			if (whole > 0.0)
				share = part / whole;
			if (field_0x3a30 > 0.5 && !field_0x3a45 && !slower && share < 0.4f &&
				level * 0.65f < limit && ship->field_0x86c >= 8 && ship->field_0x86c <= 16)
			{
				field_0x3a45 = 1;
				ship->field_0xad4->vfunction77()->fn_0043b7fe();
			}
			else if (field_0x3a30 > 0.5 && field_0x3a45 == 1 &&
				(slower || share > 0.65f || level * 0.65f > limit) &&
				ship->field_0x86c >= 8 && ship->field_0x86c <= 16)
			{
				field_0x3a45 = 0;
				ship->field_0xad4->vfunction77()->fn_0043b7fe();
			}
		}
		for (int heading = 0; heading < 4; heading++)
		{
			selector.field_0x4 = 0;
			selector.field_0x8 = heading;
			int kind;
			float helm = field_0x3810.fn_00516dc6(0, heading, &kind) * field_0x35ec;
			float score = 0.0f;
			float defense = 0.0f;
			if (kind == 2)
				continue;
			if (field_0xc8 != -1 && target && kind != 1 && field_0xc4 == field_0xc8)
			{
				score = field_0x3608.vfunction15((int *)&selector, field_0xc8) * field_0x35f0;
				defense = field_0x37a8.vfunction15((int *)&selector, field_0xc8) * field_0x35f4;
			}
			if (distance > 200.0)
			{
				score = helm;
			}
			else if (distance > 100.0)
			{
				score = defense + helm;
			}
			else
			{
				if (mode != 0 && !((mode == 5 || mode == 1) && field_0x3a3e))
				{
					if (closeRatio > 0.75f)
						score = helm * 1.5 + defense + score;
					else if (closeRatio > 0.25f)
						score = defense + helm;
					else
						score = defense * 2;
				}
				else
				{
					if (distance < (faster ? 40.0 : 80.0))
						helm = helm / (81.0f - distance);
					if (closeRatio > 0.75f)
						score = (score + helm) * 1.5 + defense;
					else if (closeRatio > 0.5f)
						score = (score + helm) * 1.2 + defense;
					else if (closeRatio > 0.25f)
						score = defense + score + helm;
					else
						score = defense * 2;
				}
				if (faster || nearCount > 0.25f)
					score = score + helm;
				if (slower)
					score = score + defense;
			}
			if (score > bestScore ||
				(score == bestScore &&
				 (field_0x3602 == heading ||
				  (bestHeading != field_0x3602 && g_0090be9c->fn_004105b4() > 0.5))))
			{
				best = selector;
				bestHeading = heading;
				bestScore = score;
			}
		}
	}
	field_0x35c8 = best;
	field_0x3602 = bestHeading;
	field_0xd4.fn_00523afd(&field_0x35c8);
	if (!done)
		field_0x3608.vfunction2((int *)&field_0x35c8, field_0xc8);
	field_0x37a8.vfunction2((int *)&field_0x35c8, field_0xc8);
	if (g_0085922a == 1)
	{
		bool jam = true;
		int size = ship->field_0x86c;
		if (size == 0 || size == 1 || size == 6 || size == 4)
			jam = field_0x3608.fn_00519bc3() < (short)(field_0x3608.fn_00519a88() >> 2);
		tObject_005159be *ecm = (tObject_005159be *)field_0x396c[0];
		tObject_005159be *eccm = (tObject_005159be *)field_0x396c[1];
		if (jam)
		{
			if (ecm)
			{
				ecm->field_0x4c = 1;
				ecm->vfunction2((int *)&field_0x35c8, field_0xc8);
				ecm->fn_005159be();
			}
			if (eccm)
			{
				eccm->field_0x4c = 0;
				eccm->field_0x4e = 0;
				eccm->vfunction2((int *)&field_0x35c8, field_0xc8);
			}
		}
		else
		{
			if (eccm)
			{
				eccm->field_0x4c = 1;
				eccm->vfunction2((int *)&field_0x35c8, field_0xc8);
				eccm->fn_005159be();
			}
			if (ecm)
			{
				ecm->field_0x4c = 0;
				ecm->field_0x4e = 0;
				ecm->vfunction2((int *)&field_0x35c8, field_0xc8);
			}
		}
		for (int i = 0; i < 13; i++)
		{
			if (i != 7 && i != 0 && i != 1 && field_0x396c[i])
				field_0x396c[i]->vfunction2((int *)&field_0x35c8, field_0xc8);
		}
	}
	field_0x3a3d = 0;
	if (field_0x3a38)
		field_0x3a38->fn_005253e8(field_0x88, 1);
}

// HYPOTHESIS: sets the flag at 0x35f8 that makes 0x0051e33f and 0x0051e454 return early.
// FUNCTION: STARFLEETOP 0x0051f0c3
void tAICaptain::fn_0051f0c3(char aValue)
{
	field_0x35f8 = aValue;
}

// FUNCTION: STARFLEETOP 0x0051f0d0
char tAICaptain::fn_0051f0d0()
{
	return field_0x35f8;
}

// HYPOTHESIS: the body of the function whose exception-handling prolog is 0x0051f0d7 (a listed boundary error).
// Written whole, so its build starts with that 10-byte prolog and the rest follows the
// original instruction for instruction from 0x0051f0e1 on. With the flag set it tells the offensive officer 0 through 0x00519130 and orders the ship to
// disengage; otherwise it clears the ship's current orders.
// FUNCTION: STARFLEETOP 0x0051f0d7
void tAICaptain::fn_0051f0e1(char aValue)
{
	if (aValue)
	{
		field_0x3608.fn_00519130(0);
		tCommandDisengage command(1, 10);
		fn_0051e33f(&command);
	}
	else
	{
		tCommandClearCurrentOrders command(0);
		fn_0051e33f(&command);
	}
}

// HYPOTHESIS: forwards a value to the officer with the given index, when there is one.
// FUNCTION: STARFLEETOP 0x0051f153
void tAICaptain::fn_0051f153(int aIndex, int aValue)
{
	if (field_0x396c[aIndex])
		field_0x396c[aIndex]->vfunction4(aValue);
}

// HYPOTHESIS: forwards a value to the cloak officer (slot 8), when there is one.
// FUNCTION: STARFLEETOP 0x0051f16e
void tAICaptain::fn_0051f16e(int aValue)
{
	if (field_0x396c[8])
		((tAICloakOfficer *)field_0x396c[8])->fn_00514bea(aValue);
}

// HYPOTHESIS: sets the ship's formation: the object for the value at 0x8c takes it when it has
// none, then the helm, the offensive officer and every other officer are given it.
// FUNCTION: STARFLEETOP 0x0051f184
void tAICaptain::fn_0051f184(tObject_00525081 *aFormation)
{
	tObject_00525081 *&formation = g_0090bed4->fn_00523574(field_0x8c)->field_0x114;
	if (!formation)
		formation = aFormation;
	field_0x3a38 = aFormation;
	field_0x3810.fn_00517edc(aFormation);
	tAIOfficer *offensive = &field_0x3608;
	offensive->vfunction5(aFormation);
	for (int i = 0; i < 13; i++)
	{
		if (field_0x396c[i])
			field_0x396c[i]->vfunction5(aFormation);
	}
}

// HYPOTHESIS: the object at 0xc4 of the object slot 76 of a ship's 0xad4 returns; 0x0051f1ed
// sets the integer at 0x18, then calls 0x006712c0 with 3 and 0.
class tObject_006712c0
{
public:
	void fn_006712c0(int param_1, int param_2);

	char field_0x0[0x18];                   // +0x0
	int field_0x18;                         // +0x18
};

// HYPOTHESIS: what slot 76 of a ship's 0xad4 returns, as 0x0051f1ed uses it while docking: a
// flag at 0x128, the object at 0xc4, the method 0x00454803 and slots 62 and 63.
class tObject_0042496f_member76
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
	// HYPOTHESIS: slot 62 (offset 0xf4) starts docking, slot 63 (offset 0xf8) stops it.
	virtual void vfunction62();
	virtual void vfunction63();

	void fn_00454803(int param_1);

	char field_0x4[0xc0];                   // +0x4
	tObject_006712c0 field_0xc4;            // +0xc4
	char field_0xe0[0x48];                  // +0xe0
	// HYPOTHESIS: set once the ship has docked.
	bool field_0x128;                       // +0x128
};

// HYPOTHESIS: carries out the current order at 0x35b4: moves on along the patrol route or ends a
// move once the ship is within 10 of its point, docks with the ship of the object at 0x35bc in
// steps kept at 0x35c4, and ends an order whose target ship is gone through 0x0051f559. The
// point at 0xb8 is the one the ship is sent to; 0x39a4 and 0x39a5 are the number of points of a
// patrol route and the index of the current one, and the route's points, ten tPoint3 of 12 bytes,
// follow at 0x39a6. Both are kept as bytes in the class: tPoint3 members there would make the
// constructor build them, which the original does not. 0x0051ffae takes the next point by value;
// 0x0051ff8b takes the ID of the ship to dock with and 1.
// FUNCTION: STARFLEETOP 0x0051f1ed
void tAICaptain::fn_0051f1ed()
{
	tObject_004b94dc *obj = g_0090bed4->fn_00523574(field_0x8c);
	if (!obj)
		return;
	switch (field_0x35b4)
	{
	case 3:
	{
		tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x88));
		if (ship && ship->field_0xad4->vfunction27()->field_0x18 == 0.0f)
		{
			field_0x3810.fn_00515db1();
			field_0x35b4 = -1;
			field_0x35c0 = 0;
		}
		break;
	}
	case 2:
	case 10:
	case 11:
		if (gObject_008e1060.fn_004247dd(field_0x88, *(tPoint3 *)field_0xb8) < 10.0f)
		{
			if (field_0x35b4 == 2)
			{
				if (++field_0x39a5 >= field_0x39a4)
					field_0x39a5 = 0;
				// HYPOTHESIS: ten tPoint3 points, 12 bytes each.
				fn_0051ffae(((tPoint3 *)field_0x39a6)[field_0x39a5]);
			}
			else
			{
				field_0x3810.fn_00515db1();
				field_0x35b4 = -1;
				field_0x35c0 = 0;
			}
		}
		break;
	case 14:
	{
		tObject_0042496f *target = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0xc4));
		if (!target || !target->field_0x24)
			fn_0051f559();
		break;
	}
	case 15:
	{
		tObject_0051e0c4_35b8 *dock = field_0x35bc;
		tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x88));
		tObject_0042496f *other = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(dock->field_0x10));
		if (!ship || !other || !other->field_0x24)
		{
			fn_0051f559();
			break;
		}
		switch (field_0x35c4)
		{
		case 1:
			// HYPOTHESIS: 0x004246e4 takes entities; the ships are passed as they are.
			if (gObject_008e1060.fn_004246e4((tEntity *)ship, (tEntity *)other) < 10.0f)
			{
				field_0x3810.fn_00515db1();
				int count = other->field_0x108;
				if (count < 1)
					count = 1;
				ship->vfunction13(dock->field_0x10);
				tObject_0042496f_member76 *docking = ship->field_0xad4->vfunction76();
				if (docking)
				{
					tObject_006712c0 &timer = docking->field_0xc4;
					timer.field_0x18 = count;
					timer.fn_006712c0(3, 0);
					docking->fn_00454803(0);
					docking->vfunction62();
				}
				field_0x35c4 = 2;
			}
			break;
		case 2:
		{
			tObject_0042496f_member76 *docking = ship->field_0xad4->vfunction76();
			if (docking && docking->field_0x128)
			{
				field_0x3810.fn_00517e1a(dock->field_0x14);
				field_0x35c4 = 3;
			}
			break;
		}
		case 3:
		{
			tObject_0042496f_member76 *docking = ship->field_0xad4->vfunction76();
			if (docking && !docking->field_0x128)
			{
				field_0x35c4 = 1;
				fn_0051ff8b(dock->field_0x10, 1);
			}
			else if (gObject_008e1060.fn_004247dd(field_0x88, dock->field_0x14) < 10.0f)
			{
				if (docking)
					docking->vfunction63();
				field_0x3810.fn_00515db1();
				field_0x35c4 = 0;
				fn_0051f559();
			}
			break;
		}
		}
		break;
	}
	case 35:
	case 45:
	{
		tObject_0042496f *target = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0xc4));
		if (!target || !target->field_0x24 || !obj->fn_0051cb40(target, 1))
			fn_0051f559();
		break;
	}
	// HYPOTHESIS: orders 30 and 31 are written as two cases with the same body; the switch tests
	// them one by one as two cases, not as one range, and the compiler merges the bodies.
	case 30:
	{
		tObject_0042496f *target = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0xc4));
		if (!target || !target->field_0x24 || !obj->fn_0051cb40(target, 0))
			fn_0051f559();
		break;
	}
	case 31:
	{
		tObject_0042496f *target = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0xc4));
		if (!target || !target->field_0x24 || !obj->fn_0051cb40(target, 0))
			fn_0051f559();
		break;
	}
	}
}

// HYPOTHESIS: three values 0x0051f559 hands to 0x0051c280 of the object 0x00523574 returns:
// 0, the ship ID and the captain's own ID; the fourth word is not set.
struct tObject_0051c280_arg
{
	int field_0x0;                          // +0x0
	int field_0x4;                          // +0x4
	tID field_0x8;                          // +0x8
	int field_0xc;                          // +0xc
	int field_0x10;                         // +0x10
};

// HYPOTHESIS: ends the current order: deletes the object at 0x35bc, clears the order and its
// targets, stops the helm, then queues the object at 0x35b8 on the list at 0x94 when there is one,
// else tells the object for the value at 0x8c.
// FUNCTION: STARFLEETOP 0x0051f559
void tAICaptain::fn_0051f559()
{
	if (field_0x35bc)
	{
		delete field_0x35bc;
		field_0x35bc = 0;
	}
	field_0x35b4 = -1;
	field_0xc4 = -1;
	field_0xc8 = -1;
	field_0x35c0 = 0;
	field_0x3810.fn_00515db1();
	if (field_0x35b8)
	{
		// HYPOTHESIS: the list holds the order objects as integers.
		field_0x94.push_back(*(int *)&field_0x35b8);
		field_0x35b8 = 0;
	}
	else
	{
		tObject_004b94dc *obj = g_0090bed4->fn_00523574(field_0x8c);
		if (obj)
		{
			tObject_0051c280_arg arg;
			arg.field_0x0 = 0;
			arg.field_0x8 = fID;
			arg.field_0x4 = field_0x88;
			obj->fn_0051c280(&arg);
		}
	}
}

// HYPOTHESIS: what slot 96 of a ship's 0xad4 returns; 0x0051f602 treats a target as hidden
// when slot 102 (offset 0x194) returns 6 and leaves the flag it is given clear.
class tObject_0042496f_member96
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
	virtual void vfunction65();
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
	virtual int vfunction102(bool *aFlag);
};

#include "unsorted/tObject_008f5170.h"








// HYPOTHESIS: chooses the target at 0xc8 and hands it to the offensive officer and the ship. A
// formation member in role 1 or 3 takes the formation's target. Otherwise, on patrol (order 2)
// or while escorting (order 14), the first enemy found around the ship or the escorted ship is
// reported to the object for the value at 0x8c; then the target at 0xc4 is kept while it is in
// range and not hidden, else the object for the value at 0x8c chooses one.
// FUNCTION: STARFLEETOP 0x0051f602
void tAICaptain::fn_0051f602()
{
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x88));
	if (!ship)
		return;
	field_0xc8 = -1;
	int role = 0;
	if (field_0x3a38)
		role = field_0x3a38->fn_0052518f(field_0x88);
	if (field_0x3a38 && field_0x3a38->fn_005251e5(field_0x88) >= 2 &&
		field_0x3a38->fn_00525228(field_0x88) && (role == 1 || role == 3))
	{
		field_0xc8 = field_0x3a38->fn_00525338();
	}
	else
	{
		tObject_004b94dc *obj = g_0090bed4->fn_00523574(field_0x8c);
		if (!obj)
			return;
		tObject_0042496f *target = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0xc4));
		if (field_0x35b4 == 11 && (ship->fn_00419c87() || ship->field_0x874 == 1))
			return;
		int order = field_0x35b4;
		if (order == 2)
		{
			std::vector<tPhysicalEntity *> *list =
				gObject_008f5170.fn_004255b5(ship->fn_00422cba(), field_0x39a0 * 10.0f);
			for (unsigned int i = 0; i < list->size(); i++)
			{
				tPhysicalEntity *entity = gObject_008e1060.fn_004249c0((*list)[i]);
				// HYPOTHESIS: 0x0051cb40 takes the entity as a ship.
				if (entity && obj->fn_0051cb40((tObject_0042496f *)entity, 0))
				{
					tObject_0051c280_arg arg;
					arg.field_0x0 = 1;
					arg.field_0x8 = fID;
					arg.field_0x4 = field_0x88;
					arg.field_0xc = ((tEntity *)entity)->fID;
					arg.field_0x10 = field_0x35c0;
					obj->fn_0051c280(&arg);
					break;
				}
			}
		}
		else if (order == 14)
		{
			tObject_0051e0c4_35b8 *escort = field_0x35bc;
			tPhysicalEntity *escorted = gObject_008e1060.fn_004249d3(escort->field_0x10);
			if (escorted)
			{
				// HYPOTHESIS: the escorted entity is a ship.
				std::vector<tPhysicalEntity *> *list = gObject_008f5170.fn_004255b5(
					((tObject_0042496f *)escorted)->fn_00422cba(), escort->field_0x14.x * 10.0f);
				for (unsigned int i = 0; i < list->size(); i++)
				{
					tPhysicalEntity *entity = gObject_008e1060.fn_004249c0((*list)[i]);
					if (entity && ((tEntity *)entity)->fID != escort->field_0x10 &&
						obj->fn_0051cb40((tObject_0042496f *)entity, 0))
					{
						tObject_0051c280_arg arg;
						arg.field_0x0 = 1;
						arg.field_0x8 = fID;
						arg.field_0x4 = field_0x88;
						arg.field_0xc = ((tEntity *)entity)->fID;
						arg.field_0x10 = field_0x35c0;
						obj->fn_0051c280(&arg);
						break;
					}
				}
			}
		}
		else if (order == 15)
		{
			if (field_0x35c4 == 2 || field_0x35c4 == 3)
				return;
		}
		bool hidden = false;
		if (target && target->field_0xad4)
		{
			tObject_0042496f_member96 *state = target->field_0xad4->vfunction96();
			bool flag = false;
			if (state && state->vfunction102(&flag) == 6 && !flag)
				hidden = true;
		}
		if (!hidden && !field_0xd0 && field_0xc4 != -1 &&
			gObject_008e1060.fn_004246ab(fID, field_0xc4) < 200.0f)
		{
			field_0xc8 = field_0xc4;
		}
		else
		{
			tObject_0051c81f_result result =
				obj->fn_0051c81f(fID, 1, 0, ship->field_0x870 == 0x16);
			field_0xc8 = result.field_0x0;
		}
	}
	field_0x3608.fn_00420f2f(field_0xc8);
	ship->vfunction13(field_0xc8);
}

// HYPOTHESIS: sets the target at 0xc8 to the ID given, hands it to the offensive officer and the
// ship, then passes 2 (flag set) or 1 to the officer's 0x00519130 and sets the order at 0x35b4
// to 23 or 22.
// FUNCTION: STARFLEETOP 0x0051f958
void tAICaptain::fn_0051f958(int aID, char aFlag)
{
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x88));
	if (!ship)
		return;
	field_0xc8 = aID;
	field_0x3608.fn_00420f2f(field_0xc8);
	ship->vfunction13(field_0xc8);
	if (aFlag)
	{
		field_0x3608.fn_00519130(2);
		field_0x35b4 = 23;
	}
	else
	{
		field_0x3608.fn_00519130(1);
		field_0x35b4 = 22;
	}
}

// HYPOTHESIS: sets the order at 0x35b4 to 14 and passes the ID given, with 1, to 0x0051ffa0.
// FUNCTION: STARFLEETOP 0x0051f9d1
void tAICaptain::fn_0051f9d1(int aID)
{
	field_0x35b4 = 14;
	fn_0051ffa0(aID, 1);
}

// HYPOTHESIS: sets the order at 0x35b4 to 45 and keeps the ID given at 0xcc; if the ship has a
// usable tractor officer, -1 calls its slot 7, any other ID goes to 0x0051ff8b with 1 and then
// to the officer's slot 6.
// FUNCTION: STARFLEETOP 0x0051f9e9
void tAICaptain::fn_0051f9e9(int aID)
{
	tAIOfficer *tractor = field_0x396c[2];
	field_0x35b4 = 45;
	field_0xcc = aID;
	if (tractor && tractor->vfunction3())
	{
		if (aID == -1)
			tractor->vfunction7();
		else
		{
			fn_0051ff8b(aID, 1);
			tractor->vfunction6();
		}
	}
}

// HYPOTHESIS: what slot 99 of a ship's 0xad4 returns; 0x0051fa3c calls its 0x00439e98 with the
// ship's entity ID at slot 13 set to the one order 17 names.
class tObject_0042496f_member99
{
public:
	void fn_00439e98();
};

// HYPOTHESIS: works through the orders queued in the list at 0x94 and empties it. An order with
// the flag at 0xc set is carried out at once and deleted; one of lower priority than 0x35c0 that
// does not wait (flag 0xd clear) is dropped; any other becomes the current order at 0x35bc (a
// waiting one keeps the order at 0x35b8, else that is dropped and its priority taken), its kind is
// copied to 0x35b4 and started, and 0x0051fce4 follows. The data from 0x10 on depends on the kind.
// FUNCTION: STARFLEETOP 0x0051fa3c
void tAICaptain::fn_0051fa3c()
{
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x88));
	if (!ship)
		return;
	for (std::list<int>::iterator it = field_0x94.begin(); it != field_0x94.end();
		 it = field_0x94.erase(it))
	{
		// HYPOTHESIS: the list holds pointers to orders; it stays list<int> as the constructor
		// declares it.
		if (((tObject_0051e0c4_35b8 *)*it)->field_0xc == 1)
		{
			switch (((tObject_0051e0c4_35b8 *)*it)->field_0x4)
			{
			case 17:
			{
				tObject_0042496f_member99 *obj = ship->field_0xad4->vfunction99();
				if (obj)
				{
					tObject_0051e0c4_35b8 *order = (tObject_0051e0c4_35b8 *)*it;
					int saved = ship->vfunction12();
					ship->vfunction13(order->field_0x10);
					obj->fn_00439e98();
					ship->vfunction13(saved);
				}
				break;
			}
			case 22:
				field_0x3608.fn_00519130(1);
				break;
			case 23:
				field_0x3608.fn_00519130(2);
				break;
			case 24:
				field_0x3608.fn_00519130(3);
				break;
			}
			if (*it)
			{
				delete (tObject_0051e0c4_35b8 *)*it;
				*it = 0;
			}
			continue;
		}
		else if (((tObject_0051e0c4_35b8 *)*it)->field_0xd ||
				 ((tObject_0051e0c4_35b8 *)*it)->field_0x8 >= field_0x35c0)
		{
			if (!((tObject_0051e0c4_35b8 *)*it)->field_0xd)
			{
				field_0x35c0 = ((tObject_0051e0c4_35b8 *)*it)->field_0x8;
				delete field_0x35b8;
				field_0x35b8 = 0;
			}
			else if (!field_0x35b8)
			{
				field_0x35b8 = field_0x35bc;
				field_0x35bc = 0;
			}
			if (field_0x35bc)
				delete field_0x35bc;
			field_0x35bc = (tObject_0051e0c4_35b8 *)*it;
			field_0x35b4 = field_0x35bc->field_0x4;
			tObject_0051e0c4_35b8 *order = (tObject_0051e0c4_35b8 *)*it;
			switch (order->field_0x4)
			{
			case 2:
				// HYPOTHESIS: a float, a count and that many bytes from 0x10 on.
				if (*((unsigned char *)order + 0x14) > 0)
					fn_00520004(*(float *)&order->field_0x10, *((unsigned char *)order + 0x14),
								(char *)order + 0x15);
				break;
			case 3:
				field_0x3810.fn_00515db1();
				break;
			case 10:
				// HYPOTHESIS: a point at 0x10.
				fn_0051ffae(*(tPoint3 *)&order->field_0x10);
				break;
			case 11:
				if (!ship->fn_00419c87() && ship->field_0x874 != 1)
				{
					tPoint3 point = fn_0051feca(ship);
					fn_0051ffae(point);
				}
				else
					fn_0051ff8b(ship->field_0xad4->vfunction165(), 1);
				break;
			case 13:
				fn_0051ff8b(order->field_0x10, 1);
				break;
			case 14:
				fn_0051ff8b(order->field_0x10, 1);
				break;
			case 15:
				field_0x35c4 = 1;
				fn_0051ff8b(order->field_0x10, 1);
				break;
			case 16:
				// HYPOTHESIS: a flag at 0x10.
				if (field_0x3a34 && *(char *)&order->field_0x10)
					field_0x3a34->clear();
				fn_0051f559();
				break;
			case 30:
				fn_0051ff8b(order->field_0x10, 0);
				break;
			case 31:
				fn_0051ff8b(order->field_0x10, 0);
				break;
			case 35:
				fn_0051ff8b(order->field_0x10, 0);
				break;
			}
			fn_0051fce4();
			continue;
		}
		if (*it)
		{
			delete (tObject_0051e0c4_35b8 *)*it;
			*it = 0;
		}
	}
}

// HYPOTHESIS: sets the weights at 0x35ec, 0x35f0 and 0x35f4 that 0x0051e454 multiplies the helm,
// offensive and defensive scores by, for the current order at 0x35b4 and the value at 0x3a1e,
// and hands the helm officer a float through 0x005169d6. A docking order (14) whose flag at 0x1c
// is set also gets two ranges at 0x14 and 0x18.
// FUNCTION: STARFLEETOP 0x0051fce4
void tAICaptain::fn_0051fce4()
{
	switch (field_0x35b4)
	{
	case 11:
		field_0x35ec = 1.0f;
		field_0x35f0 = 0.1f;
		field_0x35f4 = 0.8f;
		field_0x3810.fn_005169d6(0.0f);
		break;
	case 14:
		// HYPOTHESIS: for this order the point at 0x14 holds two ranges and a flag.
		{
			tObject_0051e0c4_35b8 *order = field_0x35bc;
			if (*(char *)&order->field_0x14.z)
			{
				order->field_0x14.x = 120.0f;
				if (field_0x3a1e > 0)
					order->field_0x14.y = 160.0f;
				else if (field_0x3a1e == 0)
					order->field_0x14.y = 200.0f;
				else
					order->field_0x14.y = 400.0f;
			}
		}
	case 2:
	case 10:
	case 13:
	case 15:
		field_0x3810.fn_005169d6(0.0f);
		if (field_0x3a1e > 0)
		{
			field_0x35f0 = 0.0f;
			field_0x35f4 = 1.0f;
			field_0x35ec = 1.0f;
		}
		else if (field_0x3a1e == 0)
		{
			field_0x35f0 = 0.5f;
			field_0x35f4 = 0.5f;
			field_0x35ec = 1.0f;
		}
		else
		{
			field_0x35f0 = 0.9f;
			field_0x35f4 = 0.1f;
			field_0x35ec = 1.0f;
		}
		break;
	case 30:
	case 35:
		if (field_0x3a1e > 0)
		{
			field_0x35f4 = 0.4f;
			field_0x35ec = 0.6f;
			field_0x35f0 = 1.0f;
			field_0x3810.fn_005169d6(0.0f);
		}
		else if (field_0x3a1e == 0)
		{
			field_0x35ec = 1.0f;
			field_0x35f0 = 0.8f;
			field_0x35f4 = 0.6f;
			field_0x3810.fn_005169d6(40.0f);
		}
		else
		{
			field_0x35f0 = 0.6f;
			field_0x35f4 = 0.8f;
			field_0x35ec = 1.0f;
			field_0x3810.fn_005169d6(90.0f);
		}
		break;
	case 31:
		if (field_0x3a1e > 0)
		{
			field_0x35ec = 1.0f;
			field_0x35f0 = 0.7f;
			field_0x35f4 = 0.6f;
			field_0x3810.fn_005169d6(50.0f);
		}
		else if (field_0x3a1e == 0)
		{
			field_0x35ec = 1.0f;
			field_0x35f0 = 0.6f;
			field_0x35f4 = 0.8f;
			field_0x3810.fn_005169d6(70.0f);
		}
		else
		{
			field_0x35f0 = 0.5f;
			field_0x35f4 = 1.0f;
			field_0x35ec = 1.0f;
			field_0x3810.fn_005169d6(90.0f);
		}
		break;
	}
}

// HYPOTHESIS: the point on the edge of the mission map nearest the ship given, one square
// off the map; 0x0051fa3c sends the ship there.
// MATCH-ONLY: included here rather than at the top, where one more line would move the line
// records the harness reads for the constructor's annotation.
#include "tMissionMap.h"

// FUNCTION: STARFLEETOP 0x0051feca
tPoint3 tAICaptain::fn_0051feca(tObject_0042496f *aShip)
{
	tMapPoint size = tMissionMap::mGetMapDimension();
	tMapPoint here = g_0090b734->m3DToMapPoint(aShip->fn_00422cba());
	tMapPoint edge = here;
	edge.x = -1;
	short distance = here.x;
	if (here.y < distance)
	{
		edge.y = -1;
		edge.x = here.x;
		distance = here.y;
	}
	if (size.x - here.x < distance)
	{
		edge.x = size.x;
		edge.y = here.y;
		distance = size.x - here.x;
	}
	if (size.y - here.y < distance)
	{
		edge.x = here.x;
		edge.y = size.y;
	}
	tPoint3 point = g_0090b734->mConvertLocation(edge);
	return point;
}

// FUNCTION: STARFLEETOP 0x0051ff8b
void tAICaptain::fn_0051ff8b(int aTargetID, char aFlag)
{
	field_0xd0 = aFlag;
	field_0xc4 = aTargetID;
	field_0x3810.fn_00517db0(aTargetID);
}

// FUNCTION: STARFLEETOP 0x0051ffae
void tAICaptain::fn_0051ffae(tPoint3 aPoint)
{
	*(tPoint3 *)field_0xb8 = aPoint;
	field_0x3810.fn_00517e1a(*(tPoint3 *)field_0xb8);
	field_0xc4 = -1;
}

// FUNCTION: STARFLEETOP 0x00520004
void tAICaptain::fn_00520004(float aValue, unsigned char aCount, char *aData)
{
	if (aCount > 0)
	{
		field_0x39a0 = aValue;
		field_0x39a5 = 0;
		field_0x39a4 = aCount;
		// HYPOTHESIS: aData holds aCount tPoint3 points, copied into the patrol list at 0x39a6.
		for (int i = 0; i < field_0x39a4; i++)
			((tPoint3 *)field_0x39a6)[i] = ((tPoint3 *)aData)[i];
		fn_0051ffae(((tPoint3 *)field_0x39a6)[field_0x39a5]);
	}
}

// FUNCTION: STARFLEETOP 0x0052008f
void tAICaptain::fn_0052008f()
{
	tObject_0051e0c4_35b8 *order = field_0x35b8;
	// HYPOTHESIS: an order of kind 0xe keeps a range in the float at +0x18 (the y of the point
	// at +0x14 in other orders); past that range from the ship it names, 0x0051f559 runs.
	if (order != 0 && order->field_0x4 == 0xe
		&& gObject_0096eca8.fn_00528fbc(field_0x88, order->field_0x10) > order->field_0x14.y)
	{
		fn_0051f559();
	}
}

// MATCH-ONLY: included here rather than at the top, where one more line would move the line
// records the harness reads for the constructor's annotation.
#include "Source/entity/entity_physical.h"

// HYPOTHESIS: three debug switches; 0x005200c8 draws the AI's lines only while the first is
// set, the ship's headings with the second and its targets with the third.
extern bool g_0096ea16;
extern bool g_0096ea17;
extern bool g_0096ea18;

// HYPOTHESIS: draws the captain's state through the object given (the same slot as
// tPhysicalEntity's model drawing): the ship's heading, its two targets with a square
// around each, four points the target may move to, and the point the captain steers to.
// FUNCTION: STARFLEETOP 0x005200c8
void tAICaptain::vfunction4(tObject_00413ac3_arg *aObject)
{
	if (g_0096ea16)
	{
		tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x88));
		if (ship == 0)
			return;

		tPoint3 shipPos = ship->field_0x34;
		tPhysicalEntity *second = gObject_008e1060.fn_004249d3(field_0xc8);
		tPhysicalEntity *target = gObject_008e1060.fn_004249d3(field_0xc4);
		tPoint3 targetPos;
		tPoint3 secondPos;
		if (target)
			targetPos = target->fCurrPos;
		if (second)
			secondPos = second->fCurrPos;

		aObject->vfunction77(0);
		int oldState20 = aObject->vfunction20(3);
		int oldState18 = aObject->vfunction18(0);

		tMatrix shipMatrix;
		shipMatrix.mTranslate(shipPos.x, shipPos.y, shipPos.z);
		aObject->vfunction57(&shipMatrix);
		if (aObject->vfunction67(1))
		{
			// HYPOTHESIS: the ship is a physical entity.
			tMatrix *rotation = ((tPhysicalEntity *)ship)->fn_00413b34(false);
			tPoint3 heading(rotation->mGetItem(0, 0), rotation->mGetItem(1, 0),
				rotation->mGetItem(2, 0));
			if (g_0096ea17)
			{
				// HYPOTHESIS: the bytes at 0x3a23 hold a direction.
				tPoint3 &direction = *(tPoint3 *)(field_0x3a1f + 4);
				aObject->vfunction76(0.0f, 1.0f, 0.0f);
				aObject->vfunction69(0.0f, 0.0f, 0.0f);
				aObject->vfunction69(direction.x * 50.0f, direction.y * 50.0f,
					direction.z * 50.0f);
				heading *= 20.0f;
				aObject->vfunction76(0.0f, 0.0f, 1.0f);
				aObject->vfunction69(0.0f, 0.0f, 0.0f);
				aObject->vfunction69(heading.x, heading.y, heading.z);
			}
			if (g_0096ea18)
			{
				if (target)
				{
					tPoint3 offset = targetPos - shipPos;
					aObject->vfunction76(1.0f, 1.0f, 0.0f);
					aObject->vfunction69(0.0f, 0.0f, 0.0f);
					aObject->vfunction69(offset.x, offset.y, offset.z);
				}
				if (second)
				{
					tPoint3 offset = secondPos - shipPos;
					aObject->vfunction76(1.0f, 0.0f, 0.0f);
					aObject->vfunction69(0.0f, 0.0f, 0.0f);
					aObject->vfunction69(offset.x, offset.y, offset.z);
				}
			}
			aObject->vfunction68();
		}

		if (g_0096ea18)
		{
			if (target && aObject->vfunction67(3))
			{
				tMatrix targetMatrix;
				targetMatrix.mTranslate(targetPos.x, targetPos.y, targetPos.z);
				aObject->vfunction57(&targetMatrix);
				float radius = target->vfunction14();
				aObject->vfunction76(1.0f, 1.0f, 0.0f);
				float negative = -radius;
				aObject->vfunction69(radius, negative, 0.0f);
				aObject->vfunction69(negative, negative, 0.0f);
				aObject->vfunction69(negative, radius, 0.0f);
				aObject->vfunction69(radius, radius, 0.0f);
				aObject->vfunction68();
			}
			if (second && aObject->vfunction67(3))
			{
				tMatrix secondMatrix;
				secondMatrix.mTranslate(secondPos.x, secondPos.y, secondPos.z);
				aObject->vfunction57(&secondMatrix);
				float radius = second->vfunction14();
				aObject->vfunction76(1.0f, 0.0f, 0.0f);
				float negative = -radius;
				aObject->vfunction69(radius, negative, 0.0f);
				aObject->vfunction69(negative, negative, 0.0f);
				aObject->vfunction69(negative, radius, 0.0f);
				aObject->vfunction69(radius, radius, 0.0f);
				aObject->vfunction68();
			}
		}

		aObject->vfunction20(oldState20);
		aObject->vfunction18(oldState18);

		if (field_0xc4 != -1 && gObject_008e1060.fn_lookup(field_0xc4))
		{
			for (int i = 0; i < 4; i++)
			{
				tObject_00528a11 selector(field_0x88, 0, i, field_0xc4, 0);
				float time = 0.025f;
				gObject_0096eca8.fn_00529485(time, selector);
				tObject_00528ddb *track = gObject_0096eca8.fn_00528ddb(field_0x88, 0, i, 1);
				if (track)
				{
					tPoint3 point = track->fn_00527bfd(time);
					tMatrix pointMatrix;
					pointMatrix.mTranslate(point.x, point.y, 0.0f);
					aObject->vfunction57(&pointMatrix);
					if (aObject->vfunction67(1))
					{
						aObject->vfunction76(1.0f, 0.0f, 0.0f);
						aObject->vfunction69(2.0f, -2.0f, 0.0f);
						aObject->vfunction69(-2.0f, -2.0f, 0.0f);
						aObject->vfunction69(-2.0f, 2.0f, 0.0f);
						aObject->vfunction69(2.0f, 2.0f, 0.0f);
						aObject->vfunction68();
					}
				}
			}
		}

		tPoint3 destination = fn_0051e37d();
		tMatrix destinationMatrix;
		destinationMatrix.mTranslate(destination.x, destination.y, 0.0f);
		aObject->vfunction57(&destinationMatrix);
		if (aObject->vfunction67(3))
		{
			aObject->vfunction76(0.0f, 0.0f, 1.0f);
			aObject->vfunction69(1.0f, -1.0f, 0.0f);
			aObject->vfunction69(-1.0f, -1.0f, 0.0f);
			aObject->vfunction69(-1.0f, 1.0f, 0.0f);
			aObject->vfunction69(1.0f, 1.0f, 0.0f);
			aObject->vfunction68();
		}
	}
	field_0x3608.fn_00519c02(aObject);
}
