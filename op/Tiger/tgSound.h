//	tgSound.h: the sound part, tgSoundPart, and the classes tgSound.cpp and taudiochannel.cpp
//	share with it.

#ifndef tgSound_h
#define tgSound_h

#include <string.h>

#include "Tiger/tsay.h"
#include "Tiger/tatomic.h"
#include "Tiger/tSndMemMgr.h"
#include "unsorted/tDynamicMemoryFreeListViaHandles.h"
#include "unsorted/t3DListenerProfile.h"

class tSoundChannel;
class tSoundChannelCommand;
class tSoundSelection;
class tInitSoundPartInfo;
class fn_0064fd10_Array;
class fn_ChannelLink;
class tChannelAttributes;
class t3DSoundProfile;
struct tPoint3;

// Sound errors raised while the sound interrupt runs are buffered and printed later, outside it.
void AddSoundErrorDuringInterrupt(const char* format, ...);
void PrintOutBufferedErrors();
extern tSay gSoundSay;

// HYPOTHESIS: tgSoundPart's channel list, held at 0x40. It shares tSoundCommandList's slots: 3
// the next handle, 26 removes a handle, 32 takes the handle given, 39 the member count.
class tSoundChannelList : public tDynamicMemoryFreeListViaHandles
{
public:
    virtual ~tSoundChannelList();
    virtual tListMemberWithHandle* mGetListMemberWithHandleFromHandle(long handle);
};

// t3DListenerProfile, which tgSoundPart holds at 0x68, is declared in its own header.







// HYPOTHESIS: tSoundTimerResponse derives from tTimerResponse and its size is a guess.
class tSoundTimerResponse
{
public:
    virtual void vfunction0();
    long field_0x4[3];
};

// HYPOTHESIS: tSoundChannelFormat is declared only as far as its constructor needs it. The
// fields are named by the exe's mDump format string; the virtual methods other than the
// destructor and mInit are not declared yet.
class tSoundChannelFormat
{
public:
    tSoundChannelFormat();
    virtual ~tSoundChannelFormat();
    tSoundChannelFormat& operator=(tSoundChannelFormat& other);
    virtual void mInit();
    // HYPOTHESIS: slot 2 sets the buffer length (twStandardSoundChannel::mAddSoundChannel rounds
    // slot 3's length up to slot 12's block size through it); slots 10 and 11 set and get the
    // sound type flags. Their names are not recovered.
    virtual void vfunction2(long value);
    // HYPOTHESIS: slots 3-12 are not recovered; tSoundChannel::mInit reads 3, 7 and 12.
    virtual long vfunction3();
    virtual void vfunction4(); virtual long vfunction5(); virtual void vfunction6();
    virtual long vfunction7();
    virtual void vfunction8(); virtual long vfunction9();
    virtual void vfunction10(long value);
    virtual long vfunction11();
    virtual long vfunction12();
    virtual long vfunction13(); virtual void vfunction14(); virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17(); virtual void vfunction18();
    virtual void vfunction19(); virtual void vfunction20(); virtual void vfunction21(); virtual void vfunction22(); virtual void vfunction23(); virtual void vfunction24();
    virtual void vfunction25();
    // tSoundChannel::mDump calls slot 26 to dump the format.
    virtual void mDump();

    long fBufferLength;
    long fNumChannels;
    long fSamplesPerSec;
    long fBitsPerSample;
    // HYPOTHESIS: a set of flags read by mAreWeStatic and the other mAreWe getters
    long fSoundType;
};

// HYPOTHESIS: t3DSoundProfile (tgsound.h in the original) is declared only as far as mInit,
// operator= and mDump need it. Field names are not recovered; mDump's strings label them, in order,
// 3DSoundPosition, 3DSoundOrientation, 3DSoundVelocity, 3DSoundReferenceDistance,
// 3DSoundConeAngleForNoAttenuation and 3DSoundConeAttenuationLevel (printed with %ld).
class t3DSoundProfile
{
public:
    // The exe's vtable (0x007b6fe8) holds a destructor in slot 0 and mInit in slot 1. HYPOTHESIS:
    // the destructor is inline here; the exe places it after the unit's last function.
    t3DSoundProfile()
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
    // The destructor is defined inline below, after operator=.
    virtual ~t3DSoundProfile();
    virtual void mInit();
    // HYPOTHESIS: the exe's setters and getters (mSet3DSoundPosition and the rest) fill slots
    // 2-20; tChannelAttributes::mDump calls mDump through slot 21. Their order is not recovered.
    virtual void vfunction2(); virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5(); virtual void vfunction6(); virtual void vfunction7();
    virtual void vfunction8(); virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11(); virtual void vfunction12(); virtual void vfunction13();
    virtual void vfunction14(); virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17(); virtual void vfunction18(); virtual void vfunction19();
    virtual void vfunction20();
    virtual void mDump();
    t3DSoundProfile& operator=(t3DSoundProfile& other);

