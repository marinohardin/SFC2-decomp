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

// HYPOTHESIS: keeps the previous three dwords at +0xe6 in +0xf2 and stores the point there.
// FUNCTION: STARFLEETOP 0x00413112
void tEntity::fn_00413112(const tPoint3 &aPoint)
{
	tPoint3 *cur = (tPoint3 *)((char *)this + 0xe6);
	tPoint3 *prev = (tPoint3 *)((char *)this + 0xf2);
	*prev = *cur;
	*cur = aPoint;
}
