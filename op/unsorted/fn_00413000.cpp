#include "Source/entity/entity_physical.h"

// HYPOTHESIS: returns by value the three dwords at +0xe6 (unaligned, inside field_0xe0).
// FUNCTION: STARFLEETOP 0x004130d0
tPoint3 tPhysicalEntity::fn_004130d0()
{
	tPoint3 *p = (tPoint3 *)((char *)this + 0xe6);
	return *p;
}

// HYPOTHESIS: returns by value the three dwords at +0xf2.
// FUNCTION: STARFLEETOP 0x004130f1
tPoint3 tPhysicalEntity::fn_004130f1()
{
	tPoint3 *p = (tPoint3 *)((char *)this + 0xf2);
	return *p;
}
