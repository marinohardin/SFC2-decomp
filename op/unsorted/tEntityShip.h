//	tEntityShip.h: tEntityShip, the ship entity.
// Evidence:  class (RTTI; original file unknown). vftable 0x00796f0c (76 slots); the constructor
//            (0x004146a7, in no symbol row) is called from 0x004097a6 with an allocation of
//            0xb10 bytes, and calls tPhysicalEntity's (0x00412f67); the destructor is 0x004158c1.

#ifndef tEntityShip_h
#define tEntityShip_h 1

#include "Source/entity/entity_physical.h"
#include "unsorted/tShipSpecs.h"

// HYPOTHESIS: members are placed by the constructor's stores and the offsets the class's
// functions use; types follow the store (fstp is a float, a byte move a byte, a pointer
// is a pointer) and are guesses where only one is seen. Names are field_<offset>: the
// executable names none of them. 0x1c0 is the ship's specification (the constructor calls
// tShipSpecs's at 0x0040891d there), 0x6ac bytes, so 0x86c is the next member, the one the
// ship systems read through their owner pointer.
//
// Not declared, because tPhysicalEntity (or tEntity) declares the slot with another return
// type or arguments and a derived declaration of a different signature would add a virtual
// instead of overriding one: slot 5 (vfunction6, 0x00417bc6, one float argument), 23
// (vfunction24, 0x00416c17, returns a float), 24 (vfunction25, 0x00419cc4, one argument),
// 30 to 32 (vfunction31 to vfunction33, 0x00416d41, 0x00416d75, 0x00416da2, return a byte),
// 62 and 63 (vfunction63, 0x00415b6b and vfunction64, 0x00414b31, return a float; 0x00410667
// is tPhysicalEntity's exact vfunction64 as a void).
class tEntityShip : public tPhysicalEntity
{
public:
	tEntityShip();
	virtual ~tEntityShip();

	virtual void vfunction3();
	virtual void vfunction4(tObject_00413ac3_arg *aObject);
	virtual void vfunction5();
	virtual void vfunction7(tObject_0041018e_arg *param_1, int param_2);
	virtual void vfunction11(tSFCStream &s, int32u mode);
	virtual void vfunction13(tID aID);
	virtual void vfunction17();
	virtual int vfunction23();
	virtual void vfunction26(short a1, int a2, int a3, int a4, int a5);
	virtual tMatrix *vfunction28(bool aValue);
	virtual void vfunction38(int aID, bool aValue);
	virtual void vfunction39(int aID);

	// Slots 64 to 75 are new in this class (tPhysicalEntity's vftable ends at slot 63).
	// HYPOTHESIS: slot 64 returns the float at 0x8a8.
	virtual float vfunction65();
	// HYPOTHESIS: the original returns a 16-bit angle (0x004155ed ends with xor ax, ax).
	virtual unsigned short vfunction66(char param_1);
	virtual void vfunction67(void *param_1);
	virtual void vfunction68();
	virtual void vfunction69(int param_1);
	// HYPOTHESIS: returns the dword at 0x878.
	virtual int vfunction70();
	virtual int vfunction71(tID aID);
	// HYPOTHESIS: copies a tPoint3 (three dwords) from the entity table's record for the ID at
	// 0x194 to the address given; 0x00417b2a pops 8 bytes, so it takes a second argument.
	virtual tPoint3 vfunction72(int param_1);
	virtual void vfunction73(int param_1, int param_2);
	virtual float vfunction74();
	// HYPOTHESIS: returns the 16-bit value at 0x92e.
	virtual short vfunction75();
	virtual void vfunction76(int param_1);

	// 0x1bc is cleared by the constructor.
	int field_1bc;
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
	void *field_a44;
	char pada48[0xa54 - 0xa48];
	int field_a54[10];
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
	void *field_ad4;
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
