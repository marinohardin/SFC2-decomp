// Unit:      unsorted/fn_00411790.cpp
// Functions: 0x00411790 to 0x004117a5, 2 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

#include "Source/entity/entity_physical.h"

void __cdecl fn_007364c0(char *aDest, const char *aSource);

// FUNCTION: STARFLEETOP 0x00411790
void tPhysicalEntity::vfunction41(int aValue)
{
	fn_007364c0((char *)&field_0x11c, (const char *)aValue);
}
