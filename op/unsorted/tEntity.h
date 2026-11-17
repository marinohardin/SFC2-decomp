//	tEntity.h: tEntity, the base entity, and the entity table at 0x008e1060.
// Evidence:  class (original file unknown)

#ifndef tEntity_h
#define tEntity_h 1

#include "Tiger/tbasic.h"

// The original includes tvectormath.h here.
#include "Tiger/tvectormath.h"

// As the SDK declares it.
typedef int32 tID;

class tSFCStream;
class tPhysicalEntity;
class tObject_00413ac3_arg;
class tObject_008e1060_slot;
class fn_004e2b02_ship_type;
class tObject_00410fd4;
class tObject_004b14db_slot8;
class tObject_0042496f;
class tNetMsg;

// The first argument of vfunction7; only its type byte at +0x10 is read by 0x0041018e and
// 0x0051e1f7.
// HYPOTHESIS: the class and layout are inferred from those two functions only.
class tObject_0041018e_arg
{
public:
	char fField00[0x10];          // +0x00
	unsigned char fType;          // +0x10
};

#include "unsorted/tObject_0090be60.h"

// HYPOTHESIS: the object at +0x14; the destructor (0x0040ffc9) deletes it through slot 0.
class tObject_0040ffc9_owned { public: virtual ~tObject_0040ffc9_owned(); };
// HYPOTHESIS: the object at +0x68, linked through +0x4 and +0x8; vfunction20 (0x00410391)
// checks that its slot 8 returns the entity and unlinks it before the destructor deletes it.
class tObject_00410391_link { public: virtual ~tObject_00410391_link(); };
// HYPOTHESIS: a list at +0x10, linked through +0x4; the destructor calls slot 2 of each.
class tObject_0040ffc9_node
{
public:
	virtual void vfunction1();
	virtual void vfunction2();
	tObject_0040ffc9_node *fNext;	// +0x04
};

// vfunctionN is slot N-1 of the vftable (0x00795cb4, 22 slots); slot 0 is the scalar deleting
// destructor.
// HYPOTHESIS: the slots not yet recovered are placeholders that keep the vftable order.
class tEntity
{
public:
	tEntity();
	virtual ~tEntity();
	tEntity *scalar_deleting_destructor(unsigned char flags);
	virtual int vfunction2();
	virtual void vfunction3();
	// HYPOTHESIS: draws the entity through the object passed in, as tPhysicalEntity's
	// override (0x00413ac3) does; the argument's type is not known.
	virtual void vfunction4(tObject_00413ac3_arg *aObject);
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7(tObject_0041018e_arg *param_1, int param_2);
	// HYPOTHESIS: returns a pointer to an object; tMyHud3D::vfunction4 (0x004b14db) calls
	// members of it and of its member at 0x50.
	virtual tObject_004b14db_slot8 *vfunction8();
	virtual iTruth vfunction9();
	virtual void vfunction10(iTruth aValue);
	virtual void vfunction11(tSFCStream &s, int32u mode);
	// HYPOTHESIS: returns an entity ID; 0x00413ea8 looks it up through 0x004249d3.
	virtual int vfunction12();
	virtual void vfunction13(tID aID);
	virtual float vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	// HYPOTHESIS: returns an int; tAICaptain's override (0x0051e09a) returns a constant.
	virtual int vfunction18();
	virtual void vfunction19();
	virtual bool vfunction20(tObject_00410391_link *aLink);	// HYPOTHESIS: unlinks the object at +0x68
	virtual void vfunction21();
	virtual void vfunction22();
	// Names of the archived fields come from the labels the archive function (0x0041004d)
	// writes. HYPOTHESIS: the other field types and meanings are read from the constructor's
	// stores only.
	tEntity *fNext;              // +0x04
	// HYPOTHESIS: tPhysicalEntity's vfunction50 to vfunction52 walk a list of entities
	// through this pointer, so it may be the next entity rather than the previous one.
	tEntity *fPrev;              // +0x08
	void *fField0C;              // +0x0c
	tObject_0040ffc9_node *fField10; // +0x10
	tObject_0040ffc9_owned *fField14; // +0x14
	// HYPOTHESIS: flag bits as well; 0x00413ea8 tests bits 1, 2 and 14.
	int32 fClassification;       // +0x18
	tID fID;                     // +0x1c
	// HYPOTHESIS: flag bits; 0x00413ac3 draws only while bit 0 is set.
	int32u fProperties;          // +0x20
	// HYPOTHESIS: tPhysicalEntity's vfunction5 sends kKillEntity once while it is set.
	iTruth fIsAlive;             // +0x24
	tID fTargetID;               // +0x28
	// HYPOTHESIS: 0x00413a8d derives it from the object it stores at 0x88.
	float fRadius;               // +0x2c
	int32 fSelectStatus;         // +0x30
	// HYPOTHESIS: 0x00413b34 builds the entity's matrix at 0x98 from it.
	tPoint3 fCurrPos;            // +0x34
	tPoint3 fLastPos;            // +0x40
	tPoint3 fLastMapPos;         // +0x4c
	float fMapRadius;            // +0x58, not set by the constructor
	iTruth fField5C;             // +0x5c
	iTruth fBelongsToHumanFleet; // +0x5d
	tID fTeam;                   // +0x60
	int fField64;                // +0x64
	tObject_00410391_link *fField68; // +0x68
	int32 fScriptNotify;         // +0x6c
	int fField70;                // +0x70, not set by the constructor
	int fField74;                // +0x74, not set by the constructor
	iTruth fField78;             // +0x78
	iTruth fField79;             // +0x79
	tPoint3 fPoint7A;            // +0x7a
	iTruth fField86;             // +0x86

