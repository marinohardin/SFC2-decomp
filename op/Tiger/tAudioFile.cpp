// Tiger/tAudioFile.cpp

#include "Tiger/tAudioFile.h"

// HYPOTHESIS: tMemoryPart is declared with its part binding (claimed at 0x004133d3) inside another
// unit; it is restated here only as far as the array destructor needs it (slot 10, offset 0x28,
// frees a block).
class tMemoryPart : public tPart { public: tMemoryPart() {} GenAbstractBinding(tMemoryPart); virtual void vfunction6(); virtual void *mAllocate(int aSize, int aLine, const char *aFile); virtual void vfunction8(); virtual void vfunction9(); virtual void vfunction10(void *aBlock); };

// MATCH-ONLY: included by its bare name so that its __FILE__ is short enough for the harness to
// pair with the original's build path; the other includes are paths from the op/ root.
#include "tindexedarray.h"
#include "Tiger/tsound_fileio.h"

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
};

template <class T>
inline void tSafeArray<T>::fn_Clear()
{
    if (field_18 == 1)
    {
        for (long i = 0; field_0c > 0 && i < field_10; i++)
        {
            if (((T **)field_04)[i])
            {
                delete ((T **)field_04)[i];
                ((T **)field_04)[i] = 0;
                field_0c--;
            }
        }
    }
    if (field_04)
    {
        tMemoryPart *mem = BindPart(tMemoryPart);
        mem->vfunction10(field_04);
        field_04 = 0;
    }
    if (field_08)
    {
        tMemoryPart *mem = BindPart(tMemoryPart);
        mem->vfunction10((void *)field_08);
        field_08 = 0;
    }
    field_0c = 0;
    field_10 = 0;
}

template <class T>
inline tSafeArray<T>::~tSafeArray()
{
    fn_Clear();
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

// HYPOTHESIS: tAudioPriorityManagerPart is declared only as far as this function needs it; the
// exe's symbol places its mIsReady in this unit's address range. It waits for the memory part
// (Test_ at 0x0064fc00, see Tiger/tgSound.cpp).
class tAudioPriorityManagerPart : public tPart
{
public:
    virtual iTruth mIsReady();
};

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
    field_0c.fn_Clear();
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
