// Unit:      unsorted/fn_00661000.cpp
// Functions: 0x00661160 to 0x006612bc, 4 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   sdk-release (harness/units.json)
#include "Tiger/tgSound.h"

// FUNCTION: STARFLEETOP 0x00661160
iTruth tSoundPart::Test_tSoundPart( void )
{
    try { tSoundPart *test = tSoundPart::Search_tSoundPart(); }
    catch (...) {}
    return kTrue;
}
