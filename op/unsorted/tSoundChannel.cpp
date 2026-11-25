#include "Tiger/tgSound.h"

// FUNCTION: STARFLEETOP 0x00646870
long tSoundChannel::mGetBytesPerSample()
{
    return field_0x94;
}

// FUNCTION: STARFLEETOP 0x00646880
long tSoundChannel::mGetBitsPerSample()
{
    return field_0x10.vfunction9();
}

// FUNCTION: STARFLEETOP 0x00646890
long tSoundChannel::mGetSoundType()
{
    return field_0x10.vfunction11();
}

// FUNCTION: STARFLEETOP 0x006468a0
void tSoundChannel::mSetSoundType(long type)
{
    field_0x10.vfunction10(type);
}

// FUNCTION: STARFLEETOP 0x006468c0
double tSoundChannel::mGetMillisecondsLeftToPlay()
{
    return mGetTotalNumberOfSecondsLeftToPlay() * 1000.0;
}
