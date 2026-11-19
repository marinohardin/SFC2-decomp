// Placeholder written by tools/units.py. Delete this block when the unit's first
// function lands; the generator never touches a file with content.
// Unit:      unsorted/tAsset.cpp
// Functions: 0x00675b80 to 0x00675cd0, 4 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "unsorted/tAsset.h"

extern short g_00882e16;

// FUNCTION: STARFLEETOP 0x00675b80
tAsset::tAsset()
{
	field_30 = 0;
	field_04 = 0;
	field_5A = g_00882e16;
}

// HYPOTHESIS: field_30 is a kind code; 3, 0x11 and 0x16 are the ones this test accepts.
// FUNCTION: STARFLEETOP 0x00675bf0
short tAsset::fn_00675bf0()
{
	int kind = (unsigned short)field_30;
	if (kind == 3 || kind == 0x11 || kind == 0x16)
		return 1;
	return 0;
}
