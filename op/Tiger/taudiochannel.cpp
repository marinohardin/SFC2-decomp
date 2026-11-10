// Tiger/taudiochannel.cpp

#include "Tiger/tsay.h"

// The original reaches these through tgSound.h, which has no unit header here yet.
extern void AddSoundErrorDuringInterrupt(const char *fmt, ...);
extern void PrintOutBufferedErrors(void);
extern tSay gSoundSay;

// HYPOTHESIS: only the virtual at vtable offset 0x1d0 is used here; it returns true while
// the sound part is inside its interrupt. Its name is not recovered.
class tgSoundPart
{
public:
    virtual void vfn_000(); virtual void vfn_004(); virtual void vfn_008(); virtual void vfn_00c(); virtual void vfn_010(); virtual void vfn_014(); virtual void vfn_018(); virtual void vfn_01c();
    virtual void vfn_020(); virtual void vfn_024(); virtual void vfn_028(); virtual void vfn_02c(); virtual void vfn_030(); virtual void vfn_034(); virtual void vfn_038(); virtual void vfn_03c();
    virtual void vfn_040(); virtual void vfn_044(); virtual void vfn_048(); virtual void vfn_04c(); virtual void vfn_050(); virtual void vfn_054(); virtual void vfn_058(); virtual void vfn_05c();
    virtual void vfn_060(); virtual void vfn_064(); virtual void vfn_068(); virtual void vfn_06c(); virtual void vfn_070(); virtual void vfn_074(); virtual void vfn_078(); virtual void vfn_07c();
    virtual void vfn_080(); virtual void vfn_084(); virtual void vfn_088(); virtual void vfn_08c(); virtual void vfn_090(); virtual void vfn_094(); virtual void vfn_098(); virtual void vfn_09c();
    virtual void vfn_0a0(); virtual void vfn_0a4(); virtual void vfn_0a8(); virtual void vfn_0ac(); virtual void vfn_0b0(); virtual void vfn_0b4(); virtual void vfn_0b8(); virtual void vfn_0bc();
    virtual void vfn_0c0(); virtual void vfn_0c4(); virtual void vfn_0c8(); virtual void vfn_0cc(); virtual void vfn_0d0(); virtual void vfn_0d4(); virtual void vfn_0d8(); virtual void vfn_0dc();
    virtual void vfn_0e0(); virtual void vfn_0e4(); virtual void vfn_0e8(); virtual void vfn_0ec(); virtual void vfn_0f0(); virtual void vfn_0f4(); virtual void vfn_0f8(); virtual void vfn_0fc();
    virtual void vfn_100(); virtual void vfn_104(); virtual void vfn_108(); virtual void vfn_10c(); virtual void vfn_110(); virtual void vfn_114(); virtual void vfn_118(); virtual void vfn_11c();
    virtual void vfn_120(); virtual void vfn_124(); virtual void vfn_128(); virtual void vfn_12c(); virtual void vfn_130(); virtual void vfn_134(); virtual void vfn_138(); virtual void vfn_13c();
    virtual void vfn_140(); virtual void vfn_144(); virtual void vfn_148(); virtual void vfn_14c(); virtual void vfn_150(); virtual void vfn_154(); virtual void vfn_158(); virtual void vfn_15c();
    virtual void vfn_160(); virtual void vfn_164(); virtual void vfn_168(); virtual void vfn_16c(); virtual void vfn_170(); virtual void vfn_174(); virtual void vfn_178(); virtual void vfn_17c();
    virtual void vfn_180(); virtual void vfn_184(); virtual void vfn_188(); virtual void vfn_18c(); virtual void vfn_190(); virtual void vfn_194(); virtual void vfn_198(); virtual void vfn_19c();
    virtual void vfn_1a0(); virtual void vfn_1a4(); virtual void vfn_1a8(); virtual void vfn_1ac(); virtual void vfn_1b0(); virtual void vfn_1b4(); virtual void vfn_1b8(); virtual void vfn_1bc();
    virtual void vfn_1c0(); virtual void vfn_1c4(); virtual void vfn_1c8(); virtual void vfn_1cc();
    virtual iTruth vfn_1d0();
};

extern tgSoundPart *atgSoundPart;

