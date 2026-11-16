//	tSndMemMgr.h: tSoundMemoryBlockManager, the sound sample memory, its blocks and its lists.

#ifndef tSndMemMgr_h
#define tSndMemMgr_h

#include "unsorted/tDynamicMemoryFreeListViaHandles.h"

// HYPOTHESIS: the base class has ten slots (+0x00 to +0x24): tSoundMemoryBlock's own
// virtuals start at +0x28 (mMergeBlocks, from its vtable at 0x007b78cc). Slots 1 and 2 are
// named from the SDK library symbols at their addresses in the class's own vtable
// (0x007b6fbc); the other slots' names are not yet known.
class tListMemberWithHandle {
public:
    // The destructor is inline and empty (defined below): derived destructors inline it.
    virtual ~tListMemberWithHandle();
    virtual void mInitListMemberWithHandle();
    virtual void mRemoveFromList();
    virtual void vfunc_0c();
    virtual void vfunc_10();
    virtual void vfunc_14();
    virtual void vfunc_18();
    virtual void vfunc_1c();
    // HYPOTHESIS: takes the member's handle (0x0066dc50 passes each block's index).
    virtual void vfunc_20(long);
    // Slot +0x24: tSoundMemoryBlock's vtable at 0x007b78cc holds mDump (0x0066db80) here.
    virtual void mDump();

    // HYPOTHESIS: the inline base constructor sets these three fields to -1 (seen inlined
    // in 0x0066d970, before the derived vtable store); names and types not yet known.
    tListMemberWithHandle() { field_04 = -1; field_08 = -1; field_0c = -1; }
    long field_04;
    long field_08;
    long field_0c;
};

// The out-of-line copy VC6 emits for the vtable is 0x00645480 (7 bytes; the exe's derived
// classes share it), placed with Tiger/twSound.cpp.
// FUNCTION: STARFLEETOP 0x00645480
inline tListMemberWithHandle::~tListMemberWithHandle()
{
}

// HYPOTHESIS: inline like the destructor; its copy follows it with Tiger/twSound.cpp.
// FUNCTION: STARFLEETOP 0x00645490
inline void tListMemberWithHandle::mInitListMemberWithHandle()
{
    field_04 = -1;
    field_08 = -1;
    field_0c = -1;
}

// HYPOTHESIS: inline like the destructor; its copy follows it with Tiger/twSound.cpp.
// FUNCTION: STARFLEETOP 0x006454a0
inline void tListMemberWithHandle::mRemoveFromList()
{
    field_04 = -1;
    field_08 = -1;
}

// HYPOTHESIS: tSoundMemoryBlock derives from tListMemberWithHandle (the list returns
// mGetBlock's result as a tListMemberWithHandle *).
class tSoundMemoryBlock : public tListMemberWithHandle {
public:
    tSoundMemoryBlock();
    // Name from the SDK library symbols (0x00645480, shared with the base destructor).
    // The compiler generates the vector deleting destructor from this declaration and
    // the array new in mInit (0x0066dc50); it is claimed at the out-of-class destructor
    // definition at the end of this file.
    virtual ~tSoundMemoryBlock();
    // Slot order from the vtable at 0x007b78cc; names from the SDK library symbols,
    // matched by address. HYPOTHESIS: the names of slots +0x40, +0x48 and +0x50 (shared
    // accessor bodies with no symbol of their own) follow the fields they read and write.
    virtual long mMergeBlocks();
    virtual void mSetNumberOfBlocks(long);
    virtual void mSetNumberOfLocks(long);
    virtual void mSetAboutToBeRemovedByManagerCallback(void (*)(long), long);
    virtual long mGetNumberOfBlocks();
    virtual unsigned short mGetTopHalfOfHandle();
    virtual long mGetPreviousBlock();
    virtual long mGetNextBlock();
    virtual void mSetPreviousBlock(long);
    virtual void mSetNextBlock(long);
    virtual long mGetNumberOfLocks();
    virtual unsigned char mAreWeInUse();
    virtual void mSetBlockInUse(unsigned char);
    virtual void mLock();
    virtual void mUnlock();
    virtual long mUseThisMuch(long);
    virtual unsigned long mGetHandle();
    virtual void mAboutToBeRemovedByManager();
    virtual void mDump();

