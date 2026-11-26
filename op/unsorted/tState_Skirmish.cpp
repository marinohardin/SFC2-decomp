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
