// Unit: unsorted/tDynamicMemoryFreeListViaHandles.cpp
// Functions of tDynamicMemoryFreeListViaHandles; original file unknown.

// HYPOTHESIS: fields recovered from the accesses in vfunction3 only; field names unknown.
// Virtual slots 2, 4 and 5 are placeholders that keep the slot numbers of the other
// declarations; nothing is known about them yet.
class tListMemberWithHandle;

class tDynamicMemoryFreeListViaHandles
{
public:
    // HYPOTHESIS: slot 0 is the scalar deleting destructor (the SDK lib has ??_G for this class).
    virtual ~tDynamicMemoryFreeListViaHandles();
    // HYPOTHESIS: slot 1 maps a handle to its list member; name unknown.
    virtual tListMemberWithHandle *vfunction1(int handle);
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual tListMemberWithHandle *vfunction6();
    virtual tListMemberWithHandle *vfunction7();
    // HYPOTHESIS: an overload pair, like the SDK's handle/member accessors (for example
    // mGetNextListMemberHandle); the member form sits at slot 8 (vtable +0x20), the
    // handle form looks the member up and forwards to it. MSVC lays overloads out in
    // reverse declaration order, so the handle form is declared first.
    virtual int vfunction8(int handle);
    virtual int vfunction8(tListMemberWithHandle *member);
    // HYPOTHESIS: a second handle/member overload pair; the member form sits at slot 10
    // (vtable +0x28) and the handle form forwards to it, as for vfunction8.
    // HYPOTHESIS: the member form returns a handle (vfunction15 passes its result to
    // vfunction1), like the slot 8 pair.
    virtual int vfunction10(int handle);
    virtual int vfunction10(tListMemberWithHandle *member);
    // HYPOTHESIS: maps a member to the member the slot 8 handle accessor names (likely a
    // next/previous member accessor). Slots 11 and 12 are not declared yet, so this is
    // declared at slot 11 for now; the slot does not change the code of the body.
    // HYPOTHESIS: the handle form of vfunction13 sits at slot 12 (vtable +0x30); it looks
    // the member up and does what the member form does. Declared first, as for the other
    // overload pairs.
    virtual tListMemberWithHandle *vfunction13(int handle);
    virtual tListMemberWithHandle *vfunction13(tListMemberWithHandle *member);
    // HYPOTHESIS: a third handle/member pair; the member form maps a member to the member
    // the slot 10 accessor names, as vfunction13 does with slot 8.
    virtual tListMemberWithHandle *vfunction15(int handle);
    virtual tListMemberWithHandle *vfunction15(tListMemberWithHandle *member);
    // HYPOTHESIS: a fourth handle/member pair; the member form sits at slot 16 (vtable
    // +0x40) and the handle form looks the member up and forwards to it.
    // HYPOTHESIS: the member form returns a handle (vfunction18 passes its result to
    // vfunction1); the handle form does nothing with the result.
    virtual void vfunction16(int handle);
    virtual int vfunction16(tListMemberWithHandle *member);
    // HYPOTHESIS: maps a member to the member the slot 16 accessor names, as vfunction13
    // and vfunction15 do with slots 8 and 10.
    virtual tListMemberWithHandle *vfunction18(tListMemberWithHandle *member);
    // HYPOTHESIS: maps a handle to a member through the slot 15 accessor (vtable +0x3c)
    // and vfunction1, like vfunction6/7 do with the stored handles.
    virtual tListMemberWithHandle *vfunction19(int handle);
    // HYPOTHESIS: a fifth handle/member pair; the member form sits at slot 20 (vtable
    // +0x50) and the handle form looks the member up and forwards to it. Return types
    // unknown; the handle form does nothing with the result.
    // HYPOTHESIS: the member form returns a handle (vfunction22 passes its result to
    // vfunction1), like the slot 16 pair.
    virtual void vfunction20(int handle);
    virtual int vfunction20(tListMemberWithHandle *member);
    // HYPOTHESIS: maps a member to the member the slot 20 accessor names, as vfunction18
    // does with slot 16.
    virtual tListMemberWithHandle *vfunction22(tListMemberWithHandle *member);
    // HYPOTHESIS: maps a handle to a member through the slot 19 accessor (vtable +0x4c)
    // and vfunction1, as vfunction19 does with slot 15.
    virtual tListMemberWithHandle *vfunction23(int handle);
    // Slot 24 is a placeholder that keeps the slot numbers of the declarations after it;
    // nothing is known about it yet.
    virtual void vfunction24();
    // HYPOTHESIS: a sixth handle/member pair; the member form sits at slot 25 (vtable
    // +0x64) and the handle form looks the member up and forwards to it, as for the
    // slot 16 and 20 pairs. Return types unknown; the handle form uses no result.
    virtual void vfunction25(int handle);
    virtual void vfunction25(tListMemberWithHandle *member);
    // HYPOTHESIS: a seventh handle/member pair; the handle form looks the member up and
    // forwards to the member form, as for the slot 25 pair. Return types unknown.
    virtual void vfunction27(int handle);
    virtual void vfunction27(tListMemberWithHandle *member);
    // HYPOTHESIS: an eighth handle/member pair taking two of each; the member form sits at
    // slot 29 (vtable +0x74) and the handle form looks both members up and forwards them.
    // Return types and parameter names unknown.
    virtual void vfunction29(int handle, int otherHandle);
    virtual void vfunction29(tListMemberWithHandle *member, tListMemberWithHandle *otherMember);
    // HYPOTHESIS: a ninth pair taking two of each, as for the slot 29 pair; the member form
    // sits at slot 31. Return types and parameter names unknown.
    virtual void vfunction31(int handle, int otherHandle);
    virtual void vfunction31(tListMemberWithHandle *member, tListMemberWithHandle *otherMember);
    // HYPOTHESIS: a tenth handle/member pair; the member form sits at slot 33 and the
    // handle form looks the member up and forwards to it. Return types unknown.
    virtual void vfunction33(int handle);
    virtual void vfunction33(tListMemberWithHandle *member);
    // HYPOTHESIS: an eleventh handle/member pair; the member form sits at slot 35 and the
    // handle form looks the member up and forwards to it, as for the slot 33 pair.
    virtual void vfunction35(int handle);
    virtual void vfunction35(tListMemberWithHandle *member);
    // HYPOTHESIS: a twelfth handle/member pair; the member form sits at slot 37 and the
    // handle form looks the member up and forwards to it, as for the slot 35 pair.
    virtual void vfunction37(int handle);
    virtual void vfunction37(tListMemberWithHandle *member);

    int field_0x4;
    int field_0x8;
    int field_0xc;
};

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
