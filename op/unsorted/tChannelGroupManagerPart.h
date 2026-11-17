//	tChannelGroupManagerPart.h: tChannelGroupManagerPart, the part that owns the channel groups,
//	and the tSafeArray template it holds them in.
// Evidence:  class; the exe's type names give tSafeArray<tChannelGroup>,
// tSafeArray<tBasicAudioFile> and tSafeArray<tClassTrackingInfo>; the SDK library names
// mAddMoreRoom, mGetItemAtThisIndex, mRemoveItemAtThisIndex, mGetNextItemIndex and
// mGetIndexFromRealIndex of tSafeArray, and mAddChannelGroup of the part.
#ifndef tChannelGroupManagerPart_h
#define tChannelGroupManagerPart_h 1
#include "Tiger/tpart.h"

class tChannelGroup;
class tBasicAudioFile;

#include "unsorted/tSafeArray.h"

// MATCH-ONLY: unsorted/tSafeArray.h now defines tSafeArray's constructor; the lines are kept so
// line numbers stay.










// HYPOTHESIS: the bodies as tChannelGroupManagerPart::mDeleteChannelGroup inlines them; the
// index carries the slot in its low word and the slot's use count in its high word.
template <class T>
inline T *tSafeArray<T>::mGetItemAtThisIndex(unsigned long aIndex)
{
    T *item = 0;
    unsigned long realIndex = aIndex & 0xffff;
    if (realIndex < (unsigned long)f10
        && (short)(aIndex >> 16) == ((short *)f08)[realIndex])
    {
        item = ((T **)f04)[realIndex];
    }
    return item;
}

template <class T>
inline void tSafeArray<T>::mRemoveItemAtThisIndex(unsigned long aIndex)
{
    if (mGetItemAtThisIndex(aIndex) != 0)
    {
        delete ((T **)f04)[aIndex & 0xffff];
        ((T **)f04)[aIndex & 0xffff] = 0;
        f0c--;
    }
}

template <class T>
inline tSafeArray<T>::~tSafeArray()
{
    if (f18 == kTrue)
    {
        for (long i = 0; f0c > 0 && i < f10; i++)
        {
            T *item = ((T **)f04)[i];
            if (item != 0)
            {
                delete item;
                ((T **)f04)[i] = 0;
                f0c--;
            }
        }
    }
    if (f04 != 0)
    {
        tMemoryPart::Search_tMemoryPart()->vfunction10((void *)f04);
        f04 = 0;
    }
    if (f08 != 0)
    {
        tMemoryPart::Search_tMemoryPart()->vfunction10((void *)f08);
        f08 = 0;
    }
    f0c = 0;
    f10 = 0;
}

// The exe's vtable (0x007b7550) holds the virtuals of tPart.
class tChannelGroupManagerPart : public tPart
{
public:
    tChannelGroupManagerPart();
    virtual ~tChannelGroupManagerPart();
    virtual iTruth mIsValid();
    virtual iTruth mIsReady();
    virtual void mActivate();
    virtual void mDeactivate();
    virtual unsigned long mAddChannelGroup(tChannelGroup &aGroup);
    virtual tChannelGroup *mGetChannelGroup(unsigned long aHandle); // HYPOTHESIS: slot +0x1c; mDeleteChannelGroup tests its pointer result
    virtual unsigned char mDeleteChannelGroup(unsigned long aHandle);
    virtual unsigned char mRemoveChannelGroupOwner(unsigned long aHandle); // HYPOTHESIS: slot follows mDeleteChannelGroup
    virtual unsigned char mAttachFileToGroup(unsigned long aHandle, long param_2); // HYPOTHESIS: slot after mRemoveChannelGroupOwner
    virtual unsigned char mAttachFileToChannel(unsigned long aHandle, unsigned long param_2, long param_3); // HYPOTHESIS: slot after mAttachFileToGroup
    virtual unsigned char mKillRetainedMode(unsigned long aHandle); // HYPOTHESIS: slot after mAttachFileToChannel
    virtual unsigned char mClearAttachments(unsigned long aHandle); // HYPOTHESIS: slot after mKillRetainedMode
    tSafeArray<tChannelGroup> fChannelGroups; // HYPOTHESIS: name; mAddChannelGroup adds to it
};

#endif