    // The double makes the class 8-aligned, so the compiler pads the vtable pointer to 8.
    float field_0x8[3];
    float field_0x14[3];
    float field_0x20[3];
    double field_0x30;
    float field_0x38;
    long field_0x3c;  // HYPOTHESIS: long (stored as the integer 50)
};

// HYPOTHESIS: tChannelAttributes (tgsound.h in the original) is declared only as far as its
// constructor needs it. The fields are named by the setters the exe exports for this class
// (mSetFrequency); the others are not recovered and keep their offsets.
class tChannelAttributes
{
public:
    tChannelAttributes();
    virtual void vfunction0();
    virtual void mInit();
    // HYPOTHESIS: slot 2 sets the frequency (mDuplicateSoundChannel passes it the format's
    // sample rate); the exe's dump of the attributes is slot 29.
    virtual void mSetFrequency(long frequency);
    virtual void vfunction3();
    // HYPOTHESIS: slot 4 records the pan (twStandardSoundChannel::mSetPan passes it the channel's pan).
    virtual void vfunction4(long pan);
    virtual void vfunction5();
    // HYPOTHESIS: slot 6 records the volume (twStandardSoundChannel::mSetVolume passes it the channel's volume).
    virtual void vfunction6(long volume);
    virtual void vfunction7(long volume);
    virtual void vfunction8(); virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11();
    // HYPOTHESIS: slot 12 records the 3D position (twStandardSoundChannel::mSet3DSoundPosition passes it the position it set).
    virtual void vfunction12(tPoint3* position);
    virtual void vfunction13();
    virtual void vfunction14(); virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17(); virtual void vfunction18(); virtual void vfunction19();
    virtual void vfunction20(); virtual void vfunction21(); virtual void vfunction22(); virtual void vfunction23(); virtual void vfunction24(); virtual void vfunction25();
    virtual void vfunction26(); virtual void vfunction27(); virtual void vfunction28();
    virtual void mDump();

    // The t3DSoundProfile member makes the class 8-aligned, so the vtable pointer is padded to 8.
    long mFrequency;  // HYPOTHESIS: 22050 at construction, so the frequency in Hz
    long field_0xc;
    long field_0x10;
    t3DSoundProfile m3DSoundProfile;  // HYPOTHESIS: returned by mGet3DSoundProfile
};

enum eSoundChannelCommand {};
enum eSoundChannelUpdateMode {};

// HYPOTHESIS: tSoundSelection is declared only as far as tSoundChannelCommand::mDump needs
// it: mDump is slot 43 of its vtable; earlier slots are not recovered yet.
class tSoundSelection
{
public:
    virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2(); virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5();
    virtual void vfunction6(long value);
    virtual void vfunction7(); virtual void vfunction8();
    virtual long vfunction9();
    virtual void vfunction10(long value);
    virtual void vfunction11(); virtual void vfunction12(); virtual void vfunction13();
    virtual void vfunction14(long value);
    virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17(); virtual void vfunction18(); virtual void vfunction19(); virtual void vfunction20();
    virtual void vfunction21(); virtual void* vfunction22(); virtual void vfunction23(); virtual void vfunction24(long value); virtual void vfunction25(long value);
    virtual long vfunction26();
    virtual long vfunction27();
    virtual void vfunction28(); virtual void vfunction29(); virtual void vfunction30(); virtual void vfunction31(); virtual void vfunction32(); virtual void vfunction33();
    virtual void vfunction34(); virtual void vfunction35(); virtual void vfunction36(); virtual void vfunction37(); virtual void vfunction38(); virtual void vfunction39();
    virtual void vfunction40(); virtual void vfunction41(); virtual tSoundSelection* vfunction42();
    virtual void mDump();
};

