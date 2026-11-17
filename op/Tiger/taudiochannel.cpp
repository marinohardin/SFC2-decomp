// Tiger/taudiochannel.cpp

#include "Tiger/tgSound.h"

// MATCH-ONLY: tgSound.h now declares what stood here; the lines are kept so line numbers stay.




// MATCH-ONLY: tgSound.h now declares tgSoundPart; the lines are kept so line numbers stay.
























// HYPOTHESIS: the element type of the array at tChannelGroup +0x20 (0x18 bytes, with a vtable;
// probably tSoundChannelFormat, which mAddSoundChannel takes). Not yet replaced by it.
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
// vtable). Its class name and the names of these virtuals are not recovered yet; they
// are named by vtable offset until evidence appears.
class fn_ChannelSlot
{
public:
    virtual ~fn_ChannelSlot(); // HYPOTHESIS: slot 0 is the destructor (mClose deletes the array)
    virtual void vfn_04();
    virtual void vfn_08();
    virtual void vfn_0c();
    virtual void vfn_10();
    virtual void vfn_14();
    virtual void vfn_18();
    virtual void vfn_1c();
    virtual void vfn_20();
    virtual void vfn_24();
    virtual void vfn_28();
    virtual void vfn_2c();
    virtual void vfn_30();
    virtual void vfn_34();
    virtual void vfn_38();
    virtual void vfn_3c();
    virtual void vfn_40();
    virtual void vfn_44();
    virtual void vfn_48();
    virtual void vfn_4c();
    virtual void vfn_50();
    virtual void vfn_54();
    virtual void vfn_58();
    virtual void vfn_5c();
    virtual void vfn_60();
    virtual void vfn_64();
    virtual void vfn_68();
    virtual void vfn_6c();
    virtual void vfn_70();
    virtual void vfn_74(); // HYPOTHESIS: mDump
    virtual void vfn_78();
    virtual long vfn_7c(); // HYPOTHESIS: returns the sound channel loaded in the slot, or -1
    virtual void vfn_80();
    virtual void vfn_84();
    virtual void vfn_88();
    virtual unsigned long vfn_8c(); // HYPOTHESIS: returns the channel's id
    virtual void vfn_90(long param_1); // HYPOTHESIS: sets the loaded sound channel
    virtual void vfn_94();
    virtual void vfn_98();
    virtual void vfn_9c(unsigned long param_1); // HYPOTHESIS: sets the channel's id
    virtual void vfn_a0();
    virtual unsigned char vfn_a4(); /* HYPOTHESIS: true when the slot is in use */ virtual void vfn_a8(); virtual void vfn_ac(); virtual void vfn_b0(iTruth param_1); // HYPOTHESIS: vfn_b0 marks the slot in use

    char pad_04[0x6c];
};

class tSoundSelection;
class tChannelGroup
{
public:
    // HYPOTHESIS: slot order; 0x10 is the one exported virtual with this signature;
    // mKillRetainedMode's slot is not recovered.
    virtual void mDump(unsigned long param_1, long param_2);
    virtual void mDump();
    virtual unsigned char mAttachFileToChannel(unsigned long param_1, long param_2);
    virtual void vfn_0c();
    virtual unsigned char mAttachFileToBaseGroup(long param_1);
    virtual unsigned char mKillRetainedMode();
    virtual unsigned char mClearAttachments();
    virtual unsigned char mAttachSoundToChannel(unsigned long param_1, tSoundSelection *param_2, long param_3);
    virtual unsigned char mPlayChannel(unsigned long param_1, long param_2);
    virtual unsigned char mPauseChannel(unsigned long param_1, long param_2);

    inline long mGetIndex(unsigned long param_1, long param_2);
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
    // MATCH-ONLY: the original's location note is one macro expansion at line 107 of its
    // source file, so both branches carry the same line; the macro is not recovered, so its
    // expansion is written on one line and the line is set to the original's. Each such
    // expansion in this file is the only line its #line renumbers: the next line sets the
    // numbering back to the file's own, so every annotation pairs by its own line.
#line 107
    if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 169

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

// MATCH-ONLY: Tiger/tAudioFile.h now declares tAudioFileInfoPart; the lines are kept so line
// numbers stay.
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
