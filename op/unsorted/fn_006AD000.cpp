// Unit:      unsorted/fn_006AD000.cpp
// Functions: 0x006ad130 to 0x006ad3e0, 9 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "unsorted/tView3DExt.h"

// FUNCTION: STARFLEETOP 0x006ad130
void tView3DExt::vfunction71( tPoint3 *point )
{
	if ( field_0x1eec.field_0x00 != -1 )
	{
		fn_006abe00_Vtx *vtx = &mVtxCache[ field_0x1eec.field_0x04 ];
		// HYPOTHESIS: +0x04..+0x10 of the vertex is a tPoint4, the out-argument of 0x0069ca30
		field_0x6b0.mTransform( *(tPoint4 *)&vtx->field_0x04, *point );
		vtx->field_0x00 = 1;
		if ( field_0x1ecc == 3 )
		{
			vtx->field_0x34 = field_0x1eec.field_0x1c;
			vtx->field_0x38 = field_0x1eec.field_0x20;
			vtx->field_0x3c = field_0x1eec.field_0x24;
		}
		else
		{
			if ( field_0x1eec.field_0x34 )
			{
				field_0x1eec.field_0x34 = 0;
				field_0x1eec.field_0x28 = field_0x1eec.field_0x1c * field_0x1eec.field_0x10.x;
				field_0x1eec.field_0x2c = field_0x1eec.field_0x20 * field_0x1eec.field_0x10.y;
				field_0x1eec.field_0x30 = field_0x1eec.field_0x24 * field_0x1eec.field_0x10.z;
			}
			vtx->field_0x34 = field_0x1eec.field_0x28;
			vtx->field_0x38 = field_0x1eec.field_0x2c;
			vtx->field_0x3c = field_0x1eec.field_0x30;
		}
		vtx->field_0x2c = field_0x1eec.field_0x08;
		vtx->field_0x30 = field_0x1eec.field_0x0c;
		if ( ++field_0x1eec.field_0x04 >= 0x150 )
			fn_006ac900( 0 );
	}
}

// FUNCTION: STARFLEETOP 0x006ad230
void tView3DExt::vfunction70( float x, float y, float z )
{
	tPoint3 point( x, y, z );
	vfunction71( &point );
}