	// HYPOTHESIS: returns float; the original leaves the callee's result in place.
	float fn_00410168();
	// HYPOTHESIS: 0x00423b3b calls these on an entity with a point: 0x00413112 on the result
	// of 0x004249c0, 0x00417d3f on the colliding entity with its swept position.
	void fn_00413112(const tPoint3 &aPoint);
	void fn_00417d3f(const tPoint3 &aPoint);
};

class tObject_00429103;
class tObject_0042496f_weapon;
class tObject_0044715d_system;

// HYPOTHESIS: an object returned by slot 80 of the object at 0xad4 of a ship; only its
// method at 0x0043c0f2 is known.
class tObject_0043c0f2
{
public:
	void fn_0043c0f2();
	// HYPOTHESIS: 0x0051692e passes 1; the argument's meaning is not known.
	void fn_0043bdb6(int param_1);
	// HYPOTHESIS: returns a truth value; tAICaptain's 0x0051e454 steers only while it is false.
	iTruth fn_0043bfc1();
	// HYPOTHESIS: returns a truth value; 0x0051e613 keeps it as a flag.
	iTruth fn_0043c01d();
};

// HYPOTHESIS: an object returned by slot 89 of the object at 0xad4 of a ship; 0x00515db1
// sets the integer at 0x4cc to 4 and nothing else of it is known.
class tObject_00515db1
{
public:
	char field_0x0[0x4cc];
	int field_0x4cc;
};

// HYPOTHESIS: an object returned by slot 77 of the object at 0xad4 of a ship; 0x0051e613
// calls its method 0x0043b7fe when it changes the flag at 0x3a45 of the captain.
class tObject_0043b7fe
{
public:
	void fn_0043b7fe();
};

// HYPOTHESIS: an object returned by slot 90 of the object at 0xad4 of a ship; 0x0051e613
// compares the floats its slots 86 and 87 (offsets 0x154 and 0x158) return.
class tObject_0042496f_member90
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
	virtual float vfunction86();
	virtual float vfunction87();
};

// HYPOTHESIS: an object returned by slot 27 of the object at 0xad4 of a ship; tAICaptain's
// 0x0051f1ed compares the float at 0x18 with 0.
class tObject_0042496f_member27
{
public:
	char field_0x0[0x18];
	float field_0x18;
};

class tObject_0042496f_member76;
class tObject_0042496f_member96;
class tObject_0042496f_member99;

// HYPOTHESIS: a 12-byte value built from a point by a constructor that is not inlined
// (0x00412f08); 0x004255b5 of the object at 0x008f5170 takes it by value, and tAICaptain's
// 0x0051f602 builds it from the point a ship's 0x00422cba returns.
class tObject_00412f08
{
public:
	tObject_00412f08(const tPoint3 &aPoint);

	char field_0x0[0xc];                    // +0x0
};

