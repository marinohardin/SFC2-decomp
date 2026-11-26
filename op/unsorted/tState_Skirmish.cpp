// Unit:      unsorted/tState_Skirmish.cpp
// Functions: 0x0047f95d to 0x004801a5, 10 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include "unsorted/tState_Skirmish.h"
#include "unsorted/tObject_0092deb4.h"

// HYPOTHESIS: partial. The original also loops i = 0..2 over a tShipSystemMask copied from slot 85
// of *0x0092deb4 and handed to 0x00482f3b on the object at this+0x1154; the copy constructor
// 0x004456a1 and that slot's signature are not declared in headers this lane may edit.
// FUNCTION: STARFLEETOP 0x0047feb1
void tState_Skirmish::fn_0047feb1()
{
	gObject_0092deb4->vfunction86();
	gObject_0092deb4->vfunction109();
}

// FUNCTION: STARFLEETOP 0x0047ff4e
void tState_Skirmish::fn_0047ff4e()
{
	int *p = (int *)((char *)this + 0x11d8);
	for (int n = 5; n != 0; n--)
	{
		p[15] = 1;
		p[0] = 0;
		p[5] = 2;
		p[10] = 0;
		p[-5] = 0;
		p++;
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
			fn_00482ed3(i, j, -1);
	}
	fn_00482ed3(0, 0, 0);
	field_0x10 = 0;
	gObject_0092deb4->vfunction38();
	fn_00482ed3(1, 0, 1);
	*(int *)((char *)this + 0x11c4) = 2;
}

long fn_004d33a8( long arg1, long arg2 );
// HYPOTHESIS: five values starting at 0x0084abb8.
extern long g_0084abb8[5];

// FUNCTION: STARFLEETOP 0x0047ffe0
void tState_Skirmish::fn_0047ffe0()
{
	for (int i = 1; i < 6; i++)
		fn_004d33a8( g_0084abb8[i - 1], ((fn_00482eef_Object *)field_0x1154)->fn_00482eef( i, 0 ) + 1 );
}