// HYPOTHESIS: the vtable (0x007b713c) holds get/set pairs from slot 18 on; the names come from the
// exe's matched methods, assigned to slots by pairing each getter with its setter. Getters whose
// names are not recovered keep their slot number.
// tgSoundPart allocates its commands with new[], so the compiler generates a vector deleting
// destructor for this class (0x00650fc0); it has no source of its own and is claimed on the
// destructor's definition.
class tSoundChannelCommand : public tListMemberWithHandle
{
public:
    tSoundChannelCommand();
    virtual ~tSoundChannelCommand();
    virtual void mDump();
    virtual iTruth mIsInvalid();
    virtual void mMakeInvalid();
    virtual void mMakeAvailable();
    virtual iTruth mMakeUnavailable();
    virtual long vfunction14();
    virtual void vfunction15(long count);
    // HYPOTHESIS: slot 16 returns the command type; slot 14 the attachment count it waits for
    virtual long vfunction16();
    virtual void mSetSoundChannelCommand(eSoundChannelCommand command);
    virtual long vfunction18();
    virtual void mSetFrequency(long frequency);
    virtual long mGetPan();
    virtual void mSetPan(long pan);
    virtual long vfunction22();
    virtual void mSetVolume(long volume);
    virtual long vfunction24();
    virtual void mSetPriority(long priority);
    virtual long mGetUserData();
    virtual void mSetUserData(long userData);
    virtual long vfunction28();
    virtual void mSetFinishSoundCallback(void (*callback)(unsigned long, iTruth));
    virtual tChannelAttributes* mGetChannelAttributes();
    virtual void mSetChannelAttributes(tChannelAttributes* attributes);
    virtual tSoundSelection* vfunction32();
    virtual void mSetSoundSelection(tSoundSelection* selection);
    virtual t3DSoundProfile* mGet3DSoundProfile();
    virtual void mSet3DSoundProfile(t3DSoundProfile* profile);
    virtual tPoint3* mGet3DSoundPosition();
    virtual void mSet3DSoundPosition(tPoint3* position);
    virtual tPoint3* mGet3DSoundOrientation();
    virtual void mSet3DSoundOrientation(tPoint3* orientation);
    virtual tPoint3* mGet3DSoundVelocity();
    virtual void mSet3DSoundVelocity(tPoint3* velocity);
    virtual float mGet3DSoundConeAngleForNoAttenuation();
    virtual void mSet3DSoundConeAngleForNoAttenuation(float angle);
    virtual double mGet3DSoundReferenceDistance();
    virtual void mSet3DSoundReferenceDistance(double distance);
    virtual long mGet3DSoundConeAttenuationLevel();
    virtual void mSet3DSoundConeAttenuationLevel(long level);
    virtual long mGetUserPositionContext();
    virtual void mSetUserPositionContext(long context);
    virtual long vfunction50();
    virtual void mSetUpdateSoundPositionFunction(void (*function)(unsigned long, long));
    virtual double mGetMillisecondsBetweenUpdates();
    virtual void mSetMillisecondsBetweenUpdates(double milliseconds);
    virtual double mGetDelayImmediateCommandsUntilThisTime();
    virtual void mSetDelayImmediateCommandsUntilThisTime(double time);
    virtual double mGetTimeToTake();
    virtual void mSetTimeToTake(double timeToTake);
    virtual long mGetFinalVolume();
    virtual void mSetFinalVolume(long volume);

    // The names of the next three fields come from the mDump format string.
    long fExecuteAfterThisNumberAttaches;
    eSoundChannelCommand fSoundChannelCommand;
    // HYPOTHESIS: guards the command between the game and the sound interrupt
    tAtomicLock fAvailable;
    long field_0x3c;
    long field_0x40;
    long field_0x44[21];
    // HYPOTHESIS: inlined where tgSoundPart grows its command pool; the handle fields at 0x4 and
    // 0x8 and the word at 0x3c are not copied, and 0x40-0x97 is copied as one block
    tSoundChannelCommand& operator=(tSoundChannelCommand& other)
    {
        field_0c = other.field_0c;
        fExecuteAfterThisNumberAttaches = other.fExecuteAfterThisNumberAttaches;
        fSoundChannelCommand = other.fSoundChannelCommand;
        fAvailable = other.fAvailable;
        memmove(&field_0x40, &other.field_0x40, 0x58);
        field_0x98 = other.field_0x98;
        return *this;
    }
    long field_0x98;
    // HYPOTHESIS: tgSoundPart's command array (0xc) steps 0xa0 bytes per command; what sits at
    // 0x9c (or whether it is alignment after a double) is not recovered
    long field_0x9c;
};