// HYPOTHESIS: the object at 0xad4 of a ship; slot 57 (offset 0xe4) returns its 25 weapons.
class tObject_0042496f_member
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
	// HYPOTHESIS: slot 27 (offset 0x68) returns an object; tAICaptain's 0x0051f1ed reads it.
	virtual tObject_0042496f_member27 *vfunction27();
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
	virtual tObject_0042496f_weapon **vfunction58();
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
	// HYPOTHESIS: slot 76 (offset 0x12c) returns an object or null; tAICaptain's 0x0051f1ed
	// docks through it.
	virtual tObject_0042496f_member76 *vfunction76();
	// HYPOTHESIS: slot 77 (offset 0x130) returns an object 0x0051e613 calls 0x0043b7fe on.
	virtual tObject_0043b7fe *vfunction77();
	// HYPOTHESIS: slots 77 to 83 each return one ship system or null; 0x0044715d reads seven.
	virtual tObject_0044715d_system *vfunction78();
	virtual tObject_0044715d_system *vfunction79();
	// HYPOTHESIS: slot 80 (offset 0x13c) returns an object or null.
	virtual tObject_0043c0f2 *vfunction80();
	virtual tObject_0044715d_system *vfunction81();
	virtual tObject_0044715d_system *vfunction82();
	virtual tObject_0044715d_system *vfunction83();
	virtual void vfunction84();
	virtual void vfunction85();
	virtual void vfunction86();
	virtual void vfunction87();
	virtual void vfunction88();
	// HYPOTHESIS: slot 89 (offset 0x160) returns an object or null.
	virtual tObject_00515db1 *vfunction89();
	// HYPOTHESIS: slot 90 (offset 0x164) returns an object or null.
	virtual tObject_0042496f_member90 *vfunction90();
	virtual void vfunction91();
	virtual void vfunction92();
	virtual void vfunction93();
	virtual void vfunction94();
	virtual void vfunction95();
	// HYPOTHESIS: slot 96 (offset 0x17c) returns an object or null; tAICaptain's 0x0051f602
	// asks it whether the target is hidden.
	virtual tObject_0042496f_member96 *vfunction96();
	virtual void vfunction97();
	virtual void vfunction98();
	// HYPOTHESIS: slot 99 (offset 0x188) returns an object or null; tAICaptain's 0x0051fa3c
	// calls its 0x00439e98.
	virtual tObject_0042496f_member99 *vfunction99();
	virtual void vfunction100();
	virtual void vfunction101();
	virtual void vfunction102();
	virtual void vfunction103();
	virtual void vfunction104();
	virtual void vfunction105();
	virtual void vfunction106();
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
	virtual void vfunction134();
	virtual void vfunction135();
	virtual void vfunction136();
	virtual void vfunction137();
	virtual void vfunction138();
	virtual void vfunction139();
	virtual void vfunction140();
	virtual void vfunction141();
	virtual void vfunction142();
	virtual void vfunction143();
	virtual void vfunction144();
	virtual void vfunction145();
	virtual void vfunction146();
	virtual void vfunction147();
	virtual void vfunction148();
	virtual void vfunction149();
	virtual void vfunction150();
	virtual void vfunction151();
	virtual void vfunction152();
	virtual void vfunction153();
	virtual void vfunction154();
	virtual void vfunction155();
	virtual void vfunction156();
	virtual void vfunction157();
	virtual void vfunction158();
	virtual void vfunction159();
	virtual void vfunction160();
	virtual void vfunction161();
	virtual void vfunction162();
	virtual void vfunction163();
	virtual void vfunction164();
	// HYPOTHESIS: slot 165 (offset 0x290) returns an entity ID; tAICaptain's 0x0051fa3c hands
	// it to 0x0051ff8b.
	virtual int vfunction165();
};

