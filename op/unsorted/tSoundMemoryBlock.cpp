#include "Tiger/tSndMemMgr.h"

// FUNCTION: STARFLEETOP 0x0066eb40
long tSoundMemoryBlock::mGetNumberOfLocks()
{
    return fNumberOfLocks;
}

// FUNCTION: STARFLEETOP 0x0066eb50
unsigned char tSoundMemoryBlock::mAreWeInUse()
{
    return fBlockInUse;
}