// HYPOTHESIS: declared only as far as its mGetListMemberWithHandleFromHandle needs it; the list forwards the lookup to
// the sound part (tgSoundPart slot 129).
class tSoundCommandList
{
public:
    virtual tListMemberWithHandle* mGetListMemberWithHandleFromHandle(long handle);
    // HYPOTHESIS: slots 1-25 are not recovered; slot 26 adds a member by handle.
    // Slot 5 returns the first member (mGetFirstAttachment); slot 14 the member after the one
    // given.
    virtual class tSoundChannelCommand* vfunction1(long handle);
    virtual void vfunction2();
    virtual long vfunction3();
    virtual void vfunction4();
    virtual class tSoundChannelCommand* vfunction5();
    virtual void vfunction6(); virtual long vfunction7(long handle); virtual void vfunction8();
    virtual long vfunction9(long handle);
    virtual void vfunction10(); virtual void vfunction11(); virtual void vfunction12(); virtual void vfunction13();
    virtual class tSoundChannelCommand* vfunction14(class tSoundChannelCommand* command);
    virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17(); virtual void vfunction18(); virtual void vfunction19(); virtual void vfunction20();
    virtual void vfunction21(); virtual void vfunction22(); virtual void vfunction23(); virtual void vfunction24(); virtual void vfunction25();
    virtual iTruth vfunction26(long handle);
    virtual void vfunction27(class tSoundChannelCommand* command);
    virtual void vfunction28(); virtual void vfunction29(); virtual void vfunction30(); virtual void vfunction31(); virtual void vfunction32();
    virtual void vfunction33(class tSoundChannelCommand* command);
    virtual void vfunction34(); virtual void vfunction35(); virtual void vfunction36(); virtual void vfunction37();
    virtual iTruth vfunction38();
    virtual long vfunction39();
    virtual void vfunction40(); virtual void vfunction41();
    // HYPOTHESIS: slot 42 dumps the list (tSoundChannel::mDump); slot 39 returns the member count
    // (mReset); slot 33 removes a member (mClearCommands)
    virtual void vfunction42();
};

