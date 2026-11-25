// Unit:      unsorted/fn_00661000.cpp
// Functions: 0x00661160 to 0x006612bc, 4 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   sdk-release (harness/units.json)
#include "Tiger/tgSound.h"
#include "Tiger/tAudioFile.h"

// FUNCTION: STARFLEETOP 0x00661160
iTruth tSoundPart::Test_tSoundPart( void )
{
    try { tSoundPart *test = tSoundPart::Search_tSoundPart(); }
    catch (...) {}
    return kTrue;
}

// FUNCTION: STARFLEETOP 0x00661220
iTruth tAudioFileInfoPart::Test_tAudioFileInfoPart( void )
{
    try { tAudioFileInfoPart *test = tAudioFileInfoPart::Search_tAudioFileInfoPart(); return kTrue; }
    catch (tPartBindingException) { return kFalse; }
}