// HYPOTHESIS: what 0x0042496f returns for a ship ID; only the pointer at 0xad4 is known. It is
// probably the class entity_physical.cpp declares as tObject_00410fd4, which is not shared.
// Slot 24 (offset 0x5c) and slot 61 (offset 0xf0) are called by 0x00514141.
class tObject_0042496f
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
	// HYPOTHESIS: slot 12 (offset 0x2c) returns the entity ID slot 13 sets; tAICaptain's
	// 0x0051fa3c saves it and sets it back.
	virtual int vfunction12();
	// HYPOTHESIS: slot 13 (offset 0x30) takes an entity ID; tAICaptain's 0x0051f1ed passes the
	// ID of the ship it has reached.
	virtual void vfunction13(int aID);
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
	// HYPOTHESIS: slot 24 (offset 0x5c) returns a float, a fraction from 0 to 1 as the fleet
	// entry's 0x004c3a36 sets a gauge with it.
	virtual float vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	// HYPOTHESIS: slot 32 (offset 0x7c) returns a truth value; the fleet entry's 0x004c27a8
	// asks it.
	virtual bool vfunction32();
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
	// HYPOTHESIS: slot 63 (offset 0xf8) returns a float; 0x0051460b adds it for two ships.
	virtual float vfunction63();
	virtual void vfunction64();
	virtual void vfunction65();
	// HYPOTHESIS: slot 66 (offset 0x104) returns an angle; 0x00515f27 passes 0 and compares
	// it with the angle at 0x90.
	virtual short vfunction66(int param_1);

	// HYPOTHESIS: returns an integer 0x005142b3 passes to slot 15 of the officer.
	tObject_00429103 *fn_004156e3();
	// HYPOTHESIS: returns a truth value; 0x00515db1 stops the ship only while it is false.
	bool fn_00419c87();
	// HYPOTHESIS: returns the ship's position (4 bytes, no argument); tAICaptain's 0x0051feca
	// hands it to the mission map and 0x0051f602 searches around it.
	tPoint3 &fn_00422cba();
	// HYPOTHESIS: returns a point for a range; tAICaptain's 0x0051f602 passes a scaled range and
	// searches around the point it returns.
	// HYPOTHESIS: takes a movement message; 0x005169e0 passes the kMoveInDirection it built.
	void fn_00414bfa(tNetMsg *aMsg);
	void fn_004168cc(int param_1, int param_2);
	// HYPOTHESIS: a yes/no question; the fleet entry's 0x004c3a36 asks it with 1.
	unsigned char fn_004168e2(int param_1);
	// HYPOTHESIS: an inline reader of the flag at +0x5c; 0x004c3a36 reads it through a
	// temporary the way an inline function's result is read. The original name is not known.
	bool fn_inline_b5c() const
	{
		return field_0x5c;
	}
	void fn_004168ff(int param_1);
	// HYPOTHESIS: sends the ship a notice; 0x004c3784 passes 0x20000, the entry's ID and three
	// zeros.
	void fn_004102d9(int param_1, int param_2, int param_3, int param_4, int param_5);

	char field_0x4[0x20];
	// HYPOTHESIS: the entity's fIsAlive, if the ship derives from tEntity.
	iTruth field_0x24;
	char field_0x25[0xf];
	// HYPOTHESIS: the entity's fCurrPos, if the ship derives from tEntity.
	tPoint3 field_0x34;
	char field_0x40[0x1c];
	// HYPOTHESIS: a flag; 0x005169e0 steers the ship only while it is set.
	bool field_0x5c;
	// HYPOTHESIS: the entity's fBelongsToHumanFleet, if the ship derives from tEntity.
	iTruth field_0x5d;
	char field_0x5e[0x2];
	// HYPOTHESIS: the fleet's ID; 0x0048c418 and the fleet entry look its list up by it.
	int field_0x60;
	char field_0x64[0x2c];
	// HYPOTHESIS: the ship's heading; 0x00515f27 steers by it.
	short field_0x90;
	char field_0x92[0x76];
	// HYPOTHESIS: a count; tAICaptain's 0x0051f1ed reads it from the ship it docks with, at
	// least 1.
	int field_0x108;
	char field_0x10c[0x690];
	// HYPOTHESIS: an integer 0x0051e613 multiplies by the ship's speed to compare two ships.
	int field_0x79c;
	char field_0x7a0[0xcc];
	// HYPOTHESIS: a class or size; 0x0051e613 acts only between 8 and 16, or on 0, 1, 4 and 6.
	int field_0x86c;
	// HYPOTHESIS: a kind; tAICaptain's 0x0051f602 compares it with 0x16.
	int field_0x870;
	// HYPOTHESIS: 0x00515db1 does not stop the ship while it is 1.
	int field_0x874;
	char field_0x878[0x5];
	// HYPOTHESIS: a name string; 0x00447aa7 copies it.
	char field_0x87d[0x1c3];
	// HYPOTHESIS: a state; tAICaptain's constructor tells its helm officer when it is 7.
	int field_0xa40;
	char field_0xa44[0x90];
	tObject_0042496f_member *field_0xad4;
};

