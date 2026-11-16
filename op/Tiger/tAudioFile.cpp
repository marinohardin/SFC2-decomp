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

// HYPOTHESIS: only the virtual destructor (slot 0) is known; the array deletes its items through it.
class tBasicAudioFile
{
public:
    virtual ~tBasicAudioFile();
};

template <class T>
inline tSafeArray<T>::~tSafeArray()
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
