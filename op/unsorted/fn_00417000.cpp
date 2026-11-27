// Unit:      unsorted/fn_00417000.cpp
// Functions: 0x00417582 to 0x00417ee8, 10 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)
#include "unsorted/tEntity.h"

// HYPOTHESIS: keeps the current position as the last one and moves the entity to the point.
// FUNCTION: STARFLEETOP 0x00417d3f
void tEntity::fn_00417d3f(const tPoint3 &aPoint)
{
	fLastPos = fCurrPos;
	fCurrPos = aPoint;
}
