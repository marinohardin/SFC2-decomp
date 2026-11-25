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

// FUNCTION: STARFLEETOP 0x0066eb60
void tSoundMemoryBlock::mSetBlockInUse(unsigned char inUse)
{
    fBlockInUse = inUse;
}

// FUNCTION: STARFLEETOP 0x0066eb70
void tSoundMemoryBlock::mLock()
{
    fNumberOfLocks++;
}
