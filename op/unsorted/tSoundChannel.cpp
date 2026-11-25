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

// FUNCTION: STARFLEETOP 0x006468d0
long tSoundChannel::mGetUserData()
{
    return field_0x8c;
}

// FUNCTION: STARFLEETOP 0x006468e0
void tSoundChannel::mSetUserData(long userData)
{
    field_0x8c = userData;
}

// FUNCTION: STARFLEETOP 0x006468f0
// HYPOTHESIS: the original returns void (*)(unsigned long, unsigned char); declaring that
// return type in tgSound.h moves a stack slot in tgSoundPart::mSetNumberOfSoundChannelCommands,
// so the slot is declared long, which gives the same bytes.
long tSoundChannel::mGetFinishSoundCallback()
{
    return (long)field_0x90;
}

// FUNCTION: STARFLEETOP 0x00646900
// HYPOTHESIS: the original takes void (*)(unsigned long, unsigned char); declared long like the getter.
void tSoundChannel::mSetFinishSoundCallback(long callback)
{
    field_0x90 = (void (*)(long, iTruth))callback;
}

// FUNCTION: STARFLEETOP 0x00646910
long tSoundChannel::mGetNumberOfAttachments()
{
    return (&field_0xec)->vfunction39();
}

// FUNCTION: STARFLEETOP 0x00646930
iTruth tSoundChannel::mAreWeLooping()
{
    unsigned char looping = (mGetSoundType() & 4) > 0;
    return looping;
}

// FUNCTION: STARFLEETOP 0x00646940
iTruth tSoundChannel::mAreWeStreaming()
{
    unsigned char streaming = (mGetSoundType() & 2) > 0;
    return streaming;
}

// FUNCTION: STARFLEETOP 0x00646950
iTruth tSoundChannel::mAreWe3D()
{
    unsigned char is3D = (mGetSoundType() & 8) > 0;
    return is3D;
}

// FUNCTION: STARFLEETOP 0x00646960
iTruth tSoundChannel::mAreWeStatic()
{
    unsigned char isStatic = (mGetSoundType() & 1) > 0;
    return isStatic;
}

// FUNCTION: STARFLEETOP 0x00646970
iTruth tSoundChannel::mAreWeFixedFrequency()
{
    unsigned char isFixed = (mGetSoundType() & 0x10) > 0;
    return isFixed;
}

// FUNCTION: STARFLEETOP 0x00646980
iTruth tSoundChannel::mAreWeARecordingChannel()
{
    unsigned char isRecording = (mGetSoundType() & 0x20) > 0;
    return isRecording;
}

// FUNCTION: STARFLEETOP 0x00646990
iTruth tSoundChannel::mTryToLocateInHardware()
{
    unsigned char inHardware = (mGetSoundType() & 0x40) > 0;
    return inHardware;
}
