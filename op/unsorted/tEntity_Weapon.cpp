#include "unsorted/tEntity_Weapon.h"

// FUNCTION: STARFLEETOP 0x0041c1ef
tEntity_Weapon::tEntity_Weapon()
{
	fProperties |= 1;
	field_0x1c4 = -1;
	field_0x1e8 = 0.025f;
	fClassification |= 4;
	field_0x1d8 = 0;
	field_0x1f0 = 0;
	field_0x1f4 = 0;
	field_0x244 = 0;
	field_0x245 = 0;
}

// FUNCTION: STARFLEETOP 0x0041c23c
short tEntity_Weapon::vfunction66( char aValue )
{
	return 0;
}
