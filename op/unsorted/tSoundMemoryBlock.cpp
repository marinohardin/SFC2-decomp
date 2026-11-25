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

// FUNCTION: STARFLEETOP 0x0066eb80
void tSoundMemoryBlock::mUnlock()
{
    fNumberOfLocks--;
    if (fNumberOfLocks < 0) {
        fNumberOfLocks = 0;
    }
}

// FUNCTION: STARFLEETOP 0x0066eba0
unsigned long tSoundMemoryBlock::mGetHandle()
{
    return ((unsigned short)fTopHalfOfHandle << 16) + field_0c;
}
