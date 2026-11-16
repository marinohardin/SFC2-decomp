#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "Tiger/tgSound.h"
#include "unsorted/tEntity.h"
// HYPOTHESIS: gSoundSay is defined in this unit in the original (its static initializer is
// here); its constructor arguments are not recovered yet, so it is declared only.
extern tSay gSoundSay;

// Error messages raised while the sound interrupt runs are kept here and printed later
// by PrintOutBufferedErrors, outside the interrupt.
static char gSoundFmtStr[32][256];
static long mNumberOfErrors; // HYPOTHESIS: long (compared signed with 32)

// FUNCTION: STARFLEETOP 0x00646ce0
void AddSoundErrorDuringInterrupt(const char* format, ...)
{
    if (mNumberOfErrors < 32)
    {
        va_list args;
        va_start(args, format);
        vsprintf(gSoundFmtStr[mNumberOfErrors++], format, args);
    }
}

// FUNCTION: STARFLEETOP 0x00646d10
void PrintOutBufferedErrors()
{
    long count = mNumberOfErrors;
    for (long i = 0; i < count; i++)
        gSoundSay.mJustSayIt("AsynchERROR: %s", gSoundFmtStr[i]);
    mNumberOfErrors = 0;
}
// MATCH-ONLY: tgSound.h now declares the classes that stood here; the lines are kept so line numbers stay.
// HYPOTHESIS: tMemoryPart is declared with its part binding (claimed at 0x004133d3) in another unit; it is restated here only as far
// as tChannelGroupManagerPart::mIsReady needs it. The exe's data names put the Test_ function of this unit's binding copy
// (0x0064fc00; cache 0x008e0b54, type descriptor 0x0083c608) on tMemoryPart, not tSoundPart; it is not claimed here.
class tMemoryPart : public tPart { public: tMemoryPart() {} GenAbstractBinding(tMemoryPart); };

// The scalar deleting destructor the compiler generates from the virtual ~tSoundChannelList is 0x0064fbc0.
inline tSoundChannelList::~tSoundChannelList()
{
}






















// HYPOTHESIS: the original reports sound errors through a macro of this shape: buffered while
// the sound interrupt runs, said at once otherwise.
#define SOUND_ERROR(args)                                   \
    if (atgSoundPart->mAreWeInInterruptMode() == kTrue)     \
        AddSoundErrorDuringInterrupt args;                  \
    else                                                    \
    {                                                       \
        long count = mNumberOfErrors;                       \
        for (long i = 0; i < count; i++)                    \
            gSoundSay.mJustSayIt("AsynchERROR: %s", gSoundFmtStr[i]); \
        mNumberOfErrors = 0;                                \
        gSoundSay.mJustSayIt args;                          \
    }

// FUNCTION: STARFLEETOP 0x00646d60
iTruth GetSilenceData(long bits)
{
    iTruth result = 0;
    if (bits == 8)
        result = 0x80;
    else if (bits == 16)
        result = 0;
    else
    {
        SOUND_ERROR(("GetSilenceData called with parameter -- %ld! (we only allow values of 8 and 16)", bits));
        SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    }
    return result;
}

// t3DListenerProfile (tgsound.h in the original) is declared in unsorted/t3DListenerProfile.h.




// MATCH-ONLY: these lines stand in for its earlier declaration here, so the __LINE__ values below
// are unchanged.











// FUNCTION: STARFLEETOP 0x00646e70
void t3DListenerProfile::mInit()
{
    f3DPosition.x = 0.0f;
    f3DPosition.y = 0.0f;
    f3DPosition.z = 1.0f;
    fForwardDirection.x = 0.0f;
    fForwardDirection.y = 0.0f;
    fForwardDirection.z = -1.0f;
    fUpDirection.x = 0.0f;
    fUpDirection.y = 1.0f;
    fUpDirection.z = 0.0f;
    fVelocity.x = 0.0f;
    fVelocity.y = 0.0f;
    fVelocity.z = 0.0f;
    fUnitScaleInMeters = 1.0;
}

// FUNCTION: STARFLEETOP 0x00646eb0
t3DListenerProfile& t3DListenerProfile::operator=(t3DListenerProfile& other)
{
    f3DPosition.x = other.f3DPosition.x;
    f3DPosition.y = other.f3DPosition.y;
    f3DPosition.z = other.f3DPosition.z;
    fVelocity.x = other.fVelocity.x;
    fVelocity.y = other.fVelocity.y;
    fVelocity.z = other.fVelocity.z;
    fUnitScaleInMeters = other.fUnitScaleInMeters;
    fForwardDirection.x = other.fForwardDirection.x;
    fForwardDirection.y = other.fForwardDirection.y;
    fForwardDirection.z = other.fForwardDirection.z;
    fUpDirection.x = other.fUpDirection.x;
    fUpDirection.y = other.fUpDirection.y;
    fUpDirection.z = other.fUpDirection.z;
    return *this;
}

// MATCH-ONLY: the original's mDump reports its lines as 261-265 through __LINE__; the functions
// that sat between operator= and mDump in the original are not recovered yet, so blank lines
// stand in for them.








































































































