#include "unsorted/tState_Tactical.h"

using namespace tState_Namespace_Tactical;

// HYPOTHESIS: returns the rectangle at +0x25c when it is non-empty and field_274 is not negative.
// FUNCTION: STARFLEETOP 0x004ad878
int *tState_Tactical::fn_004ad878()
{
	int *rect = field_25c;
	if ( rect[0] >= rect[2] || rect[1] >= rect[3] || field_274 < 0 )
		return 0;
	return rect;
}

// HYPOTHESIS: returns the rectangle at +0x278 when it is non-empty and field_290 is not negative.
// FUNCTION: STARFLEETOP 0x004ad89d
int *tState_Tactical::fn_004ad89d()
{
	int *rect = field_278;
	if ( rect[0] >= rect[2] || rect[1] >= rect[3] || field_290 < 0 )
		return 0;
	return rect;
}

// HYPOTHESIS: the one camera manager; its name is not known.
// FUNCTION: STARFLEETOP 0x004add2b
// FUNCTION: STARFLEETOP 0x004add21
tCameraMgrEx g_00918890;
