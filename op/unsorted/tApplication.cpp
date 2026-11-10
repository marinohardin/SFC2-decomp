// Unit:      unsorted/tApplication.cpp
// Functions: 0x0040c2ed to 0x0040c30f, 6 in game/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include "unsorted/tApplication.h"

// FUNCTION: STARFLEETOP 0x0040c2ed
tApplication::~tApplication()
{
}

// FUNCTION: STARFLEETOP 0x0040c2f4
void tApplication::vfunction5(char *param_1)
{
	*param_1 = 0;
}

// FUNCTION: STARFLEETOP 0x0040c2fe
int tApplication::vfunction7()
{
	// HYPOTHESIS: 0x280 (640) is a default screen width; nothing names it yet.
	return 0x280;
}

// FUNCTION: STARFLEETOP 0x0040c304
int tApplication::vfunction8()
{
	// HYPOTHESIS: 0x1e0 (480) is a default screen height; nothing names it yet.
	return 0x1e0;
}

// FUNCTION: STARFLEETOP 0x0040c30a
bool tApplication::vfunction19(int)
{
	return true;
}
