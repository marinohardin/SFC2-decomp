// Unit:      unsorted/fn_006AC000.cpp
// Functions: 0x006ac5a0 to 0x006ac900, 6 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "unsorted/tView3DExt.h"

// FUNCTION: STARFLEETOP 0x006ac780
void fn_006ac780_State::fn_006ac780()
{
	field_0x00 = -1;
	field_0x04 = 0;
	field_0x08 = 0.0f;
	field_0x0c = 0.0f;
	field_0x10.x = 1.0f;
	field_0x10.y = 1.0f;
	field_0x10.z = 1.0f;
	field_0x1c = 1.0f;
	field_0x20 = 1.0f;
	field_0x24 = 1.0f;
	field_0x28 = 1.0f;
	field_0x2c = 1.0f;
	field_0x30 = 1.0f;
	field_0x34 = 0;
	field_0x38 = 0;
	field_0x3c = 0;
}

// FUNCTION: STARFLEETOP 0x006ac7c0
bool tView3DExt::vfunction68( int param_1 )
{
	if ( param_1 >= 0 && param_1 <= 9 )
	{
		field_0x1eec.field_0x00 = param_1;
		field_0x1eec.field_0x04 = 0;
		field_0x1eec.field_0x10.mSet( 1.0f, 1.0f, 1.0f );
		field_0x1eec.field_0x3c = 0;
		return true;
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x006ac810
void tView3DExt::vfunction69()
{
	fn_006ac900( 1 );
	field_0x1eec.field_0x00 = -1;
	field_0x1eec.field_0x04 = 0;
}