// The slots follow the exe's vtable of the class (0x007b6d1c, 122 slots). Slot 0 is the
// destructor and slots 1-8 are tListMemberWithHandle's, so tSoundChannel derives from it and
// overrides its mDump (slot 9). Slots the exe fills with _purecall are pure here; the derived
// channel classes fill them.
// HYPOTHESIS: the return types and arguments of the getters this unit does not call are not
// recovered; they are declared void with no argument. The command list at 0xcc is the
// delayed-command list (mGetFirstDelayedCommand reads it); its slot 26 adds a handle.
class tSoundChannel : public tListMemberWithHandle
{
public:
    virtual ~tSoundChannel();
    virtual void mDump();
    virtual tSoundCommandList* mGetAttachmentsList();
    virtual iTruth mPlaySpecialLoop(long param_1, long param_2, long param_3, long param_4);
    virtual iTruth mEndSpecialLoop();
    virtual iTruth mAddDelayedCommand(long handle);
    virtual iTruth mAddImmediateCommand(long handle);
    virtual iTruth mAddAttachment(long handle);
    virtual iTruth mAddImmediateNonInterruptCommandCommand(long handle);
    virtual long mGetUpdateMode();
    virtual long mGetAmountThatCanBeAdded();
    virtual void mDone();
    virtual void mInit();
    virtual void mLockInData();
    virtual void mClearCommands();
    virtual void mClearAllCommands();
    virtual void mClearAttachments();
    virtual void mClearAllAttachments();
    virtual void mSetUpdateMode(eSoundChannelUpdateMode mode);
    virtual void mGetFirstImmediateNonInterruptCommand();
    virtual tSoundChannelCommand* mGetFirstImmediateCommand();
    virtual tSoundChannelCommand* mGetFirstDelayedCommand();
    virtual tSoundChannelCommand* mGetFirstAttachment();
    virtual tSoundSelection* mGetLastSelectionToAttach();
    virtual unsigned char mDoFillSelection(unsigned char param_1) = 0;
    virtual void vfunction33();
    virtual tSoundSelection* mGetSelectionToAttach();
    virtual tSoundChannelCommand* mGetFirstPlayingCommand();
    virtual void mClear();
    virtual void mClearAll();
    virtual void mReset(iTruth param_1);
    virtual iTruth mTestInList();
    virtual long mGetFrequency();
    virtual long mGetPan();
    virtual long mGetVolume();
    virtual void mGetChannelPriority();
    virtual void mGetChannelAttributes();
    virtual void mGet3DSoundProfile();
    virtual void mGet3DSoundPosition();
    virtual void mGet3DSoundOrientation();
    virtual void mGet3DSoundVelocity();
    virtual void mGet3DSoundReferenceDistance();
    virtual void mGet3DSoundConeAngleForNoAttenuation();
    virtual void mGet3DSoundConeAttenuationLevel();
    virtual long vfunction52();
    virtual tSoundChannelFormat* mGetSoundChannelFormat();
    virtual long mGetBufferLength();
    virtual void mGetNumChannels();
    virtual void mGetBytesPerSample();
    virtual void vfunction57();
    virtual void mGetBitsPerSample();
    virtual void mGetSoundType();
    virtual void mSetSoundType();
    virtual double mGetMillisecondsLeftToPlay();
    virtual void vfunction62(); virtual void vfunction63();
    virtual void mGetUserData();
    virtual void mSetUserData(long userData);
    virtual void mGetFinishSoundCallback();
    virtual void mSetFinishSoundCallback(long callback);
    virtual long mGetTotalBytesLeftToPlay();
    virtual double mGetTotalNumberOfSecondsLeftToPlay();
    virtual void mGetNumberOfAttachments();
    virtual iTruth mAreWeLooping();
    virtual iTruth mAreWeStreaming();
    virtual iTruth mAreWe3D();
    virtual iTruth mAreWeStatic();
    virtual iTruth mAreWeFixedFrequency();
    virtual void mAreWeARecordingChannel();
    virtual iTruth mTryToLocateInHardware();
    virtual iTruth mTryToLocateInSoftware();
    virtual iTruth mAreWeFixedPan();
    virtual iTruth mAreWeFixedVolume();
    virtual iTruth mAreWeAGlobalSound();
    virtual iTruth mDoWeRetainSelections();
    virtual iTruth mDoWeAutoRemoveChannel();
    virtual iTruth mDoWeAutoRemoveSelections();
    virtual unsigned char mAutoRemoveChannel();
    virtual unsigned char mAutoRemoveSelections();
    virtual iTruth mUpdateRegular(double time);
    virtual iTruth mUpdateAtInterrupt(double time);
    virtual iTruth mDoCommand(tSoundChannelCommand* command);
    virtual unsigned char mUpdateBuffersAndSelections(double time) = 0;
    virtual iTruth mAreWeStillActive();
    virtual iTruth mCleanupSelections();
    virtual unsigned char mUpdateAmountPlayed() = 0;
    virtual void mAddFinishCommand(iTruth param_1);
    // Slots 32 and 95: twStandardSoundChannel::mAddActiveSoundChannel calls them with these arguments.
    virtual unsigned char mAddSoundChannel(tSoundChannelFormat* format, long handle) = 0;
    virtual void vfunction96() = 0;
    virtual iTruth mRemoveSoundChannel();
    virtual void vfunction98() = 0;
    virtual unsigned char mPlayChannel() = 0;
    virtual unsigned char mSetFrequency(long frequency) = 0;
    virtual unsigned char mSetPan(long pan) = 0;
    virtual unsigned char mSetVolume(long volume) = 0;
    // Slots 103 and 104: mFadeToStopAndClearNow's fade (state 3) ends in 103, the stop-and-reset fade (state 2) in 104.
    virtual unsigned char mStopAndClearChannel();
    virtual unsigned char mStopAndResetChannel() = 0;
    virtual iTruth mSetChannelPriority(long priority);
    virtual unsigned char mSet3DSoundPosition(tPoint3* position) = 0;
    virtual void vfunction107(tPoint3* orientation) = 0;
    virtual void vfunction108(tPoint3* velocity) = 0;
    virtual void vfunction109(float angle, long level) = 0;
    virtual void vfunction110(double distance) = 0;
    virtual void vfunction111(t3DSoundProfile* profile) = 0;
    virtual void mGetUserPositionContext();
    virtual void mSetUserPositionContext(long context);
    virtual void mGetUpdateSoundPositionFunction();
    virtual void mSetUpdateSoundPositionFunction(long function);
    virtual void vfunction116();
    virtual void mSetMillisecondsBetweenUpdates(double milliseconds);
    virtual iTruth mAddGetNextAttachmentFunction(void (*function)(unsigned long, long), long userData, double milliseconds);
    virtual iTruth mFadeToVolume(long volume, double timeToTake);
    virtual iTruth mFadeToStopAndResetNow(double timeToTake);
    virtual iTruth mFadeToStopAndClearNow(double timeToTake);

