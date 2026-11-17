// Tiger/tAudioFile.cpp

#include "Tiger/tAudioFile.h"

// HYPOTHESIS: tMemoryPart is declared with its binding (claimed at 0x004133d3) in another unit;
// unsorted/tMemoryPart.h, which unsorted/tSafeArray.h includes, restates it for the Tiger units:
// slot 8 (offset 0x20) reallocates a block, slot 10 (offset 0x28) frees one.
#include "unsorted/tMemoryPart.h"

// MATCH-ONLY: included by its bare name so that its __FILE__ is short enough for the harness to
// pair with the original's build path; the other includes are paths from the op/ root.
#include "tindexedarray.h"
#include "Tiger/tsound_fileio.h"
#include "Tiger/tclock.h"

// HYPOTHESIS: the virtual destructor (slot 0) deletes the array's items; the virtual at offset 0x94
// attaches the file to a channel (see tAudioFileInfoPart::mAttachToThisChannel). The other slots'
// names are not recovered.
class tBasicAudioFile
{
public:
    virtual ~tBasicAudioFile();
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
    virtual void vfn_74();
    virtual void vfn_78();
    virtual void vfn_7c();
    virtual void vfn_80();
    virtual void vfn_84();
    virtual void vfn_88();
    virtual void vfn_8c();
    virtual void vfn_90();
    virtual unsigned char vfn_94(long param_1, long param_2);
    virtual long vfn_98(); // HYPOTHESIS: returns the file's sample count
    virtual long vfn_9c(); // HYPOTHESIS: returns the file's number of selections per sound
    virtual void vfn_a0();
    virtual long vfn_a4(); // HYPOTHESIS: returns the file's total size in bytes
    virtual long vfn_a8(); // HYPOTHESIS: returns the size of the file's sample data in bytes
    virtual long vfn_ac(); // HYPOTHESIS: returns the file's sample depth
    virtual long vfn_b0(); // HYPOTHESIS: returns the file's link depth
    virtual void vfn_b4();
    virtual void vfn_b8(unsigned long param_1); // HYPOTHESIS: stores the file's handle
};

template <class T>
inline T *tSafeArray<T>::fn_Get(unsigned long aHandle)
{
    unsigned long index = aHandle & 0xffff;
    if (index < (unsigned long) f10 && (short) (aHandle >> 16) == ((short *) f08)[index])
        return ((T **) f04)[index];
    return 0;
}

template <class T>
inline void tSafeArray<T>::fn_Remove(unsigned long aHandle)
{
    if (fn_Get(aHandle))
    {
        delete ((T **) f04)[aHandle & 0xffff];
        ((T **) f04)[aHandle & 0xffff] = 0;
        f0c--;
    }
}

template <class T>
inline tSafeArray<T>::~tSafeArray()
{
    mDeleteAll();
}

// FUNCTION: STARFLEETOP 0x00657770
tAudioFileInfoPart::tAudioFileInfoPart()
{
    field_28 = 0;
    field_2c = 0;
}

// FUNCTION: STARFLEETOP 0x006577d0
tAudioFileInfoPart::~tAudioFileInfoPart()
{
}

// The exe's symbol places tAudioPriorityManagerPart's mIsReady in this unit's address range. It
// waits for the memory part (Test_ at 0x0064fc00, see Tiger/tgSound.cpp).
#include "Tiger/taudiopriority.h"





// FUNCTION: STARFLEETOP 0x00657970
iTruth tAudioPriorityManagerPart::mIsReady()
{
    if (TestPart(tMemoryPart) == kTrue)
        return kTrue;
    return kFalse;
}

// HYPOTHESIS: a 16-byte item of the asynchronous read list; its fields are not recovered.
struct tAsynchReadInfo
{
    long field_00;
    long field_04;
    long field_08;
    long field_0c;
};

// FUNCTION: STARFLEETOP 0x00657980
void tAudioFileInfoPart::mActivate()
{
    _InitSoundFileIO();
    field_2c = new tStandardMemoryArray<tAsynchReadInfo>(0x80);
}

// FUNCTION: STARFLEETOP 0x00657b60
void tAudioFileInfoPart::mDeactivate()
{
    field_0c.mDeleteAll();
    if (field_2c)
    {
        delete field_2c;
        field_2c = 0;
    }
    _DoneSoundFileIO();
}

