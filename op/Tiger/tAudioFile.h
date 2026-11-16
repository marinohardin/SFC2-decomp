// Tiger/tAudioFile.h

#ifndef tAudioFile_h
#define tAudioFile_h

#include "Tiger/tpart.h"

class tBasicAudioFile;

// The name is in the exe's symbols (tSafeArray<tBasicAudioFile>, tSafeArray<tChannelGroup>,
// tSafeArray<tClassTrackingInfo>). HYPOTHESIS: field roles (item pointer, counts, grow step 0x40,
// a flag set to 1) are read from the inlined constructor only; their names are not recovered.
template <class T>
class tSafeArray
{
public:
    tSafeArray();
    virtual ~tSafeArray();

    T *field_04;
    long field_08;
    long field_0c;
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
    virtual unsigned char vfn_48(long param_1, long param_2, long param_3);

    tSafeArray<tBasicAudioFile> field_0c;
    long field_28;
    long field_2c;
};

#endif // tAudioFile_h