    // The doubles make the class 8-aligned; tListMemberWithHandle's fields fill 0x4-0xf.
    tSoundChannelFormat field_0x10;
    long field_0x28;
    tChannelAttributes field_0x30;
    long field_0x88, field_0x8c;
    void (*field_0x90)(long, iTruth);
    long field_0x94, field_0x98;
    void (*field_0x9c)(long, long);
    long field_0xa0;
    double field_0xa8, field_0xb0;
    long field_0xb8;
    tSoundCommandList field_0xbc;
    long field_0xc0[3];
    tSoundCommandList field_0xcc;
    long field_0xd0[3];
    tSoundCommandList field_0xdc;
    long field_0xe0[3];
    tSoundCommandList field_0xec;
    long field_0xf0[3];
    long field_0xfc;
    long field_0x100;
    long field_0x104;
    long field_0x108;
    long field_0x10c;
    long field_0x110;
    long field_0x114;
    long field_0x118;
    long fFadeVolume;  // named by mFadeToVolume's message
    double field_0x120;
    double field_0x128;
    double field_0x130;
    // HYPOTHESIS: the get-next-attachment function
    void (*field_0x138)(unsigned long, long);
    long field_0x13c;  // HYPOTHESIS: its user position context
    double field_0x140;
    unsigned char field_0x148;
    long field_0x14c;
    long field_0x150;
    long field_0x154;
    long field_0x158;
    long field_0x15c, field_0x160, field_0x164, field_0x168;
    unsigned char field_0x16c;
    // The fill-selection function and its user data: twStandardSoundChannel::mAddActiveSoundChannel's
    // mangled name gives both function types; mDoFillSelection calls this one.
    typedef tSoundSelection* (*tFillSelectionFunction)(unsigned long&, unsigned char, tSoundSelection*&);
    tFillSelectionFunction field_0x170;
    // HYPOTHESIS: a function that hands the channel its next selection; its name is not recovered.
    typedef tSoundSelection* (*tSelectionFunction)(long, long);
    tSelectionFunction field_0x174;
    unsigned long field_0x178;
};

// HYPOTHESIS: the sound part, bound by name through its part cache (its cache pointer is at
// 0x00c64794). tPart's virtuals fill slots 0-5; the exe's vtable of the class (0x00798cc4) has 114
// slots, and the names of slots 6-113 are those of tgSoundPart's and twSoundPart's functions in
// the same slots of their vtables (0x00798fb0, 0x00798a3c). Slots whose functions have no name
// are numbered vfunction<slot>. Parameters and return types are the ones the callers use; the
// others are not recovered.
class tSoundPart : public tPart
{
public:
    tSoundPart() {}
    GenAbstractBinding(tSoundPart);

