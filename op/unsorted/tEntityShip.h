//	tEntityShip.h: tEntityShip, the ship entity.
// Evidence:  class (RTTI; original file unknown). vftable 0x00796f0c (76 slots); the constructor
//            (0x004146a7, in no symbol row) is called from 0x004097a6 with an allocation of
//            0xb10 bytes, and calls tPhysicalEntity's (0x00412f67); the destructor is 0x004158c1.

#ifndef tEntityShip_h
#define tEntityShip_h 1

#include "Source/entity/entity_physical.h"
#include "unsorted/tShipSpecs.h"
#include "unsorted/tIndividualShipSystem.h"

class tCloakSystem;
class tSensorGroupSystem;

// HYPOTHESIS: tTractorSystem (RTTI, vftable 0x0079e2a4, base tRepairableShipSystem) has no header
// of its own yet; only the method the ship calls is declared, until one is written.
class tTractorSystem : public tRepairableShipSystem
{
public:
	// HYPOTHESIS: a test tEntityShip::vfunction38 (0x00417b57) makes with its two arguments
	// before handing them on.
	bool fn_004546f0(int aID, bool aValue);
	// HYPOTHESIS: releases the tractor lock; tEntityShip::vfunction39 (0x00417b92) calls it when
	// field_144 holds the ID it is given.
	void fn_004546e1();

	// tRepairableShipSystem ends at 0x7c and is aligned to 8, so this class starts at 0x80.
	char pad80[0x144 - 0x80];
	// HYPOTHESIS: the ID of the entity held by the beam (compared with an ID at 0x00417b92).
	int field_144;
};

// HYPOTHESIS: the record slot 34 (offset 0x88) of the object at 0xad4 returns; 0x004162b9 reads
// three floats from it, at 0x18, 0x1c and 0x20.
struct tObject_00416d41_bounds
{
	char pad00[0x18];
	float field_18;
	float field_1c;
	float field_20;
};

// HYPOTHESIS: the object slot 86 (offset 0x158) of the object at 0xad4 returns; 0x004162b9 calls
// slot 9 (offset 0x24) for a count.
class tObject_00416d41_list
{
public:
	virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2();
	virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5();
	virtual void vfunction6(); virtual void vfunction7(); virtual void vfunction8();
	virtual int vfunction9();
};

// HYPOTHESIS: the object slot 88 (offset 0x160) of the object at 0xad4 returns; 0x00416085 calls
// slot 116 (offset 0x1d0) with the indices 0 to 3 and compares each result with an entity ID.
class fn_00416085_Object
{
public:
	virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2();
	virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5();
	virtual void vfunction6(); virtual void vfunction7(); virtual void vfunction8();
	virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11();
	virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14();
	virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17();
	virtual void vfunction18(); virtual void vfunction19(); virtual void vfunction20();
	virtual void vfunction21(); virtual void vfunction22(); virtual void vfunction23();
	virtual void vfunction24(); virtual void vfunction25(); virtual void vfunction26();
	virtual void vfunction27(); virtual void vfunction28(); virtual void vfunction29();
	virtual void vfunction30(); virtual void vfunction31(); virtual void vfunction32();
	virtual void vfunction33(); virtual void vfunction34(); virtual void vfunction35();
	virtual void vfunction36(); virtual void vfunction37(); virtual void vfunction38();
	virtual void vfunction39(); virtual void vfunction40(); virtual void vfunction41();
	virtual void vfunction42(); virtual void vfunction43(); virtual void vfunction44();
	virtual void vfunction45(); virtual void vfunction46(); virtual void vfunction47();
	virtual void vfunction48(); virtual void vfunction49(); virtual void vfunction50();
	virtual void vfunction51(); virtual void vfunction52(); virtual void vfunction53();
	virtual void vfunction54(); virtual void vfunction55(); virtual void vfunction56();
	virtual void vfunction57(); virtual void vfunction58(); virtual void vfunction59();
	virtual void vfunction60(); virtual void vfunction61(); virtual void vfunction62();
	virtual void vfunction63(); virtual void vfunction64(); virtual void vfunction65();
	virtual void vfunction66(); virtual void vfunction67(); virtual void vfunction68();
	virtual void vfunction69(); virtual void vfunction70(); virtual void vfunction71();
	virtual void vfunction72(); virtual void vfunction73(); virtual void vfunction74();
	virtual void vfunction75(); virtual void vfunction76(); virtual void vfunction77();
	virtual void vfunction78(); virtual void vfunction79(); virtual void vfunction80();
	virtual void vfunction81(); virtual void vfunction82(); virtual void vfunction83();
	virtual void vfunction84(); virtual void vfunction85(); virtual void vfunction86();
	virtual void vfunction87(); virtual void vfunction88(); virtual void vfunction89();
	virtual void vfunction90(); virtual void vfunction91(); virtual void vfunction92();
	virtual void vfunction93(); virtual void vfunction94(); virtual void vfunction95();
	virtual void vfunction96(); virtual void vfunction97(); virtual void vfunction98();
	virtual void vfunction99(); virtual void vfunction100(); virtual void vfunction101();
	virtual void vfunction102(); virtual void vfunction103(); virtual void vfunction104();
	virtual void vfunction105(); virtual void vfunction106(); virtual void vfunction107();
	virtual void vfunction108(); virtual void vfunction109(); virtual void vfunction110();
	virtual void vfunction111(); virtual void vfunction112(); virtual void vfunction113();
	virtual void vfunction114(); virtual void vfunction115();
	virtual tID vfunction116(int aIndex);
};

