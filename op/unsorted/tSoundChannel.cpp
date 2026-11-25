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

// FUNCTION: STARFLEETOP 0x006469a0
iTruth tSoundChannel::mTryToLocateInSoftware()
{
    unsigned char inSoftware = (mGetSoundType() & 0x80) > 0;
    return inSoftware;
}

// FUNCTION: STARFLEETOP 0x006469c0
iTruth tSoundChannel::mAreWeFixedPan()
{
    unsigned char fixedPan = (mGetSoundType() & 0x100) > 0;
    return fixedPan;
}

// FUNCTION: STARFLEETOP 0x006469e0
iTruth tSoundChannel::mAreWeFixedVolume()
{
    unsigned char fixedVolume = (mGetSoundType() & 0x200) > 0;
    return fixedVolume;
}

// FUNCTION: STARFLEETOP 0x00646a00
iTruth tSoundChannel::mAreWeAGlobalSound()
{
    unsigned char globalSound = (mGetSoundType() & 0x400) > 0;
    return globalSound;
}

// FUNCTION: STARFLEETOP 0x00646a20
iTruth tSoundChannel::mDoWeRetainSelections()
{
    unsigned char retain = (mGetSoundType() & 0x800) > 0;
    return retain;
}

// FUNCTION: STARFLEETOP 0x00646a40
iTruth tSoundChannel::mDoWeAutoRemoveChannel()
{
    unsigned char autoRemove = (mGetSoundType() & 0x1000) > 0;
    return autoRemove;
}

// FUNCTION: STARFLEETOP 0x00646a60
iTruth tSoundChannel::mDoWeAutoRemoveSelections()
{
    unsigned char autoRemove = (mGetSoundType() & 0x2000) > 0;
    return autoRemove;
}

// FUNCTION: STARFLEETOP 0x00646a80
unsigned char tSoundChannel::mAutoRemoveChannel()
{
    if ((mGetSoundType() & 0x1000) > 0) {
        return 1;
    }
    mSetSoundType(mGetSoundType() + 0x1000);
    return 1;
}

// FUNCTION: STARFLEETOP 0x00646ac0
unsigned char tSoundChannel::mAutoRemoveSelections()
{
    if ((mGetSoundType() & 0x2000) > 0) {
        return 1;
    }
    mSetSoundType(mGetSoundType() + 0x2000);
    return 1;
}

// FUNCTION: STARFLEETOP 0x00646b00
long tSoundChannel::mGetUserPositionContext()
{
    return field_0xa0;
}

// FUNCTION: STARFLEETOP 0x00646b10
void tSoundChannel::mSetUserPositionContext(long context)
{
    field_0xa0 = context;
}
