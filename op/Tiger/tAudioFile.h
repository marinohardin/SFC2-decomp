// Tiger/tAudioFile.h

#ifndef tAudioFile_h
#define tAudioFile_h

#include "Tiger/tpart.h"
#include "unsorted/tSafeArray.h"

class tBasicAudioFile;
struct tAsynchReadInfo;
template <class T> class tStandardMemoryArray;

// HYPOTHESIS: the audio file part, bound by name through its part cache (at 0x00c669d0).
// Tiger/taudiochannel.cpp uses only the virtual at offset 0x48: it attaches a file to a
// channel for a link index and returns false on failure. The other virtuals' names are not
// recovered.
class tAudioFileInfoPart : public tPart
{
public:
    // GenAbstractBinding(tAudioFileInfoPart) written out: Test_tAudioFileInfoPart has its own address in the exe.
    static tAudioFileInfoPart *Search_tAudioFileInfoPart( void )
    {
        static tAudioFileInfoPart *partCachePtr = (tAudioFileInfoPart*) NULL;
        if (partCachePtr) return partCachePtr;
        else
        {
            for( int idx = 0; idx<tPart::fStaticPartActivateCount; idx++)
            {
                tAudioFileInfoPart *bindResult = dynamic_cast< tAudioFileInfoPart *>(tPart::fStaticPartActivateList[idx] );
                if (bindResult)
                {
                    tPart::mAddToPartCachePtrList( (tPart**)&partCachePtr );
                    partCachePtr = bindResult;
                    return bindResult;
                }
            }
            throw tPartBindingException();
            return (kFalse);
        }
    };
    static iTruth Test_tAudioFileInfoPart( void );

    tAudioFileInfoPart();
    virtual ~tAudioFileInfoPart();

    virtual void mActivate();
    virtual void mDeactivate();
    virtual iTruth mIsReady();   // slot 2: 0x00657970

    // Slot order from the exe's vtable (0x007b73ec).
    virtual long mGetNumberOfSamples(unsigned long param_1);
    virtual long mGetNumberOfSelectionPerSound(unsigned long param_1);
    virtual long mGetTotalSizeInBytes(unsigned long param_1);
    virtual long mGetTotalDataSizeInBytes(unsigned long param_1);
    virtual long mGetDepth(unsigned long param_1);
    virtual long mGetLinkDepth(unsigned long param_1);
    virtual long mGetNewAsynchReadInfo(unsigned long param_1, long param_2);
    virtual tAsynchReadInfo *mGetAsynchReadInfo(long param_1);
    virtual void mFreeAsynchReadInfo(long param_1);
    virtual unsigned long mAddSoundFile(tBasicAudioFile &aFile);
    virtual void mDeleteSoundFile(unsigned long param_1);
    virtual tBasicAudioFile *mGetBasicAudioFile(unsigned long param_1);
    virtual unsigned char vfn_48(long param_1, long param_2, long param_3);

    tSafeArray<tBasicAudioFile> field_0c; // 0x20 bytes: its constructor also clears +0x28
    tStandardMemoryArray<tAsynchReadInfo> *field_2c;
};

#endif // tAudioFile_h
