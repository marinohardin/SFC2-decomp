// tSndMemMgr.cpp

#include <string.h>

#include "Tiger/tgSound.h"

#include "Tiger/tSndMemMgr.h"

// FUNCTION: STARFLEETOP 0x0066d8e0
tListMemberWithHandle *tSoundMemoryBlockList::mGetListMemberWithHandleFromHandle(long handle)
{
    return aSoundMemoryBlockManager->mGetBlock(handle);
}

// FUNCTION: STARFLEETOP 0x0066d900
unsigned char tSoundMemoryBlockList::mInsertIntoList(long handle)
{
    long firstHandle = vfunction3();
    tListMemberWithHandle *newMember = mGetListMemberWithHandleFromHandle(handle);
    tListMemberWithHandle *member = mGetListMemberWithHandleFromHandle(firstHandle);
    while (member != 0) {
        // HYPOTHESIS: the sort key is the block count, read through the derived class
        // (slot +0x38 is tSoundMemoryBlock::mGetNumberOfBlocks).
        if (((tSoundMemoryBlock *)newMember)->mGetNumberOfBlocks() >=
            ((tSoundMemoryBlock *)member)->mGetNumberOfBlocks()) {
            mInsertBeforeListMember(newMember, member);
            return 1;
        }
        member = vfunction14(member);
    }
    mAddToBackOfList(newMember);
    return 1;
}

// FUNCTION: STARFLEETOP 0x0066d970
tSoundMemoryBlock::tSoundMemoryBlock()
{
    fTopHalfOfHandle = 0;
    fPreviousBlock = -1;
    fNextBlock = -1;
    fNumberOfBlocks = -1;
    fBlockInUse = 0;
    fNumberOfLocks = 0;
}

// FUNCTION: STARFLEETOP 0x0066d9a0
long tSoundMemoryBlock::mMergeBlocks()
{
    long previous = fPreviousBlock;
    long handle = field_0c;
    long next = fNextBlock;
    tSoundMemoryBlock *block = this;

    fTopHalfOfHandle++;
    fBlockInUse = 0;

    if (previous >= 0) {
        tSoundMemoryBlock *previousBlock = aSoundMemoryBlockManager->mGetBlock(previous);
        if (!previousBlock->mAreWeInUse()) {
            handle = previous;
            previousBlock->mSetNextBlock(fNextBlock);
            previousBlock->mSetNumberOfBlocks(previousBlock->mGetNumberOfBlocks() + fNumberOfBlocks);
            fPreviousBlock = -1;
            fNextBlock = -1;
            fNumberOfBlocks = 0;
            fNumberOfLocks = 0;
            block = previousBlock;
        }
    }

    if (next != -1) {
        tSoundMemoryBlock *nextBlock = aSoundMemoryBlockManager->mGetBlock(next);
        if (!nextBlock->mAreWeInUse()) {
            long nextNext = nextBlock->mGetNextBlock();
            if (nextNext != -1) {
                aSoundMemoryBlockManager->mGetBlock(nextNext)->mSetPreviousBlock(handle);
            }
            block->mSetNextBlock(nextNext);
            block->mSetNumberOfBlocks(nextBlock->mGetNumberOfBlocks() + block->mGetNumberOfBlocks());
            nextBlock->mSetNumberOfBlocks(0);
            nextBlock->mSetPreviousBlock(-1);
            nextBlock->mSetNextBlock(-1);
            nextBlock->mSetNumberOfBlocks(0);
            nextBlock->mSetNumberOfLocks(0);
            return handle;
        }
        nextBlock->mSetPreviousBlock(handle);
    }
    return handle;
}

// FUNCTION: STARFLEETOP 0x0066dae0
long tSoundMemoryBlock::mUseThisMuch(long size)
{
    long remainder = -1;

    if (size < fNumberOfBlocks) {
        remainder = field_0c + size;
        if (fNextBlock != -1) {
            aSoundMemoryBlockManager->mGetBlock(fNextBlock)->mSetPreviousBlock(remainder);
        }
        tSoundMemoryBlock *remainderBlock = aSoundMemoryBlockManager->mGetBlock(remainder);
        if (remainderBlock != 0) {
            remainderBlock->mSetPreviousBlock(field_0c);
            remainderBlock->mSetNextBlock(fNextBlock);
            remainderBlock->mSetNumberOfBlocks(fNumberOfBlocks - size);
            remainderBlock->mSetBlockInUse(0);
        }
        fNextBlock = remainder;
    }

    if (fTopHalfOfHandle == 0) {
        fTopHalfOfHandle = 1;
    }
    fNumberOfBlocks = size;
    fBlockInUse = 1;
    fNumberOfLocks = 0;
    return remainder;
}

