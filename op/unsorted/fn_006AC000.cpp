// Unit:      unsorted/fn_006AC000.cpp
// Functions: 0x006ac5a0 to 0x006ac900, 6 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "unsorted/tView3DExt.h"

// FUNCTION: STARFLEETOP 0x006ac780
void tView3DExt::tPrimitive::mInit()
{
	field_0x00 = -1;
	field_0x04 = 0;
	field_0x08 = 0.0f;
	field_0x0c = 0.0f;
	field_0x10.x = 1.0f;
	field_0x10.y = 1.0f;
	field_0x10.z = 1.0f;
	field_0x1c.x = 1.0f;
	field_0x1c.y = 1.0f;
	field_0x1c.z = 1.0f;
	field_0x28 = 1.0f;
	field_0x2c = 1.0f;
	field_0x30 = 1.0f;
	field_0x34 = 0;
	field_0x38 = 0;
	field_0x3c = 0;
}

// FUNCTION: STARFLEETOP 0x006ac7c0
unsigned char tView3DExt::mBeginPrimitive( ePrimitiveType param_1 )
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
unsigned char tView3DExt::mEndPrimitive()
{
	unsigned char result = mFlushPrimitiveVtxs( 1 );
	field_0x1eec.field_0x00 = -1;
	field_0x1eec.field_0x04 = 0;
	return result;
}

// HYPOTHESIS: a free function of two ints returning a vertex count.
int fn_006b8190( int, int );
extern float g_0079561c;    // 0.0f

// FUNCTION: STARFLEETOP 0x006ac830
void tView3DExt::mFlushPolygon( unsigned long param_1, int param_2, int param_3 )
{
	if ( param_3 == 3 )
		field_0x1f2c[2]++;
	else if ( param_3 == 4 )
		field_0x1f2c[3]++;
	else if ( param_3 > 4 )
		field_0x1f2c[4]++;
	int count = fn_006b8190( param_3, param_1 );
	if ( count > 2 )
	{
		float area = mGetPolyNormalZ();
		if ( area != g_0079561c )
		{
			int back = area > g_0079561c ? 0 : 1;
			if ( !field_0x1ec8 || ( ( field_0x1ebc == 7 ) ^ field_0x1ec8 ^ back ) & 1 )
			{
				field_0x1f2c[7] += count - 2;
				// HYPOTHESIS: the original pushes the low byte of the xor unmasked, as for a
				// char-sized parameter; with vfunction98's bool the & 1 is the closest form.
				vfunction98( count, 0, param_2, ( field_0x1ec0 ^ back ) & 1 );
			}
		}
	}
}