// HYPOTHESIS: the object slot 83 (offset 0x14c) of the object at 0xad4 returns; 0x00416944 calls
// 0x00437d8d on it for a truth value.
class fn_00437d8d_Object
{
public:
	bool fn_00437d8d();
};

// HYPOTHESIS: the object at 0xad4 (0x00416da2 calls through it); only slots 34 and 86 are called
// by 0x004162b9, the slots before them are not known.
class tObject_00416d41_ad4
{
public:
	virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2();
	virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5();
	virtual void vfunction6(); virtual void vfunction7(); virtual void vfunction8();
	virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11();
	virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14();
	virtual void vfunction15();
	// HYPOTHESIS: a test; 0x00416a0e acts on its result.
	virtual bool vfunction16();
	virtual void vfunction17();
	// HYPOTHESIS: takes two values; 0x004168cc forwards its two arguments.
	virtual void vfunction18(int param_1, int param_2);
	virtual void vfunction19();
	// HYPOTHESIS: a yes/no question; 0x004168e2 forwards its argument and the fleet entry's
	// 0x004c3a36 reads the result.
	virtual unsigned char vfunction20(int param_1);
	virtual void vfunction21();
	// HYPOTHESIS: takes a float and returns one; 0x00416bea forwards its argument and drops the result.
	virtual float vfunction22(float param_1);
	virtual void vfunction23();
	virtual void vfunction24(); virtual void vfunction25(); virtual void vfunction26();
	// HYPOTHESIS: returns a record with a float at 0x18; 0x004198e0 reads it.
	virtual tObject_00416d41_bounds *vfunction27();
	// HYPOTHESIS: returns a record with a float at 0x18; 0x0041687b reads it.
	virtual tObject_00416d41_bounds *vfunction28();
	virtual void vfunction29();
	virtual void vfunction30(); virtual void vfunction31(); virtual void vfunction32();
	virtual void vfunction33();
	virtual tObject_00416d41_bounds *vfunction34();
	virtual void vfunction35(); virtual void vfunction36(); virtual void vfunction37();
	virtual void vfunction38(); virtual void vfunction39(); virtual void vfunction40();
	virtual void vfunction41(); virtual void vfunction42(); virtual void vfunction43();
	virtual void vfunction44(); virtual void vfunction45(); virtual void vfunction46();
	virtual void vfunction47(); virtual void vfunction48(); virtual void vfunction49();
	virtual void vfunction50(); virtual void vfunction51(); virtual void vfunction52();
	virtual void vfunction53(); virtual void vfunction54(); virtual void vfunction55();
	virtual void vfunction56(); virtual void vfunction57(); virtual void vfunction58();
	virtual void vfunction59(); virtual void vfunction60(); virtual void vfunction61();
	virtual void vfunction62(); virtual void vfunction63(); virtual void vfunction64();
	virtual void vfunction65(); virtual void vfunction66(); virtual void vfunction67();
	virtual void vfunction68(); virtual void vfunction69(); virtual void vfunction70();
	virtual void vfunction71(); virtual void vfunction72(); virtual void vfunction73();
	virtual void vfunction74();
	// HYPOTHESIS: returns the ship's tractor system or 0; tEntityShip::vfunction38 (0x00417b57)
	// calls 0x004546f0, in tTractorSystem's range, on it.
	virtual tTractorSystem *vfunction75();
	virtual void vfunction76();
	virtual void vfunction77(); virtual void vfunction78(); virtual void vfunction79();
	virtual void vfunction80(); virtual void vfunction81(); virtual void vfunction82();
	// HYPOTHESIS: returns a system or 0; 0x00416944 calls 0x00437d8d on it.
	virtual fn_00437d8d_Object *vfunction83();
	// HYPOTHESIS: returns the ship's sensor group; 0x00415ff4 calls 0x0043eca6 on it.
	virtual tSensorGroupSystem *vfunction84();
	virtual void vfunction85();
	virtual tObject_00416d41_list *vfunction86();
	virtual void vfunction87();
	// HYPOTHESIS: returns an object holding four entity IDs; 0x00416085 reads them.
	virtual fn_00416085_Object *vfunction88();
	virtual void vfunction89();
	virtual void vfunction90(); virtual void vfunction91(); virtual void vfunction92();
	// HYPOTHESIS: returns an object whose slot 97 0x00415672 calls.
	virtual void *vfunction93();
	virtual void vfunction94();
	// HYPOTHESIS: returns the ship's cloak system; 0x00416a0e calls 0x00438390 on it.
	virtual tCloakSystem *vfunction95();
	virtual void vfunction96(); virtual void vfunction97(); virtual void vfunction98();
	virtual void vfunction99(); virtual void vfunction100(); virtual void vfunction101();
	virtual void vfunction102(); virtual void vfunction103(); virtual void vfunction104();
	virtual void vfunction105(); virtual void vfunction106(); virtual void vfunction107();
	virtual void vfunction108(); virtual void vfunction109(); virtual void vfunction110();
	virtual void vfunction111(); virtual void vfunction112();
		// HYPOTHESIS: takes a float (0x00414d70 passes 1.0).
		virtual void vfunction113(float aValue);
	virtual void vfunction114(); virtual void vfunction115(); virtual void vfunction116();
	virtual void vfunction117(); virtual void vfunction118(); virtual void vfunction119();
	virtual void vfunction120(); virtual void vfunction121(); virtual void vfunction122();
	virtual void vfunction123(); virtual void vfunction124(); virtual void vfunction125();
	virtual void vfunction126(); virtual void vfunction127(); virtual void vfunction128();
	virtual void vfunction129(); virtual void vfunction130(); virtual void vfunction131();
	virtual void vfunction132(); virtual void vfunction133(); virtual void vfunction134();
	virtual void vfunction135(); virtual void vfunction136(); virtual void vfunction137();
	virtual void vfunction138(); virtual void vfunction139(); virtual void vfunction140();
	// HYPOTHESIS: returns a value tEntityShip::vfunction23 (0x00416c02) passes on.
	virtual int vfunction141();
	virtual void vfunction142(); virtual void vfunction143();
	// HYPOTHESIS: takes the arguments of tEntityShip::vfunction26 and returns a truth value.
	virtual bool vfunction144(short a1, int a2, int a3, int a4, int a5);
	virtual void vfunction145(); virtual void vfunction146();
	// HYPOTHESIS: returns the system at an index (0x004157ee passes 0x31 to 0x49), or 0.
	virtual void *vfunction147(int aIndex);
	virtual void vfunction148(); virtual void vfunction149(); virtual void vfunction150();
	virtual void vfunction151(); virtual void vfunction152(); virtual void vfunction153();
	virtual int vfunction154();
	virtual int vfunction155();
};

