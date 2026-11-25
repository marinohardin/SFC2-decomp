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
