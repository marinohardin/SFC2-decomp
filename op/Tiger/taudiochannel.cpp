// Tiger/taudiochannel.cpp
#include "Tiger/tgSound.h"
class tSoundSelection; // MATCH-ONLY: tgSound.h now declares what stood here; the lines are kept so line numbers stay.
// HYPOTHESIS: the element type of the array at tChannelGroup +0x20 (0x18 bytes, with a vtable; probably tSoundChannelFormat, which mAddSoundChannel takes). Not yet replaced by it.
class fn_ChannelLink
{
public:
    virtual ~fn_ChannelLink(); // HYPOTHESIS: slot 0 is the destructor (mClose deletes the array)
    virtual void vfn_04(); virtual void vfn_08(); virtual void vfn_0c();
    virtual void vfn_10(); virtual void vfn_14(); virtual void vfn_18(); virtual void vfn_1c();
    virtual void vfn_20(); virtual void vfn_24(); virtual void vfn_28(long param_1); virtual long vfn_2c(); // HYPOTHESIS: set and get of a flags word
    virtual void vfn_30(); virtual void vfn_34(); virtual void vfn_38(); virtual unsigned char vfn_3c(); // HYPOTHESIS: a flag test
    virtual void vfn_40(); virtual unsigned char vfn_44(); virtual void vfn_48(); virtual void vfn_4c(); // HYPOTHESIS: vfn_44 and vfn_64 are flag tests
    virtual void vfn_50(); virtual void vfn_54(); virtual void vfn_58(); virtual void vfn_5c();
    virtual void vfn_60(); virtual unsigned char vfn_64();
    virtual void vfn_68(); // HYPOTHESIS: mDump
    char pad_04[0x14];
};
// HYPOTHESIS: the element type of tChannelGroup's channel array (0x70 bytes, with a
// vtable). Its class name and the names of these virtuals are not recovered yet; they are named by vtable offset until evidence appears.
class fn_ChannelSlot
{
public:
    virtual ~fn_ChannelSlot(); // HYPOTHESIS: slot 0 is the destructor (mClose deletes the array)
    virtual void vfn_04();
    virtual void vfn_08(long param_1); // HYPOTHESIS: records the frequency (mSetFrequency passes it)
    virtual long vfn_0c(); // HYPOTHESIS: returns the recorded frequency (mGetFrequency returns it)
    virtual void vfn_10(long param_1); // HYPOTHESIS: records the pan (mSetPan passes it)
    virtual long vfn_14(); // HYPOTHESIS: returns the recorded pan (mGetPan returns it)
    virtual void vfn_18(long param_1); // HYPOTHESIS: records the volume (mSetVolume passes it)
    virtual long vfn_1c(); // HYPOTHESIS: returns the recorded volume (mGetVolume returns it)
    virtual void vfn_20(t3DSoundProfile *param_1); // HYPOTHESIS: records the 3D profile (mSet3DSoundProfile passes it)
    virtual t3DSoundProfile *vfn_24(); // HYPOTHESIS: returns the recorded 3D profile (mGet3DSoundProfile returns it)
    virtual void vfn_28();
    virtual void vfn_2c();
    virtual void vfn_30(tPoint3 *param_1); // HYPOTHESIS: records the 3D position (mSet3DSoundPosition passes it)
    virtual tPoint3 *vfn_34(); // HYPOTHESIS: returns the recorded 3D position (mGet3DSoundPosition returns it)
    virtual void vfn_38();
    virtual void vfn_3c();
    virtual void vfn_40(tPoint3 *param_1); // HYPOTHESIS: records the 3D orientation (mSet3DSoundOrientation passes it)
    virtual tPoint3 *vfn_44(); // HYPOTHESIS: returns the recorded 3D orientation (mGet3DSoundOrientation returns it)
    virtual void vfn_48();
    virtual void vfn_4c();
    virtual void vfn_50(tPoint3 *param_1); // HYPOTHESIS: records the 3D velocity (mSet3DSoundVelocity passes it)
    virtual tPoint3 *vfn_54(); // HYPOTHESIS: returns the recorded 3D velocity (mGet3DSoundVelocity returns it)
    virtual void vfn_58(double param_1); // HYPOTHESIS: records the reference distance (mSet3DSoundReferenceDistance passes it)
    virtual double vfn_5c(); // HYPOTHESIS: returns the recorded reference distance (mGet3DSoundReferenceDistance returns it)
    virtual void vfn_60();
    virtual float vfn_64(); // HYPOTHESIS: returns the recorded cone angle (mGet3DSoundConeAngleForNoAttenuation returns it)
    virtual void vfn_68();
    virtual long vfn_6c(); // HYPOTHESIS: returns the recorded cone attenuation level (mGet3DSoundConeAttenuationLevel returns it)
    virtual void vfn_70(float param_1, long param_2); // HYPOTHESIS: records the cone angle and level (mSet3DSoundCones passes them)
    virtual void vfn_74(); // HYPOTHESIS: mDump
    virtual void vfn_78();
    virtual long vfn_7c(); // HYPOTHESIS: returns the sound channel loaded in the slot, or -1
    virtual tChannelAttributes *vfn_80(); // HYPOTHESIS: returns the recorded attributes (mGetChannelAttributes returns them)
    virtual long vfn_84(); // HYPOTHESIS: returns the recorded priority (mGetChannelPriority returns it)
    virtual void vfn_88();
    virtual unsigned long vfn_8c(); // HYPOTHESIS: returns the channel's id
    virtual void vfn_90(long param_1); // HYPOTHESIS: sets the loaded sound channel
    virtual void vfn_94(long param_1); // HYPOTHESIS: records the priority (mSetPriority passes it)
    virtual void vfn_98();
    virtual void vfn_9c(unsigned long param_1); // HYPOTHESIS: sets the channel's id
    virtual void vfn_a0(tChannelAttributes *param_1); // HYPOTHESIS: records the attributes (mSetChannelAttributes passes them)
    virtual unsigned char vfn_a4(); /* HYPOTHESIS: true when the slot is in use */ virtual void vfn_a8(); virtual void vfn_ac(); virtual void vfn_b0(iTruth param_1); // HYPOTHESIS: vfn_b0 marks the slot in use
    char pad_04[0x6c];
};
class tChannelGroup
{
public:
    virtual void mDump(); // HYPOTHESIS: declared first; VC6 lays overloads out in reverse, and mRemoveChannelGroupOwner calls the no-argument mDump through slot +4
    virtual void mDump(unsigned long param_1, long param_2); // HYPOTHESIS: slot order; 0x10 is the one exported virtual with this signature; mKillRetainedMode's slot is not recovered.
    virtual void vfn_08();
    virtual unsigned char mAttachFileToChannel(unsigned long param_1, long param_2); // slot +0xc: tChannelGroupManagerPart::mAttachFileToChannel calls it there
    virtual unsigned char mAttachFileToBaseGroup(long param_1);
    virtual unsigned char mKillRetainedMode();
    virtual unsigned char mClearAttachments();
    virtual unsigned char mAttachSoundToChannel(unsigned long param_1, tSoundSelection *param_2, long param_3);
    virtual unsigned char mPlayChannel(unsigned long param_1, long param_2);
    virtual unsigned char mPauseChannel(unsigned long param_1, long param_2);
    virtual unsigned char mSetFrequency(unsigned long param_1, long param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSetPan(unsigned long param_1, long param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSetVolume(unsigned long param_1, long param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSetPriority(unsigned long param_1, long param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSetFinishFunction(unsigned long param_1, void (* const param_2)(unsigned long, unsigned char), long param_3, long param_4, unsigned char param_5); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSetChannelAttributes(unsigned long param_1, tChannelAttributes *param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSet3DSoundProfile(unsigned long param_1, t3DSoundProfile *param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSet3DSoundPosition(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSet3DSoundOrientation(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSet3DSoundVelocity(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSet3DSoundCones(unsigned long param_1, float param_2, long param_3, long param_4, unsigned char param_5); // HYPOTHESIS: slot not recovered
    virtual unsigned char mSet3DSoundReferenceDistance(unsigned long param_1, double param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual long mGetFrequency(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual long mGetPan(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual long mGetVolume(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual tChannelAttributes *mGetChannelAttributes(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual t3DSoundProfile *mGet3DSoundProfile(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual tPoint3 *mGet3DSoundPosition(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual tPoint3 *mGet3DSoundOrientation(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual tPoint3 *mGet3DSoundVelocity(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual double mGet3DSoundReferenceDistance(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual float mGet3DSoundConeAngleForNoAttenuation(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual long mGet3DSoundConeAttenuationLevel(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual long mGetChannelPriority(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual eSoundChannelStatus mGetChannelStatus(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual long mTotalBytesLeftToPlay(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual double mTotalNumberOfSecondsLeftToPlay(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual long mNumberOfAttachmentsQueuedUp(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual tSoundChannelFormat *mGetSoundChannelFormat(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mDoWeRetainSelections(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAreWeLooping(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAreWeStreaming(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAreWe3D(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAreWeStatic(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAreWeFixedFrequency(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAreWeARecordingChannel(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mTryToLocateInHardware(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mTryToLocateInSoftware(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAreWeFixedPan(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAreWeFixedVolume(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAreWeAGlobalSound(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAddUpdateSoundPositionFunction(unsigned long param_1, void (*param_2)(unsigned long, long), long param_3, double param_4, long param_5, unsigned char param_6); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAddDelayCommand(unsigned long param_1, double param_2, long param_3, unsigned char param_4); // HYPOTHESIS: slot not recovered
    virtual unsigned char mFadeToVolume(unsigned long param_1, long param_2, double param_3, long param_4, unsigned char param_5); // HYPOTHESIS: slot not recovered
    virtual unsigned char mFadeToStopAndResetNow(unsigned long param_1, double param_2, long param_3); // HYPOTHESIS: slot not recovered
    virtual unsigned char mFadeToStopAndClearNow(unsigned long param_1, double param_2, long param_3); // HYPOTHESIS: slot not recovered
    virtual unsigned char mPlaySpecialLoop(unsigned long param_1, long param_2, long param_3, long param_4, long param_5, long param_6); // HYPOTHESIS: slot not recovered
    virtual unsigned char mEndSpecialLoop(unsigned long param_1, long param_2); // HYPOTHESIS: slot not recovered
    virtual unsigned char mAddGetNextAttachmentFunction(unsigned long param_1, void (*param_2)(unsigned long, long), long param_3, double param_4, long param_5); // HYPOTHESIS: slot not recovered
    inline long mGetIndex(unsigned long param_1, long param_2);
    ~tChannelGroup(); // HYPOTHESIS: not virtual; tSafeArray's destructor inlines it with a direct mClose call
    void mClose();
    void mUnload(long param_1);
    void mLoad(long param_1);
    inline long mGetChannelNumberFromIndex(long param_1);
    long mAddDuplicate(unsigned long param_1);
    long mGetAvailableChannel(unsigned long param_1);
    long pad_04;
    long field_08; // fDepth, per the dump text; HYPOTHESIS: number of channels
    long pad_0c;
    long field_10; // HYPOTHESIS: duplicate count; mAddDuplicate clears it
    long field_14; // fLinkDepth, per the dump text; HYPOTHESIS: stride between channel numbers
    iTruth field_18; // fStopIfChannelIsNeeded, per the dump text
    fn_ChannelSlot *field_1c; // fChannelInfos, per the dump text
    fn_ChannelLink *field_20; // HYPOTHESIS: one entry per link
    long field_24; // fNumberOfOwners, per the dump text
};
// FUNCTION: STARFLEETOP 0x006587f0
void tChannelGroup::mDump(unsigned long param_1, long param_2)
{
    field_1c[mGetIndex(param_1, param_2)].vfn_74();
}
// FUNCTION: STARFLEETOP 0x00658890
void tChannelGroup::mDump()
{
    if (atgSoundPart->mAreWeInInterruptMode() == kTrue)
    {
        AddSoundErrorDuringInterrupt("Dumping tChannelGorup -- fDepth=%ld, fLinkDepth=%ld, fStopIfChannelIsNeeded = %ld, fChannelInfos=%ld,fNumberOfOwners=%ld",
            field_08, field_14, field_18, field_1c, field_24);
    }
    else
    {
        PrintOutBufferedErrors();
        gSoundSay.mJustSayIt("Dumping tChannelGorup -- fDepth=%ld, fLinkDepth=%ld, fStopIfChannelIsNeeded = %ld, fChannelInfos=%ld,fNumberOfOwners=%ld",
            field_08, field_14, field_18, field_1c, field_24);
    }
    // MATCH-ONLY: the original's location note is one macro expansion at line 107 of its source file, so both branches carry the same line; the macro is not recovered, so its expansion is written on one line at the original's line; the next #line restores the file's own numbering.
#line 107
    if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 170

    long i;
    for (i = 0; i < field_14; i++)
    {
        field_20[i].vfn_68();
    }
    for (i = 0; i < field_08 * field_14; i++)
    {
        field_1c[i].vfn_74();
    }
}

// FUNCTION: STARFLEETOP 0x006589f0
void tChannelGroup::mClose()
{
    if (field_24 > 0)
    {
        if (atgSoundPart->mAreWeInInterruptMode() == kTrue)
        {
            AddSoundErrorDuringInterrupt("Warning: Closing while we still are owned!");
        }
        else
        {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt("Warning: Closing while we still are owned!");
        }
        // MATCH-ONLY: as in mDump above, the location note is one macro expansion whose
        // line is the original's.
#line 204
        if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 200
    }
    mUnload(-1);
    if (field_20 != 0)
    {
        delete [] field_20;
        field_20 = 0;
    }
    if (field_1c != 0)
    {
        delete [] field_1c;
        field_1c = 0;
    }
    field_08 = 0;
    field_14 = 0;
}

// MATCH-ONLY: tgSound.h now declares tSoundChannelFormat; the lines are kept so line numbers stay.









// The compiler generates tSoundChannelFormat's vector deleting destructor (0x00658990) from
// this definition, with no source line of its own. It is emitted in this unit only where
// something here allocates an array of tSoundChannelFormat, which no function recovered so
// far does, so 0x00658990 is not claimed yet: when that function lands, stack
// `// FUNCTION: STARFLEETOP 0x00658990` on this definition.
tSoundChannelFormat::~tSoundChannelFormat()
{
}

// MATCH-ONLY: tgSound.h now declares tSoundPart; the lines are kept so line numbers stay.















// FUNCTION: STARFLEETOP 0x00658ac0
void tChannelGroup::mUnload(long param_1)
{
    long i;
    long j;
    long index;
    long channel;

    if (param_1 < 0)
    {
        long offset = 0;
        for (i = 0; i < field_08; i++, offset += field_14)
        {
            for (j = 0; j < field_14; j++)
            {
                index = offset + j;
                if (index >= 0 && field_1c != 0)
                {
                    channel = field_1c[index].vfn_7c();
                    if (channel != -1)
                    {
                        BindPart(tSoundPart)->mRemoveSoundChannel(channel);
                        field_1c[index].vfn_90(-1);
                    }
                }
            }
        }
    }
    else
    {
        if (param_1 > field_08)
        {
            if (atgSoundPart->mAreWeInInterruptMode() == kTrue)
            {
                AddSoundErrorDuringInterrupt("Bad Depth!");
            }
            else
            {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("Bad Depth!");
            }
            // MATCH-ONLY: as in mDump above, the location note is one macro expansion whose
            // line is the original's.
#line 274
            if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 297
        }
        long offset = field_14 * param_1;
        for (j = 0; j < field_14; j++)
        {
            index = offset + j;
            if (index >= 0 && field_1c != 0)
            {
                channel = field_1c[index].vfn_7c();
                if (channel != -1)
                {
                    BindPart(tSoundPart)->mRemoveSoundChannel(channel);
                    field_1c[index].vfn_90(-1);
                    if (atgSoundPart->mAreWeInInterruptMode() == kTrue)
                    {
                        AddSoundErrorDuringInterrupt("Clearing %ld from index %ld", channel, index);
                    }
                    else
                    {
                        PrintOutBufferedErrors();
                        gSoundSay.mJustSayIt("Clearing %ld from index %ld", channel, index);
                    }
#line 285
                    if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 321
                }
            }
        }
    }
}

// FUNCTION: STARFLEETOP 0x00658e30
void tChannelGroup::mLoad(long param_1)
{
    long i;
    long j;
    long index;
    long channel;

    if (param_1 == -1)
    {
        if (field_20 == 0)
        {
            if (atgSoundPart->mAreWeInInterruptMode() == kTrue)
            {
                AddSoundErrorDuringInterrupt("trying to load a channel group before we have formats!");
            }
            else
            {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("trying to load a channel group before we have formats!");
            }
#line 362
            if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 351
            return;
        }
        for (j = 0; j < field_14; j++)
        {
            if (field_1c[j].vfn_7c() == -1)
            {
                channel = BindPart(tSoundPart)->mAddSoundChannel((tSoundChannelFormat*)&field_20[j]);
                field_1c[j].vfn_90(channel);
            }
        }
        long offset = field_14;
        for (i = 1; i < field_08; i++, offset += field_14)
        {
            for (j = 0; j < field_14; j++)
            {
                index = offset + j;
                if (index < 0 || field_1c == 0 || field_1c[index].vfn_7c() == -1)
                {
                    channel = BindPart(tSoundPart)->mAddSoundChannel((tSoundChannelFormat*)&field_20[j]);
                    field_1c[index].vfn_90(channel);
                }
            }
        }
    }
    else if (param_1 == 0)
    {
        if (field_20 == 0)
        {
            return;
        }
        for (j = 0; j < field_14; j++)
        {
            if (field_1c[j].vfn_7c() == -1)
            {
                channel = BindPart(tSoundPart)->mAddSoundChannel((tSoundChannelFormat*)&field_20[j]);
                field_1c[j].vfn_90(channel);
            }
        }
    }
    else
    {
        if (field_20 == 0)
        {
            if (atgSoundPart->mAreWeInInterruptMode() == kTrue)
            {
                AddSoundErrorDuringInterrupt("trying to load a channel group before we have formats!");
            }
            else
            {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("trying to load a channel group before we have formats!");
            }
#line 413
            if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 406
            return;
        }
        for (j = 0; j < field_14; j++)
        {
            if (field_1c[j].vfn_7c() == -1)
            {
                channel = BindPart(tSoundPart)->mAddSoundChannel((tSoundChannelFormat*)&field_20[j]);
                field_1c[j].vfn_90(channel);
            }
        }
        long offset = field_14 * param_1;
        for (j = 0; j < field_14; j++)
        {
            if (field_1c[offset + j].vfn_7c() == -1)
            {
                field_1c[offset + j].vfn_90(BindPart(tSoundPart)->mAddSoundChannel((tSoundChannelFormat*)&field_20[j]));
            }
        }
    }
}

// FUNCTION: STARFLEETOP 0x00659470
long tChannelGroup::mGetChannelNumberFromIndex(long param_1)
{
    if (param_1 >= 0 && field_1c != 0)
    {
        return field_1c[param_1].vfn_7c();
    }
    return -1;
}

// HYPOTHESIS: declared inline; mDump(unsigned long, long) above inlines it, and the
// out-of-line copy is emitted for a caller in this unit that is not recovered yet.
// FUNCTION: STARFLEETOP 0x006594a0
long tChannelGroup::mGetIndex(unsigned long param_1, long param_2)
{
    if (field_1c != 0)
    {
        if (param_2 < 0)
        {
            param_2 = 0;
        }
        long offset = 0;
        for (long i = 0; i < field_08; i++, offset += field_14)
        {
            if (field_1c[offset + param_2].vfn_8c() == param_1 &&
                field_1c[offset + param_2].vfn_a4() == 1)
            {
                return offset + param_2;
            }
        }
    }
    return -1;
}

// The out-of-line copies of mGetIndex and mGetChannelNumberFromIndex are emitted for
// mPlayChannel below, which calls each of them once without inlining it.

// FUNCTION: STARFLEETOP 0x00659530
long tChannelGroup::mAddDuplicate(unsigned long param_1)
{
    // HYPOTHESIS: duplicates are not supported in this build; the id is ignored.
    field_10 = 0;
    return -1;
}

// MATCH-ONLY: Tiger/tAudioFile.h now declares tAudioFileInfoPart; the lines are kept so line numbers stay.
#include "unsorted/tChannelGroupManagerPart.h"
#include "Tiger/tAudioFile.h"











// FUNCTION: STARFLEETOP 0x00659540
unsigned char tChannelGroup::mAttachFileToChannel(unsigned long param_1, long param_2)
{
    unsigned char result = 1;
    long depth = mGetIndex(param_1, 0) / field_14;

    if (field_20->vfn_64() == 1 && field_20->vfn_44() == 1 && depth < field_08)
    {
        return mAttachFileToBaseGroup(param_2);
    }
    for (long i = 0; i < field_14; i++)
    {
        if (depth > 0 && field_20[i].vfn_44() == 1 && field_20[i].vfn_64() == 1)
        {
            long index = field_14 * depth + i;
            if (index >= 0 && field_1c != 0)
            {
                long channel = field_1c[index].vfn_7c();
                if (channel != -1)
                {
                    if (!BindPart(tAudioFileInfoPart)->vfn_48(param_2, channel, i))
                    {
                        result = 0;
                    }
                }
            }
        }
        else
        {
            long index = field_14 * depth + i;
            if (index >= 0 && field_1c != 0)
            {
                long channel = field_1c[index].vfn_7c();
                if (channel != -1)
                {
                    if (!BindPart(tAudioFileInfoPart)->vfn_48(param_2, channel, i))
                    {
                        result = 0;
                    }
                }
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x006597f0
unsigned char tChannelGroup::mAttachFileToBaseGroup(long param_1)
{
    unsigned char result = 1;

    for (long depth = 0; depth < field_08; depth++)
    {
        for (long i = 0; i < field_14; i++)
        {
            if (depth > 0 && field_20[i].vfn_44() == 1 && field_20[i].vfn_64() == 1)
            {
                long index = field_14 * depth + i;
                if (index >= 0 && field_1c != 0)
                {
                    long channel = field_1c[index].vfn_7c();
                    if (channel != -1)
                    {
                        if (!BindPart(tAudioFileInfoPart)->vfn_48(param_1, channel, i))
                        {
                            result = 0;
                        }
                    }
                }
            }
            else
            {
                long index = field_14 * depth + i;
                if (index >= 0 && field_1c != 0)
                {
                    long channel = field_1c[index].vfn_7c();
                    if (channel != -1)
                    {
                        if (!BindPart(tAudioFileInfoPart)->vfn_48(param_1, channel, i))
                        {
                            result = 0;
                        }
                    }
                }
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x00659a20
unsigned char tChannelGroup::mKillRetainedMode()
{
    if (field_20 != 0)
    {
        long i;
        for (i = 0; i < field_14; i++)
        {
            if (field_20[i].vfn_64() == 1)
            {
                // HYPOTHESIS: 0x800 is the retained-mode flag of the link
                long flags = field_20[i].vfn_2c() - 0x800;
                field_20[i].vfn_28(flags);
            }
        }
        for (long depth = 0; depth < field_08; depth++)
        {
            for (i = 0; i < field_14; i++)
            {
                long index = field_14 * depth + i;
                if (index >= 0 && field_1c != 0)
                {
                    long channel = field_1c[index].vfn_7c();
                    if (channel != -1)
                    {
                        BindPart(tSoundPart)->mWeDoNotRetainSelections(channel);
                    }
                }
            }
        }
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x00659b80
unsigned char tChannelGroup::mClearAttachments()
{
    unsigned char result = 1;

    for (long depth = 0; depth < field_08; depth++)
    {
        for (long i = 0; i < field_14; i++)
        {
            long index = field_14 * depth + i;
            if (index >= 0 && field_1c != 0)
            {
                long channel = field_1c[index].vfn_7c();
                if (channel != -1)
                {
                    if (!BindPart(tSoundPart)->mStopAndClearChannel(channel))
                    {
                        result = 0;
                    }
                }
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x00659ca0
long tChannelGroup::mGetAvailableChannel(unsigned long param_1)
{
    long depth;
    long i;
    long j;

    if (field_1c == 0)
    {
        return -1;
    }

    unsigned char ranked = 1;
    for (i = 0; i < field_14; i++)
    {
        if (!field_20[i].vfn_64() || field_20[i].vfn_3c() == 1)
        {
            i = field_14;
            ranked = 0;
        }
    }

    long lowest = 0x20;
    if (ranked == 1)
    {
        long index;
        for (depth = 0, index = 0; depth < field_08; depth++)
        {
            for (i = 0; i < field_14; i++, index++)
            {
                if (field_1c[index].vfn_7c() == -1)
                {
                    mLoad(depth);
                }
                if (!BindPart(tSoundPart)->mIsChannelActive(field_1c[index].vfn_7c()))
                {
                    if (i == field_14 - 1)
                    {
                        long offset = field_14 * depth;
                        for (j = 0; j < field_14; j++)
                        {
                            field_1c[offset + j].vfn_9c(param_1);
                            field_1c[offset + j].vfn_b0(kTrue);
                        }
                        return depth * field_14;
                    }
                }
                else
                {
                    i = field_14;
                }
                if (depth == 0)
                {
                    long rank = BindPart(tSoundPart)->mGetChannelStatus(field_1c[index].vfn_7c());
                    if (rank < lowest)
                    {
                        lowest = rank;
                    }
                }
            }
        }
        if (lowest != -1)
        {
            return mAddDuplicate(param_1);
        }
    }
    else
    {
        long index;
        for (depth = 0, index = 0; depth < field_08; depth++, index = depth * field_14)
        {
            for (i = 0; i < field_14; i++, index++)
            {
                if (field_1c[index].vfn_7c() == -1)
                {
                    mLoad(depth);
                }
                if (!BindPart(tSoundPart)->mIsChannelActive(field_1c[index].vfn_7c()))
                {
                    if (i == field_14 - 1)
                    {
                        long offset = field_14 * depth;
                        for (j = 0; j < field_14; j++)
                        {
                            field_1c[offset + j].vfn_9c(param_1);
                            field_1c[offset + j].vfn_b0(kTrue);
                        }
                        return depth * field_14;
                    }
                }
                else
                {
                    i = field_14;
                }
            }
        }
        return mAddDuplicate(param_1);
    }
    return -1;
}

// FUNCTION: STARFLEETOP 0x0065a0f0
unsigned char tChannelGroup::mAttachSoundToChannel(unsigned long param_1, tSoundSelection *param_2, long param_3)
{
    if (param_3 >= 0)
    {
        return BindPart(tSoundPart)->mAttachSoundToChannel(mGetChannelNumberFromIndex(mGetIndex(param_1, param_3)), param_2);
    }

    unsigned char result = 1;
    for (long i = 0; i < field_14; i++)
    {
        if (!BindPart(tSoundPart)->mAttachSoundToChannel(mGetChannelNumberFromIndex(mGetIndex(param_1, i)), param_2))
        {
            result = 0;
        }
    }
    return result;
}

// MATCH-ONLY: the original leaves two of the helper calls out of line (mGetChannelNumberFromIndex
// in the single-link branch, the second mGetIndex in the loop). The SDK's Tiger library was built
// with auto-inlining (/Ob2), whose limits left them there; the unit's profile inlines every inline
// call, so those two are made through pointers to member, which the compiler folds to direct calls.
// FUNCTION: STARFLEETOP 0x0065a370
unsigned char tChannelGroup::mPlayChannel(unsigned long param_1, long param_2)
{
    if (param_2 >= 0)
    {
        long index = mGetIndex(param_1, param_2);
        if (index < 0)
        {
            if (atgSoundPart->mAreWeInInterruptMode() == kTrue)
            {
                AddSoundErrorDuringInterrupt("Bad tracking Number! %ld", param_1);
            }
            else
            {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("Bad tracking Number! %ld", param_1);
            }
            // MATCH-ONLY: as in mDump above, the location note is one macro expansion whose
            // line is the original's.
#line 949
            if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 782
            return 0;
        }
        field_1c[index].vfn_b0(kTrue);
        long (tChannelGroup::*getChannel)(long) = &tChannelGroup::mGetChannelNumberFromIndex;
        return BindPart(tSoundPart)->mPlayChannel((this->*getChannel)(mGetIndex(param_1, param_2)));
    }

    unsigned char result = 1;
    for (long i = 0; i < field_14; i++)
    {
        long index = mGetIndex(param_1, i);
        if (index < 0)
        {
            if (atgSoundPart->mAreWeInInterruptMode() == kTrue)
            {
                AddSoundErrorDuringInterrupt("Bad tracking Number! %ld", param_1);
            }
            else
            {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("Bad tracking Number! %ld", param_1);
            }
#line 964
            if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 807
            result = 0;
        }
        else
        {
            field_1c[index].vfn_b0(kTrue);
            long (tChannelGroup::*getIndex)(unsigned long, long) = &tChannelGroup::mGetIndex;
            long channel = mGetChannelNumberFromIndex((this->*getIndex)(param_1, i));
            if (!BindPart(tSoundPart)->mPlayChannel(channel))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065a7c0
unsigned char tChannelGroup::mPauseChannel(unsigned long param_1, long param_2)
{
    if (param_2 >= 0)
    {
        return BindPart(tSoundPart)->mPauseChannel(mGetChannelNumberFromIndex(mGetIndex(param_1, param_2)));
    }

    unsigned char result = 1;
    for (long i = 0; i < field_14; i++)
    {
        if (!BindPart(tSoundPart)->mPauseChannel(mGetChannelNumberFromIndex(mGetIndex(param_1, i))))
        {
            result = 0;
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065b0d0
unsigned char tChannelGroup::mSetFrequency(unsigned long param_1, long param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            field_1c[index].vfn_08(param_2);
            return BindPart(tSoundPart)->mSetFrequency(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_08(param_2);
            if (!BindPart(tSoundPart)->mSetFrequency(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065b390
unsigned char tChannelGroup::mSetPan(unsigned long param_1, long param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            field_1c[index].vfn_10(param_2);
            return BindPart(tSoundPart)->mSetPan(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_10(param_2);
            if (!BindPart(tSoundPart)->mSetPan(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065b650
unsigned char tChannelGroup::mSetVolume(unsigned long param_1, long param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            field_1c[index].vfn_18(param_2);
            return BindPart(tSoundPart)->mSetVolume(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_18(param_2);
            if (!BindPart(tSoundPart)->mSetVolume(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065b910
unsigned char tChannelGroup::mSetPriority(unsigned long param_1, long param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            field_1c[index].vfn_94(param_2);
            return BindPart(tSoundPart)->mSetPriority(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_94(param_2);
            if (!BindPart(tSoundPart)->mSetPriority(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065bbd0
unsigned char tChannelGroup::mSetFinishFunction(unsigned long param_1, void (* const param_2)(unsigned long, unsigned char), long param_3, long param_4, unsigned char param_5)
{
    if (param_4 >= 0)
    {
        long index = mGetIndex(param_1, param_4);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mSetFinishFunction(mGetChannelNumberFromIndex(index), param_2, param_3, param_5);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            if (!BindPart(tSoundPart)->mSetFinishFunction(mGetChannelNumberFromIndex(index), param_2, param_3, param_5))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065be70
unsigned char tChannelGroup::mSetChannelAttributes(unsigned long param_1, tChannelAttributes *param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            field_1c[index].vfn_a0(param_2);
            return BindPart(tSoundPart)->mSetChannelAttributes(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_a0(param_2);
            if (!BindPart(tSoundPart)->mSetChannelAttributes(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065c130
unsigned char tChannelGroup::mSet3DSoundProfile(unsigned long param_1, t3DSoundProfile *param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            field_1c[index].vfn_20(param_2);
            return BindPart(tSoundPart)->mSet3DSoundProfile(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_20(param_2);
            if (!BindPart(tSoundPart)->mSet3DSoundProfile(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065c3f0
unsigned char tChannelGroup::mSet3DSoundPosition(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            field_1c[index].vfn_30(param_2);
            return BindPart(tSoundPart)->mSet3DSoundPosition(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_30(param_2);
            if (!BindPart(tSoundPart)->mSet3DSoundPosition(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065c6b0
unsigned char tChannelGroup::mSet3DSoundOrientation(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            field_1c[index].vfn_40(param_2);
            return BindPart(tSoundPart)->mSet3DSoundOrientation(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_40(param_2);
            if (!BindPart(tSoundPart)->mSet3DSoundOrientation(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065c970
unsigned char tChannelGroup::mSet3DSoundVelocity(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            field_1c[index].vfn_50(param_2);
            return BindPart(tSoundPart)->mSet3DSoundVelocity(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_50(param_2);
            if (!BindPart(tSoundPart)->mSet3DSoundVelocity(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065cc30
unsigned char tChannelGroup::mSet3DSoundCones(unsigned long param_1, float param_2, long param_3, long param_4, unsigned char param_5)
{
    if (param_4 >= 0)
    {
        long index = mGetIndex(param_1, param_4);
        if (index >= 0)
        {
            field_1c[index].vfn_70(param_2, param_3);
            return BindPart(tSoundPart)->mSet3DSoundCones(mGetChannelNumberFromIndex(index), param_2, param_3, param_5);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_70(param_2, param_3);
            if (!BindPart(tSoundPart)->mSet3DSoundCones(mGetChannelNumberFromIndex(index), param_2, param_3, param_5))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065cf10
unsigned char tChannelGroup::mSet3DSoundReferenceDistance(unsigned long param_1, double param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            field_1c[index].vfn_58(param_2);
            return BindPart(tSoundPart)->mSet3DSoundReferenceDistance(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            field_1c[index].vfn_58(param_2);
            if (!BindPart(tSoundPart)->mSet3DSoundReferenceDistance(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065d1f0
long tChannelGroup::mGetFrequency(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_0c();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d2a0
long tChannelGroup::mGetPan(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_14();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d350
long tChannelGroup::mGetVolume(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_1c();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d400
tChannelAttributes *tChannelGroup::mGetChannelAttributes(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_80();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d4b0
t3DSoundProfile *tChannelGroup::mGet3DSoundProfile(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_24();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d560
tPoint3 *tChannelGroup::mGet3DSoundPosition(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_34();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d610
tPoint3 *tChannelGroup::mGet3DSoundOrientation(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_44();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d6c0
tPoint3 *tChannelGroup::mGet3DSoundVelocity(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_54();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d770
double tChannelGroup::mGet3DSoundReferenceDistance(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_5c();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d820
float tChannelGroup::mGet3DSoundConeAngleForNoAttenuation(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_64();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d8d0
long tChannelGroup::mGet3DSoundConeAttenuationLevel(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_6c();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065d980
long tChannelGroup::mGetChannelPriority(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return field_1c[index].vfn_84();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065da30
eSoundChannelStatus tChannelGroup::mGetChannelStatus(unsigned long param_1, long param_2)
{
    long status = -1;
    if (param_2 >= 0)
    {
        long index = mGetIndex(param_1, param_2);
        if (index >= 0)
        {
            return (eSoundChannelStatus)BindPart(tSoundPart)->mGetChannelStatus(mGetChannelNumberFromIndex(index));
        }
    }
    else
    {
        long index = mGetIndex(param_1, 0);
        for (long i = 0; i < field_14; i++, index++)
        {
            if (index >= 0)
            {
                long current = BindPart(tSoundPart)->mGetChannelStatus(mGetChannelNumberFromIndex(index));
                if (current > status)
                {
                    status = current;
                }
            }
        }
    }
    return (eSoundChannelStatus)status;
}

// FUNCTION: STARFLEETOP 0x0065dcb0
long tChannelGroup::mTotalBytesLeftToPlay(unsigned long param_1, long param_2)
{
    long total = 0;
    if (param_2 >= 0)
    {
        long index = mGetIndex(param_1, param_2);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mTotalBytesLeftToPlay(mGetChannelNumberFromIndex(index));
        }
    }
    else
    {
        long index = mGetIndex(param_1, 0);
        for (long i = 0; i < field_14; i++, index++)
        {
            if (index >= 0)
            {
                long current = BindPart(tSoundPart)->mTotalBytesLeftToPlay(mGetChannelNumberFromIndex(index));
                if (current > total)
                {
                    total = current;
                }
            }
        }
    }
    return total;
}

// FUNCTION: STARFLEETOP 0x0065df30
double tChannelGroup::mTotalNumberOfSecondsLeftToPlay(unsigned long param_1, long param_2)
{
    double total = 0;
    if (param_2 >= 0)
    {
        long index = mGetIndex(param_1, param_2);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mTotalNumberOfSecondsLeftToPlay(mGetChannelNumberFromIndex(index));
        }
    }
    else
    {
        long index = mGetIndex(param_1, 0);
        for (long i = 0; i < field_14; i++, index++)
        {
            if (index >= 0)
            {
                double current = BindPart(tSoundPart)->mTotalNumberOfSecondsLeftToPlay(mGetChannelNumberFromIndex(index));
                if (current > total)
                {
                    total = current;
                }
            }
        }
    }
    return total;
}

// FUNCTION: STARFLEETOP 0x0065e1d0
long tChannelGroup::mNumberOfAttachmentsQueuedUp(unsigned long param_1, long param_2)
{
    long total = 0;
    if (param_2 >= 0)
    {
        long index = mGetIndex(param_1, param_2);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mNumberOfAttachmentsQueuedUp(mGetChannelNumberFromIndex(index));
        }
    }
    else
    {
        long index = mGetIndex(param_1, 0);
        for (long i = 0; i < field_14; i++, index++)
        {
            if (index >= 0)
            {
                long current = BindPart(tSoundPart)->mNumberOfAttachmentsQueuedUp(mGetChannelNumberFromIndex(index));
                if (current > total)
                {
                    total = current;
                }
            }
        }
    }
    return total;
}

// FUNCTION: STARFLEETOP 0x0065e450
tSoundChannelFormat *tChannelGroup::mGetSoundChannelFormat(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mGetSoundChannelFormat(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065e580
unsigned char tChannelGroup::mDoWeRetainSelections(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mDoWeRetainSelections(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065e6b0
unsigned char tChannelGroup::mAreWeLooping(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mAreWeLooping(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065e7e0
unsigned char tChannelGroup::mAreWeStreaming(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mAreWeStreaming(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065e910
unsigned char tChannelGroup::mAreWe3D(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mAreWe3D(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065ea40
unsigned char tChannelGroup::mAreWeStatic(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mAreWeStatic(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065eb70
unsigned char tChannelGroup::mAreWeFixedFrequency(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mAreWeFixedFrequency(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065eca0
unsigned char tChannelGroup::mAreWeARecordingChannel(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mAreWeARecordingChannel(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065edd0
unsigned char tChannelGroup::mTryToLocateInHardware(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mTryToLocateInHardware(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065ef00
unsigned char tChannelGroup::mTryToLocateInSoftware(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mTryToLocateInSoftware(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065f030
unsigned char tChannelGroup::mAreWeFixedPan(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mAreWeFixedPan(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065f160
unsigned char tChannelGroup::mAreWeFixedVolume(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mAreWeFixedVolume(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065f290
unsigned char tChannelGroup::mAreWeAGlobalSound(unsigned long param_1, long param_2)
{
    if (param_2 < 0)
    {
        param_2 = 0;
    }
    long index = mGetIndex(param_1, param_2);
    if (index >= 0)
    {
        return BindPart(tSoundPart)->mAreWeAGlobalSound(mGetChannelNumberFromIndex(index));
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0065f3c0
unsigned char tChannelGroup::mAddUpdateSoundPositionFunction(unsigned long param_1, void (*param_2)(unsigned long, long), long param_3, double param_4, long param_5, unsigned char param_6)
{
    if (param_5 >= 0)
    {
        long index = mGetIndex(param_1, param_5);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mAddUpdateSoundPositionFunction(mGetChannelNumberFromIndex(index), param_2, param_3, param_4, param_6);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            if (!BindPart(tSoundPart)->mAddUpdateSoundPositionFunction(mGetChannelNumberFromIndex(index), param_2, param_3, param_4, param_6))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065f680
unsigned char tChannelGroup::mAddDelayCommand(unsigned long param_1, double param_2, long param_3, unsigned char param_4)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mAddDelayCommand(mGetChannelNumberFromIndex(index), param_2, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            if (!BindPart(tSoundPart)->mAddDelayCommand(mGetChannelNumberFromIndex(index), param_2, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065f940
unsigned char tChannelGroup::mFadeToVolume(unsigned long param_1, long param_2, double param_3, long param_4, unsigned char param_5)
{
    if (param_4 >= 0)
    {
        long index = mGetIndex(param_1, param_4);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mFadeToVolume(mGetChannelNumberFromIndex(index), param_2, param_3, param_5);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            if (!BindPart(tSoundPart)->mFadeToVolume(mGetChannelNumberFromIndex(index), param_2, param_3, param_5))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065fbf0
unsigned char tChannelGroup::mFadeToStopAndResetNow(unsigned long param_1, double param_2, long param_3)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mFadeToStopAndResetNow(mGetChannelNumberFromIndex(index), param_2);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            if (!BindPart(tSoundPart)->mFadeToStopAndResetNow(mGetChannelNumberFromIndex(index), param_2))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0065fe90
unsigned char tChannelGroup::mFadeToStopAndClearNow(unsigned long param_1, double param_2, long param_3)
{
    if (param_3 >= 0)
    {
        long index = mGetIndex(param_1, param_3);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mFadeToStopAndClearNow(mGetChannelNumberFromIndex(index), param_2);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            if (!BindPart(tSoundPart)->mFadeToStopAndClearNow(mGetChannelNumberFromIndex(index), param_2))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x00660130
unsigned char tChannelGroup::mPlaySpecialLoop(unsigned long param_1, long param_2, long param_3, long param_4, long param_5, long param_6)
{
    if (param_6 >= 0)
    {
        long index = mGetIndex(param_1, param_6);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mPlaySpecialLoop(mGetChannelNumberFromIndex(index), param_2, param_3, param_4, param_5);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            if (!BindPart(tSoundPart)->mPlaySpecialLoop(mGetChannelNumberFromIndex(index), param_2, param_3, param_4, param_5))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x006603e0
unsigned char tChannelGroup::mEndSpecialLoop(unsigned long param_1, long param_2)
{
    if (param_2 >= 0)
    {
        long index = mGetIndex(param_1, param_2);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mEndSpecialLoop(mGetChannelNumberFromIndex(index));
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            if (!BindPart(tSoundPart)->mEndSpecialLoop(mGetChannelNumberFromIndex(index)))
            {
                result = 0;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x00660660
unsigned char tChannelGroup::mAddGetNextAttachmentFunction(unsigned long param_1, void (*param_2)(unsigned long, long), long param_3, double param_4, long param_5)
{
    if (param_5 >= 0)
    {
        long index = mGetIndex(param_1, param_5);
        if (index >= 0)
        {
            return BindPart(tSoundPart)->mAddGetNextAttachmentFunction(mGetChannelNumberFromIndex(index), param_2, param_3, param_4);
        }
        return 0;
    }

    unsigned char result = 1;
    long index = mGetIndex(param_1, 0);
    for (long i = 0; i < field_14; i++, index++)
    {
        if (index < 0)
        {
            result = 0;
        }
        else
        {
            if (!BindPart(tSoundPart)->mAddGetNextAttachmentFunction(mGetChannelNumberFromIndex(index), param_2, param_3, param_4))
            {
                result = 0;
            }
        }
    }
    return result;
}

inline tChannelGroup::~tChannelGroup()
{
    mClose();
}

#include "unsorted/tMemoryPart.h"


// FUNCTION: STARFLEETOP 0x00660910
tChannelGroupManagerPart::tChannelGroupManagerPart()
{
}

// FUNCTION: STARFLEETOP 0x00660950
// FUNCTION: STARFLEETOP 0x00660970
tChannelGroupManagerPart::~tChannelGroupManagerPart()
{
}

// HYPOTHESIS: the allocation lines belong to the array template's header in the original build
#define kArrayFile "D:\\work\\QSI\\Projects\\QSIInc\\Tiger\\tindexedarray.h"

// FUNCTION: STARFLEETOP 0x00660b20
unsigned long tChannelGroupManagerPart::mAddChannelGroup(tChannelGroup &aGroup)
{
    tSafeArray<tChannelGroup> &array = fChannelGroups;
    tMemoryPart *(*search)() = &tMemoryPart::Search_tMemoryPart; // MATCH-ONLY: keeps the part lookup out of line
    unsigned long cap;
    unsigned long count = array.f0c;
    cap = array.f10;
    unsigned long index = 0;
    if (count >= cap)
    {
        unsigned long newSize = array.f14 + cap;
        if (cap == 0)
        {
            array.f04 = (tChannelGroup **)(*search)()->mAllocate(newSize * 4, 713, kArrayFile);
            array.f08 = (unsigned short *)(*search)()->mAllocate(newSize * 2, 714, kArrayFile);
        }
        else
        {
            array.f04 = (tChannelGroup **)(*search)()->vfunction8((void *)array.f04, newSize * 4);
            array.f08 = (unsigned short *)(*search)()->vfunction8((void *)array.f08, newSize * 2);
        }
        while ((unsigned long)array.f10 < newSize)
        {
            ((long *)array.f04)[array.f10] = 0;
            ((short *)array.f08)[array.f10] = 1;
            array.f10++;
        }
        index = array.f0c;
    }
    else
    {
        long *items = (long *)array.f04;
        if (items[count] == 0)
        {
            index = count;
        }
        else
        {
            for (cap = 0; (long)cap < (long)count; cap++)
            {
                if (items[cap] == 0)
                {
                    index = cap;
                    cap = count;
                }
            }
        }
    }
    ((long *)array.f04)[index] = (long)&aGroup;
    array.f0c++;
    ((short *)array.f08)[index]++;
    if (((short *)array.f08)[index] == 0)
    {
        ((short *)array.f08)[index] = 1;
    }
    unsigned long handle = ((unsigned short *)array.f08)[index] * 0x10000 + index;
    aGroup.pad_04 = handle;
    return handle;
}

// FUNCTION: STARFLEETOP 0x00660c50
unsigned char tChannelGroupManagerPart::mDeleteChannelGroup(unsigned long aHandle)
{
    if (mGetChannelGroup(aHandle) == 0)
    {
        return 0;
    }
    fChannelGroups.mRemoveItemAtThisIndex(aHandle);
    return 1;
}

// FUNCTION: STARFLEETOP 0x00660cd0
unsigned char tChannelGroupManagerPart::mRemoveChannelGroupOwner(unsigned long aHandle)
{
    tChannelGroup *group = mGetChannelGroup(aHandle);
    if (group == 0)
    {
        return 0;
    }
    group->mDump(); // HYPOTHESIS: the exe calls the no-argument virtual at slot +4; fNumberOfOwners is tested after it
    if (group->field_24 == 0)
    {
        fChannelGroups.mRemoveItemAtThisIndex(aHandle);
    }
    return 1;
}

// FUNCTION: STARFLEETOP 0x00660d60
tBasicAudioFile *tAudioFileInfoPart::mGetBasicAudioFile(unsigned long aHandle)
{
    tSafeArray<tBasicAudioFile> &files = field_0c;
    if (files.mGetItemAtThisIndex(aHandle) == 0)
    {
        return 0;
    }
    return ((tBasicAudioFile **)files.f04)[aHandle & 0xffff];
}

// FUNCTION: STARFLEETOP 0x00660da0
iTruth tChannelGroupManagerPart::mIsReady()
{
    if (TestPart(tMemoryPart) == kTrue && TestPart(tSoundPart) == kTrue && TestPart(tAudioFileInfoPart) == kTrue)
        return kTrue;
    return kFalse;
}

// HYPOTHESIS: the body is the array's clean-up as its destructor inlines it, without the vtable store
// FUNCTION: STARFLEETOP 0x00660dc0
void tChannelGroupManagerPart::mDeactivate()
{
    tSafeArray<tChannelGroup> &array = fChannelGroups;
    if (array.f18 == kTrue)
    {
        for (long i = 0; array.f0c > 0 && i < array.f10; i++)
        {
            tChannelGroup *item = ((tChannelGroup **)array.f04)[i];
            if (item != 0)
            {
                delete item;
                ((tChannelGroup **)array.f04)[i] = 0;
                array.f0c--;
            }
        }
    }
    if (array.f04 != 0)
    {
        tMemoryPart::Search_tMemoryPart()->vfunction10((void *)array.f04);
        array.f04 = 0;
    }
    if (array.f08 != 0)
    {
        tMemoryPart::Search_tMemoryPart()->vfunction10((void *)array.f08);
        array.f08 = 0;
    }
    array.f0c = 0;
    array.f10 = 0;
}

// FUNCTION: STARFLEETOP 0x00660f30
unsigned char tChannelGroupManagerPart::mAttachFileToGroup(unsigned long aHandle, long param_2)
{
    tChannelGroup *group = mGetChannelGroup(aHandle);
    if (group != 0)
    {
        return group->mAttachFileToBaseGroup(param_2);
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x00660f60
unsigned char tChannelGroupManagerPart::mAttachFileToChannel(unsigned long aHandle, unsigned long param_2, long param_3)
{
    tChannelGroup *group = mGetChannelGroup(aHandle);
    if (group != 0)
    {
        return group->mAttachFileToChannel(param_2, param_3);
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x00660f90
unsigned char tChannelGroupManagerPart::mKillRetainedMode(unsigned long aHandle)
{
    tChannelGroup *group = mGetChannelGroup(aHandle);
    if (group != 0)
    {
        return group->mKillRetainedMode();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x00660fb0
unsigned char tChannelGroupManagerPart::mClearAttachments(unsigned long aHandle)
{
    tChannelGroup *group = mGetChannelGroup(aHandle);
    if (group != 0)
    {
        return group->mClearAttachments();
    }
    return 0;
}