// FUNCTION: STARFLEETOP 0x00657cf0
unsigned char tAudioFileInfoPart::vfn_48(long param_1, long param_2, long param_3)
{
    tBasicAudioFile *file = mGetBasicAudioFile(param_1);
    if (file)
        return file->vfn_94(param_2, param_3);
    return 0;
}

// FUNCTION: STARFLEETOP 0x00657d20
long tAudioFileInfoPart::mGetNumberOfSamples(unsigned long param_1)
{
    tBasicAudioFile *file = mGetBasicAudioFile(param_1);
    if (file)
        return file->vfn_98();
    return 0;
}

// FUNCTION: STARFLEETOP 0x00657d40
long tAudioFileInfoPart::mGetNumberOfSelectionPerSound(unsigned long param_1)
{
    tBasicAudioFile *file = mGetBasicAudioFile(param_1);
    if (file)
        return file->vfn_9c();
    return 0;
}

// FUNCTION: STARFLEETOP 0x00657d60
long tAudioFileInfoPart::mGetTotalSizeInBytes(unsigned long param_1)
{
    tBasicAudioFile *file = mGetBasicAudioFile(param_1);
    if (file)
        return file->vfn_a4();
    return 0;
}

// FUNCTION: STARFLEETOP 0x00657d80
long tAudioFileInfoPart::mGetTotalDataSizeInBytes(unsigned long param_1)
{
    tBasicAudioFile *file = mGetBasicAudioFile(param_1);
    if (file)
        return file->vfn_a8();
    return 0;
}

// FUNCTION: STARFLEETOP 0x00657da0
long tAudioFileInfoPart::mGetDepth(unsigned long param_1)
{
    tBasicAudioFile *file = mGetBasicAudioFile(param_1);
    if (file)
        return file->vfn_ac();
    return 0;
}

// FUNCTION: STARFLEETOP 0x00657dc0
long tAudioFileInfoPart::mGetLinkDepth(unsigned long param_1)
{
    tBasicAudioFile *file = mGetBasicAudioFile(param_1);
    if (file)
        return file->vfn_b0();
    return 0;
}

// FUNCTION: STARFLEETOP 0x00657de0
unsigned long tAudioFileInfoPart::mAddSoundFile(tBasicAudioFile &aFile)
{
    unsigned long handle = field_0c.fn_Add(&aFile);
    aFile.vfn_b8(handle);
    return handle;
}

// FUNCTION: STARFLEETOP 0x00658070
void tAudioFileInfoPart::mDeleteSoundFile(unsigned long param_1)
{
    field_0c.fn_Remove(param_1);
}

// FUNCTION: STARFLEETOP 0x006580c0
long tAudioFileInfoPart::mGetNewAsynchReadInfo(unsigned long param_1, long param_2)
{
    long index = field_2c->fn_New();
    tAsynchReadInfo *info = field_2c->fn_Get(index);
    info->field_00 = param_2;
    info->field_04 = param_1;
    *(double *) &info->field_08 = aClockPart->mGetMicroseconds();
    return index;
}

// FUNCTION: STARFLEETOP 0x00658290
tAsynchReadInfo *tAudioFileInfoPart::mGetAsynchReadInfo(long param_1)
{
    return field_2c->fn_Get(param_1);
}

// HYPOTHESIS: statistics of the asynchronous read times (0x00c66968 count, 0x00c66970 longest,
// 0x00c669c0 total, 0x00c669c8 average); their names are not recovered.
static long g_00c66968;
static double g_00c66970;
static double g_00c669c0;
static double g_00c669c8;

// FUNCTION: STARFLEETOP 0x006582c0
void tAudioFileInfoPart::mFreeAsynchReadInfo(long param_1)
{
    tAsynchReadInfo *info = mGetAsynchReadInfo(param_1);
    double elapsed = aClockPart->mGetMicroseconds() - *(double *) &info->field_08;
    if (elapsed > g_00c66970)
        g_00c66970 = elapsed;
    g_00c66968++;
    g_00c669c0 += elapsed;
    g_00c669c8 = g_00c669c0 / g_00c66968;
    field_2c->fn_Free(param_1);
}