// tgSoundPart, tSay and the sound error reporting are declared in Tiger/tgSound.h.

// FUNCTION: STARFLEETOP 0x0066db80
void tSoundMemoryBlock::mDump()
{
    // HYPOTHESIS: both prints are one expanded macro: buffer the message while the sound
    // part reports it is at interrupt, otherwise flush the buffer and say it directly.
    if (atgSoundPart->mAreWeInInterruptMode() == 1) {
        AddSoundErrorDuringInterrupt("tSoundMemoryBlock %ld:\n", field_0c);
    } else {
        PrintOutBufferedErrors();
        gSoundSay.mJustSayIt("tSoundMemoryBlock %ld:\n", field_0c);
    }
    if (atgSoundPart->mAreWeInInterruptMode() == 1) {
        AddSoundErrorDuringInterrupt(
            "  fPreviousBlock=%ld, fNextBlock=%ld, fNumberOfBlocks=%ld, fBlockInUse=%ld, fTopHalfOfHandle=%ld, fNumberOfLocks=%ld\n",
            fPreviousBlock, fNextBlock, fNumberOfBlocks, (long)fBlockInUse,
            (long)(unsigned short)fTopHalfOfHandle, fNumberOfLocks);
    } else {
        PrintOutBufferedErrors();
        gSoundSay.mJustSayIt(
            "  fPreviousBlock=%ld, fNextBlock=%ld, fNumberOfBlocks=%ld, fBlockInUse=%ld, fTopHalfOfHandle=%ld, fNumberOfLocks=%ld\n",
            fPreviousBlock, fNextBlock, fNumberOfBlocks, (long)fBlockInUse,
            (long)(unsigned short)fTopHalfOfHandle, fNumberOfLocks);
    }
    tListMemberWithHandle::mDump();
}

// HYPOTHESIS: the original reports through one error macro that prints the message and
// then " -- %s %d\n" with __FILE__ and __LINE__; the file name is the build machine's path,
// so it is written out here.
#define kThisFile "D:\\work\\QSI\\Projects\\QSIInc\\Tiger\\tSndMemMgr.cpp"

// FUNCTION: STARFLEETOP 0x0066dc50
void tSoundMemoryBlockManager::mInit(unsigned short numberOfBlocks, long blockSize)
{
    aSoundMemoryBlockManager = this;

    if (numberOfBlocks == 0) {
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt("Huh?");
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt("Huh?");
        }
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt(" -- %s %d\n", kThisFile, 184);
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt(" -- %s %d\n", kThisFile, 184);
        }
        return;
    }

    field_04 = new tSoundMemoryBlock[numberOfBlocks];
    if (field_04 == 0) {
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt("we failed to allocate enough sound sample memory!");
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt("we failed to allocate enough sound sample memory!");
        }
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt(" -- %s %d\n", kThisFile, 191);
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt(" -- %s %d\n", kThisFile, 191);
        }
        return;
    }

    field_08 = numberOfBlocks;
    field_0c = blockSize;
    field_10 = new unsigned char[numberOfBlocks * blockSize];
    if (field_10 == 0) {
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt("we failed to allocate enough sound sample memory!");
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt("we failed to allocate enough sound sample memory!");
        }
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt(" -- %s %d\n", kThisFile, 201);
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt(" -- %s %d\n", kThisFile, 201);
        }
        // HYPOTHESIS: scalar delete of the array, as the original's virtual-destructor
        // call with flag 1 shows.
        delete field_04;
        field_04 = 0;
        return;
    }

    for (long i = 0; i < numberOfBlocks; i++) {
        field_04[i].vfunc_20(i);
    }
    field_04->mSetNumberOfBlocks(numberOfBlocks);
    field_14.vfunction2();
    field_14.mInsertIntoList(0);
    field_24.vfunction2();
}

