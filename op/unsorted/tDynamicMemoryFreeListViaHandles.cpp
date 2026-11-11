// Unit: unsorted/tDynamicMemoryFreeListViaHandles.cpp
// Functions of tDynamicMemoryFreeListViaHandles; original file unknown.

#include "unsorted/tDynamicMemoryFreeListViaHandles.h"

// FUNCTION: STARFLEETOP 0x004355a5
tDynamicMemoryFreeListViaHandles::~tDynamicMemoryFreeListViaHandles()
{
}

// FUNCTION: STARFLEETOP 0x00435391
void tDynamicMemoryFreeListViaHandles::vfunction3()
{
    field_0xc = 0;
    field_0x4 = -1;
    field_0x8 = -1;
}

// FUNCTION: STARFLEETOP 0x0043539e
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction6()
{
    return vfunction1(field_0x4);
}

// FUNCTION: STARFLEETOP 0x004353a7
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction7()
{
    return vfunction1(field_0x8);
}

// FUNCTION: STARFLEETOP 0x004353b0
int tDynamicMemoryFreeListViaHandles::vfunction8(int handle)
{
    return vfunction8(vfunction1(handle));
}

// FUNCTION: STARFLEETOP 0x004353c8
int tDynamicMemoryFreeListViaHandles::vfunction10(int handle)
{
    return vfunction10(vfunction1(handle));
}

// FUNCTION: STARFLEETOP 0x004353e0
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction13(tListMemberWithHandle *member)
{
    return vfunction1(vfunction8(member));
}

// FUNCTION: STARFLEETOP 0x004353f8
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction13(int handle)
{
    return vfunction1(vfunction8(vfunction1(handle)));
}

// FUNCTION: STARFLEETOP 0x00435416
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction15(tListMemberWithHandle *member)
{
    return vfunction1(vfunction10(member));
}

// FUNCTION: STARFLEETOP 0x0043542e
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction15(int handle)
{
    return vfunction1(vfunction10(vfunction1(handle)));
}

// FUNCTION: STARFLEETOP 0x0043544c
void tDynamicMemoryFreeListViaHandles::vfunction16(int handle)
{
    vfunction16(vfunction1(handle));
}

// FUNCTION: STARFLEETOP 0x0043547c
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction18(tListMemberWithHandle *member)
{
    return vfunction1(vfunction16(member));
}

// FUNCTION: STARFLEETOP 0x00435464
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction19(int handle)
{
    // HYPOTHESIS: the slot 15 function (declared here as vfunction15(int)) returns a
    // handle, since its result is passed straight to vfunction1; its declared member
    // return type is likely wrong, hence the cast.
    return vfunction1((int)vfunction15(handle));
}

// FUNCTION: STARFLEETOP 0x00435494
void tDynamicMemoryFreeListViaHandles::vfunction20(int handle)
{
    vfunction20(vfunction1(handle));
}

// FUNCTION: STARFLEETOP 0x004354c4
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction22(tListMemberWithHandle *member)
{
    return vfunction1(vfunction20(member));
}

// FUNCTION: STARFLEETOP 0x004354ac
tListMemberWithHandle *tDynamicMemoryFreeListViaHandles::vfunction23(int handle)
{
    // HYPOTHESIS: the slot 19 function returns a handle, since its result is passed
    // straight to vfunction1, as vfunction19 does with slot 15; hence the cast.
    return vfunction1((int)vfunction19(handle));
}

// FUNCTION: STARFLEETOP 0x004354dc
void tDynamicMemoryFreeListViaHandles::vfunction25(int handle)
{
    vfunction25(vfunction1(handle));
}

// FUNCTION: STARFLEETOP 0x004354f4
void tDynamicMemoryFreeListViaHandles::vfunction27(int handle)
{
    // HYPOTHESIS: the member is held in a local; the same expression written as one nested
    // call keeps the vtable pointer in a register instead of reloading it, unlike the original.
    tListMemberWithHandle *member = vfunction1(handle);
    vfunction27(member);
}

// FUNCTION: STARFLEETOP 0x0043550c
void tDynamicMemoryFreeListViaHandles::vfunction29(int handle, int otherHandle)
{
    vfunction29(vfunction1(handle), vfunction1(otherHandle));
}

// FUNCTION: STARFLEETOP 0x00435530
void tDynamicMemoryFreeListViaHandles::vfunction31(int handle, int otherHandle)
{
    vfunction31(vfunction1(handle), vfunction1(otherHandle));
}

// FUNCTION: STARFLEETOP 0x00435554
void tDynamicMemoryFreeListViaHandles::vfunction33(int handle)
{
    vfunction33(vfunction1(handle));
}

// FUNCTION: STARFLEETOP 0x0043556f
void tDynamicMemoryFreeListViaHandles::vfunction35(int handle)
{
    vfunction35(vfunction1(handle));
}

// FUNCTION: STARFLEETOP 0x0043558a
void tDynamicMemoryFreeListViaHandles::vfunction37(int handle)
{
    vfunction37(vfunction1(handle));
}