// HYPOTHESIS: the object at 0x1bc; 0x004162b9 calls 0x00426563 on it with the view.
class tObject_00426563_1bc
{
public:
	void fn_00426563(tObject_00413ac3_arg *aView);
	// HYPOTHESIS: the object is a fn_00426141_Class (declared only in fn_00426000.cpp), whose
	// member this is; declared here until that class has a header.
	void fn_00426886(unsigned int aMask);
};

// HYPOTHESIS: the object of 0x20e0 bytes at 0xa44 of the ship, built by 0x004287d9; its class
// is not known. 0x004156e3 calls 0x004288d1 on it before returning it.
class fn_004288d1_Object
{
public:
	void fn_004288d1();
};

// HYPOTHESIS: members are placed by the constructor's stores and the offsets the class's
// functions use; types follow the store (fstp is a float, a byte move a byte, a pointer
// is a pointer) and are guesses where only one is seen. Names are field_<offset>: the
// executable names none of them. 0x1c0 is the ship's specification (the constructor calls
// tShipSpecs's at 0x0040891d there), 0x6ac bytes, so 0x86c is the next member, the one the
// ship systems read through their owner pointer.
//
// The slots whose base declaration was wrong are declared now that tPhysicalEntity and tEntity
// type them: 5 (0x00417bc6), 23 (0x00416c17), 24 (0x00419cc4), 30 to 32 (0x00416d41, 0x00416d75,
// 0x00416da2) and 62 to 65 (0x00415b6b, 0x00414b31, 0x00414b38, 0x004155ed).
class tEntityShip : public tPhysicalEntity
{
public:
	tEntityShip();
	virtual ~tEntityShip();