// FUNCTION: STARFLEETOP 0x0066df50
tSoundMemoryBlockManager::~tSoundMemoryBlockManager()
{
    field_14.mClearList();
    field_24.mClearList();
    if (field_04 != 0) {
        delete[] field_04;
        field_04 = 0;
    }
    if (field_10 != 0) {
        delete field_10;
        field_10 = 0;
    }
    aSoundMemoryBlockManager = 0;
}

// FUNCTION: STARFLEETOP 0x0066e000
unsigned char *tSoundMemoryBlockManager::mNewLockedBlock(unsigned long size, void (*callback)(long), long data)
{
    unsigned long handle = mNewBlock(size, callback, data);
    if (handle == 0) {
        return 0;
    }
    return mLockBlock(handle);
}

// FUNCTION: STARFLEETOP 0x0066e030
void tSoundMemoryBlockManager::mFreeLockedBlock(unsigned char *block)
{
    long handle = (block - field_10) / field_0c;

    field_04[handle].mUnlock();
    // HYPOTHESIS: slot +0x80 of the list (the handle form of mRemoveFromList) takes the
    // block out of the list here.
    field_24.vfunction32(handle);

    long neighbour = field_04[handle].mGetPreviousBlock();
    if (neighbour >= 0 && !field_04[neighbour].mAreWeInUse()) {
        field_14.vfunction32(neighbour);
    }
    neighbour = field_04[handle].mGetNextBlock();
    if (neighbour >= 0 && !field_04[neighbour].mAreWeInUse()) {
        field_14.vfunction32(neighbour);
    }
    long merged = field_04[handle].mMergeBlocks();
    field_14.mInsertIntoList(merged);
}

// FUNCTION: STARFLEETOP 0x0066e0f0
unsigned long tSoundMemoryBlockManager::mNewBlock(unsigned long size, void (*callback)(long), long data)
{
    if (size <= 0) {
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt("trying to allocate too small of a block!\n");
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt("trying to allocate too small of a block!\n");
        }
        return 0;
    }

    long blocksNeeded = mNumberOfBlocksNeeded(size);
    if (blocksNeeded >= field_08) {
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt("trying to allocate too large of a block!");
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt("trying to allocate too large of a block!");
        }
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt(" -- %s %d\n", kThisFile, 295);
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt(" -- %s %d\n", kThisFile, 295);
        }
        return 0;
    }

    if (field_14.vfunction39() == 0) {
        field_34 = -1;
        if (!mFreeLowestPriorityBlockWithNoLocks()) {
            if (atgSoundPart->mAreWeInInterruptMode() == 1) {
                AddSoundErrorDuringInterrupt("Nothing we can remove!");
            } else {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("Nothing we can remove!");
            }
            if (atgSoundPart->mAreWeInInterruptMode() == 1) {
                AddSoundErrorDuringInterrupt(" -- %s %d\n", kThisFile, 308);
            } else {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt(" -- %s %d\n", kThisFile, 308);
            }
            return 0;
        }
    }

    long handle = field_14.vfunction3();
    field_34 = -1;
    while (field_04[handle].mGetNumberOfBlocks() < blocksNeeded) {
        if (!mFreeLowestPriorityBlockWithNoLocks()) {
            if (atgSoundPart->mAreWeInInterruptMode() == 1) {
                AddSoundErrorDuringInterrupt("Nothing left we can remove!");
            } else {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("Nothing left we can remove!");
            }
            if (atgSoundPart->mAreWeInInterruptMode() == 1) {
                AddSoundErrorDuringInterrupt(" -- %s %d\n", kThisFile, 323);
            } else {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt(" -- %s %d\n", kThisFile, 323);
            }
            return 0;
        }
        handle = field_14.vfunction3();
    }

    // HYPOTHESIS: slot +0x80 takes the block out of the free list here, as in
    // mFreeLockedBlock (0x0066e030).
    field_14.vfunction32(handle);
    long remainder = field_04[handle].mUseThisMuch(blocksNeeded);
    if (remainder > 0) {
        field_14.mInsertIntoList(remainder);
    }
    field_24.vfunction24(handle);
    field_04[handle].mSetAboutToBeRemovedByManagerCallback(callback, data);
    memset(field_10 + field_0c * handle, 0, field_0c * blocksNeeded);
    return field_04[handle].mGetHandle();
}