// The object at 0x008e1060: a table of 0x4000 entity pointers indexed by ID at +0x30, then 16
// more objects at +0x10030.
// HYPOTHESIS: the class and its lookups are inferred from 0x00410168 and tPhysicalEntity's
// functions only, the layout after the entity table from 0x00413448 only.
class tObject_008e1060
{
public:
	tEntity *fn_lookup(tID aID)
	{
		if (aID >= 0 && aID < 0x4000)
			return fEntities[aID];
		return 0;
	}
	float fn_004246e4(tEntity *aFrom, tEntity *aTo);
	// HYPOTHESIS: the distance from the entity with the ID to the point; tAICaptain's
	// 0x0051f1ed compares it with 10.
	float fn_004247dd(int aID, const tPoint3 &aPoint);
	// HYPOTHESIS: a float between a weapon's ship and its target, as fn_004246e4 between two
	// entities; tPhaserXBank's 0x004e55af stores it and compares it with a limit.
	float fn_00424705(fn_004e2b02_ship_type *aShip, tEntity *aTarget);
	// HYPOTHESIS: the same float between two entities given by ID; tPhaserXBank's 0x004e5958
	// passes its ship's ID and the target's.
	float fn_004246ab(int aFromID, int aToID);
	// HYPOTHESIS: looks a ship up by ID; tPhaserXBank's 0x004e5958 calls its slot at 0x7c.
	fn_004e2b02_ship_type *fn_0042496f(int aID);
	tObject_008e1060_slot *fn_slot(int aIndex)
	{
		if (aIndex >= 0 && aIndex < 16)
			return fSlots[aIndex];
		return 0;
	}
	// HYPOTHESIS: looks an entity up by ID, as the inlined table lookup does.
	tPhysicalEntity *fn_00424a2f(int aID);
	// HYPOTHESIS: the inlined table lookup, as 0x00413541 does it.
	tPhysicalEntity *fn_entity(int aID)
	{
		return (aID >= 0 && aID < 0x4000) ? (tPhysicalEntity *)fEntities[aID] : 0;
	}
	// HYPOTHESIS: the argument and result types; 0x00413541 passes an int and stores the
	// result as the damage message's seed.
	int fn_004235ac(int aValue);
	// HYPOTHESIS: takes an entity ID and returns a truth value; 0x004139ec sends kKillEntity
	// only when it is true.
	bool fn_0040fc04(int aID);
	// HYPOTHESIS: takes an entity and returns an object related to it; 0x00413a37 calls it
	// with this.
	tObject_00410fd4 *fn_00410fd4(tPhysicalEntity *aEntity);
	// HYPOTHESIS: takes an entity ID and returns a truth value; 0x00413ea8 goes on only when
	// it is true for this entity's ID.
	bool fn_004235f0(int aID);
	// HYPOTHESIS: looks an entity up by an ID that 0x00413ea8 gets from vfunction12.
	tPhysicalEntity *fn_004249d3(int aID);
	// HYPOTHESIS: returns the entity that owns the one given; 0x00413ea8 compares its team.
	tPhysicalEntity *fn_004249c0(tPhysicalEntity *aEntity);
	// HYPOTHESIS: returns the first entity of a list linked through 0x8; 0x00413f59 walks it.
	tPhysicalEntity *fn_0055f269();
	// HYPOTHESIS: returns an int; 0x00421bbe stores it in a new ship-control entry.
	int fn_004239d6();
	// fn_0042496f (declared above) is also the lookup 0x00421ea9 and 0x004220d5 make for a
	// ship-control entry's target; they read the result as a tEntity.

	// HYPOTHESIS: returns an entity ID; 0x00421ea9 compares it with its ship's fID.
	int fn_00525081();
	// HYPOTHESIS: takes the ID 0x004c3784 passes; its result is not used there.
	int fn_004248b5(int aValue);
	// fn_0042496f is also the lookup tMyHud3D::vfunction9 (0x004b26e4) makes; it reads the
	// result as a tObject_0042496f and takes a point from it through 0x00422cba.

	// fn_00525081 (declared above) returns an entity ID; 0x0051e454 does nothing for the ship
	// with that ID. The address is also the formation object's method of that name, so the
	// linker may have folded two identical getters.

	// HYPOTHESIS: looks a ship up by ID, like 0x0042496f; 0x0051e613 uses it for its own ship.
	tObject_0042496f *fn_004249ab(int aID);
	// HYPOTHESIS: looks a ship up by ID; 0x0051e613 uses it for the target ship.
	tObject_0042496f *fn_004249de(int aID);
	bool fn_004235f0(tID aID);	// HYPOTHESIS: a test on the entity with the ID (0x00413614)
	tID mGetNextPlayerID();	// 0x004239d6
	void mSetPlayerNumber(const int aNumber);	// 0x004239ba
	void mAddEntity(tEntity *pNew);	// 0x00423749

	char fField00[0x30];                  // +0x00
	tEntity *fEntities[0x4000];           // +0x30
	tObject_008e1060_slot *fSlots[16];    // +0x10030
};

extern tObject_008e1060 gObject_008e1060;

#endif	// tEntity_h