    virtual iTruth mInit(tInitSoundPartInfo* info);
    virtual void mDone();
    // HYPOTHESIS: loads a sound for a channel format and returns its channel; the argument is
    // an element of tChannelGroup's array at +0x20 (mLoad's error text calls them formats).
    virtual long mAddSoundChannel(fn_ChannelLink* link);
    virtual void mAddActiveSoundChannel(); virtual void mDuplicateSoundChannel();
    virtual unsigned char mRemoveSoundChannel(long channel);
    virtual unsigned char mRemoveAllSoundChannels();
    virtual long vfunction13();
    virtual unsigned char mAttachSoundToChannel(long channel, tSoundSelection* selection);
    virtual unsigned char mPlayChannel(long channel);
    virtual unsigned char mPauseChannel(long channel);
    virtual void mStopAndResetChannel();
    virtual unsigned char mStopAndClearChannel(long channel);
    virtual void mPauseAllChannels(); virtual void mResumeAllChannels(); virtual void mPlayAllChannels(); virtual void mStopAndResetAllChannels();
    virtual void mStopAndClearAllChannels(); virtual void vfunction24(); virtual void vfunction25(); virtual void mSetFrequency();
    virtual void mSetPan(); virtual void mSetVolume(); virtual void mSetPriority(); virtual void mSetFinishFunction();
    virtual void mSetChannelAttributes(); virtual void mSet3DSoundProfile(); virtual void mSet3DSoundPosition(); virtual void mSet3DSoundOrientation();
    virtual void mSet3DSoundVelocity(); virtual void mSet3DSoundCones(); virtual void mSet3DSoundReferenceDistance(); virtual void mSetGlobalFrequency();
    virtual void mSetGlobalPan(); virtual void mSetGlobalVolume(); virtual void mAdjust3DListener(); virtual void mSet3DListener3DPosition();
    virtual void mSet3DListenerOrientation(); virtual void mSet3DListenerVelocity(); virtual void mSet3DListenerUnitScaleInMeters();
    virtual long mGetChannelStatus(long channel);
    virtual void mGetHardwareInformation(); virtual void mTotalBytesLeftToPlay(); virtual void mTotalNumberOfSecondsLeftToPlay(); virtual void mNumberOfAttachmentsQueuedUp();
    virtual void mGetFrequency(); virtual void mGetPan(); virtual void mGetVolume(); virtual void mGetChannelAttributes();
    virtual void mGet3DSoundProfile(); virtual void mGet3DSoundPosition(); virtual void mGet3DSoundOrientation(); virtual void mGet3DSoundVelocity();
    virtual void mGet3DSoundReferenceDistance(); virtual void mGet3DSoundConeAngleForNoAttenuation(); virtual void mGet3DSoundConeAttenuationLevel(); virtual void mGetChannelPriority();
    virtual void mGetSoundChannelFormat(); virtual void mAreWeLooping(); virtual void mAreWeStreaming(); virtual void mAreWe3D();
    virtual void mAreWeStatic(); virtual void mAreWeFixedFrequency(); virtual void mAreWeARecordingChannel(); virtual void mTryToLocateInHardware();
    virtual void mTryToLocateInSoftware(); virtual void mAreWeFixedPan(); virtual void mAreWeFixedVolume(); virtual void mAreWeAGlobalSound();
    // HYPOTHESIS: slot 75 returns the global frequency factor; twStandardSoundChannel::mSetFrequency
    // multiplies the channel's frequency by it.
    virtual double vfunction75();
    // HYPOTHESIS: slot 76 returns the global pan, which twStandardSoundChannel::mSetPan adds to the channel's.
    virtual long vfunction76();
    virtual void vfunction77(); virtual void mGet3DListener();
    virtual void mGet3DListener3DPosition(); virtual void mGet3DListenerForwardDirection(); virtual void mGet3DListenerUpDirection(); virtual void mGet3DListenerVelocity();
    virtual void mGet3DListenerUnitScaleInMeters();
    // Slots 84 and 85 are mDump(int) and mDump(); VC6 lays overloads out in reverse
    // declaration order.
    virtual void mDump();
    virtual void mDump(int level);
    virtual void mUpdate();
    virtual void mAddUpdateSoundPositionFunction(); virtual void mAddUpdateListenerPositionFunction(); virtual void mAddDelayCommand(); virtual void vfunction90();
    virtual void mFadeToVolume(); virtual void mFadeToStopAndResetNow(); virtual void mFadeToStopAndClearNow(); virtual void mPlaySpecialLoop();
    virtual void mEndSpecialLoop(); virtual void mAddGetNextAttachmentFunction(); virtual void mGetNumberCurrentlyPlaying(); virtual void mDoWeRetainSelections();
    virtual void mWeRetainSelections();
    virtual void mWeDoNotRetainSelections(long channel);
    virtual void mDoWeAutoRemoveChannel(); virtual void mWeAutoRemoveChannel(); virtual void mWeDoNotAutoRemoveChannel(); virtual void mDoWeAutoRemoveSelections();
    virtual void mWeAutoRemoveSelections(); virtual void mWeDoNotAutoRemoveSelections();
    virtual unsigned char mIsChannelActive(long channel);
    virtual void vfunction108(); virtual void mRemoveRecordChannel(); virtual void mAttachSoundToRecordChannel(); virtual void mRecordChannel();
    virtual void mPauseRecordChannel(); virtual void mStopAndClearRecordChannel();
};

