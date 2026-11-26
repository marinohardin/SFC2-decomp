//	tEntity_Weapon.h: tEntity_Weapon, the weapon entity, and the base of tEntity_Seeker
//	(RTTI, vtable 0x00797430).
// Evidence:  class (RTTI: tEntity_Weapon is a tPhysicalEntity; original file unknown)

#ifndef tEntity_Weapon_h
#define tEntity_Weapon_h 1

#include "Source/entity/entity_physical.h"

// HYPOTHESIS: the size is 0x248: the constructor (0x0041c1ef) writes up to the byte at +0x245,
// and tEntity_Seeker's constructor, which runs it first, writes its own members from +0x260.
// The vtable (0x00797430) has 69 slots; tPhysicalEntity's header declares 64 of its 66, so the
// two slots it leaves out (both pure there, 0x00737f86) are declared here as slots 64 and 65,
// which the vtable shows overridden. vfunctionN is slot N-1, as in tPhysicalEntity.
class tEntity_Weapon : public tPhysicalEntity
{
public:
	tEntity_Weapon();
	~tEntity_Weapon();

	// HYPOTHESIS: slot 64, left out of tPhysicalEntity's header; overridden here by the function
	// that also fills slot 62 (0x00504078, which loads a float constant).
	virtual void vfunction65();
	// HYPOTHESIS: takes one stack argument and returns a short; this class returns 0.
	virtual short vfunction66( int aValue );
	// HYPOTHESIS: takes at least eight stack arguments (0x0041c271 reads up to +0x24 of its
	// frame) and stores two of them at +0x1c and +0x28.
	virtual void vfunction67();
	// HYPOTHESIS: returns the ID at +0x1c4.
	virtual tID vfunction68();
	// HYPOTHESIS: returns the 4-byte member at +0x1f0.
	virtual int vfunction69();

	// The constructor sets bit 0 of the properties at +0x20 and bit 2 of the flags at +0x18.
	char field_0x1bc[0x8];
	// HYPOTHESIS: an ID; the constructor sets it to -1, and slot 67 returns it.
	tID field_0x1c4;
	char field_0x1c8[0x10];
	int field_0x1d8;
	char field_0x1dc[0xc];
	// HYPOTHESIS: a double; the constructor loads it from the constant at 0x00797548.
	double field_0x1e8;
	int field_0x1f0;
	int field_0x1f4;
	char field_0x1f8[0x4c];
	char field_0x244;
	char field_0x245;
	char field_0x246[0x2];
};

typedef char tEntity_Weapon_size_is_0x248[ sizeof( tEntity_Weapon ) == 0x248 ? 1 : -1 ];

#endif	// tEntity_Weapon_h