// FUNCTION: STARFLEETOP 0x00646f10
void t3DListenerProfile::mDump()
{
    SOUND_ERROR(("Listener: 3DPosition=(%f,%f,%f)", f3DPosition.x, f3DPosition.y, f3DPosition.z)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    SOUND_ERROR(("Listener: ForwardDirection=(%f,%f,%f)", fForwardDirection.x, fForwardDirection.y, fForwardDirection.z)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    SOUND_ERROR(("Listener: UpDirection=(%f,%f,%f)", fUpDirection.x, fUpDirection.y, fUpDirection.z)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    SOUND_ERROR(("Listener: Velocity=(%f,%f,%f)", fVelocity.x, fVelocity.y, fVelocity.z)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    SOUND_ERROR(("Listener: fUnitScaleInMeters=%f", fUnitScaleInMeters)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
}

// MATCH-ONLY: tgSound.h now declares t3DSoundProfile; the lines are kept so line numbers stay.







































// FUNCTION: STARFLEETOP 0x00647460
void t3DSoundProfile::mInit()
{
    field_0x8[0] = 0.0f;
    field_0x8[1] = 0.0f;
    field_0x8[2] = 1.0f;
    field_0x14[0] = 0.0f;
    field_0x14[1] = 0.0f;
    field_0x14[2] = -1.0f;
    field_0x20[0] = 0.0f;
    field_0x20[1] = 0.0f;
    field_0x20[2] = 0.0f;
    field_0x30 = 1.0;
    field_0x38 = 90.0f;
    field_0x3c = 50;
}

// FUNCTION: STARFLEETOP 0x006474a0
t3DSoundProfile& t3DSoundProfile::operator=(t3DSoundProfile& other)
{
    field_0x8[0] = other.field_0x8[0];
    field_0x8[1] = other.field_0x8[1];
    field_0x8[2] = other.field_0x8[2];
    field_0x14[0] = other.field_0x14[0];
    field_0x14[1] = other.field_0x14[1];
    field_0x14[2] = other.field_0x14[2];
    field_0x20[0] = other.field_0x20[0];
    field_0x20[1] = other.field_0x20[1];
    field_0x20[2] = other.field_0x20[2];
    field_0x30 = other.field_0x30;
    field_0x38 = other.field_0x38;
    field_0x3c = other.field_0x3c;
    return *this;
}

// MATCH-ONLY: the original's mDump reports its lines as 378-382 through __LINE__; blank lines
// stand in for the code that sat between operator= and mDump in the original.
// FUNCTION: STARFLEETOP 0x0064bb10
inline t3DSoundProfile::~t3DSoundProfile()
{
}


























// FUNCTION: STARFLEETOP 0x00647500
void t3DSoundProfile::mDump()
{
    SOUND_ERROR(("3DSound: 3DSoundPosition=(%f,%f,%f)", field_0x8[0], field_0x8[1], field_0x8[2])); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    SOUND_ERROR(("3DSound: 3DSoundOrientation=(%f,%f,%f)", field_0x14[0], field_0x14[1], field_0x14[2])); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    SOUND_ERROR(("3DSound: 3DSoundVelocity=(%f,%f,%f)", field_0x20[0], field_0x20[1], field_0x20[2])); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    SOUND_ERROR(("3DSound: 3DSoundReferenceDistance=%f", field_0x30)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    SOUND_ERROR(("3DSound: 3DSoundConeAngleForNoAttenuation=%f, 3DSoundConeAttenuationLevel=%ld", field_0x38, field_0x3c)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
}

// MATCH-ONLY: tgSound.h now declares tChannelAttributes; the lines are kept so line numbers stay.

















// FUNCTION: STARFLEETOP 0x00647a30
tChannelAttributes::tChannelAttributes()
{
    mFrequency = 22050;
    field_0xc = 0;
    field_0x10 = 80;
    m3DSoundProfile.mInit();
}

// FUNCTION: STARFLEETOP 0x00647ad0
void tChannelAttributes::mInit()
{
    mFrequency = 22050;
    field_0xc = 0;
    field_0x10 = 80;
    m3DSoundProfile.mInit();
}

// MATCH-ONLY: the original's mDump reports its line as 463 through __LINE__; blank lines stand in for the code that sat between mInit and mDump in the original.






































// FUNCTION: STARFLEETOP 0x00647af0
void tChannelAttributes::mDump()
{
    SOUND_ERROR(("Frequency=%ld, Pan=%ld, Volume=%ld", mFrequency, field_0xc, field_0x10)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    // HYPOTHESIS: the original calls the profile's mDump through its vtable, so through a pointer
    // (perhaps an inline accessor); calling it on the member directly would bind statically.
    t3DSoundProfile* profile = &m3DSoundProfile;
    profile->mDump();
}

// MATCH-ONLY: tgSound.h now declares the classes that stood here; the lines are kept so line numbers stay.





















// FUNCTION: STARFLEETOP 0x00647c10
tSoundChannelFormat::tSoundChannelFormat()
{
    fBufferLength = 65000;
    fNumChannels = 1;
    fSamplesPerSec = 22050;
    fBitsPerSample = 16;
    fSoundType = 1;
}

// FUNCTION: STARFLEETOP 0x00647c40
tSoundChannelFormat& tSoundChannelFormat::operator=(tSoundChannelFormat& other)
{
    fBufferLength = other.fBufferLength;
    fNumChannels = other.fNumChannels;
    fSamplesPerSec = other.fSamplesPerSec;
    fBitsPerSample = other.fBitsPerSample;
    fSoundType = other.fSoundType;
    return *this;
}

// FUNCTION: STARFLEETOP 0x00647c70
void tSoundChannelFormat::mInit()
{
    fBufferLength = 65000;
    fNumChannels = 1;
    fSamplesPerSec = 22050;
    fBitsPerSample = 16;
    fSoundType = 1;
}

// MATCH-ONLY: the original's mDump reports its line as 546 through __LINE__; blank lines stand in
// for the code that sat between mInit and mDump in the original.


















// FUNCTION: STARFLEETOP 0x00647ca0
void tSoundChannelFormat::mDump()
{
    SOUND_ERROR(("fBufferLength=%ld, fNumChannels=%ld, fSamplesPerSec=%ld, fBitsPerSample=%ld, fSoundType=%ld", fBufferLength, fNumChannels, fSamplesPerSec, fBitsPerSample, fSoundType)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
}

// MATCH-ONLY: Tiger/tSndMemMgr.h declares tListMemberWithHandle; the lines are kept so line numbers stay.
























// tAtomicLock is declared before tgSoundPart, which holds two.
// MATCH-ONLY: these lines stand in for its earlier declaration here, so the __LINE__ values below
// are unchanged.




// MATCH-ONLY: tPoint3 is declared in unsorted/tEntity.h; the lines are kept so line numbers stay.




// MATCH-ONLY: tgSound.h now declares the sound command enums; the lines are kept so line numbers stay.



// MATCH-ONLY: tgSound.h now declares tSoundSelection; the lines are kept so line numbers stay.















// MATCH-ONLY: tgSound.h now declares tSoundChannelCommand; the lines are kept so line numbers stay.















































// FUNCTION: STARFLEETOP 0x00647dc0
tSoundChannelCommand::tSoundChannelCommand()
{
    fExecuteAfterThisNumberAttaches = 0;
    fAvailable.mRelease();
    fSoundChannelCommand = (eSoundChannelCommand)-1;
    field_0x40 = 0;
    field_0x98 = 0;
}
// FUNCTION: STARFLEETOP 0x00650fc0
// FUNCTION: STARFLEETOP 0x00650f70
tSoundChannelCommand::~tSoundChannelCommand()
{
}

// MATCH-ONLY: the original's mDump reports its lines as 1122-1231 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line number is set here.
#line 1119
// FUNCTION: STARFLEETOP 0x00647e40
void tSoundChannelCommand::mDump()
{
    SOUND_ERROR(("command: soundChannelCommand=%ld, fAvailable=%ld, executeAfterThisNumberAttaches =%ld", fSoundChannelCommand, fAvailable, fExecuteAfterThisNumberAttaches)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    switch (fSoundChannelCommand)
    {
    case 0:
        vfunction32()->mDump();
        break;





    case 5:
    {
        long frequency = vfunction18();
        SOUND_ERROR(("    frequency=%ld", frequency)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 6:
    {
        long pan = mGetPan();
        SOUND_ERROR(("    pan=%ld", pan)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 7:
    {
        long volume = vfunction22();
        SOUND_ERROR(("    volume=%ld", volume)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 8:
        mGet3DSoundProfile()->mDump();
        break;









    case 14:
    {
        tPoint3* position = mGet3DSoundPosition();
        SOUND_ERROR(("    3DSoundPosition=(%f,%f,%f)", position->x, position->y, position->z)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 15:
    {
        tPoint3* orientation = mGet3DSoundOrientation();
        SOUND_ERROR(("    3DSoundOrientation=(%f,%f,%f)", orientation->x, orientation->y, orientation->z)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 16:
    {
        float angle = mGet3DSoundConeAngleForNoAttenuation();
        long level = mGet3DSoundConeAttenuationLevel();
        SOUND_ERROR(("    coneAngleForNoAttentuation=%f, attenuationLevel=%ld", angle, level)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 17:
    {
        tPoint3* velocity = mGet3DSoundVelocity();
        SOUND_ERROR(("    3DSoundVelocity=(%f,%f,%f)", velocity->x, velocity->y, velocity->z)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 18:
    {
        double distance = mGet3DSoundReferenceDistance();
        SOUND_ERROR(("    3DSoundReferenceDistance=%f", distance)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 19:
    {
        long priority = vfunction24();
        SOUND_ERROR(("    SoundPriority=%ld", priority)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 22:
    {
        double delay = mGetDelayImmediateCommandsUntilThisTime();
        SOUND_ERROR(("    DelayImmediateCommandsUntilThisTime=%f", delay)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 23:
    {
        long volume = mGetFinalVolume();
        double timeToTake = mGetTimeToTake();
        SOUND_ERROR(("    volume=%ld, timeToTake = %f", volume, timeToTake)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }

    case 11:
    {
        long userData = mGetUserData();
        long callback = vfunction28();
        SOUND_ERROR(("    FinishSoundCallBack=%ld, UserData = %ld", callback, userData)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 20:
    {
        long userData = mGetUserPositionContext();
        long function = vfunction50();
        SOUND_ERROR(("    UpdateSoundPositionFunction=%ld, UserData = %ld", function, userData)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    case 21:
    {
        double milliseconds = mGetMillisecondsBetweenUpdates();
        SOUND_ERROR(("    millisecondsBetweenUpdates=%f", milliseconds)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    }
    tListMemberWithHandle::mDump();
}

// MATCH-ONLY: the harness reads the listing's source-line records, which stop past the end of
// this file; the line number is reset so the function carries one.
#line 792
// 0x00648cea is a jump table after the preceding function's ret, not code.





// MATCH-ONLY: tgSound.h now declares tSoundCommandList; the lines are kept so line numbers stay.














// MATCH-ONLY: the original reports this error on line 1253 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set here.
#line 1248
// FUNCTION: STARFLEETOP 0x00648d50
tListMemberWithHandle* tSoundCommandList::mGetListMemberWithHandleFromHandle(long handle)
{
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    return atgSoundPart->mGetSoundChannelCommandPtrFromHandle(handle);
}

// tSoundChannelList is declared before tgSoundPart, which holds one.
// MATCH-ONLY: these lines stand in for its earlier declaration here, so the
// __LINE__ values below are unchanged.




// MATCH-ONLY: the original reports this error on line 1270 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set here.
// The list forwards the lookup to tgSoundPart::mGetSoundChannelPtrFromHandle (slot 132).
// The error text is the original's, a "No sound memory block manager!" string.
#line 1265
// FUNCTION: STARFLEETOP 0x00648e60
tListMemberWithHandle* tSoundChannelList::mGetListMemberWithHandleFromHandle(long handle)
{
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound memory block manager!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    return (tListMemberWithHandle*)atgSoundPart->mGetSoundChannelPtrFromHandle(handle);
}

// MATCH-ONLY: tgSound.h now declares tSoundChannel; the lines are kept so line numbers stay.



































































// MATCH-ONLY: the original reports its errors on lines 1295-1306 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line number is set here.
#line 1290
// FUNCTION: STARFLEETOP 0x00648f70
iTruth tSoundChannel::mAddDelayedCommand(long handle)
{
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("Can't add delayed commands in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 1;
    }
    tSoundChannelCommand* command = (tSoundChannelCommand*)atgSoundPart->mGetSoundChannelCommandPtrFromHandle(handle);
    if (atgSoundPart == 0) // HYPOTHESIS: the exe tests the sound part here, not the command "No command!" suggests
    {
        SOUND_ERROR(("No command!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    command->vfunction15(field_0x104 - field_0x100 - 1);
    // HYPOTHESIS: the original calls the list through its vtable, so through a pointer.
    tSoundCommandList* delayedCommands = &field_0xcc;
    delayedCommands->vfunction26(handle);
    atgSoundPart->vfunction123(field_0c);
    return 1;
}

// MATCH-ONLY: the original reports its errors on lines 1322 and 1340 through __LINE__; the code
// that sat before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: tgSoundPart slot 125 returns the current time in milliseconds; timeToTake is in
// seconds. The field at 0x118 is set while a fade runs, 0x120 holds the time the fade ends and
// 0x128 the volume change per millisecond.
#line 1317
// FUNCTION: STARFLEETOP 0x00649290
iTruth tSoundChannel::mFadeToVolume(long volume, double timeToTake)
{
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (field_0x118 <= 0)
    {
        field_0x118 = 1;
        fFadeVolume = volume;
        field_0x120 = timeToTake;
        double now = atgSoundPart->vfunction125();
        double duration = timeToTake * 1000.0;
        field_0x120 = now + duration;
        if (field_0x120 <= now)
            field_0x128 = 0;
        else
            field_0x128 = (mGetVolume() - fFadeVolume) / duration;



        SOUND_ERROR(("Fade-> mGetVolume%ld fFadeVolume%ld %f %f", mGetVolume(), fFadeVolume, timeToTake, field_0x128)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 1;
    }
    return 0;
}

// MATCH-ONLY: the original reports its error on line 1352 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: the command list at 0xec is the attachment list ("on attachment list"); its slot 5
// returns the first member. A command of type 24 is the one kind the list may hold; any other
// type is reported.
#line 1344
// FUNCTION: STARFLEETOP 0x00649570
tSoundChannelCommand* tSoundChannel::mGetFirstAttachment()
{
    // HYPOTHESIS: the original calls the list through its vtable, so through a pointer.
    tSoundCommandList* attachments = &field_0xec;
    tSoundChannelCommand* command = (tSoundChannelCommand*)attachments->vfunction5();
    if (command && command->vfunction16() && command->vfunction16() != 24)
    {
        SOUND_ERROR(("Big trouble -- command of this type %ld on attachment list", command->vfunction16())); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    }
    return (tSoundChannelCommand*)attachments->vfunction5();
}

// MATCH-ONLY: the original reports its error on line 1363 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: the fade is the one mFadeToVolume starts, to volume 0, marked 2 in field 0x118.
// Unless the sound part runs in active mode (slot 13) or slot 82 says otherwise, the update
// function at 0x138 is run, bracketed by tgSoundPart slots 121 and 122, until slot 61's time
// reaches twice timeToTake (at most 32 times); then a type-24 command from tgSoundPart slot 138 is
// handed to slot 15.
#line 1358
// FUNCTION: STARFLEETOP 0x006496c0
iTruth tSoundChannel::mFadeToStopAndResetNow(double timeToTake)
{
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (field_0x118 <= 2)
    {
        field_0x118 = 2;
        fFadeVolume = 0;
        field_0x120 = timeToTake;
        double now = atgSoundPart->vfunction125();
        double duration = timeToTake * 1000.0;
        field_0x120 = now + duration;
        if (field_0x120 <= now)
            field_0x128 = 0;
        else
            field_0x128 = (mGetVolume() - fFadeVolume) / duration;
        if (atgSoundPart->vfunction13() == 0 && !mDoWeRetainSelections())
        {
            if (field_0x138 != 0)
            {
                timeToTake += timeToTake;
                for (long i = 0; mGetMillisecondsLeftToPlay() < timeToTake && i < 32; i++)
                {
                    atgSoundPart->vfunction121();
                    field_0x16c = 1;
                    field_0x138(field_0x13c, field_0c);
                    atgSoundPart->vfunction122();
                }
            }
            long command = atgSoundPart->mCreateCommand((eSoundChannelCommand)24, 0, 0);
            mAddAttachment(command);
        }
        return 1;
    }
    return 0;
}

// MATCH-ONLY: the original reports its error on line 1414 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: as mFadeToStopAndResetNow, with the fade marked 3 in field 0x118 and without the
// slot 82 test.
#line 1409
// FUNCTION: STARFLEETOP 0x00649920
iTruth tSoundChannel::mFadeToStopAndClearNow(double timeToTake)
{
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (field_0x118 <= 3)
    {
        field_0x118 = 3;
        fFadeVolume = 0;
        field_0x120 = timeToTake;
        double now = atgSoundPart->vfunction125();
        double duration = timeToTake * 1000.0;
        field_0x120 = now + duration;
        if (field_0x120 <= now)
            field_0x128 = 0;
        else
            field_0x128 = (mGetVolume() - fFadeVolume) / duration;
        if (atgSoundPart->vfunction13() == 0)
        {
            if (field_0x138 != 0)
            {
                timeToTake += timeToTake;
                for (long i = 0; mGetMillisecondsLeftToPlay() < timeToTake && i < 32; i++)
                {
                    atgSoundPart->vfunction121();
                    field_0x16c = 1;
                    field_0x138(field_0x13c, field_0c);
                    atgSoundPart->vfunction122();
                }
            }
            long command = atgSoundPart->mCreateCommand((eSoundChannelCommand)24, 0, 0);
            mAddAttachment(command);
        }
        return 1;
    }
    return 0;
}

// MATCH-ONLY: the original reports its errors on lines 1469-1484 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: the attachment list at 0xec adds the handle through slot 26 and reports success.
// The fields at 0x108-0x114 and 0x148-0x158 are not recovered: 0x108 holds the first attached
// handle (-1 while none is), 0x114 sums a value from each command's sound selection. Slots 71 and
// 72 are two tests that, when both pass, copy the attachment range into 0x148-0x158.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation; the error lines are then set
// inside the body.
#line 1108
// FUNCTION: STARFLEETOP 0x00649b70
iTruth tSoundChannel::mAddAttachment(long handle)
{
#line 1467
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("Can't add attachments in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 1;
    }
    field_0x16c = 0;
    tSoundChannelCommand* command = (tSoundChannelCommand*)atgSoundPart->mGetSoundChannelCommandPtrFromHandle(handle);



    if (command == 0)
    {
        SOUND_ERROR(("No command!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    tSoundSelection* selection = command->vfunction32();
    if (selection != 0)
    {
        if (selection->vfunction26() <= selection->vfunction9())
            selection->vfunction14(0);
    }
    // HYPOTHESIS: the original calls the list through its vtable, so through a pointer.
    tSoundCommandList* attachments = &field_0xec;
    if (attachments->vfunction26(handle) == 1)
    {
        if (field_0x108 == -1)
        {
            field_0x108 = handle;
            field_0x10c = field_0x104 + field_0x100;
        }
        if (selection != 0)
            field_0x114 += selection->vfunction27();
        field_0x104++;
    }
    atgSoundPart->vfunction123(field_0c);
    if (mAreWeLooping() == 1 && mAreWeStreaming() == 1)
    {
        field_0x148 = 1;
        field_0x14c = field_0x100;
        field_0x150 = field_0x100;
        field_0x154 = field_0x104 + field_0x100 - 1;
        field_0x158 = field_0x104 + field_0x100 - 1;
    }
    return 1;
}

// MATCH-ONLY: the original reports its error on line 1542 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: the command list at 0xdc is the immediate-command list; its slot 26 adds a handle,
// as the delayed list at 0xcc does. Field 0xc goes to tgSoundPart slot 123.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation; the error line is then set
// inside the body.
#line 1172
// FUNCTION: STARFLEETOP 0x00649f50
iTruth tSoundChannel::mAddImmediateCommand(long handle)
{
#line 1540
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    // HYPOTHESIS: the original calls the list through its vtable, so through a pointer.
    tSoundCommandList* immediateCommands = &field_0xdc;
    immediateCommands->vfunction26(handle);
    atgSoundPart->vfunction123(field_0c);
    return 1;
}

// MATCH-ONLY: the original reports its error on line 1559 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: the command list at 0xbc is the immediate non-interrupt command list; its slot 26
// adds a handle, as the immediate list at 0xdc does. Field 0xc goes to tgSoundPart slot 123.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation; the error line is then set
// inside the body.
#line 1196
// FUNCTION: STARFLEETOP 0x0064a080
iTruth tSoundChannel::mAddImmediateNonInterruptCommandCommand(long handle)
{
#line 1557
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    // HYPOTHESIS: the original calls the list through its vtable, so through a pointer.
    tSoundCommandList* immediateNonInterruptCommands = &field_0xbc;
    immediateNonInterruptCommands->vfunction26(handle);
    atgSoundPart->vfunction123(field_0c);
    return 1;
}

// MATCH-ONLY: the original reports its error on line 1577 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: the format at 0x10 and the attributes at 0x30 are reset through their mInit; the
// command lists are reset through their slot 2. The other fields are not recovered.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation; the error line is then set
// inside the body.
#line 1220
// FUNCTION: STARFLEETOP 0x0064a1d0
void tSoundChannel::mInit()
{
#line 1575
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    // HYPOTHESIS: the original calls the members through their vtables (perhaps inline accessors).
    (&field_0x10)->mInit();
    field_0x28 = 2;
    field_0x16c = 0;
    (&field_0x30)->mInit();
    field_0x88 = 0;
    field_0x8c = 0;
    field_0x90 = 0;
    field_0x9c = 0;
    field_0xa0 = 0;
    field_0xa8 = atgSoundPart->vfunction125();
    field_0xb0 = 500000.0;
    field_0x118 = 0;
    fFadeVolume = 0;
    field_0x120 = 0;
    field_0x128 = 0;
    field_0x178 = 0;
    field_0x94 = (&field_0x10)->vfunction12();
    field_0x98 = (&field_0x10)->vfunction7() * field_0x94;
    (&field_0xcc)->vfunction2();
    (&field_0xdc)->vfunction2();
    (&field_0xec)->vfunction2();
    (&field_0xbc)->vfunction2();
    field_0xfc = 1;
    field_0x100 = 0;
    field_0x104 = 0;
    field_0x108 = -1;
    field_0x10c = 0;
    field_0x110 = 0;
    field_0xb8 = 0;
    field_0x114 = 0;
    field_0x130 = 0;
    field_0x138 = 0;
    field_0x13c = -1;
    field_0x140 = 1000.0;
    field_0x148 = 0;
    field_0x14c = 0;
    field_0x150 = -1;
    field_0x154 = -2;
    field_0x158 = -3;
    field_0x15c = 0;
    field_0x160 = 0;
    field_0x164 = 0;
    field_0x168 = (&field_0x10)->vfunction3();
}

// MATCH-ONLY: the original reports its error on line 1659 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: mSetUpdateMode (slot 26) sets mode 4 when done, which field 0xfc then holds; slot
// 103 is not recovered. Field 0xc goes to tgSoundPart slot 123.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation; the error line is then set
// inside the body.
#line 1285
// FUNCTION: STARFLEETOP 0x0064a460
void tSoundChannel::mDone()
{
#line 1657
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    mSetUpdateMode((eSoundChannelUpdateMode)4);
    mStopAndClearChannel();
    mClearAll();
    if (field_0xfc == 4)
        atgSoundPart->vfunction123(field_0c);
}

// MATCH-ONLY: the original reports its error on line 1686 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: slot 29 returns the first delayed command and slot 28 the first immediate command;
// each command is taken off its list (list slot 33) and released (command slot 12) until the list
// is empty or a type-24 command is reached. A type-11 command hands its finish callback and user
// data to slots 67 and 65; a type-1 command runs the finish callback at 0x90 with the user data at
// 0x8c, bracketed by tgSoundPart slots 121 and 122. Slot 33, called first, is not recovered.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation; the error line is then set
// inside the body.
#line 1310
// FUNCTION: STARFLEETOP 0x0064a590
void tSoundChannel::mClearCommands()
{
#line 1683
    vfunction33();
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    tSoundChannelCommand* command = mGetFirstDelayedCommand();
    while (command != 0)
    {
        if (command->vfunction16() == 11)
        {
            mSetFinishSoundCallback(command->vfunction28());
            mSetUserData(command->mGetUserData());
            (&field_0xcc)->mRemoveFromList(command);
            command->mMakeAvailable();
            command = mGetFirstDelayedCommand();
        }
        else if (command->vfunction16() == 1)
        {
            if (field_0x90 != 0)
            {
                atgSoundPart->vfunction121();
                field_0x90(field_0x8c, 1);
                atgSoundPart->vfunction122();
            }
            (&field_0xcc)->mRemoveFromList(command);
            command->mMakeAvailable();
            command = mGetFirstDelayedCommand();
        }
        else if (command->vfunction16() == 24)
        {
            (&field_0xcc)->mRemoveFromList(command);
            command->mMakeAvailable();
            command = 0;
        }
        else
        {
            (&field_0xcc)->mRemoveFromList(command);
            command->mMakeAvailable();
            command = mGetFirstDelayedCommand();
        }
    }
    command = mGetFirstImmediateCommand();
    while (command != 0)
    {
        if (command->vfunction16() == 11)
        {
            mSetFinishSoundCallback(command->vfunction28());
            mSetUserData(command->mGetUserData());
            (&field_0xdc)->mRemoveFromList(command);
            command->mMakeAvailable();
            command = mGetFirstImmediateCommand();
        }
        else if (command->vfunction16() == 1)
        {
            if (field_0x90 != 0)
            {
                atgSoundPart->vfunction121();
                field_0x90(field_0x8c, 1);
                atgSoundPart->vfunction122();
            }
            (&field_0xdc)->mRemoveFromList(command);
            command->mMakeAvailable();
            command = mGetFirstImmediateCommand();
        }
        else if (command->vfunction16() == 24)
        {
            (&field_0xdc)->mRemoveFromList(command);
            command->mMakeAvailable();
            command = 0;
        }
        else
        {
            (&field_0xdc)->mRemoveFromList(command);
            command->mMakeAvailable();
            command = mGetFirstImmediateCommand();
        }
    }
}

// MATCH-ONLY: the original reports its errors on lines 1782-1792 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: slot 74 says whether the channel plays a streaming sound; only those take the
// function, which is kept with its user data and period at 0x138-0x140.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation; the error lines are then set
// inside the body.
#line 1402
// FUNCTION: STARFLEETOP 0x0064a890
iTruth tSoundChannel::mAddGetNextAttachmentFunction(void (*function)(unsigned long, long), long userData, double milliseconds)
{
#line 1780
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("Can't do AddGetNextAttachmentFunction in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 1;
    }
    if (mAreWeStatic() == 1 && function != 0)
    {
        SOUND_ERROR(("Only streaming sounds can have a getNextAttachment Function!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    field_0x138 = function;
    field_0x13c = userData;
    field_0x140 = milliseconds;
    field_0x16c = 0;
    return 1;
}

// MATCH-ONLY: the original reports its errors on lines 1807-1841 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: the attachment list at 0xec is walked by handle: slot 3 returns the first handle,
// slot 1 the command for a handle and slot 9 the next handle. Commands of type 0 play a sound
// selection whose slot 27 gives its bytes left; type 24 is the one other type the list may hold.
// Slots 72 and 82 are the channel tests the "retained or static" warning names.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation; the error lines are then set
// inside the body.
#line 1438
// FUNCTION: STARFLEETOP 0x0064abb0
long tSoundChannel::mGetTotalBytesLeftToPlay()
{
#line 1805
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("mGetTotalBytesLeftToPlay not currently supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (!mAreWeStreaming() || mDoWeRetainSelections() == 1)
    {

        SOUND_ERROR(("Getting Total bytes left to play for either a retained or static channel -- results will be wrong!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    }
    // HYPOTHESIS: the original calls the list through its vtable, so through a pointer.
    tSoundCommandList* attachments = &field_0xec;
    long handle = attachments->vfunction3();
    long total = 0;
    tSoundChannelCommand* command = (tSoundChannelCommand*)attachments->mGetListMemberWithHandleFromHandle(handle);
    while (command != 0)
    {
        if (command->vfunction16() == 0)
        {
            tSoundSelection* selection = command->vfunction32();
            if (selection != 0)
                total += selection->vfunction27();
        }
        else if (command->vfunction16() != 24)
        {
#line 1841
            SOUND_ERROR(("Big trouble -- command of this type %ld on attachment list", command->vfunction16())); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        }
        handle = attachments->vfunction9(handle);
        command = (tSoundChannelCommand*)attachments->mGetListMemberWithHandleFromHandle(handle);
    }
    return total;
}

// HYPOTHESIS: slot 68 returns the bytes left to play and the field at 0x98 holds the bytes per
// second of the channel's format.
// FUNCTION: STARFLEETOP 0x0064b030
double tSoundChannel::mGetTotalNumberOfSecondsLeftToPlay()
{
    return (double)mGetTotalBytesLeftToPlay() / field_0x98;
}

// MATCH-ONLY: the original reports its error on line 1868 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set inside the body.
// HYPOTHESIS: as mClearCommands, but every command on both lists is taken off, type 24 included;
// the delayed list (slot 29) is cleared only when tgSoundPart slot 13 returns 0.
// FUNCTION: STARFLEETOP 0x0064b050
void tSoundChannel::mClearAllCommands()
{
#line 1865
    vfunction33();
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    if (atgSoundPart->vfunction13() == 0)
    {
        tSoundChannelCommand* command = mGetFirstDelayedCommand();
        while (command != 0)
        {
            if (command->vfunction16() == 11)
            {
                mSetFinishSoundCallback(command->vfunction28());
                mSetUserData(command->mGetUserData());
            }
            else if (command->vfunction16() == 1)
            {
                if (field_0x90 != 0)
                {
                    atgSoundPart->vfunction121();
                    field_0x90(field_0x8c, 1);
                    atgSoundPart->vfunction122();
                }
            }
            (&field_0xcc)->mRemoveFromList(command);
            command->mMakeAvailable();
            command = mGetFirstDelayedCommand();
        }
    }
    tSoundChannelCommand* command = mGetFirstImmediateCommand();
    while (command != 0)
    {
        if (command->vfunction16() == 11)
        {
            mSetFinishSoundCallback(command->vfunction28());
            mSetUserData(command->mGetUserData());
        }
        else if (command->vfunction16() == 1)
        {
            if (field_0x90 != 0)
            {
                atgSoundPart->vfunction121();
                field_0x90(field_0x8c, 1);
                atgSoundPart->vfunction122();
            }
        }
        (&field_0xdc)->mRemoveFromList(command);
        command->mMakeAvailable();
        command = mGetFirstImmediateCommand();
    }
}

// MATCH-ONLY: the original reports its errors on lines 1941 and 1946 through __LINE__; the code
// that sat before it in the original is not recovered yet, so the line number is set inside the
// body.
// HYPOTHESIS: slot 30 returns the first attachment. Each attachment is taken off the attachment
// list at 0xec until a type-24 command is reached, which is taken off and released last; a sound
// selection on a command is released (selection slot 2) after slot 39 when channel slot 84 says
// so. Once the list is empty the delayed commands (slot 29) are moved from the delayed list at
// 0xcc to the immediate list at 0xdc (list slot 27). The attachment range is then reset, or
// restarted at the first handle left on the list.
// FUNCTION: STARFLEETOP 0x0064b2d0
void tSoundChannel::mClearAttachments()
{
#line 1939
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("mClearAttachments not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    tSoundChannelCommand* command = mGetFirstAttachment();
    while (command != 0)
    {
        if (command->vfunction16() == 24)
        {
            (&field_0xec)->mRemoveFromList(command);
            command->mMakeAvailable();
            break;
        }
        else
        {
            (&field_0xec)->mRemoveFromList(command);
            tSoundSelection* selection = command->vfunction32();
            if (selection != 0)
            {
                if (mDoWeAutoRemoveSelections() == 1)
                    selection->vfunction39();
                selection->vfunction2();
            }
            field_0x100++;
            command->mMakeAvailable();
            field_0x104--;
            field_0x100++;
            command = mGetFirstAttachment();
            if (command == 0)
            {
                command = mGetFirstDelayedCommand();
                while (command != 0)
                {
                    (&field_0xcc)->mRemoveFromList(command);
                    (&field_0xdc)->mAddToBackOfList(command);
                    command = mGetFirstDelayedCommand();
                }
            }
        }
    }
    if (mGetFirstAttachment() == 0)
    {
        field_0x100 = 0;
        field_0x104 = 0;
        field_0x10c = 0;
        field_0x108 = -1;
        field_0x148 = 0;
        field_0x14c = 0;
        field_0x150 = -1;
        field_0x154 = -2;
        field_0x158 = -3;
        field_0x114 = 0;
    }
    else
    {
        field_0x10c = field_0x100;
        field_0x108 = (&field_0xec)->vfunction3();
    }
}

// MATCH-ONLY: the original reports its errors on lines 2035 and 2040 through __LINE__; the code
// that sat before it in the original is not recovered yet, so the line number is set inside the
// body.
// HYPOTHESIS: as mClearAttachments, but every attachment is taken off and released, type 24
// included; the delayed commands are then moved to the immediate list and the attachment range
// is reset.
// FUNCTION: STARFLEETOP 0x0064b610
void tSoundChannel::mClearAllAttachments()
{
#line 2033
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("mClearAllAttachments not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    tSoundChannelCommand* command = mGetFirstAttachment();
    while (command != 0)
    {
        (&field_0xec)->mRemoveFromList(command);
        tSoundSelection* selection = command->vfunction32();
        if (selection != 0)
        {
            if (mDoWeAutoRemoveSelections() == 1)
                selection->vfunction39();
            selection->vfunction2();
        }
        command->mMakeAvailable();
        field_0x104--;
        field_0x100++;
        command = mGetFirstAttachment();
        if (command == 0)
        {
            command = mGetFirstDelayedCommand();
            while (command != 0)
            {
                (&field_0xcc)->mRemoveFromList(command);
                (&field_0xdc)->mAddToBackOfList(command);
                command = mGetFirstDelayedCommand();
            }
        }
    }
    field_0x100 = 0;
    field_0x104 = 0;
    field_0x10c = 0;
    field_0x108 = -1;
    field_0x148 = 0;
    field_0x14c = 0;
    field_0x150 = -1;
    field_0x154 = -2;
    field_0x158 = -3;
    field_0x114 = 0;
}

// FUNCTION: STARFLEETOP 0x0064b8f0
void tSoundChannel::mSetUpdateMode(eSoundChannelUpdateMode mode)
{
    // HYPOTHESIS: field_0xfc holds the update mode, and the mode only ever rises.
    if (field_0xfc <= mode)
        field_0xfc = mode;
}

// MATCH-ONLY: the original reports its errors on lines 2119 and 2124 through __LINE__; the code
// that sat before it in the original is not recovered yet, so the line number is set inside the
// body.
// HYPOTHESIS: the field at 0x108 is the handle of the next attachment; its command carries the
// selection when it is of type 0.
// FUNCTION: STARFLEETOP 0x0064b910
tSoundSelection* tSoundChannel::mGetSelectionToAttach()
{
#line 2117
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("mGetSelectionToAttach not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    tSoundChannelCommand* command = (tSoundChannelCommand*)atgSoundPart->mGetSoundChannelCommandPtrFromHandle(field_0x108);
    if (command != 0 && command->vfunction16() == 0)
        return command->vfunction32();
    return 0;
}

// MATCH-ONLY: the original reports its errors on lines 2222, 2227 and 2261 through __LINE__; the
// code that sat before it in the original is not recovered yet, so the line numbers are set
// inside the body.
// HYPOTHESIS: the attachments from the handle at 0x108 on are walked until at least one sound
// selection has been seen and 44100 bytes are locked in; each selection is bracketed by its slots
// 22 and 23 around slot 26, which gives its bytes. Channel slot 39 is not recovered.
// FUNCTION: STARFLEETOP 0x0064bb20
void tSoundChannel::mLockInData()
{
#line 2220
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("mLockInData not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    tSoundSelection* selection = 0;
    mTestInList();
    long handle = field_0x108;
    long bytes = 0;
    tSoundChannelCommand* command = (tSoundChannelCommand*)atgSoundPart->mGetSoundChannelCommandPtrFromHandle(handle);
    while (command != 0 && (selection == 0 || bytes < 44100))
    {
        if (command->vfunction16() == 0)
        {
            selection = command->vfunction32();
            if (selection != 0)
            {
                selection->vfunction22();
                bytes += selection->vfunction26();
                selection->vfunction23();
            }
        }
        else if (command->vfunction16() != 24)
        {
#line 2261
            SOUND_ERROR(("Big trouble -- command of this type %ld on attachment list", command->vfunction16())); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        }
        // HYPOTHESIS: the original calls the list through its vtable, so through a pointer.
        tSoundCommandList* attachments = &field_0xec;
        handle = attachments->vfunction9(handle);
        command = (tSoundChannelCommand*)atgSoundPart->mGetSoundChannelCommandPtrFromHandle(handle);
    }
}

// MATCH-ONLY: the original reports its errors on lines 2436 and 2441 through __LINE__; the code
// that sat before it in the original is not recovered yet, so the line number is set inside the
// body.
// HYPOTHESIS: the attachment list at 0xec is walked past the attachments already played (the
// range start at 0x14c against the first index at 0x100), then on to the first command whose
// sound selection (slot 27) still has bytes left, within the range that ends at 0x158.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation.
#line 1778
// FUNCTION: STARFLEETOP 0x0064bed0
tSoundChannelCommand* tSoundChannel::mGetFirstPlayingCommand()
{
#line 2434
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("mGetFirstPlayingCommand not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (field_0x104 == 0)
        return 0;
    long count = field_0x104;
    long skip = 0;
    if (field_0x14c > field_0x100)
        skip = field_0x14c - field_0x100;
    // HYPOTHESIS: the original calls the list through its vtable, so through a pointer.
    tSoundCommandList* attachments = &field_0xec;
    tSoundChannelCommand* command = (tSoundChannelCommand*)attachments->vfunction5();
    while (command != 0 && skip > 0)
    {
        command = (tSoundChannelCommand*)attachments->vfunction14(command);
        skip--;
        count--;
    }
    long last = field_0x100 + field_0x104 - 1;
    if (field_0x158 < last && field_0x158 >= field_0x14c && field_0x158 >= field_0x100)
        count += field_0x158 - field_0x100 - field_0x104 + 1;
    if (command == 0)
        return 0;
    tSoundSelection* selection = 0;
    if (command->vfunction16() == 0)
        selection = command->vfunction32();
    while (count > 0 && selection != 0 && selection->vfunction27() == 0)
    {
        command = (tSoundChannelCommand*)attachments->vfunction14(command);
        if (command == 0)
            return 0;
        if (command->vfunction16() == 0)
            selection = command->vfunction32();
        count--;
    }
    if (count == 0)
        return 0;
    return command;
}

// MATCH-ONLY: the original reports its errors on lines 2514 and 2519 through __LINE__; the code
// that sat before it in the original is not recovered yet, so the line number is set inside the
// body.
// HYPOTHESIS: the attachment list at 0xec is walked to the last attachment to hand over (the
// range end at 0x154 against the first index at 0x100), whose command carries the selection.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation.
#line 1837
// FUNCTION: STARFLEETOP 0x0064c1c0
tSoundSelection* tSoundChannel::mGetLastSelectionToAttach()
{
#line 2512
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("mGetLastSelectionToAttach not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (field_0x104 == 0)
        return 0;
    long index;
    if (field_0x154 >= field_0x100)
        index = field_0x154 - field_0x100;
    else
        index = field_0x104 - field_0x100 - 1;
    tSoundCommandList* attachments = &field_0xec;
    // HYPOTHESIS: the original reuses one variable as the loop counter and the handle, so the
    // first handle is dropped and the walk counts on from each returned handle.
    long handle = attachments->vfunction3();
    for (handle = 0; handle < index; handle++)
        handle = attachments->vfunction9(handle);
    tSoundChannelCommand* command = (tSoundChannelCommand*)atgSoundPart->mGetSoundChannelCommandPtrFromHandle(handle);
    if (command == 0)
        return 0;
    return command->vfunction32();
}

// MATCH-ONLY: the original reports its errors on lines 2562, 2567 and 2658 through __LINE__; the
// code that sat before it in the original is not recovered yet, so the line numbers are set
// inside the body.
// HYPOTHESIS: the attachments at the head of the list at 0xec are taken off while their sound
// selections (slot 27) have no bytes left; each one taken off counts one more attachment played
// (0x100), and the delayed commands waiting for that count (command slot 14) are run through slot
// 89 and released. Slots 82, 84, 93 and selection slot 39 are not recovered.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation.
#line 1879
// FUNCTION: STARFLEETOP 0x0064c410
iTruth tSoundChannel::mCleanupSelections()
{
#line 2560
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 1;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("mCleanupSelections not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 1;
    }
    if (mDoWeRetainSelections() == 1)
        return 1;
    mUpdateAmountPlayed();
    tSoundChannelCommand* command = (tSoundChannelCommand*)(&field_0xec)->vfunction5();
    long played = field_0x100;
    while (command != 0)
    {
        tSoundSelection* selection = command->vfunction32();
        if (command->vfunction16() == 0 && selection != 0)
        {
            if (selection->vfunction27() != 0)
                return 1;
            tSoundChannelCommand* delayed = mGetFirstDelayedCommand();
            while (delayed != 0 && delayed->vfunction14() <= played && mDoCommand(delayed) == 1)
            {
                (&field_0xcc)->mRemoveFromList(delayed);
                delayed->mMakeAvailable();
                delayed = mGetFirstDelayedCommand();
            }
            if (mDoWeAutoRemoveSelections() == 1)
                selection->vfunction39();
            (&field_0xec)->mRemoveFromList(command);
            selection->vfunction2();
            field_0x104--;
            command->mMakeAvailable();
            field_0x100++;
            command = (tSoundChannelCommand*)(&field_0xec)->vfunction5();
            if (command == 0)
            {
                field_0x108 = -1;
                delayed = mGetFirstDelayedCommand();
                while (delayed != 0 && mDoCommand(delayed) == 1)
                {
                    (&field_0xcc)->mRemoveFromList(delayed);
                    delayed->mMakeAvailable();
                    delayed = mGetFirstDelayedCommand();
                    // MATCH-ONLY: the loop test already covers this; it gives the original's
                    // branch layout at the loop's end.
                    if (delayed == 0)
                        break;
                }
            }
        }
        else
        {
            if (command->vfunction16() != 24)
            {
#line 2658
                SOUND_ERROR(("Big trouble -- command of this type %ld on attachment list", command->vfunction16())); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            }
            (&field_0xec)->mRemoveFromList(command);
            field_0x104--;
            command->mMakeAvailable();
            field_0x100++;
            command = (tSoundChannelCommand*)(&field_0xec)->vfunction5();
            if (command == 0)
            {
                field_0x108 = -1;
                tSoundChannelCommand* delayed = mGetFirstDelayedCommand();
                while (delayed != 0 && mDoCommand(delayed) == 1)
                {
                    (&field_0xcc)->mRemoveFromList(delayed);
                    delayed->mMakeAvailable();
                    delayed = mGetFirstDelayedCommand();
                }
            }
        }
    }
    if (mGetFirstAttachment() == 0)
    {
        field_0x100 = 0;
        field_0x104 = 0;
        field_0x10c = 0;
        field_0x108 = -1;
        field_0x148 = 0;
        field_0x14c = 0;
        field_0x150 = -1;
        field_0x154 = -2;
        field_0x158 = -3;
        field_0x114 = 0;
    }
    return 1;
}

// MATCH-ONLY: the original reports its error on line 2723 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set inside the body.
// HYPOTHESIS: outside active mode the attachment range is reset and field 0x168 takes the
// format's slot 3. A channel in update mode 3 (field 0xfc) goes back to mode 1 unless slot 83
// says otherwise, in which case slot 19 runs instead. Slots 19, 22, 24 and 83 are not recovered.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation.
#line 1986
// FUNCTION: STARFLEETOP 0x0064c970
void tSoundChannel::mClear()
{
#line 2720
    mClearCommands();
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    if (atgSoundPart->vfunction13() == 0)
    {
        mClearAttachments();
        field_0x148 = 0;
        field_0x14c = 0;
        field_0x150 = -1;
        field_0x154 = -2;
        field_0x158 = -3;
        field_0x160 = 0;
        field_0x168 = field_0x10.vfunction3();
    }
    field_0x15c = 0;
    field_0x164 = 0;
    if (field_0xfc == 3)
    {
        if (mDoWeAutoRemoveChannel() == 1 && field_0xfc != 4)
        {
            mDone();
            return;
        }
        field_0xfc = 1;
    }
}

// MATCH-ONLY: the original reports its error on line 2764 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set inside the body.
// HYPOTHESIS: as mClear, with slots 23 and 25 in place of 22 and 24. Slots 23 and 25 are not
// recovered.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation.
#line 2027
// FUNCTION: STARFLEETOP 0x0064cb00
void tSoundChannel::mClearAll()
{
#line 2761
    mClearAllCommands();
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    if (atgSoundPart->vfunction13() == 0)
    {
        mClearAllAttachments();
        field_0x148 = 0;
        field_0x14c = 0;
        field_0x150 = -1;
        field_0x154 = -2;
        field_0x158 = -3;
        field_0x160 = 0;
        field_0x168 = field_0x10.vfunction3();
    }
    field_0x15c = 0;
    field_0x164 = 0;
    if (field_0xfc == 3)
    {
        if (mDoWeAutoRemoveChannel() == 1 && field_0xfc != 4)
        {
            mDone();
            return;
        }
        field_0xfc = 1;
    }
}

// MATCH-ONLY: the original reports its errors on lines 2808, 2865 and 2902 through __LINE__; the
// code that sat before it in the original is not recovered yet, so the line numbers are set
// inside the body.
// HYPOTHESIS: param_1 says whether the channel's data is reset too (slots 22 and 24 run only when
// it is false). Outside active mode the attachment range is restarted at the first handle of the
// attachment list at 0xec (list slot 39 gives its member count); in active mode slot 32 runs
// instead when slot 72 says so. Slots 19, 22, 24, 32, 54, 72, 82, 83 and selection slots 6 and
// 10 are not recovered.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation.
#line 2072
// FUNCTION: STARFLEETOP 0x0064cc90
void tSoundChannel::mReset(iTruth param_1)
{
#line 2806
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    if (!param_1)
        mClearCommands();
    field_0x15c = 0;
    field_0x164 = 0;
    if (atgSoundPart->vfunction13() == 1)
    {
        if (mAreWeStreaming() == 1)
            mDoFillSelection(0);
    }
    else
    {
        field_0x160 = 0;
        field_0x110 = 0;
        if (!mDoWeRetainSelections())
        {
            if (!param_1)
                mClearAttachments();
            field_0x168 = mGetBufferLength();
        }
        if (mDoWeRetainSelections() == 1 || param_1 == 1)
        {
            field_0x10c = field_0x100;
            field_0x108 = (&field_0xec)->vfunction3();
            // HYPOTHESIS: the original calls the list through its vtable, so through a pointer.
            tSoundCommandList* attachments = &field_0xec;
            field_0x114 = 0;
            long handle = (&field_0xec)->vfunction3();
            if (attachments->vfunction39() > 0)
            {
                tSoundChannelCommand* command = (tSoundChannelCommand*)attachments->mGetListMemberWithHandleFromHandle(handle);
                while (command != 0)
                {
                    if (command->vfunction16() == 0)
                    {
                        tSoundSelection* selection = command->vfunction32();
                        if (selection != 0)
                        {
                            selection->vfunction10(0);
                            selection->vfunction6(0);
                            field_0x114 += selection->vfunction27();
                        }
                    }
                    else if (command->vfunction16() != 24)
                    {
#line 2865
                        SOUND_ERROR(("Big trouble -- command of this type %ld on attachment list", command->vfunction16())); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
                    }
                    field_0x168 = mGetBufferLength();
                    attachments = &field_0xec;
                    handle = attachments->vfunction9(handle);
                    command = (tSoundChannelCommand*)attachments->mGetListMemberWithHandleFromHandle(handle);
                }
            }
            else
            {
                tSoundChannelCommand* command = (tSoundChannelCommand*)attachments->mGetListMemberWithHandleFromHandle(handle);
                if (command != 0)
                {
                    if (param_1 == 1)
                        field_0x168 = mGetBufferLength();
                    if (command->vfunction16() == 0)
                    {
                        tSoundSelection* selection = command->vfunction32();
                        if (selection != 0)
                        {
                            selection->vfunction10(0);
                            if (param_1 == 1)
                                selection->vfunction6(0);
                            field_0x114 += selection->vfunction27();
                        }
                    }
                    else if (command->vfunction16() != 24)
                    {
#line 2902
                        SOUND_ERROR(("Big trouble -- command of this type %ld on attachment list", command->vfunction16())); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
                    }
                }
            }
            if (field_0x148 == 1)
            {
                for (long i = 0; i < field_0x14c - field_0x100; i++)
                    field_0x108 = (&field_0xec)->vfunction9(field_0x108);
                field_0x10c = field_0x14c;
            }
        }
    }
    if (field_0xfc == 2)
    {
        if (mDoWeAutoRemoveChannel() == 1 && field_0xfc != 4)
        {
            mDone();
            return;
        }
        field_0xfc = 1;
    }
}

// MATCH-ONLY: the original reports its error on line 2957 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set inside the body.
// HYPOTHESIS: tests that the current attachment handle (field 0x108) is still on the attachment
// list at 0xec; if it is not, the handle is dropped.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation.
#line 2186
// FUNCTION: STARFLEETOP 0x0064d200
iTruth tSoundChannel::mTestInList()
{
#line 2950
    if (field_0x108 != -1)
    {
        for (long handle = (&field_0xec)->vfunction3(); handle != -1; handle = (&field_0xec)->vfunction9(handle))
        {
            if (handle == field_0x108)
                return 1;
        }
        SOUND_ERROR(("f'ed up  here!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        field_0x108 = -1;
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0064d350
iTruth tSoundChannel::mSetChannelPriority(long priority)
{
    field_0x88 = priority;
    return 1;
}

// MATCH-ONLY: the original reports its lines as 2985 and 2994-2996 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line numbers are set inside the body.
// HYPOTHESIS: the channel dumps its format (slot 26), the base dump at the exe's
// tListMemberWithHandle::mDump, its attributes (slot 29) and its four command lists (slot 42).
// The declaration above does not derive tSoundChannel from tListMemberWithHandle, so the base
// dump is reached through a cast.
// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, so the opening line is reset to follow the annotation.
#line 2218
// FUNCTION: STARFLEETOP 0x0064d360
void tSoundChannel::mDump()
{
#line 2985
    SOUND_ERROR(("tSoundChannel: Channel %ld Status =%ld, UpdateMode=%ld, ", field_0c, field_0x28, field_0xfc)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    (&field_0x10)->mDump();
    ((tListMemberWithHandle*)this)->tListMemberWithHandle::mDump();
    (&field_0x30)->mDump();
    (&field_0xcc)->vfunction42();
    (&field_0xdc)->vfunction42();
    (&field_0xbc)->vfunction42();
    (&field_0xec)->vfunction42();
#line 2994
    SOUND_ERROR(("   Priority %ld BytesPerSec =%ld, BytesPerSample=%ld,fNumberAttaching=%ld,fAmountPlayed=%ld ", field_0x88, field_0x98, field_0x94, field_0x10c, field_0x110)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    SOUND_ERROR(("   BaseAttachNum %ld TotalAttach =%ld, NextAttachmentHandle=%ld, ", field_0x100, field_0x104, field_0x108)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    SOUND_ERROR(("   TotalBytesLeftToPlay %ld UserData =%ld, finsihSoundCallback=%ld, previousTime=%f END Channel DUMP \n", field_0x114, field_0x8c, field_0x90, field_0x130)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
}

#line 2237
// FUNCTION: STARFLEETOP 0x0064d820
// HYPOTHESIS: the exe tests the command against -1, the value the constructor stores, and reports
// "invalid" when it differs; the sense is as the bytes say, though the name suggests the opposite.
iTruth tSoundChannelCommand::mIsInvalid()
{
    if (fSoundChannelCommand != (eSoundChannelCommand)-1)
        return kTrue;
    return kFalse;
}

// FUNCTION: STARFLEETOP 0x0064d830
void tSoundChannelCommand::mMakeInvalid()
{
    fSoundChannelCommand = (eSoundChannelCommand)-1;
}

// FUNCTION: STARFLEETOP 0x0064d840
void tSoundChannelCommand::mMakeAvailable()
{
    fAvailable.mRelease();
}

// FUNCTION: STARFLEETOP 0x0064d850
iTruth tSoundChannelCommand::mMakeUnavailable()
{
    return fAvailable.mAcquire();
}

// FUNCTION: STARFLEETOP 0x0064d860
// HYPOTHESIS: the volume setter; mDump reports field_0x10 as "Volume"
void tChannelAttributes::vfunction7(long volume)
{
    field_0x10 = volume;
}

// FUNCTION: STARFLEETOP 0x0064d870
// HYPOTHESIS: the getter for the sound-type flags at +0x14
long tSoundChannelFormat::vfunction12()
{
    return fSoundType;
}

// FUNCTION: STARFLEETOP 0x0064d880
long tSoundChannelCommand::mGetUserData()
{
    return field_0x98;
}

// MATCH-ONLY: the original reports its error on line 3008 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line number is set inside the body.
// HYPOTHESIS: time is in milliseconds (tgSoundPart slot 125). The function at 0x9c is called with
// 0xa0 and the channel number every 0xb0 milliseconds since 0xa8; outside active mode the
// get-next-attachment function at 0x138 is run when slot 61 falls below 0x140. A running fade
// (0x118) steps the volume (slot 102) toward fFadeVolume until 0x120 and then ends through slot
// 104 or 103. The immediate commands (list 0xdc, slot 28) are then run through slot 89.
#line 2293
// FUNCTION: STARFLEETOP 0x0064d890
iTruth tSoundChannel::mUpdateRegular(double time)
{
#line 3006
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 0;
    }
    if (field_0x9c != 0 && time - field_0xa8 > field_0xb0)
    {
        atgSoundPart->vfunction121();
        field_0x9c(field_0xa0, field_0c);
        atgSoundPart->vfunction122();
        field_0xa8 = time;
    }
    iTruth notActive = atgSoundPart->vfunction13() == 0;
    vfunction33();
    if (notActive)
    {
        mLockInData();
        if (mAreWeStreaming() == 1 && field_0x138 != 0 && mGetMillisecondsLeftToPlay() < field_0x140 && !field_0x16c)
        {
            atgSoundPart->vfunction121();
            field_0x16c = 1;
            field_0x138(field_0x13c, field_0c);
            atgSoundPart->vfunction122();
        }
    }
    if (field_0x118 != 0)
    {
        if (time >= field_0x120)
        {
            mSetVolume(fFadeVolume);
            if (field_0x118 == 2)
                mStopAndResetChannel();
            else if (field_0x118 == 3)
                mStopAndClearChannel();
            field_0x118 = 0;
        }
        else
        {
            long volume = mGetVolume();
            double remaining = field_0x120 - time;
            if (remaining < 0.0)
                remaining = 0.0;
            long newVolume = (long)(remaining * field_0x128) + fFadeVolume;
            long change = newVolume - volume;
            if (change < 0)
                change = -change;
            if (newVolume == fFadeVolume || change > 0)
                mSetVolume(newVolume);
            if (newVolume == fFadeVolume)
            {
                if (field_0x118 == 2)
                    mStopAndResetChannel();
                else if (field_0x118 == 3)
                    mStopAndClearChannel();
                field_0x118 = 0;
            }
        }
    }
    if (field_0xfc == 3)
        mClear();
    else if (field_0xfc == 2)
        mReset(0);
    if (field_0xfc == 4)
    {
        mClear();
        return 1;
    }
    tSoundChannelCommand* command = mGetFirstImmediateCommand();
    while (command != 0 && mDoCommand(command) == 1)
    {
        (&field_0xdc)->mRemoveFromList(command);
        command->mMakeAvailable();
        command = mGetFirstImmediateCommand();
    }
    mUpdateBuffersAndSelections(time);
    if (notActive)
        mCleanupSelections();
    return mAreWeStillActive();
}

// FUNCTION: STARFLEETOP 0x0064dc40
void tSoundChannelCommand::mSetPan(long pan)
{
    field_0x40 = pan;
}

// FUNCTION: STARFLEETOP 0x0064dc50
void tSoundChannelCommand::mSetChannelAttributes(tChannelAttributes* attributes)
{
    // HYPOTHESIS: the command keeps a 0x58-byte copy of the attributes from field_0x40 on
    memmove(&field_0x40, attributes, 0x58);
}

// MATCH-ONLY: the harness pairs this annotation with the function whose opening line comes next
// after it, and the #line in mUpdateRegular shifted the listing, so the line is reset here.
#line 2393
// FUNCTION: STARFLEETOP 0x0064dc70
long tSoundChannelCommand::mGetPan()
{
    return field_0x40;
}

// FUNCTION: STARFLEETOP 0x0064dc80
tPoint3* tSoundChannelCommand::mGet3DSoundPosition()
{
    // HYPOTHESIS: the command's parameter block at field_0x40 holds the position
    return (tPoint3*)&field_0x40;
}

// FUNCTION: STARFLEETOP 0x0064dc90
void tSoundChannelCommand::mSet3DSoundProfile(t3DSoundProfile* profile)
{
    // HYPOTHESIS: the command keeps a 0x40-byte copy of the profile from field_0x40 on
    memmove(&field_0x40, profile, 0x40);
}

// FUNCTION: STARFLEETOP 0x0064dcb0
void tSoundChannelCommand::mSet3DSoundPosition(tPoint3* position)
{
    // HYPOTHESIS: the position shares field_0x40 with the profile, as mGet3DSoundPosition reads it
    memmove(&field_0x40, position, sizeof(tPoint3));
}

// FUNCTION: STARFLEETOP 0x0064dcd0
float tSoundChannelCommand::mGet3DSoundConeAngleForNoAttenuation()
{
    // HYPOTHESIS: the angle is a float in the command's parameter block at field_0x40
    return *(float*)&field_0x40;
}

// FUNCTION: STARFLEETOP 0x0064dce0
void tSoundChannelCommand::mSetUserData(long userData)
{
    field_0x98 = userData;
}

// FUNCTION: STARFLEETOP 0x0064dcf0
double tSoundChannelCommand::mGetTimeToTake()
{
    // HYPOTHESIS: the time is a double in the command's parameter block at field_0x40
    return *(double*)&field_0x40;
}

// FUNCTION: STARFLEETOP 0x0064dd00
void tSoundChannelCommand::mSetTimeToTake(double timeToTake)
{
    // HYPOTHESIS: the time is a double in the command's parameter block at field_0x40
    *(double*)&field_0x40 = timeToTake;
}

// MATCH-ONLY: tclock.h declares tClockPart; the lines are kept so line numbers stay.
#include "Tiger/tclock.h"











// FUNCTION: STARFLEETOP 0x0064dd20
iTruth tSoundChannel::mUpdateAtInterrupt(double time)
{
// MATCH-ONLY: the original reports its errors on lines 3164 and 3195 through __LINE__; the code
// that sat before it in the original is not recovered yet, so the line numbers are set here.
#line 3162
    if (atgSoundPart == 0 || aClockPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return kFalse;
    }
    if (mAreWeStreaming() == kTrue)
    {
        iTruth notActive = atgSoundPart->vfunction13() == 0;
        if (notActive)
            mLockInData();
        mUpdateBuffersAndSelections(time);
        if (atgSoundPart->fSomeoneIsWaitingForControl == kTrue)
            return kTrue;
        double elapsed = aClockPart->mGetMicroseconds() - time;
        if (elapsed > 10000.0)
        {
#line 3194
            AddSoundErrorDuringInterrupt("Giving up at 1 %f %f", elapsed, elapsed); // HYPOTHESIS: the exe passes the same value for both
            AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__);
            return kTrue;
        }
        tSoundChannelCommand* command = mGetFirstImmediateCommand();
        while (command != 0)
        {
            if (mDoCommand(command) == kTrue)
            {
                (&field_0xdc)->mRemoveFromList(command);
                command->mMakeAvailable();
                command = mGetFirstImmediateCommand();
            }
            else
                command = 0;
            if (atgSoundPart->fSomeoneIsWaitingForControl == kTrue)
                return kTrue;
        }
        mUpdateAmountPlayed();
        if (atgSoundPart->fSomeoneIsWaitingForControl == kTrue)
            return kTrue;
        if (notActive && field_0x138 != 0 && mGetMillisecondsLeftToPlay() < field_0x140 && !field_0x16c)
        {
            atgSoundPart->vfunction121();
            field_0x16c = 1;
            field_0x138(field_0x13c, field_0c);
            atgSoundPart->vfunction122();
        }
    }
    else
        mAreWeARecordingChannel();
    return mAreWeStillActive();
}
#line 2517 // MATCH-ONLY: the #line directive in the function above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x0064e0e0
iTruth tSoundChannel::mAreWeStillActive()
{
    // HYPOTHESIS: list slot 38 says whether the list is empty; a command left on the attachment,
    // immediate or delayed list keeps the channel active, as do states 8 and 32 (field_0x28) and
    // field_0xfc being other than 1.
    if (!(&field_0xdc)->vfunction38() || !(&field_0xbc)->vfunction38() || !(&field_0xcc)->vfunction38())
        return kTrue;
    if (field_0x28 == 8 || field_0x28 == 32)
        return kTrue;
    if (field_0xfc == 1)
        return kFalse;
    return kTrue;
}

// FUNCTION: STARFLEETOP 0x0064e150
iTruth tSoundChannel::mDoCommand(tSoundChannelCommand* command)
{
// MATCH-ONLY: the original reports its errors on lines 3335-3503 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line numbers are set here.
// HYPOTHESIS: command types and the channel slots they go to are read from the exe's jump table;
// the slots 98-119 are taken to be the channel's own setters for each command's value.
#line 3333
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return kFalse;
    }
#line 3338
    if (command == 0)
    {
        SOUND_ERROR(("Null command")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return kFalse;
    }
    switch (command->vfunction16())
    {
    case 7:
        if (field_0x118 != 0)
        {
            double now = atgSoundPart->vfunction125();
            if (field_0x120 <= now)
                field_0x128 = 0.0;
            else
                field_0x128 = (double)(command->vfunction22() - fFadeVolume) / (field_0x120 - now);
            mSetVolume((long)((field_0x120 - now) * field_0x128) + fFadeVolume);
            return kTrue;
        }
        mSetVolume(command->vfunction22());
        return kTrue;
    case 6:
        mSetPan(command->mGetPan());
        return kTrue;
    case 8:
        mSet3DSoundProfile(command->mGet3DSoundProfile());
        return kTrue;
    case 1:
        return mPlayChannel();
    case 2:
        mPauseChannel();
        return kTrue;
    case 3:
        mStopAndResetChannel();
        return kTrue;
    case 4:
        mStopAndClearChannel();
        return kTrue;
    case 5:
        field_0x98 = command->vfunction18() * field_0x94;
        mSetFrequency(command->vfunction18());
        return kTrue;
    case 14:
        mSet3DSoundPosition(command->mGet3DSoundPosition());
        return kTrue;
    case 15:
        mSet3DSoundOrientation(command->mGet3DSoundOrientation());
        return kTrue;
    case 16:
        mSet3DSoundCones(command->mGet3DSoundConeAngleForNoAttenuation(), command->mGet3DSoundConeAttenuationLevel());
        return kTrue;
    case 17:
        mSet3DSoundVelocity(command->mGet3DSoundVelocity());
        return kTrue;
    case 18:
        mSet3DSoundReferenceDistance(command->mGet3DSoundReferenceDistance());
        return kTrue;
    case 12:
    {
        // HYPOTHESIS: slot 28 returns the finish-sound callback mSetFinishSoundCallback stored
        void (*callback)(unsigned long, iTruth) = (void (*)(unsigned long, iTruth))command->vfunction28();
        long userData = command->mGetUserData();
        if (callback != 0)
        {
            callback(userData, kTrue);
            return kTrue;
        }
#line 3413
        SOUND_ERROR(("trouble with finish call!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return kTrue;
    }
    case 13:
    {
        void (*callback)(unsigned long, iTruth) = (void (*)(unsigned long, iTruth))command->vfunction28();
        long userData = command->mGetUserData();
        if (callback != 0)
        {
            callback(userData, kFalse);
            if (atgSoundPart->vfunction13() == 0 && !mDoWeRetainSelections() && mAreWeStatic() == kTrue)
            {
                field_0x168 = mGetBufferLength();
                tSoundChannelCommand* attachment = (tSoundChannelCommand*)(&field_0xec)->vfunction5();
                if (attachment->vfunction16() == 0)
                {
                    tSoundSelection* selection = attachment->vfunction32();
                    if (selection != 0)
                        selection->vfunction10(selection->vfunction9());
                }
                else if (attachment->vfunction16() != 24)
                {
#line 3451
                    SOUND_ERROR(("Big trouble -- command of this type %ld on attachment list", attachment->vfunction16())); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
                }
                mCleanupSelections();
                return kTrue;
            }
            return kTrue;
        }
#line 3427
        SOUND_ERROR(("trouble with finish call!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return kTrue;
    }
    case 20:
        mSetUpdateSoundPositionFunction(command->vfunction50());
        mSetUserPositionContext(command->mGetUserPositionContext());
        return kTrue;
    case 21:
        mSetMillisecondsBetweenUpdates(command->mGetMillisecondsBetweenUpdates());
        return kTrue;
    case 23:
        mFadeToVolume(command->mGetFinalVolume(), command->mGetTimeToTake());
        return kTrue;
    case 22:
    {
        double time = command->mGetDelayImmediateCommandsUntilThisTime();
        if (atgSoundPart->vfunction125() < time)
            return kFalse;
        return kTrue;
    }
    case 19:
        return mSetChannelPriority(command->vfunction24());
    case 11:
        mSetUserData(command->mGetUserData());
        mSetFinishSoundCallback(command->vfunction28());
        return kTrue;
    default:
#line 3503
        SOUND_ERROR(("Unrecognized command")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        break;
    }
    return kTrue;
}

// FUNCTION: STARFLEETOP 0x0064eb70
iTruth tSoundChannel::mRemoveSoundChannel()
{
    // HYPOTHESIS: slot 19 does the removal; its name is not recovered.
    mDone();
    return kTrue;
}

// FUNCTION: STARFLEETOP 0x0064eb80
void tSoundChannel::mAddFinishCommand(iTruth param_1)
{
    if (atgSoundPart == 0)
    {
#line 3529
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    if (field_0x90 != 0)
    {
        if (atgSoundPart->mAreWeInInterruptMode() == kTrue)
        {
            // HYPOTHESIS: in interrupt mode the finish call is queued as a command of type 12
            // (finished) or 13 (not finished), which mDoCommand runs later.
            long command;
            if (param_1 == kTrue)
                command = atgSoundPart->mCreateCommand((eSoundChannelCommand)12, (void*)field_0x90, &field_0x8c);
            else
                command = atgSoundPart->mCreateCommand((eSoundChannelCommand)13, (void*)field_0x90, &field_0x8c);
            if (command == -1) // the exe queues the command only when creating it failed (byte-exact)
                mAddImmediateNonInterruptCommandCommand(command);
        }
        else
        {
            atgSoundPart->vfunction121();
            field_0x90(field_0x8c, param_1);
            atgSoundPart->vfunction122();
        }
    }
}

// FUNCTION: STARFLEETOP 0x0064ed00
iTruth tSoundChannel::mPlaySpecialLoop(long param_1, long param_2, long param_3, long param_4)
{
// MATCH-ONLY: the original reports its errors on lines 3570-3581 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line numbers are set here.
// HYPOTHESIS: slots 82 and 72 are the streaming and retained-selection tests, slot 52 returning 4
// the stopped state, as the error text lists them. The loop points are kept as offsets from
// field_0x100, and the attachment list's slot 3 gives the handle the loop starts from.
#line 3568
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return kFalse;
    }
#line 3573
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("mmPlaySpecialLoop not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return kTrue;
    }
    if (mDoWeRetainSelections() && mAreWeStreaming() && vfunction52() == 4)
    {
        field_0x148 = kTrue;
        field_0x14c = field_0x100 + param_1;
        field_0x150 = field_0x100 + param_2;
        field_0x154 = field_0x100 + param_3;
        field_0x158 = field_0x100 + param_4;
        field_0x108 = (&field_0xec)->vfunction3();
        mReset(0);
        return kFalse;
    }
#line 3581
    SOUND_ERROR(("Only streaming stopped retained selection channels can special loop!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    return kFalse;
}

// FUNCTION: STARFLEETOP 0x0064f040
iTruth tSoundChannel::mEndSpecialLoop()
{
// MATCH-ONLY: the original reports its errors on lines 3604 and 3609 through __LINE__; the code
// that sat before it in the original is not recovered yet, so the line number is set here.
// HYPOTHESIS: field_0x148 is the special-loop flag mPlaySpecialLoop sets; the original stores
// kTrue back into it when the loop is ending.
#line 3602
    if (atgSoundPart == 0)
    {
        SOUND_ERROR(("No sound part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return kFalse;
    }
    if (atgSoundPart->vfunction13() == 1)
    {
        SOUND_ERROR(("mEndSpecialLoop not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return kTrue;
    }
    if (field_0x148 == kTrue)
    {
        field_0x148 = kTrue;
        return kTrue;
    }
    return kFalse;
}

// HYPOTHESIS: a count of channels deleted by tgSoundPart::mUpdate; its name is not recovered.
static long DAT_00c66960;
// MATCH-ONLY: the #line in mEndSpecialLoop shifted the listing, so the line is reset here.
#line 2784
// FUNCTION: STARFLEETOP 0x0064f250
void tgSoundPart::mUpdate()
{
// MATCH-ONLY: the original reports its errors on lines 3635-3670 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line numbers are set here.
// HYPOTHESIS: slots 114 and 115 take and release the sound part's lock (114 with the caller's file
// and line); 126 runs before the channels are visited and 125 returns the time stored at 0xc0.
// Each channel the list hands back is updated with the current time, removed from the list when
// that says it is done, and deleted once stopped (state 4) unless a callback is running.
    if (field_0x128 == kTrue)
        return;
#line 3635
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    long count = mNumberOfErrors;
    for (long i = 0; i < count; i++)
        gSoundSay.mJustSayIt("AsynchERROR: %s", gSoundFmtStr[i]);
    mNumberOfErrors = 0;
    vfunction126();
    if (field_0x28 != 0 && field_0x30 + field_0x38 * 1000.0 < field_0xb8)
    {
        field_0x30 = field_0xb8;
        field_0xc8++;
        field_0x28(field_0x2c);
        field_0xc8--;
    }
    long numberOfChannels = field_0x40.vfunction39();
    mReleaseControl();
    for (long channelIndex = 0; channelIndex < numberOfChannels; channelIndex++)
    {
#line 3656
        xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
        try
        {
            long handle = field_0x40.vfunction3();
            field_0x40.vfunction32(handle);
            if (handle != -1)
            {
                // HYPOTHESIS: the lookup returns a tSoundChannel as its list member; it is deleted
                // through the member's virtual destructor.
                tListMemberWithHandle* member = (tListMemberWithHandle*)mGetSoundChannelPtrFromHandle(handle);
                tSoundChannel* channel = (tSoundChannel*)member;
                if (channel == 0)
                {
#line 3667
                    SOUND_ERROR(("Trouble %ld-- shouldn't be active,%ld", handle, field_0x40.vfunction39())); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
#line 3670
                    SOUND_ERROR((" REMOVE -- %ld", field_0x40.vfunction39())); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
                }
                else if (channel->mGetUpdateMode() == 4)
                {
                    if (field_0xc8 == 0)
                    {
                        DAT_00c66960++;
                        delete member;
                        field_0x18[handle] = 0;
                        field_0x20--;
                    }
                }
                else if (channel->mUpdateRegular(field_0xb8) == kTrue)
                    field_0x40.vfunction26(handle);
            }
        }
        catch (...)
        {
        }
        mReleaseControl();
    }
    field_0xc0 = vfunction125();
    mReleaseControl();
}

// MATCH-ONLY: the #line directives in mUpdate shifted the listing, so the line is reset here.
#line 2857
// 0x0064f629 is the tail of tgSoundPart::mUpdate that VC6 placed after its catch (...) funclet.







// HYPOTHESIS: ReadAtInterrupt reads the asynchronous sound queue (its unit is not recovered).
void ReadAtInterrupt();

// FUNCTION: STARFLEETOP 0x0064f670
void tgSoundPart::mUpdateAtInterrupt()
{
// MATCH-ONLY: the original reports its errors on lines 3738-3802 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line numbers are set here.
// HYPOTHESIS: the lock word of the lock at 0xf0 (0x110) is the active-mode flag, field_0x128 says the interrupt is running and
// field_0xc0 holds the time of the last interrupt update; each active channel is updated with the
// time and removed from the active list when its update says it is done.
    if (field_0xf0.fLock == 1)
        return;
#line 3736
    if (aClockPart == 0)
    {
        SOUND_ERROR(("No clock part!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return;
    }
    double now = aClockPart->mGetMicroseconds();
    double lastTime = now;
    if (now - field_0xc0 < 100000.0)
        return;
    field_0xc0 = now;
#line 3752
    if (xAttemptToGainControl(__FILE__, __LINE__, 1, 1) == 0)
        return;
    field_0x128 = kTrue;
    if (field_0xf0.fLock == 1)
    {
        field_0x128 = kFalse;
        mReleaseControl();
        return;
    }
    // HYPOTHESIS: the original calls the list through its vtable, so through a pointer.
    tSoundChannelList* activeChannels = &field_0x40;
    long handle = activeChannels->vfunction3();
    vfunction126();
    while (handle != -1)
    {
        tSoundChannel* channel = (tSoundChannel*)mGetSoundChannelPtrFromHandle(handle);
        long nextHandle = activeChannels->vfunction9(handle);
        if (channel != 0 && channel->mUpdateAtInterrupt(now) == kFalse)
            activeChannels->vfunction32(handle);
        handle = nextHandle;
        if (fSomeoneIsWaitingForControl == kTrue)
        {
#line 3790
            AddSoundErrorDuringInterrupt("Someone is waiting for control!");
            AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__);
            field_0x128 = kFalse;
            mReleaseControl();
            return;
        }
        double time = aClockPart->mGetMicroseconds();
        if (time - now > 10000.0)
        {
#line 3801
            AddSoundErrorDuringInterrupt("Giving up at 1 %f %f", time - now, time - lastTime);
            AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__);
            field_0x128 = kFalse;
            mReleaseControl();
            return;
        }
        lastTime = time;
    }
    ReadAtInterrupt();
    field_0x128 = kFalse;
    mReleaseControl();
}
#line 2936 // MATCH-ONLY: the #line directives in mUpdateAtInterrupt shifted the listing, so the line is reset here.
// HYPOTHESIS: the sound part registers itself as the global atgSoundPart and clears the sound
// memory block manager's global, which its own part sets later.
extern class tSoundMemoryBlockManager* aSoundMemoryBlockManager;
// FUNCTION: STARFLEETOP 0x0064faa0
tgSoundPart::tgSoundPart()
{
    field_0xc8 = 0;
    field_0x128 = kFalse;
    field_0xac = 0;
    aSoundMemoryBlockManager = 0;
    field_0x124 = 0;
    fSomeoneIsWaitingForControl = kFalse;
    field_0x12a = kTrue;
    atgSoundPart = this;
}

// HYPOTHESIS: tChannelGroupManagerPart is declared only as far as this function needs it. It waits
// for the memory, clock and debug parts: the SDK libraries' data names give the cache pointer
// (0x008e0b54) and type descriptor (0x0083c608) of the first Test_ function (0x0064fc00) to
// tMemoryPart, and the other two are 0x00640580 and 0x00640640.
class tChannelGroupManagerPart : public tPart {
public: virtual iTruth mIsReady();
};
// FUNCTION: STARFLEETOP 0x0064fbe0
iTruth tChannelGroupManagerPart::mIsReady()
{
    if (TestPart(tMemoryPart) == kTrue && TestPart(tClockPart) == kTrue && TestPart(tDebugPart) == kTrue)
        return kTrue;
    return kFalse;
}

// FUNCTION: STARFLEETOP 0x0064fcc0
// HYPOTHESIS: the timer response at 0x114 is set up (its fields at 0x11c and 0x120 are not
// named), slot 126 starts the sound system, and the last interrupt update time is set 150000
// microseconds in the past, more than the 100000 mUpdateAtInterrupt waits, so the first one runs.
void tgSoundPart::mActivate()
{
    field_0x114.field_0x4[1] = 1;
    field_0x114.field_0x4[2] = 4;
    vfunction126();
    field_0xc0 = vfunction125() - 150000.0;
}

// FUNCTION: STARFLEETOP 0x0064fd00
// HYPOTHESIS: mDone (slot 7) stops the sound system, the counterpart of slot 126 in mActivate.
void tgSoundPart::mDeactivate()
{
    mDone();
}

// HYPOTHESIS: the object at 0xc is an array of a class with a virtual destructor (it is deleted
// with delete[]); its class is not recovered yet. tgSound.h declares the memory block manager.
class fn_0064fd10_Array { public: virtual ~fn_0064fd10_Array(); };
// tSoundMemoryBlockList's destructor is implicit (Tiger/tSndMemMgr.h), so its scalar deleting
// destructor, 0x00650210, has no definition to be claimed on.

// FUNCTION: STARFLEETOP 0x0064fd10
// HYPOTHESIS: the timer response's state at 0x120 is 8 once the part is done; slot 11 stops the
// channel at the index given. The channel timer callback is cleared and its interval reset to
// 500000.
void tgSoundPart::mDone()
{
// MATCH-ONLY: the original reports its lines as 3891-3923 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line numbers are set here.
#line 3891
    while (!xAttemptToGainControl(__FILE__, __LINE__, 0, 0))
        ;
    if (field_0x114.field_0x4[2] != 8)
    {
        field_0x114.field_0x4[2] = 8;
        if (field_0x20 > 0)
        {
            for (long i = 0; i < field_0x1c; i++)
            {
                if (field_0x18[i] != 0)
                {
                    mReleaseControl();
                    mRemoveSoundChannel(i);
#line 3917
                    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
                }
            }
            if (field_0x20 != 0)
            {
#line 3923
                SOUND_ERROR(("trouble")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            }
        }
        field_0x1c = 0;
        field_0x20 = 0;
        if (field_0x18 != 0)
        {
            delete[] field_0x18;
            field_0x18 = 0;
        }
        field_0x10 = 0;
        field_0x14 = 0;
        if (field_0xc != 0)
        {
            delete[] field_0xc;
            field_0xc = 0;
        }
        field_0x28 = 0;
        field_0x2c = 0;
        field_0x30 = vfunction125();
        field_0x38 = 500000.0;
    }
    if (field_0xa8 > 0)
        field_0xa8 = -1;
    field_0x114.field_0x4[2] = 8;
    if (field_0xac != 0)
    {
        delete field_0xac;
        field_0xac = 0;
        aSoundMemoryBlockManager = 0;
    }
    mReleaseControl();
}

// MATCH-ONLY: the #line directives in mDone shifted the listing, so the line is reset here.
#line 3056
// HYPOTHESIS: tInitSoundPartInfo is declared only as far as mInit needs it: slots 7 and 8 give
// the memory block manager's arguments and slot 15 the value kept at 0x124.
class tInitSoundPartInfo { public: virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2(); virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5(); virtual void vfunction6(); virtual unsigned short vfunction7(); virtual long vfunction8(); virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11(); virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14(); virtual long vfunction15(); };
double SoundPartCurrentTime();
// FUNCTION: STARFLEETOP 0x0064ff30
// HYPOTHESIS: the part initializes only from the activated state (4 at 0x120) and leaves it in
// state 1; slots 131 and 133 size its tables.
iTruth tgSoundPart::mInit(tInitSoundPartInfo* info)
{
// MATCH-ONLY: the original reports its lines as 3974 and 3986 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line numbers are set here.
    if (info == 0)
    {
#line 3974
        SOUND_ERROR(("Yo -- bad info!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return kFalse;
    }
    if (field_0x114.field_0x4[2] != 4)
        return kFalse;
#line 3986
    if (xAttemptToGainControl(__FILE__, __LINE__, 0, 0) == kFalse)
        return kFalse;
    field_0x128 = kFalse;
    field_0xac = new tSoundMemoryBlockManager(info->vfunction7(), info->vfunction8());
    field_0xa8 = -1;
    field_0xb0 = SoundPartCurrentTime;
    vfunction126();
    mSetNumberOfSoundChannelCommands(0x1000);
    mSetNumberOfSoundChannels(0x200);
    field_0x14 = 0;
    field_0x20 = 0;
    field_0x124 = info->vfunction15();
    field_0x50 = 0;
    field_0x54 = 100;
    field_0x60 = 1.0f;
    field_0x58 = 1.0;
    field_0x28 = 0;
    field_0x2c = 0;
    field_0x30 = vfunction125();
    field_0x38 = 500000.0;
    // HYPOTHESIS: the original calls the channel list and listener profile through their
    // vtables, so through pointers.
    (&field_0x40)->vfunction2();
    (&field_0x68)->mInit();
    field_0x114.field_0x4[2] = 1;
    field_0x114.field_0x4[1] = 2;
    mReleaseControl();
    return kTrue;
}

// FUNCTION: STARFLEETOP 0x006501c0
// HYPOTHESIS: the low byte of a handle is the block's true handle; the higher bits are not
// recovered (perhaps a reuse count).
long tSoundMemoryBlockManager::mGetTrueHandle(unsigned long handle)
{
    return handle & 0xff;
}

// FUNCTION: STARFLEETOP 0x006501d0
// HYPOTHESIS: the field at 0xc is the size of one block; the result is the size rounded up to
// whole blocks.
long tSoundMemoryBlockManager::mNumberOfBlocksNeeded(long size)
{
    return (size + field_0c - 1) / field_0c;
}

// FUNCTION: STARFLEETOP 0x006501f0
// MATCH-ONLY: the scalar deleting destructor is compiler-generated and the destructor is
// tSndMemMgr.cpp's, so it is written out as a member that destroys, then deletes on bit 0.
tSoundMemoryBlockManager *tSoundMemoryBlockManager::scalar_deleting_destructor(unsigned char flags)
{
    this->tSoundMemoryBlockManager::~tSoundMemoryBlockManager();
    if (flags & 1) ::operator delete(this);
    return this; }

// FUNCTION: STARFLEETOP 0x00650230
// The sound part's time source (kept at 0xb0 by mInit): the clock part's time in microseconds.
double SoundPartCurrentTime()
{
    return aClockPart->mGetMicroseconds();
}

// FUNCTION: STARFLEETOP 0x006502b0
// HYPOTHESIS: slot 132 looks the channel up by handle; when it is found, its entry in the channel
// table (0x18) is cleared and the count (0x20) drops by one.
unsigned char tgSoundPart::mClearOutSoundChannel(long channel)
{
    if (mGetSoundChannelPtrFromHandle(channel) == 0)
        return 0;
    field_0x20--;
    field_0x18[channel] = 0;
    return 1;
}

// FUNCTION: STARFLEETOP 0x006502f0
// HYPOTHESIS: under the part's lock, slot 132 finds the channel by handle and the channel's slot
// 97 removes it; slot 86 then runs, and when no channel is left (0x20) the channel table is freed.
unsigned char tgSoundPart::mRemoveSoundChannel(long channel)
{
// MATCH-ONLY: the original reports its lines as 4058 and 4077 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line numbers are set here.
#line 4058
    if (xAttemptToGainControl(__FILE__, __LINE__, 0, 0) == kFalse)
        return 0;
    tSoundChannel* soundChannel = (tSoundChannel*)mGetSoundChannelPtrFromHandle(channel);
    if (soundChannel == 0)
    {
        mReleaseControl();
        return 0;
    }
    soundChannel->mRemoveSoundChannel();
    mReleaseControl();
    mUpdate();
#line 4077
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    if (field_0x20 == 0)
    {
        field_0x1c = 0;
        delete[] field_0x18;
        field_0x18 = 0;
    }
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x006503a0
// HYPOTHESIS: under the part's lock, every slot of the channel table (0x18, size 0x1c) that holds
// a channel is removed through slot 11 with the lock released around it; the table is then freed.
unsigned char tgSoundPart::mRemoveAllSoundChannels()
{
// MATCH-ONLY: the original reports its lines as 4100 and 4118 through __LINE__; the code that
// sat before it in the original is not recovered yet, so the line numbers are set here.
#line 4100
    if (xAttemptToGainControl(__FILE__, __LINE__, 0, 0) == kFalse)
        return 0;
    for (long i = 0; i < field_0x1c; i++)
    {
        if (mGetSoundChannelPtrFromHandle(i) != 0)
        {
            mReleaseControl();
            mRemoveSoundChannel(i);
#line 4118
            xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
        }
    }
    field_0x20 = 0;
    field_0x1c = 0;
    if (field_0x18 != 0)
    {
        delete[] field_0x18;
        field_0x18 = 0;
    }
    mReleaseControl();
    return 1;
}

// HYPOTHESIS: the array at 0xc (count at 0x10) is the part's pool of channel commands, indexed by
// handle; its declared element class (fn_0064fd10_Array) is not yet unified with tSoundChannelCommand.
// FUNCTION: STARFLEETOP 0x00650440
tSoundChannelCommand* tgSoundPart::mGetSoundChannelCommandPtrFromHandle(long handle)
{
    if (handle < 0 || handle >= field_0x10 || field_0xc == 0)
        return 0;
    return &((tSoundChannelCommand*)field_0xc)[handle];
}

// HYPOTHESIS: slot 131 grows the command pool (0xc, count 0x10) to the size it is given. The
// search starts at the cursor (0x14), steps round the pool once, and claims the first command
// that mMakeUnavailable gives up; the cursor is left past it and its index is the handle. When
// every command is taken the pool is grown by 0x400 and the search runs again through this
// method's own slot (130). In interrupt mode (0x128) the pool is never grown.
// MATCH-ONLY: the original reports its lines as 4172-4203 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line numbers are set inside the body.
// FUNCTION: STARFLEETOP 0x00650470
long tgSoundPart::mFindAvailableSoundChannelCommandPtrAndReturnAHandle()
{
    if (field_0x10 == 0)
    {
        if (field_0x128 == kTrue)
            return -1;
        mSetNumberOfSoundChannelCommands(0x1000);
        if (field_0x10 == 0)
        {
#line 4172
            SOUND_ERROR(("Trouble!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            return -1;
        }
    }
    tSoundChannelCommand* commands;
    if (field_0x14 >= field_0x10)
        field_0x14 = 0;
    long start = field_0x14;
    commands = (tSoundChannelCommand*)field_0xc;
    while ((field_0x14 + (tSoundChannelCommand*)field_0xc)->mMakeUnavailable() == kFalse)
    {
        if (++field_0x14 >= field_0x10)
            field_0x14 = 0;
        if (field_0x14 == start)
        {
            if (field_0x128 == kTrue)
                return -1;
#line 4198
            SOUND_ERROR(("We ran out of available SoundChannel commands")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            long count = field_0x10;
            mSetNumberOfSoundChannelCommands(count + 0x400);
            if (field_0x10 <= count)
            {
#line 4203
                SOUND_ERROR(("Trouble!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
                return -1;
            }
            return mFindAvailableSoundChannelCommandPtrAndReturnAHandle();
        }
        commands = (tSoundChannelCommand*)field_0xc;
    }
    return ++field_0x14 - 1;
}

// HYPOTHESIS: slot 133 grows the channel table (0x18, size 0x1c) to the size it is given; 0x20
// is the number of handles in use. The slot just past them is tried first, then the table is
// searched from the start for a free one. In interrupt mode (0x128) the table is never grown.
// MATCH-ONLY: the original reports its lines as 4232 and 4246 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line numbers are set inside the body.
// FUNCTION: STARFLEETOP 0x00650800
long tgSoundPart::mFindAvailableSoundChannelHandle()
{
    if (field_0x1c == 0)
    {
        if (field_0x128 == kTrue)
            return -1;
        mSetNumberOfSoundChannels(0x200);
        if (field_0x1c == 0)
        {
#line 4232
            SOUND_ERROR(("Trouble!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            return -1;
        }
    }
    if (field_0x20 >= field_0x1c)
    {
        if (field_0x128 == kTrue)
            return -1;
        mSetNumberOfSoundChannels(field_0x1c + 0x80);
        if (field_0x20 >= field_0x1c)
        {
#line 4246
            SOUND_ERROR(("Trouble!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            return -1;
        }
    }
    if (field_0x18[field_0x20] == 0)
        return field_0x20;
    for (long i = 0; i < field_0x20; i++)
    {
        if (field_0x18[i] == 0)
            return i;
    }
    return -1;
}

// HYPOTHESIS: a free slot of the channel table (0x18, size 0x1c) takes the channel, which is told
// its handle through slot 8; 0x20 counts the handles in use.
// MATCH-ONLY: the original reports its line as 4281 through __LINE__; the code that sat before it
// in the original is not recovered yet, so the line number is set inside the body.
// FUNCTION: STARFLEETOP 0x00650a40
unsigned char tgSoundPart::mAddChannelAtHandle(tSoundChannel* channel, long handle)
{
    if (handle >= 0 && handle < field_0x1c && field_0x18[handle] == 0 && channel != 0)
    {
        field_0x18[handle] = channel;
        channel->vfunc_20(handle);
        field_0x20++;
        return 1;
    }
#line 4281
    SOUND_ERROR(("trouble!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    return 0;
}

// HYPOTHESIS: the command pool (0xc, count 0x10) is created at the size given the first time; later
// it only grows, outside interrupt mode: the new commands are told their handles through slot 8,
// the old ones are copied across and the old pool is freed. The result is the pool's size.
// MATCH-ONLY: the original reports its lines as 4315 and 4348 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line numbers are set inside the body.
// FUNCTION: STARFLEETOP 0x00650b70
long tgSoundPart::mSetNumberOfSoundChannelCommands(long count)
{
    long i;
    if (field_0x10 == 0)
    {
        if (field_0x128 == kTrue)
            return 0;
        field_0x10 = count;
        field_0xc = (fn_0064fd10_Array*)new tSoundChannelCommand[count];
        if (field_0xc == 0)
        {
#line 4315
            SOUND_ERROR(("Memory Troubles!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            field_0x10 = 0;
        }
        for (i = 0; i < field_0x10; i++)
            ((tSoundChannelCommand*)field_0xc)[i].vfunc_20(i);
        return field_0x10;
    }
    if (count > field_0x10 && field_0x128 != kTrue)
    {
        long oldCount = field_0x10;
        field_0x10 = count;
        tSoundChannelCommand* commands = new tSoundChannelCommand[count];
        if (commands == 0)
        {
#line 4348
            SOUND_ERROR(("Memory Troubles!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            return -1;
        }
        for (i = oldCount; i < field_0x10; i++)
            commands[i].vfunc_20(i);
        for (i = 0; i < oldCount; i++)
            commands[i] = ((tSoundChannelCommand*)field_0xc)[i];
        fn_0064fd10_Array* oldCommands = field_0xc;
        field_0xc = (fn_0064fd10_Array*)commands;
        delete[] oldCommands;
        return field_0x10;
    }
    return field_0x10;
}

// HYPOTHESIS: the channel table (0x18, size 0x1c) is created at the size given the first time and
// cleared; later it only grows, outside interrupt mode, keeping the channels it held. The result is
// the table's size. In interrupt mode the original returns the command pool's size (0x10).
// MATCH-ONLY: the original reports its lines as 4398 and 4431 through __LINE__; the code that sat
// before it in the original is not recovered yet, so the line numbers are set inside the body.
// FUNCTION: STARFLEETOP 0x00651020
long tgSoundPart::mSetNumberOfSoundChannels(long count)
{
    long i;
    if (field_0x1c == 0)
    {
        if (field_0x128 == kTrue)
            return field_0x10;
        field_0x1c = count;
        field_0x18 = new tSoundChannel*[count];
        if (field_0x18 == 0)
        {
#line 4398
            SOUND_ERROR(("Memory Troubles!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            field_0x1c = 0;
        }
        for (i = 0; i < field_0x1c; i++)
            field_0x18[i] = 0;
        return field_0x1c;
    }
    if (count > field_0x1c)
    {
        if (field_0x128 == kTrue)
            return field_0x10;
        long oldCount = field_0x1c;
        field_0x1c = count;
        tSoundChannel** channels = new tSoundChannel*[count];
        if (channels == 0)
        {
#line 4431
            SOUND_ERROR(("Memory Troubles!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            return -1;
        }
        for (i = oldCount; i < field_0x1c; i++)
            channels[i] = 0;
        for (i = 0; i < oldCount; i++)
            channels[i] = field_0x18[i];
        tSoundChannel** oldChannels = field_0x18;
        field_0x18 = channels;
        delete[] oldChannels;
        return field_0x1c;
    }
    return field_0x1c;
}

// HYPOTHESIS: a handle outside the channel table (0x18, size 0x1c), or no table, gives no channel.
// FUNCTION: STARFLEETOP 0x006512e0
tSoundChannel* tgSoundPart::mGetSoundChannelPtrFromHandle(long handle)
{
    if (handle >= 0 && handle < field_0x1c && field_0x18 != 0)
        return field_0x18[handle];
    return 0;
}

// HYPOTHESIS: the timer response's state (0x120) is 1 while the part runs; every channel in the
// table (0x18, size 0x1c, count 0x20) is played under the part's lock.
// FUNCTION: STARFLEETOP 0x00651300
unsigned char tgSoundPart::mPlayAllChannels()
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4480
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    if (field_0x20 > 0)
    {
        for (long i = 0; i < field_0x1c; i++)
        {
            tSoundChannel* channel = field_0x18[i];
            if (channel != 0)
                channel->mPlayChannel();
        }
    }
    mReleaseControl();
    return 1;
}

// HYPOTHESIS: channel slot 52 returns the channel's state; 8 is paused, so those channels play again.
// FUNCTION: STARFLEETOP 0x00651370
unsigned char tgSoundPart::mResumeAllChannels()
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4504
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    if (field_0x20 > 0)
    {
        for (long i = 0; i < field_0x1c; i++)
        {
            tSoundChannel* channel = field_0x18[i];
            if (channel != 0 && channel->vfunction52() == 8)
                field_0x18[i]->mPlayChannel();
        }
    }
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x006513f0
unsigned char tgSoundPart::mStopAndResetAllChannels()
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4529
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    if (field_0x20 > 0)
    {
        for (long i = 0; i < field_0x1c; i++)
        {
            tSoundChannel* channel = field_0x18[i];
            if (channel != 0)
                channel->mStopAndResetChannel();
        }
    }
    mReleaseControl();
    return 1;
}

#line 3506 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00651460
unsigned char tgSoundPart::mStopAndClearAllChannels()
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4553
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    if (field_0x20 > 0)
    {
        for (long i = 0; i < field_0x1c; i++)
        {
            tSoundChannel* channel = field_0x18[i];
            if (channel != 0)
                channel->mStopAndClearChannel();
        }
    }
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x006514d0
unsigned char tgSoundPart::mPauseAllChannels()
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4582
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    if (field_0x20 > 0)
    {
        for (long i = 0; i < field_0x1c; i++)
        {
            tSoundChannel* channel = field_0x18[i];
            if (channel != 0)
                channel->mPauseChannel();
        }
    }
    mReleaseControl();
    return 1;
}

#line 3548 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// HYPOTHESIS: the two data pointers carry the command's arguments; most commands read one value
// through the first, the paired commands a second value through the second.
// FUNCTION: STARFLEETOP 0x00651540
long tgSoundPart::mCreateCommand(eSoundChannelCommand command, void* data1, void* data2)
{
    long handle = mFindAvailableSoundChannelCommandPtrAndReturnAHandle();
    tSoundChannelCommand* commandPtr = mGetSoundChannelCommandPtrFromHandle(handle);
    if (commandPtr == 0)
    {
#line 4620
        SOUND_ERROR(("Error: Ran out of commands!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return -1;
    }
    commandPtr->mSetSoundChannelCommand(command);
    switch (command)
    {
    case 24:
        commandPtr->mSetSoundSelection(0);
        break;
    case 0:
        commandPtr->mSetSoundSelection((tSoundSelection*)data1);
        break;
    case 5:
        commandPtr->mSetFrequency(*(long*)data1);
        break;
    case 6:
        commandPtr->mSetPan(*(long*)data1);
        break;
    case 7:
        commandPtr->mSetVolume(*(long*)data1);
        break;
    case 11:
    case 12:
    case 13:
        commandPtr->mSetFinishSoundCallback((void (*)(unsigned long, iTruth))data1);
        commandPtr->mSetUserData(*(long*)data2);
        break;
    case 10:
        commandPtr->mSetChannelAttributes((tChannelAttributes*)data1);
        break;
    case 19:
        commandPtr->mSetPriority(*(long*)data1);
        break;
    case 8:
        commandPtr->mSet3DSoundProfile((t3DSoundProfile*)data1);
        break;
    case 14:
        commandPtr->mSet3DSoundPosition((tPoint3*)data1);
        break;
    case 15:
        commandPtr->mSet3DSoundOrientation((tPoint3*)data1);
        break;
    case 17:
        commandPtr->mSet3DSoundVelocity((tPoint3*)data1);
        break;
    case 16:
        commandPtr->mSet3DSoundConeAngleForNoAttenuation(*(float*)data1);
        commandPtr->mSet3DSoundConeAttenuationLevel(*(long*)data2);
        break;
    case 18:
        commandPtr->mSet3DSoundReferenceDistance(*(double*)data1);
        break;
    case 20:
        commandPtr->mSetUpdateSoundPositionFunction((void (*)(unsigned long, long))data1);
        commandPtr->mSetUserPositionContext(*(long*)data2);
        break;
    case 21:
        commandPtr->mSetMillisecondsBetweenUpdates(*(double*)data1);
        break;
    case 22:
        commandPtr->mSetDelayImmediateCommandsUntilThisTime(*(double*)data1);
        break;
    case 23:
        commandPtr->mSetTimeToTake(*(double*)data1);
        commandPtr->mSetFinalVolume(*(long*)data2);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 9:
        break;
    default:
#line 4706
        SOUND_ERROR(("Adding Command That doesn't exist!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return -1;
    }
    return handle;
}

#line 3638 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00651990
unsigned char tgSoundPart::mAttachSoundToChannel(long channel, tSoundSelection* selection)
{
    if (vfunction13() == 1)
    {
#line 4731
        SOUND_ERROR(("mAttachSoundToChannel not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        return 1;
    }
    if (field_0x114.field_0x4[2] != 1)
        return 0;
    if (selection == 0)
        return 0;
#line 4741
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0)
    {
        mReleaseControl();
        return 0;
    }
    long handle = mCreateCommand((eSoundChannelCommand)0, selection, 0);
    if (handle == -1)
    {
        mReleaseControl();
        return 0;
    }
    channelPtr->mAddAttachment(handle);
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x00651b40
unsigned char tgSoundPart::mPlayChannel(long channel)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4771
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0)
    {
        mReleaseControl();
        return 0;
    }
    long handle = mCreateCommand((eSoundChannelCommand)1, 0, 0);
    if (handle == -1)
    {
        mReleaseControl();
        return 0;
    }
    channelPtr->mAddImmediateCommand(handle);
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x00651bf0
unsigned char tgSoundPart::mPauseChannel(long channel)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4801
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0)
    {
        mReleaseControl();
        return 0;
    }
    long handle = mCreateCommand((eSoundChannelCommand)2, 0, 0);
    if (handle == -1)
    {
        mReleaseControl();
        return 0;
    }
    channelPtr->mAddImmediateCommand(handle);
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x00651ca0
unsigned char tgSoundPart::mStopAndResetChannel(long channel)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4832
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0)
    {
        mReleaseControl();
        return 0;
    }
    long handle = mCreateCommand((eSoundChannelCommand)3, 0, 0);
    if (handle == -1)
    {
        mReleaseControl();
        return 0;
    }
    channelPtr->mAddImmediateCommand(handle);
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x00651d50
unsigned char tgSoundPart::mStopAndClearChannel(long channel)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4863
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0)
    {
        mReleaseControl();
        return 0;
    }
    long handle = mCreateCommand((eSoundChannelCommand)4, 0, 0);
    if (handle == -1)
    {
        mReleaseControl();
        return 0;
    }
    channelPtr->mAddImmediateCommand(handle);
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x00651e00
unsigned char tgSoundPart::mSetFrequency(long channel, long frequency, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4893
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mAreWeFixedFrequency() == 1)
    {
#line 4901
        SOUND_ERROR(("you are trying to change the frequency of a fixed Frequency Channel!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        mReleaseControl();
        return 0;
    }
    long handle = mCreateCommand((eSoundChannelCommand)5, &frequency, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1)
        channelPtr->mAddImmediateCommand(handle);
    else
    {
        if (vfunction13() == 1)
        {
#line 4918
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            return 1;
        }
        channelPtr->mAddDelayedCommand(handle);
    }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x006520e0
unsigned char tgSoundPart::mSetPan(long channel, long pan, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4942
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mAreWeFixedPan() == 1)
    {
#line 4950
        SOUND_ERROR(("you are trying to change the pan of a fixed pan Channel!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
        // The original returns here without releasing control (mSetFrequency releases it).
        return 0;
    }
    long handle = mCreateCommand((eSoundChannelCommand)6, &pan, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1)
        channelPtr->mAddImmediateCommand(handle);
    else
    {
        if (vfunction13() == 1)
        {
#line 4967
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            return 1;
        }
        channelPtr->mAddDelayedCommand(handle);
    }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x006523b0
unsigned char tgSoundPart::mAddDelayCommand(long channel, double delay, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 4990
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)22, &delay, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1)
        channelPtr->mAddImmediateCommand(handle);
    else
    {
        if (vfunction13() == 1)
        {
#line 5009
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            return 1;
        }
        channelPtr->mAddDelayedCommand(handle);
    }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x00652580
unsigned char tgSoundPart::mAddUpdateSoundPositionFunction(long channel, void (*function)(unsigned long, long), long data, double interval, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 5033
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)20, &function, (void*)data);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1)
        channelPtr->mAddImmediateCommand(handle);
    else
    {
        if (vfunction13() == 1)
        {
#line 5052
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            return 1;
        }
        channelPtr->mAddDelayedCommand(handle);
    }
    handle = mCreateCommand((eSoundChannelCommand)21, &interval, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1)
        channelPtr->mAddImmediateCommand(handle);
    else
    {
        if (vfunction13() == 1)
        {
#line 5071
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
            return 1;
        }
        channelPtr->mAddDelayedCommand(handle);
    }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x00652890
unsigned char tgSoundPart::mAddUpdateListenerPositionFunction(void (*function)(unsigned long), long data, double interval)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 5095
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    field_0x28 = function;
    field_0x2c = data;
    field_0x38 = interval * 1000.0;
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x006528f0
unsigned char tgSoundPart::mFadeToStopAndResetNow(long channel, double timeToTake)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 5118
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0)
    {
        mReleaseControl();
        return 0;
    }
    channelPtr->mFadeToStopAndResetNow(timeToTake);
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x00652990
unsigned char tgSoundPart::mFadeToStopAndClearNow(long channel, double timeToTake)
{
    if (field_0x114.field_0x4[2] != 1)
        return 0;
#line 5143
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0)
    {
        mReleaseControl();
        return 0;
    }
    channelPtr->mFadeToStopAndClearNow(timeToTake);
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl();
    return 1;
}

// FUNCTION: STARFLEETOP 0x00652a30
unsigned char tgSoundPart::mFadeToVolume(long channel, long volume, double timeToTake, unsigned char immediate)
{   // MATCH-ONLY: compact (as the functions from 0x00651e00 on) so the unit stays under 4021 lines; past that, the #line numbers of the functions from 0x006501c0 on take this claim (claims pair by physical line).
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5168
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mAreWeFixedVolume() == 1) {
#line 5176
        SOUND_ERROR(("you are trying to change the volume of a fixed Volume Channel!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 0; } // no mReleaseControl here, as in mSetPan
    long handle = mCreateCommand((eSoundChannelCommand)23, &timeToTake, &volume);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5192
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}

// FUNCTION: STARFLEETOP 0x00652d00
unsigned char tgSoundPart::mSetVolume(long channel, long volume, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5216
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mAreWeFixedVolume() == 1) {
#line 5224
        SOUND_ERROR(("you are trying to change the volume of a fixed Volume Channel!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 0; } // no mReleaseControl here, as in mSetPan
    long handle = mCreateCommand((eSoundChannelCommand)7, &volume, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5240
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}
#line 4009 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00652fd0
unsigned char tgSoundPart::mSetPriority(long channel, long priority, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5264
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)19, &priority, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5283
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}
#line 4028 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x006531a0
unsigned char tgSoundPart::mSetFinishFunction(long channel, void (* const function)(unsigned long, unsigned char), long data, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5307
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)11, function, &data);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5326
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}
#line 4048 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00653370
unsigned char tgSoundPart::mSetChannelAttributes(long channel, tChannelAttributes* attributes, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
    if (attributes == 0) return 0;
#line 5352
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)10, attributes, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5371
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}
#line 4070 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00653550
unsigned char tgSoundPart::mSet3DSoundProfile(long channel, t3DSoundProfile* profile, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
    if (profile == 0) return 0;
#line 5398
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mAreWe3D() == 0) {
#line 5406
        SOUND_ERROR(("We are not a 3D channel!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)8, profile, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5423
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}

// HYPOTHESIS: the file and line of the caller that holds control (0x00c64798, 0x00c6695c).
static char g_00c64798[256];
static long g_00c6695c;

#line 4098 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00653830
iTruth tgSoundPart::xAttemptToGainControl(char* file, long line, unsigned char a, long b)
{
    if (a == kTrue)
    {
        if (field_0x128 == kTrue) return kFalse;
        if (field_0xc8 > 0) return kFalse;
        iTruth result = field_0xcc.mAcquire();
        if (result == kTrue)
        {
            strcpy(g_00c64798, file);
            g_00c6695c = line;
        }
        return result;
    }
    if (field_0xc8 > 0 && b == 1)
    {
        if (field_0xcc.fLock != 1)
        {
#line 5488
            SOUND_ERROR(("callback mode without control -- ouch!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return kTrue;
        }
        return kTrue;
    }
    long count = 0;
    while (field_0xcc.mAcquire() == kFalse)
    {
        count++;
        fSomeoneIsWaitingForControl = 1;
    }
    fSomeoneIsWaitingForControl = 0;
    if (count == 10000)
    {
#line 5510
        AddSoundErrorDuringInterrupt("Warning -- Lock up controlled by %s %ld asking is %s %ld", g_00c64798, g_00c6695c, file, line); AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__);
    }
    strcpy(g_00c64798, file);
    g_00c6695c = line;
    return kTrue;
}

// FUNCTION: STARFLEETOP 0x00653a60
void tgSoundPart::mReleaseControl()
{
    if (field_0xc8 == 0)
        field_0xcc.mRelease();
}
#line 4147 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00653a80
unsigned char tgSoundPart::mSet3DSoundPosition(long channel, tPoint3* position, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
    if (position == 0) return 0;
#line 5553
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mAreWe3D() == 0) {
#line 5561
        SOUND_ERROR(("We are not a 3D channel!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)14, position, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5578
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}
// FUNCTION: STARFLEETOP 0x00653d60
unsigned char tgSoundPart::mSet3DSoundOrientation(long channel, tPoint3* orientation, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
    if (orientation == 0) return 0;
#line 5603
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mAreWe3D() == 0) {
#line 5611
        SOUND_ERROR(("We are not a 3D channel!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)15, orientation, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5628
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}
// FUNCTION: STARFLEETOP 0x00654040
unsigned char tgSoundPart::mSet3DSoundVelocity(long channel, tPoint3* velocity, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
    if (velocity == 0) return 0;
#line 5653
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mAreWe3D() == 0) {
#line 5661
        SOUND_ERROR(("We are not a 3D channel!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)17, velocity, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5678
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}
#line 4217 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00654320
unsigned char tgSoundPart::mSet3DSoundCones(long channel, float angle, long level, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5703
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mAreWe3D() == 0) {
#line 5711
        SOUND_ERROR(("We are not a 3D channel!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)16, &angle, &level);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5728
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}
#line 4240 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x006545f0
unsigned char tgSoundPart::mSet3DSoundReferenceDistance(long channel, double distance, unsigned char immediate)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5752
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mAreWe3D() == 0) {
#line 5760
        SOUND_ERROR(("We are not a 3D channel!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 0; }
    long handle = mCreateCommand((eSoundChannelCommand)18, &distance, 0);
    if (handle == -1) { mReleaseControl(); return 0; }
    if (immediate == 1) channelPtr->mAddImmediateCommand(handle);
    else {
        if (vfunction13() == 1) {
#line 5777
            SOUND_ERROR(("immediate must be kTrue!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
        channelPtr->mAddDelayedCommand(handle); }
    channelPtr->mUpdateRegular(vfunction125());
    mReleaseControl(); return 1;
}
#line 4263 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x006548c0
unsigned char tgSoundPart::mPlaySpecialLoop(long channel, long param_2, long param_3, long param_4, long param_5)
{
#line 5800
    if (vfunction13() == 1) {
        SOUND_ERROR(("mPlaySpecialLoop not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5808
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    unsigned char result = channelPtr->mPlaySpecialLoop(param_2, param_3, param_4, param_5);
    mReleaseControl(); return result;
}
#line 4278 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00654a40
unsigned char tgSoundPart::mEndSpecialLoop(long channel)
{
#line 5830
    if (vfunction13() == 1) {
        SOUND_ERROR(("mEndSpecialLoop not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5838
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    unsigned char result = channelPtr->mEndSpecialLoop();
    mReleaseControl(); return result;
}
#line 4293 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00654bb0
unsigned char tgSoundPart::mAddGetNextAttachmentFunction(long channel, void (*function)(unsigned long, long), long data, double interval)
{
#line 5859
    if (vfunction13() == 1) {
        SOUND_ERROR(("mAddGetNextAttachmentFunction not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 1; }
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5867
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    unsigned char result = channelPtr->mAddGetNextAttachmentFunction(function, data, interval);
    mReleaseControl(); return result;
}
#line 4308 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00654d30
long tgSoundPart::mGetNumberCurrentlyPlaying()
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5891
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    long count = 0;
    for (long handle = field_0x40.vfunction3(); handle != -1; handle = field_0x40.vfunction9(handle))
    {
        tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(handle);
        if (channelPtr != 0 && channelPtr->vfunction52() == 32)
            count++;
    }
    mReleaseControl();
    return count;
}
#line 4325 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00654db0
unsigned char tgSoundPart::mDoWeAutoRemoveChannel(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5919
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    unsigned char result = channelPtr->mDoWeAutoRemoveChannel();
    mReleaseControl(); return result;
}

// FUNCTION: STARFLEETOP 0x00654e20
unsigned char tgSoundPart::mWeAutoRemoveChannel(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5942
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (!channelPtr->mDoWeAutoRemoveChannel())
    {
        long type = channelPtr->mGetSoundType();
        channelPtr->mSetSoundType(type + 0x1000);
    }
    mReleaseControl(); return 1;
}
#line 4353 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00654eb0
unsigned char tgSoundPart::mWeDoNotAutoRemoveChannel(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 5972
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mDoWeAutoRemoveChannel() == 1)
    {
        long type = channelPtr->mGetSoundType();
        type -= 0x1000;
        channelPtr->mSetSoundType(type);
    }
    mReleaseControl(); return 1;
}
#line 4369 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00654f40
unsigned char tgSoundPart::mDoWeAutoRemoveSelections(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6004
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    unsigned char result = channelPtr->mDoWeAutoRemoveSelections();
    mReleaseControl(); return result;
}
#line 4381 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00654fb0
unsigned char tgSoundPart::mWeAutoRemoveSelections(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6027
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (!channelPtr->mDoWeAutoRemoveSelections())
    {
        long type = channelPtr->mGetSoundType();
        channelPtr->mSetSoundType(type + 0x2000);
    }
    mReleaseControl(); return 1;
}
#line 4398 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00655040
unsigned char tgSoundPart::mWeDoNotAutoRemoveSelections(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6057
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mDoWeAutoRemoveSelections() == 1)
    {
        long type = channelPtr->mGetSoundType();
        type -= 0x2000;
        channelPtr->mSetSoundType(type);
    }
    mReleaseControl(); return 1;
}
#line 4415 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x006550d0
unsigned char tgSoundPart::mDoWeRetainSelections(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6090
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    unsigned char result = channelPtr->mDoWeRetainSelections();
    mReleaseControl(); return result;
}
#line 4426 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00655140
unsigned char tgSoundPart::mWeRetainSelections(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6113
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (!channelPtr->mDoWeRetainSelections())
    {
        long type = channelPtr->mGetSoundType();
        channelPtr->mSetSoundType(type + 0x800);
    }
    mReleaseControl(); return 1;
}
#line 4442 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x006551d0
unsigned char tgSoundPart::mWeDoNotRetainSelections(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6143
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    if (channelPtr->mDoWeRetainSelections() == 1)
    {
        long type = channelPtr->mGetSoundType();
        type -= 0x800;
        channelPtr->mSetSoundType(type);
    }
    mReleaseControl(); return 1;
}

// FUNCTION: STARFLEETOP 0x00655260
unsigned char tgSoundPart::mSetGlobalFrequency(float frequency)
{
#line 6169
    SOUND_ERROR(("tgSoundPart mSetGlobalFrequency %f", frequency)); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__));
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6176
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    field_0x60 = frequency;
    for (long handle = field_0x40.vfunction3(); handle != -1; handle = field_0x40.vfunction9(handle))
    {
        tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(handle);
        if (channelPtr != 0 && !channelPtr->mAreWeFixedFrequency())
        {
            long channelFrequency = channelPtr->mGetFrequency();
            long command = mCreateCommand((eSoundChannelCommand)5, &channelFrequency, 0);
            if (command == -1) { mReleaseControl(); return 0; }
            channelPtr->mAddImmediateNonInterruptCommandCommand(command);
            channelPtr->mUpdateRegular(vfunction125());
        }
    }
    mReleaseControl();
    return 1;
}
#line 4484 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00655440
unsigned char tgSoundPart::mSetGlobalPan(long pan)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6221
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    field_0x50 = pan;
    for (long handle = field_0x40.vfunction3(); handle != -1; handle = field_0x40.vfunction9(handle))
    {
        tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(handle);
        if (channelPtr != 0 && !channelPtr->mAreWeFixedPan())
        {
            long channelPan = channelPtr->mGetPan();
            long command = mCreateCommand((eSoundChannelCommand)7, &channelPan, 0);
            if (command == -1) { mReleaseControl(); return 0; }
            channelPtr->mAddImmediateNonInterruptCommandCommand(command);
            channelPtr->mUpdateRegular(vfunction125());
        }
    }
    mReleaseControl();
    return 1;
}
#line 4508 // MATCH-ONLY: the #line directives above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00655550
unsigned char tgSoundPart::mSetGlobalVolume(long volume)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6267
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    long globalVolume = volume;
    if (volume > 100) globalVolume = 100;
    else if (volume < 0) globalVolume = 0;
    field_0x54 = globalVolume;
    field_0x58 = globalVolume * 0.01;
    for (long handle = field_0x40.vfunction3(); handle != -1; handle = field_0x40.vfunction9(handle))
    {
        tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(handle);
        if (channelPtr != 0 && !channelPtr->mAreWeFixedVolume())
        {
            long channelVolume = channelPtr->mGetVolume();
            long command = mCreateCommand((eSoundChannelCommand)7, &channelVolume, 0);
            if (command == -1) { mReleaseControl(); return 0; }
            channelPtr->mAddImmediateNonInterruptCommandCommand(command);
            channelPtr->mUpdateRegular(vfunction125());
        }
    }
    mReleaseControl();
    return 1;
}
#line 4535 // MATCH-ONLY: the #line directive in the function above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00655680
eSoundChannelStatus tgSoundPart::mGetChannelStatus(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return (eSoundChannelStatus)-1;
#line 6320
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return (eSoundChannelStatus)-1; }
    eSoundChannelStatus status = channelPtr->vfunction52();
    mReleaseControl();
    return status;
}
#line 4548 // MATCH-ONLY: the #line directive in the function above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x006556f0
long tgSoundPart::mTotalBytesLeftToPlay(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6346
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    long bytes = channelPtr->mGetTotalBytesLeftToPlay();
    mReleaseControl();
    return bytes;
}
#line 4561 // MATCH-ONLY: the #line directive in the function above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00655760
double tgSoundPart::mTotalNumberOfSecondsLeftToPlay(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6371
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    double seconds = channelPtr->mGetTotalNumberOfSecondsLeftToPlay();
    mReleaseControl();
    return seconds;
}
#line 4574 // MATCH-ONLY: the #line directive in the function above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x006557f0
long tgSoundPart::mNumberOfAttachmentsQueuedUp(long channel)
{
#line 6393
    if (vfunction13() == 1) {
        SOUND_ERROR(("mNumberOfAttachmentsQueuedUp not supported in active mode!")); SOUND_ERROR((" -- %s %d\n", __FILE__, __LINE__)); return 0; }
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6401
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    long count = channelPtr->mGetNumberOfAttachments();
    mReleaseControl();
    return count;
}
#line 4590 // MATCH-ONLY: the #line directive in the function above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00655960
long tgSoundPart::mGetFrequency(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6427
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    long frequency = channelPtr->mGetFrequency();
    mReleaseControl();
    return frequency;
}
#line 4602 // MATCH-ONLY: the #line directive in the function above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x006559d0
long tgSoundPart::mGetPan(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6451
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    long pan = channelPtr->mGetPan();
    mReleaseControl();
    return pan;
}
#line 4615 // MATCH-ONLY: the #line directive in the function above shifted the listing, so the line is reset here.
// FUNCTION: STARFLEETOP 0x00655a40
long tgSoundPart::mGetVolume(long channel)
{
    if (field_0x114.field_0x4[2] != 1) return 0;
#line 6475
    xAttemptToGainControl(__FILE__, __LINE__, 0, 1);
    tSoundChannel* channelPtr = mGetSoundChannelPtrFromHandle(channel);
    if (channelPtr == 0) { mReleaseControl(); return 0; }
    long volume = channelPtr->mGetVolume();
    mReleaseControl();
    return volume;
}