// HYPOTHESIS: the element type of the array at tChannelGroup +0x20 (0x18 bytes, with a
// vtable). Its class and the virtual at offset 0x68 are not named yet.
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
    if (atgSoundPart->vfn_1d0() == kTrue)
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
    if (atgSoundPart->vfn_1d0() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
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
        if (atgSoundPart->vfn_1d0() == kTrue)
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
        if (atgSoundPart->vfn_1d0() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
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

class tSoundChannelFormat
{
public:
    tSoundChannelFormat() {}
    virtual ~tSoundChannelFormat();
    virtual void mInit();

    char pad_04[0x14]; // HYPOTHESIS: fields not recovered; the array stride is 0x18
};

// The compiler generates tSoundChannelFormat's vector deleting destructor (0x00658990) from
// this definition, with no source line of its own. It is emitted in this unit only where
// something here allocates an array of tSoundChannelFormat, which no function recovered so
// far does, so 0x00658990 is not claimed yet: when that function lands, stack
// `// FUNCTION: STARFLEETOP 0x00658990` on this definition.
tSoundChannelFormat::~tSoundChannelFormat()
{
}

// HYPOTHESIS: the sound part bound by name through its part cache (tSoundPart's cache pointer
// is at 0x00c64794). Only the virtual at offset 0x2c is used here; its name is not recovered.
class tSoundPart : public tPart
{
public:
    GenAbstractBinding(tSoundPart);

    virtual void vfn_18(); virtual void vfn_1c();
    // HYPOTHESIS: loads a sound for a channel format and returns its channel; the argument is
    // an element of tChannelGroup's array at +0x20 (mLoad's error text calls them formats).
    virtual long vfn_20(fn_ChannelLink *param_1);
    virtual void vfn_24();
    virtual void vfn_28();
    virtual void vfn_2c(long param_1); /* HYPOTHESIS: releases the sound on a channel */ virtual void vfn_30(); virtual void vfn_34(); virtual unsigned char vfn_38(long param_1, tSoundSelection *param_2); /* HYPOTHESIS: attaches a sound selection to a channel; false on failure */ virtual unsigned char vfn_3c(long param_1); /* HYPOTHESIS: plays the sound on a channel; false on failure */ virtual unsigned char vfn_40(long param_1); /* HYPOTHESIS: pauses the sound on a channel; false on failure */ virtual void vfn_44(); virtual unsigned char vfn_48(long param_1); /* HYPOTHESIS: detaches the file on a channel; false on failure */ virtual void vfn_4c(); virtual void vfn_50(); virtual void vfn_54(); virtual void vfn_58(); virtual void vfn_5c(); virtual void vfn_60(); virtual void vfn_64(); virtual void vfn_68(); virtual void vfn_6c(); virtual void vfn_70(); virtual void vfn_74(); virtual void vfn_78(); virtual void vfn_7c(); virtual void vfn_80(); virtual void vfn_84(); virtual void vfn_88(); virtual void vfn_8c(); virtual void vfn_90(); virtual void vfn_94(); virtual void vfn_98(); virtual void vfn_9c(); virtual void vfn_a0(); virtual void vfn_a4(); virtual void vfn_a8(); virtual void vfn_ac(); virtual void vfn_b0(); virtual void vfn_b4(); virtual long vfn_b8(long param_1); /* HYPOTHESIS: a ranking value of the sound on a channel */ virtual void vfn_bc(); virtual void vfn_c0(); virtual void vfn_c4(); virtual void vfn_c8(); virtual void vfn_cc(); virtual void vfn_d0(); virtual void vfn_d4(); virtual void vfn_d8(); virtual void vfn_dc(); virtual void vfn_e0(); virtual void vfn_e4(); virtual void vfn_e8(); virtual void vfn_ec(); virtual void vfn_f0(); virtual void vfn_f4(); virtual void vfn_f8(); virtual void vfn_fc(); virtual void vfn_100(); virtual void vfn_104(); virtual void vfn_108(); virtual void vfn_10c(); virtual void vfn_110(); virtual void vfn_114(); virtual void vfn_118(); virtual void vfn_11c(); virtual void vfn_120(); virtual void vfn_124(); virtual void vfn_128(); virtual void vfn_12c(); virtual void vfn_130(); virtual void vfn_134(); virtual void vfn_138(); virtual void vfn_13c(); virtual void vfn_140(); virtual void vfn_144(); virtual void vfn_148(); virtual void vfn_14c(); virtual void vfn_150(); virtual void vfn_154(); virtual void vfn_158(); virtual void vfn_15c(); virtual void vfn_160(); virtual void vfn_164(); virtual void vfn_168(); virtual void vfn_16c(); virtual void vfn_170(); virtual void vfn_174(); virtual void vfn_178(); virtual void vfn_17c(); virtual void vfn_180(); virtual void vfn_184(); virtual void vfn_188(); virtual void vfn_18c(); virtual void vfn_190(long param_1); /* HYPOTHESIS: vfn_190 stops the sound on a channel */ virtual void vfn_194(); virtual void vfn_198(); virtual void vfn_19c(); virtual void vfn_1a0(); virtual void vfn_1a4(); virtual void vfn_1a8(); virtual unsigned char vfn_1ac(long param_1); // HYPOTHESIS: true while a channel is busy
};

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
                        BindPart(tSoundPart)->vfn_2c(channel);
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
            if (atgSoundPart->vfn_1d0() == kTrue)
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
            if (atgSoundPart->vfn_1d0() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
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
                    BindPart(tSoundPart)->vfn_2c(channel);
                    field_1c[index].vfn_90(-1);
                    if (atgSoundPart->vfn_1d0() == kTrue)
                    {
                        AddSoundErrorDuringInterrupt("Clearing %ld from index %ld", channel, index);
                    }
                    else
                    {
                        PrintOutBufferedErrors();
                        gSoundSay.mJustSayIt("Clearing %ld from index %ld", channel, index);
                    }
#line 285
                    if (atgSoundPart->vfn_1d0() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
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
            if (atgSoundPart->vfn_1d0() == kTrue)
            {
                AddSoundErrorDuringInterrupt("trying to load a channel group before we have formats!");
            }
            else
            {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("trying to load a channel group before we have formats!");
            }
#line 362
            if (atgSoundPart->vfn_1d0() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 351
            return;
        }
        for (j = 0; j < field_14; j++)
        {
            if (field_1c[j].vfn_7c() == -1)
            {
                channel = BindPart(tSoundPart)->vfn_20(&field_20[j]);
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
                    channel = BindPart(tSoundPart)->vfn_20(&field_20[j]);
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
                channel = BindPart(tSoundPart)->vfn_20(&field_20[j]);
                field_1c[j].vfn_90(channel);
            }
        }
    }
    else
    {
        if (field_20 == 0)
        {
            if (atgSoundPart->vfn_1d0() == kTrue)
            {
                AddSoundErrorDuringInterrupt("trying to load a channel group before we have formats!");
            }
            else
            {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("trying to load a channel group before we have formats!");
            }
#line 413
            if (atgSoundPart->vfn_1d0() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 406
            return;
        }
        for (j = 0; j < field_14; j++)
        {
            if (field_1c[j].vfn_7c() == -1)
            {
                channel = BindPart(tSoundPart)->vfn_20(&field_20[j]);
                field_1c[j].vfn_90(channel);
            }
        }
        long offset = field_14 * param_1;
        for (j = 0; j < field_14; j++)
        {
            if (field_1c[offset + j].vfn_7c() == -1)
            {
                field_1c[offset + j].vfn_90(BindPart(tSoundPart)->vfn_20(&field_20[j]));
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

// HYPOTHESIS: the audio file part bound by name through its part cache (at 0x00c669d0). Only
// the virtual at offset 0x48 is used here: it attaches a file to a channel for a link index
// and returns false on failure. Its name is not recovered.
class tAudioFileInfoPart : public tPart
{
public:
    GenAbstractBinding(tAudioFileInfoPart);

    virtual void vfn_18(); virtual void vfn_1c(); virtual void vfn_20(); virtual void vfn_24();
    virtual void vfn_28(); virtual void vfn_2c(); virtual void vfn_30(); virtual void vfn_34();
    virtual void vfn_38(); virtual void vfn_3c(); virtual void vfn_40(); virtual void vfn_44();
    virtual unsigned char vfn_48(long param_1, long param_2, long param_3);
};

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
                        BindPart(tSoundPart)->vfn_190(channel);
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
                    if (!BindPart(tSoundPart)->vfn_48(channel))
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
                if (!BindPart(tSoundPart)->vfn_1ac(field_1c[index].vfn_7c()))
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
                    long rank = BindPart(tSoundPart)->vfn_b8(field_1c[index].vfn_7c());
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
                if (!BindPart(tSoundPart)->vfn_1ac(field_1c[index].vfn_7c()))
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
        return BindPart(tSoundPart)->vfn_38(mGetChannelNumberFromIndex(mGetIndex(param_1, param_3)), param_2);
    }

    unsigned char result = 1;
    for (long i = 0; i < field_14; i++)
    {
        if (!BindPart(tSoundPart)->vfn_38(mGetChannelNumberFromIndex(mGetIndex(param_1, i)), param_2))
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
            if (atgSoundPart->vfn_1d0() == kTrue)
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
            if (atgSoundPart->vfn_1d0() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 782
            return 0;
        }
        field_1c[index].vfn_b0(kTrue);
        long (tChannelGroup::*getChannel)(long) = &tChannelGroup::mGetChannelNumberFromIndex;
        return BindPart(tSoundPart)->vfn_3c((this->*getChannel)(mGetIndex(param_1, param_2)));
    }

    unsigned char result = 1;
    for (long i = 0; i < field_14; i++)
    {
        long index = mGetIndex(param_1, i);
        if (index < 0)
        {
            if (atgSoundPart->vfn_1d0() == kTrue)
            {
                AddSoundErrorDuringInterrupt("Bad tracking Number! %ld", param_1);
            }
            else
            {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("Bad tracking Number! %ld", param_1);
            }
#line 964
            if (atgSoundPart->vfn_1d0() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 807
            result = 0;
        }
        else
        {
            field_1c[index].vfn_b0(kTrue);
            long (tChannelGroup::*getIndex)(unsigned long, long) = &tChannelGroup::mGetIndex;
            long channel = mGetChannelNumberFromIndex((this->*getIndex)(param_1, i));
            if (!BindPart(tSoundPart)->vfn_3c(channel))
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
        return BindPart(tSoundPart)->vfn_40(mGetChannelNumberFromIndex(mGetIndex(param_1, param_2)));
    }

    unsigned char result = 1;
    for (long i = 0; i < field_14; i++)
    {
        if (!BindPart(tSoundPart)->vfn_40(mGetChannelNumberFromIndex(mGetIndex(param_1, i))))
        {
            result = 0;
        }
    }
    return result;
}