// FUNCTION: STARFLEETOP 0x0066e420
void tSoundMemoryBlockManager::mFreeBlock(unsigned long handle)
{
    long trueHandle = mGetTrueHandle(handle);
    if (mIsThisHandleValid(handle)) {
        // HYPOTHESIS: slot +0x80 takes the block out of the list, as in mFreeLockedBlock
        // (0x0066e030).
        field_24.vfunction32(trueHandle);

        long neighbour = field_04[trueHandle].mGetPreviousBlock();
        if (neighbour >= 0 && !field_04[neighbour].mAreWeInUse()) {
            field_14.vfunction32(neighbour);
        }
        neighbour = field_04[trueHandle].mGetNextBlock();
        if (neighbour >= 0 && !field_04[neighbour].mAreWeInUse()) {
            field_14.vfunction32(neighbour);
        }
        long merged = field_04[trueHandle].mMergeBlocks();
        field_14.mInsertIntoList(merged);
    }
}

// FUNCTION: STARFLEETOP 0x0066e4e0
unsigned char *tSoundMemoryBlockManager::mLockBlock(unsigned long handle)
{
    long trueHandle = mGetTrueHandle(handle);
    if (trueHandle >= field_08) {
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt("Major Problem!!");
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt("Major Problem!!");
        }
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt(" -- %s %d\n", kThisFile, 382);
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt(" -- %s %d\n", kThisFile, 382);
        }
        return 0;
    }
    if (!mIsThisHandleValid(handle)) {
        return 0;
    }
    field_04[trueHandle].mLock();
    // HYPOTHESIS: slot +0x80 takes the block out of the list and slot +0x60 puts it back,
    // as in mNewBlock (0x0066e0f0).
    field_24.vfunction32(trueHandle);
    field_24.vfunction24(trueHandle);
    return field_10 + field_0c * trueHandle;
}

// FUNCTION: STARFLEETOP 0x0066e5e0
void tSoundMemoryBlockManager::mUnlockBlock(long handle)
{
    long trueHandle = mGetTrueHandle(handle);
    if (mIsThisHandleValid(handle)) {
        field_04[trueHandle].mUnlock();
    }
}

