// Unit:      unsorted/tControlAsset.cpp
// Functions: 0x00678ce0 to 0x006793b0, 12 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "unsorted/tAsset.h"

// HYPOTHESIS: the body is empty and the members clean up themselves. The original destroys a
// vector of 0x30-byte entries, a tree and a tString inside field_60; c_00678500 is still
// declared as bytes, because declaring those members lowers the factory fn_00676a30.
// FUNCTION: STARFLEETOP 0x00678db0
tControlAsset::~tControlAsset()
{
}