// HYPOTHESIS: a part like tClockPart: tPart's virtuals fill slots 0-5 and its fields 0x4-0xb, so
// the vtable pointer is not padded. Its vtable (0x00798fb0) has 139 slots, 0-138; twSoundPart's
// first own virtual is slot 139. It overrides most of tSoundPart's slots; only the ones this
// part's units define are declared again here.
class tgSoundPart : public tSoundPart
{
public:
    tgSoundPart();
    virtual void mActivate();
    virtual void mDeactivate();
    virtual iTruth mInit(tInitSoundPartInfo* info);
    virtual void mDone();
    virtual unsigned char mRemoveSoundChannel(long channel);
    virtual unsigned char mRemoveAllSoundChannels();
    virtual void mUpdate();
    // HYPOTHESIS: slots 114 and 115 take and release the part between the game and the sound
    // interrupt; xAttemptToGainControl passes the caller's file and line.
    virtual iTruth xAttemptToGainControl(const char* file, long line, long a, long b);
    virtual void mReleaseControl();
    // HYPOTHESIS: slot 116 (vtable offset 0x1d0); true while the sound part is inside its
    // interrupt.
    virtual iTruth mAreWeInInterruptMode();
    virtual void vfunction117(); virtual void vfunction118(); virtual void vfunction119(); virtual void vfunction120();
    virtual void vfunction121(); virtual void vfunction122();
    virtual void vfunction123(long field);
    // HYPOTHESIS: slot 124 returns the global volume factor; twStandardSoundChannel::mSetVolume
    // multiplies the channel's volume by it.
    virtual double vfunction124();
    virtual double vfunction125();
    virtual void vfunction126();
    virtual void vfunction127();
    virtual void vfunction128();
    virtual tSoundChannelCommand* mGetSoundChannelCommandPtrFromHandle(long handle);
    virtual long mFindAvailableSoundChannelCommandPtrAndReturnAHandle();
    virtual long mSetNumberOfSoundChannelCommands(long count);
    virtual tSoundChannel* mGetSoundChannelPtrFromHandle(long handle);
    virtual long mSetNumberOfSoundChannels(long count);
    virtual long mFindAvailableSoundChannelHandle();
    virtual unsigned char mAddChannelAtHandle(tSoundChannel* channel, long handle);
    virtual void mUpdateAtInterrupt();
    virtual unsigned char mClearOutSoundChannel(long channel);
    virtual long mCreateCommand(long type, long a, long b);

    fn_0064fd10_Array* field_0xc;
    long field_0x10;
    long field_0x14;
    tSoundChannel** field_0x18; // HYPOTHESIS: the channel table; 0x1c its size, 0x20 its count
    long field_0x1c;
    long field_0x20;
    long field_0x24;
    void (*field_0x28)(long); // HYPOTHESIS: a callback run with 0x2c when 0x38 seconds have passed since 0x30
    long field_0x2c;
    double field_0x30;
    double field_0x38;
    tSoundChannelList field_0x40;
    long field_0x50;
    long field_0x54;
    double field_0x58;
    float field_0x60;
    long field_0x64;
    t3DListenerProfile field_0x68;
    long field_0xa8;
    tSoundMemoryBlockManager* field_0xac;
    double (*field_0xb0)();
    long field_0xb4;
    double field_0xb8; // HYPOTHESIS: the current time
    double field_0xc0;
    long field_0xc8; // HYPOTHESIS: a reentry count
    tAtomicLock field_0xcc;
    tAtomicLock field_0xf0;
    tSoundTimerResponse field_0x114;
    long field_0x124;
    unsigned char field_0x128;
    // HYPOTHESIS: set while another thread waits for the sound part; mUpdateAtInterrupt stops on it
    // ("Someone is waiting for control!"). The name is ours.
    int8u fSomeoneIsWaitingForControl;
    unsigned char field_0x12a;
};

extern tgSoundPart* atgSoundPart;

#endif