// FUNCTION: STARFLEETOP 0x0066e620
tSoundMemoryBlock *tSoundMemoryBlockManager::mGetBlock(long handle)
{
    if (handle >= 0 && handle < field_08 && field_10) {
        return &field_04[handle];
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0066e650
void tSoundMemoryBlockManager::mDumpBlock(unsigned long handle)
{
    long trueHandle = mGetTrueHandle(handle);
    if (trueHandle < field_08) {
        field_04[trueHandle].mDump();
    } else {
        // HYPOTHESIS: the string's text past "tSoundMemoryBlockManager: %ld is" is inferred
        // from its 0x36-byte length.
        if (atgSoundPart->mAreWeInInterruptMode() == 1) {
            AddSoundErrorDuringInterrupt("tSoundMemoryBlockManager: %ld is not a valid handle!\n", handle);
        } else {
            PrintOutBufferedErrors();
            gSoundSay.mJustSayIt("tSoundMemoryBlockManager: %ld is not a valid handle!\n", handle);
        }
    }
}

// FUNCTION: STARFLEETOP 0x0066e6c0
void tSoundMemoryBlockManager::mTestBlockIntegrity()
{
    long handle = 0;
    long previous = -1;
    long i;

    // Walk the chain of blocks from the first one.
    do {
        if (field_04[handle].mGetPreviousBlock() != previous) {
            if (atgSoundPart->mAreWeInInterruptMode() == 1) {
                AddSoundErrorDuringInterrupt("Integrity Problem\n");
            } else {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("Integrity Problem\n");
            }
        }
        previous = handle;
        handle = field_04[previous].mGetNextBlock();
        long end = handle;
        if (end == -1) {
            end = field_08;
        }
        if (end - previous != field_04[previous].mGetNumberOfBlocks()) {
            if (atgSoundPart->mAreWeInInterruptMode() == 1) {
                AddSoundErrorDuringInterrupt("Integrity Problem\n");
            } else {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("Integrity Problem\n");
            }
        }
        for (i = previous + 1; i < end; i++) {
            if (field_04[i].mGetNumberOfBlocks() > 0 ||
                field_04[i].mGetPreviousBlock() != -1 ||
                field_04[i].mAreWeInUse() == 1 ||
                field_04[i].mGetNextBlock() != -1 ||
                field_04[i].mGetNumberOfLocks() > 0) {
                if (atgSoundPart->mAreWeInInterruptMode() == 1) {
                    AddSoundErrorDuringInterrupt("Integrity Problem\n");
                } else {
                    PrintOutBufferedErrors();
                    gSoundSay.mJustSayIt("Integrity Problem\n");
                }
            }
        }
        if ((handle < previous && handle != -1) || handle >= field_08) {
            if (atgSoundPart->mAreWeInInterruptMode() == 1) {
                AddSoundErrorDuringInterrupt("Integrity Problem\n");
            } else {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("Integrity Problem\n");
            }
        }
    } while (handle != -1);

    // Every block in use must be in the in-use list, and no other.
    for (i = 0; i < field_08; i++) {
        if (field_24.vfunction34(i) != field_04[i].mAreWeInUse()) {
            if (atgSoundPart->mAreWeInInterruptMode() == 1) {
                AddSoundErrorDuringInterrupt("Integrity Problem\n");
            } else {
                PrintOutBufferedErrors();
                gSoundSay.mJustSayIt("Integrity Problem\n");
            }
        }
    }
}

// FUNCTION: STARFLEETOP 0x0066e910
void tSoundMemoryBlockManager::mDump()
{
    if (atgSoundPart->mAreWeInInterruptMode() == 1) {
        AddSoundErrorDuringInterrupt("tSoundMemoryBlockManager at address %ld:\n", this);
    } else {
        PrintOutBufferedErrors();
        gSoundSay.mJustSayIt("tSoundMemoryBlockManager at address %ld:\n", this);
    }
    // The format string names the fields: fHugeChunkOfMemory is field_10, fNumberOfBlocks
    // field_08, fBlockSize field_0c and fBlocks field_04.
    if (atgSoundPart->mAreWeInInterruptMode() == 1) {
        AddSoundErrorDuringInterrupt(
            "\tfHugeChunkOfMemory=%ld,fNumberOfBlocks=%ld, fBlockSize=%ld, fBlocks=%ld\n",
            field_10, field_08, field_0c, field_04);
    } else {
        PrintOutBufferedErrors();
        gSoundSay.mJustSayIt(
            "\tfHugeChunkOfMemory=%ld,fNumberOfBlocks=%ld, fBlockSize=%ld, fBlocks=%ld\n",
            field_10, field_08, field_0c, field_04);
    }
    // HYPOTHESIS: slot +0xa8 of the list dumps it; name not yet known.
    field_14.vfunction42();
    field_24.vfunction42();
}

// FUNCTION: STARFLEETOP 0x0066e9c0
unsigned char tSoundMemoryBlockManager::mIsThisHandleValid(unsigned long handle)
{
    long trueHandle = mGetTrueHandle(handle);
    if (trueHandle >= 0 && trueHandle < field_08 &&
        (unsigned short)(handle >> 16) == field_04[trueHandle].mGetTopHalfOfHandle() &&
        field_10) {
        return field_04[trueHandle].mAreWeInUse();
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0066ea20
unsigned char tSoundMemoryBlockManager::mFreeLowestPriorityBlockWithNoLocks()
{
    // Resume the walk where the last call stopped, or start it over.
    long handle = field_34;
    if (handle == -1) {
        handle = field_24.vfunction4();
    }
    while (handle >= 0) {
        if (field_04[handle].mGetNumberOfLocks() <= 0) {
            break;
        }
        handle = field_24.vfunction7(handle);
    }
    if (handle < 0) {
        return 0;
    }
    field_34 = field_24.vfunction7(handle);
    field_04[handle].mAboutToBeRemovedByManager();
    mFreeBlock(field_04[handle].mGetHandle());
    return 1;
}

// The destructor's own body is 0x00645480, which the linker folded with the base class's;
// this definition carries the claim on the vector deleting destructor the compiler
// generates from it.
// FUNCTION: STARFLEETOP 0x0066def0
tSoundMemoryBlock::~tSoundMemoryBlock()
{
}