	virtual void vfunction3();
	virtual void vfunction4(tObject_00413ac3_arg *aObject);
	virtual void vfunction5();
	// HYPOTHESIS: takes a float (0x00417bc6 subtracts it from the member at 0xa90).
	virtual void vfunction6(float aValue);
	virtual void vfunction7(tObject_0041018e_arg *param_1, int param_2);
	virtual void vfunction11(tSFCStream &s, int32u mode);
	virtual void vfunction13(tID aID);
	virtual void vfunction17();
	virtual int vfunction23();
	// HYPOTHESIS: returns a float: 0x00416c17 ends in fld of one of two constants.
	virtual float vfunction24();
	// HYPOTHESIS: takes an entity ID and indexes the entity table with it (0x00419cc4).
	virtual void vfunction25(tID aID);
	virtual void vfunction26(short a1, int a2, int a3, int a4, int a5);
	virtual tMatrix *vfunction28(bool aValue);
	// HYPOTHESIS: slots 30 and 31 return a truth value (0x00416d41 ends in mov al, 1; 0x00416d75
	// in setne al); slot 32 (0x00416da2) jumps to a slot of the object at 0xad4 and returns
	// its result, or 0.
	virtual bool vfunction31();
	virtual bool vfunction32();
	virtual void *vfunction33();
	virtual void vfunction38(int aID, bool aValue);
	virtual void vfunction39(int aID);

	// Slots 62 to 65 override tPhysicalEntity's (0x00415b6b divides by the float at 0x8a4 after
	// a call through slot 54; 0x00414b31 returns the float at 0xac8); slots 66 to 75 are new in
	// this class (tPhysicalEntity's vftable ends at slot 65).
	virtual float vfunction63();
	virtual float vfunction64();
	// HYPOTHESIS: slot 64 returns the float at 0x8a8.
	virtual float vfunction65();
	// HYPOTHESIS: the original returns a 16-bit angle (0x004155ed ends with xor ax, ax).
	virtual short vfunction66(char param_1);
	virtual void vfunction67(void *param_1);
	// HYPOTHESIS: a truth value: 0x004160ee ends in mov al, 1 or xor al, al.
	virtual bool vfunction68();
	// HYPOTHESIS: a truth value: 0x00416944 tests it as a byte.
	virtual void vfunction69(bool param_1);
	// HYPOTHESIS: returns the dword at 0x878.
	virtual int vfunction70();
	// HYPOTHESIS: takes an entity (0x00415ff4 reads its ID at 0x1c and looks it up).
	virtual int vfunction71(tEntity *aEntity);
	// HYPOTHESIS: copies a tPoint3 (three dwords) from the entity table's record for the ID at
	// 0x194 to the address given; 0x00417b2a pops 8 bytes, so it takes a second argument.
	virtual tPoint3 vfunction72(int param_1);
	virtual void vfunction73(int param_1, int param_2);
	// HYPOTHESIS: a factor from the cloak system's level (0x00416db7); 1.0 without a cloak.
	virtual float vfunction74();
	// HYPOTHESIS: returns the 16-bit value at 0x92e.
	virtual short vfunction75();
	virtual void vfunction76(int param_1);

