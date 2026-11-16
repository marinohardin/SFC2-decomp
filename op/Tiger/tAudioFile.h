// Tiger/tAudioFile.h

#ifndef tAudioFile_h
#define tAudioFile_h

#include "Tiger/tpart.h"

class tBasicAudioFile;
struct tAsynchReadInfo;
template <class T> class tStandardMemoryArray;

// The name is in the exe's symbols (tSafeArray<tBasicAudioFile>, tSafeArray<tChannelGroup>,
// tSafeArray<tClassTrackingInfo>). HYPOTHESIS: field roles (item pointer, counts, grow step 0x40,
// a flag set to 1) are read from the inlined constructor only; their names are not recovered.
template <class T>
class tSafeArray
{
public:
    tSafeArray();
    virtual ~tSafeArray();

    // HYPOTHESIS: the destructor and tAudioFileInfoPart::mDeactivate (0x00657b60) share this
    // inlined clear (delete the items, free both blocks, zero the counts); a separate method is
    // what makes the counts' stores precede the next member's load. Its name is not recovered.
    void fn_Clear();
    // HYPOTHESIS: puts an item in a free slot (growing both blocks by the grow step when full),
    // bumps the slot's 16-bit use count and returns (count << 16) + slot. Inlined into
    // tAudioFileInfoPart::mAddSoundFile (0x00657de0); its name is not recovered.
    unsigned long fn_Add(T *aItem);
    // HYPOTHESIS: returns the item a handle names, or 0 when the handle is out of range or its
    // use count is stale. Its name is not recovered.
    T *fn_Get(unsigned long aHandle);
    // HYPOTHESIS: deletes the item a handle names when the handle's use count is current.
    // Inlined into tAudioFileInfoPart::mDeleteSoundFile (0x00658070); its name is not recovered.
    void fn_Remove(unsigned long aHandle);

    T *field_04;
    long field_08;
    unsigned long field_0c;
    long field_10;
    long field_14;
    unsigned char field_18;
};

template <class T>
inline tSafeArray<T>::tSafeArray()
{
    field_18 = 1;
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
    field_10 = 0;
    field_14 = 0x40;
}

// HYPOTHESIS: the audio file part, bound by name through its part cache (at 0x00c669d0).
// Tiger/taudiochannel.cpp uses only the virtual at offset 0x48: it attaches a file to a
// channel for a link index and returns false on failure. The other virtuals' names are not
// recovered.
class tAudioFileInfoPart : public tPart
{
public:
    GenAbstractBinding(tAudioFileInfoPart);

    tAudioFileInfoPart();
    virtual ~tAudioFileInfoPart();

    virtual void mActivate();
    virtual void mDeactivate();

    virtual unsigned long mAddSoundFile(tBasicAudioFile &aFile); // HYPOTHESIS: slot 6 (offset 0x18)
    virtual void mDeleteSoundFile(unsigned long param_1); // HYPOTHESIS: slot 7 (offset 0x1c)
    virtual long mGetNewAsynchReadInfo(unsigned long param_1, long param_2); // HYPOTHESIS: slot 8 (offset 0x20), by address order
    virtual void vfn_24();
    virtual void vfn_28();
    virtual void vfn_2c();
    virtual void vfn_30();
    virtual void vfn_34();
    virtual void vfn_38();
    virtual void vfn_3c();
    virtual void vfn_40();
    virtual tBasicAudioFile *mGetBasicAudioFile(unsigned long param_1);
    virtual unsigned char vfn_48(long param_1, long param_2, long param_3);
    virtual long mGetNumberOfSamples(unsigned long param_1);
    virtual long mGetNumberOfSelectionPerSound(unsigned long param_1);
    virtual long mGetTotalSizeInBytes(unsigned long param_1);
    virtual long mGetTotalDataSizeInBytes(unsigned long param_1);
    virtual long mGetDepth(unsigned long param_1);
    virtual long mGetLinkDepth(unsigned long param_1);

    tSafeArray<tBasicAudioFile> field_0c;
    long field_28;
    tStandardMemoryArray<tAsynchReadInfo> *field_2c;
};

#endif // tAudioFile_h