    // Names from mDump's format string; types from the accessors' load widths.
    long fPreviousBlock;
    long fNextBlock;
    long fNumberOfBlocks;
    long fNumberOfLocks;
    // HYPOTHESIS: set together by mSetAboutToBeRemovedByManagerCallback; names not yet known.
    void (*field_20)(long);
    long field_24;
    short fTopHalfOfHandle;
    unsigned char fBlockInUse;
};

class tSoundMemoryBlockManager;
extern tSoundMemoryBlockManager *aSoundMemoryBlockManager;

// The destructor is implicit: 0x0066df50 stores no vtable of this class (0x007b732c)
// when it destroys the two lists.
class tSoundMemoryBlockList : public tDynamicMemoryFreeListViaHandles {
public:
    virtual tListMemberWithHandle *mGetListMemberWithHandleFromHandle(long handle);
    // A virtual of this class's own, slot 44 (+0xb0) of its vtable (0x007b732c).
    virtual unsigned char mInsertIntoList(long);
};

// HYPOTHESIS: vtable order. mGetBlock at slot 14 (+0x38) is evidenced by 0x0066d8e0, and
// mNewBlock at +0x08 and mLockBlock at +0x10 by 0x0066e000; the names are the class's
// virtuals from the SDK library symbols.
class tSoundMemoryBlockManager {
    friend class tSoundMemoryBlockList;
    friend class tSoundMemoryBlock;
public:
    // HYPOTHESIS: an inline constructor that initialises the manager (tgSoundPart creates it).
    tSoundMemoryBlockManager(unsigned short numberOfBlocks, long blockSize) { mInit(numberOfBlocks, blockSize); }
    virtual ~tSoundMemoryBlockManager();
    // MATCH-ONLY: tgSound.cpp writes the scalar deleting destructor out as this member.
    tSoundMemoryBlockManager *scalar_deleting_destructor(unsigned char flags);
    virtual void mInit(unsigned short, long);
    virtual unsigned long mNewBlock(unsigned long size, void (*callback)(long), long data);
    // mFreeBlock at +0x0c is evidenced by 0x0066ea20, which passes it a block's handle.
    // HYPOTHESIS: mNewLockedBlock takes the slot at +0x14.
    virtual void mFreeBlock(unsigned long);
    virtual unsigned char *mLockBlock(unsigned long);
    virtual unsigned char *mNewLockedBlock(unsigned long size, void (*callback)(long), long data);
    virtual void mFreeLockedBlock(unsigned char *);
    virtual void mUnlockBlock(long);
    virtual unsigned char mIsThisHandleValid(unsigned long);
    virtual long mGetBlockSize();
    virtual void mDump();
    virtual void mDumpBlock(unsigned long);
    virtual void mTestBlockIntegrity();
protected:
    // mFreeLowestPriorityBlockWithNoLocks at +0x3c is evidenced by 0x0066e0f0, which
    // tests its byte result before reporting there is nothing to remove.
    virtual long mGetTrueHandle(unsigned long);
    virtual tSoundMemoryBlock *mGetBlock(long);
    virtual unsigned char mFreeLowestPriorityBlockWithNoLocks();
    virtual long mNumberOfBlocksNeeded(long);
    // HYPOTHESIS: layout from mInit (0x0066dc50); names not yet known. +0x08 and +0x0c
    // hold mInit's block count and block size, +0x10 the sample memory.
    tSoundMemoryBlock *field_04;
    long field_08;
    long field_0c;
    unsigned char *field_10;
    // HYPOTHESIS: both lists are tSoundMemoryBlockList; their roles are not yet known.
    tSoundMemoryBlockList field_14;
    tSoundMemoryBlockList field_24;
    // HYPOTHESIS: reset to -1 by mNewBlock (0x0066e0f0) before it frees blocks; name and
    // role not yet known.
    long field_34;
};
#endif