	// HYPOTHESIS: 0x004103e2 lies in tEntity's range; it is declared here because the headers
	// of tEntity and tPhysicalEntity are outside this lane. It takes the view.
	void fn_004103e2(tObject_00413ac3_arg *aView);
	// HYPOTHESIS: a test of the ship, called by 0x004162b9 to choose between two ways to draw.
	bool fn_00419c87();
	void fn_00419818(const tPoint3 &aPoint);
	void fn_00419873(short param_1, short param_2, short param_3);
	void fn_004198e0();
	void fn_0041994b();
	// HYPOTHESIS: takes the ID of the entity 0x00416a0e acted for; the symbol table names 0x00419e84
	// a destructor of tNetMsg_kSendVoice, which its callers' use contradicts.
	void fn_00419e84(int aID);
	void fn_00414b47(int param_1);
	// HYPOTHESIS: queues a movement message; callers reach it through tObject_0042496f.
	void fn_00414bfa(tNetMsg *aMsg);
	void fn_00414cad(tNetMsg *aMsg);
	void fn_00414cf5(tNetMsg *aMsg);
	void fn_00414d2f(tNetMsg *aMsg);
	int fn_00414d69();
	tPoint3 fn_00415479();
	void fn_0041553a(tPoint3 *aPoint, char *aFlag);
	void fn_004155c1(tPoint3 *aPoint);
	void fn_00415628(int param_1, int param_2);
	void fn_0041563f(int param_1);
	void fn_00415652(int param_1);
	int fn_00415672(int param_1);
	int fn_004156b8(int param_1);
	fn_004288d1_Object *fn_004156e3();
	int fn_004157ee(int param_1, int param_2, int param_3);
	// HYPOTHESIS: orders two ships by the ratio of slot 13 to slot 62, then by radius and ID.
	bool fn_00415aed(tEntityShip *aOther);
	void fn_00419e4a(int param_1);
	bool fn_00416085();
	void fn_004160c2();
	void fn_004165b7();
	void fn_004165d4();
	void fn_0041687b();
	double fn_0041689a(int param_1);
	void fn_004168b5(double param_1, int param_2);
	void fn_004168cc(int param_1, int param_2);
	unsigned char fn_004168e2(int param_1);
	void fn_004168f4();
	void fn_00416bea(float param_1);

	// 0x1bc is cleared by the constructor.
	tObject_00426563_1bc *field_1bc;
	tShipSpecs field_1c0;
	int field_86c;
	int field_870;
	int field_874;
	int field_878;
	bool field_87c;
	bool field_87d;
	char pad87e[0x8a0 - 0x87e];
	float field_8a0;
	float field_8a4;
	float field_8a8;
	float field_8ac;
	float field_8b0;
	float field_8b4;
	float field_8b8;
	float field_8bc;
	float field_8c0;
	float field_8c4;
	float field_8c8;
	tPoint3 field_8cc;
	tPoint3 field_8d8;
	float field_8e4;
	tPoint3 field_8e8;
	tPoint3 field_8f4;
	tPoint3 field_900;
	tPoint3 field_90c;
	char pad918[0x928 - 0x918];
	short field_928;
	short field_92a;
	short field_92c;
	short field_92e;
	float field_930;
	int field_934;
	int field_938;
	char pad93c[0x940 - 0x93c];
	float field_940;
	int field_944;
	float field_948[12];
	char pad978[0x980 - 0x978];
	tMatrix field_980;
	tMatrix field_9c4;
	int field_a08;
	bool field_a0c;
	char pada0d[0xa10 - 0xa0d];
	int field_a10;
	bool field_a14;
	char pada15[0xa18 - 0xa15];
	// HYPOTHESIS: four objects the constructor allocates, of 8, 16, 8 and 8 bytes, each with a
	// vftable of its own (0x00796efc, 0x00796eec, 0x00796edc, 0x00796ecc); their classes are
	// not known.
	void *field_a18;
	void *field_a1c;
	void *field_a20;
	void *field_a24;
	float field_a28[6];
	int field_a40;
	// HYPOTHESIS: an object of 0x20e0 bytes, built by 0x004287d9 with this entity.
	fn_004288d1_Object *field_a44;
	char pada48[0xa54 - 0xa48];
	// HYPOTHESIS: records of 100 bytes, allocated and cleared by 0x00414b47.
	void *field_a54[10];
	float field_a7c;
	float field_a80;
	float field_a84;
	float field_a88;
	int field_a8c;
	float field_a90;
	int field_a94;
	// HYPOTHESIS: five doubles set from the global time at 0x00c7c148 by the constructor.
	double field_a98[5];
	double field_ac0;
	float field_ac8;
	int field_acc;
	char padad0[0xad4 - 0xad0];
	// HYPOTHESIS: the object 0x00448e4b returns when given this entity and 0.
	tObject_00416d41_ad4 *field_ad4;
	bool field_ad8;
	bool field_ad9;
	bool field_ada;
	bool field_adb;
	bool field_adc;
	bool field_add;
	char padade[0xae0 - 0xade];
	char field_ae0;
	char padae1[0xae4 - 0xae1];
	int field_ae4;
	int field_ae8;
	int field_aec;
	bool field_af0;
	char padaf1[0xaf4 - 0xaf1];
	int field_af4;
	bool field_af8[3];
	char padafb[0xafc - 0xafb];
	float field_afc[3];
	char padb08[0xb0c - 0xb08];
	float field_b0c;
};

typedef char tEntityShip_size_is_0xb10[ sizeof( tEntityShip ) == 0xb10 ? 1 : -1 ];

#endif	// tEntityShip_h
