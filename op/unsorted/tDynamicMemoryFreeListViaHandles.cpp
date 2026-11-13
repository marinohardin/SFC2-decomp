// Unit: unsorted/tDynamicMemoryFreeListViaHandles.cpp
// Functions of tDynamicMemoryFreeListViaHandles; original file unknown.

#include "unsorted/tDynamicMemoryFreeListViaHandles.h"

// FUNCTION: STARFLEETOP 0x00435391
void tDynamicMemoryFreeListViaHandles::vfunction2()
{
    field_0xc = 0;
    field_0x4 = -1;
    field_0x8 = -1;
}

// FUNCTION: STARFLEETOP 0x0043539e
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction5()
{
    return mGetListMemberWithHandleFromHandle(field_0x4);
}

// FUNCTION: STARFLEETOP 0x004353a7
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction6()
{
    return mGetListMemberWithHandleFromHandle(field_0x8);
}

// FUNCTION: STARFLEETOP 0x004353b0
long tDynamicMemoryFreeListViaHandles::vfunction7(long handle)
{
    return mGetPreviousListMemberHandle(mGetListMemberWithHandleFromHandle(handle));
}

// FUNCTION: STARFLEETOP 0x004353c8
long tDynamicMemoryFreeListViaHandles::vfunction9(long handle)
{
    return mGetNextListMemberHandle(mGetListMemberWithHandleFromHandle(handle));
}

// FUNCTION: STARFLEETOP 0x004353e0
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction12(tListMemberWithHandle *member)
{
    return mGetListMemberWithHandleFromHandle(mGetPreviousListMemberHandle(member));
}

// FUNCTION: STARFLEETOP 0x004353f8
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction11(long handle)
{
    return mGetListMemberWithHandleFromHandle(
        mGetPreviousListMemberHandle(mGetListMemberWithHandleFromHandle(handle)));
}

// FUNCTION: STARFLEETOP 0x00435416
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction14(tListMemberWithHandle *member)
{
    return mGetListMemberWithHandleFromHandle(mGetNextListMemberHandle(member));
}

// FUNCTION: STARFLEETOP 0x0043542e
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction13(long handle)
{
    return mGetListMemberWithHandleFromHandle(
        mGetNextListMemberHandle(mGetListMemberWithHandleFromHandle(handle)));
}

// FUNCTION: STARFLEETOP 0x0043544c
long tDynamicMemoryFreeListViaHandles::vfunction15(long handle)
{
    return mGetPreviousCircularListMemberHandle(mGetListMemberWithHandleFromHandle(handle));
}

// FUNCTION: STARFLEETOP 0x0043547c
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction17(tListMemberWithHandle *member)
{
    return mGetListMemberWithHandleFromHandle(mGetPreviousCircularListMemberHandle(member));
}

// FUNCTION: STARFLEETOP 0x00435464
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction18(long handle)
{
    return mGetListMemberWithHandleFromHandle(vfunction15(handle));
}

// FUNCTION: STARFLEETOP 0x00435494
long tDynamicMemoryFreeListViaHandles::vfunction19(long handle)
{
    return mGetNextCircularListMemberHandle(mGetListMemberWithHandleFromHandle(handle));
}

// FUNCTION: STARFLEETOP 0x004354c4
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction21(tListMemberWithHandle *member)
{
    return mGetListMemberWithHandleFromHandle(mGetNextCircularListMemberHandle(member));
}

// FUNCTION: STARFLEETOP 0x004354ac
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction22(long handle)
{
    return mGetListMemberWithHandleFromHandle(vfunction19(handle));
}

// FUNCTION: STARFLEETOP 0x004354dc
void tDynamicMemoryFreeListViaHandles::vfunction24(long handle)
{
    mAddToFrontOfList(mGetListMemberWithHandleFromHandle(handle));
}

// FUNCTION: STARFLEETOP 0x004354f4
unsigned char tDynamicMemoryFreeListViaHandles::vfunction26(long handle)
{
    // HYPOTHESIS: the member is held in a local; the same expression written as one nested
    // call keeps the vtable pointer in a register instead of reloading it, unlike the original.
    tListMemberWithHandle *member = mGetListMemberWithHandleFromHandle(handle);
    return mAddToBackOfList(member);
}

// FUNCTION: STARFLEETOP 0x0043550c
void tDynamicMemoryFreeListViaHandles::vfunction28(long handle, long otherHandle)
{
    mInsertAfterListMember(mGetListMemberWithHandleFromHandle(handle),
                           mGetListMemberWithHandleFromHandle(otherHandle));
}

// FUNCTION: STARFLEETOP 0x00435530
void tDynamicMemoryFreeListViaHandles::vfunction30(long handle, long otherHandle)
{
    mInsertBeforeListMember(mGetListMemberWithHandleFromHandle(handle),
                            mGetListMemberWithHandleFromHandle(otherHandle));
}

// FUNCTION: STARFLEETOP 0x00435554
void tDynamicMemoryFreeListViaHandles::vfunction32(long handle)
{
    mRemoveFromList(mGetListMemberWithHandleFromHandle(handle));
}

// FUNCTION: STARFLEETOP 0x0043556f
unsigned char tDynamicMemoryFreeListViaHandles::vfunction34(long handle)
{
    return mIsListMember(mGetListMemberWithHandleFromHandle(handle));
}

// FUNCTION: STARFLEETOP 0x0043558a
void tDynamicMemoryFreeListViaHandles::vfunction36(long handle)
{
    mGetZeroIndexedListMemberPosition(mGetListMemberWithHandleFromHandle(handle));
}
