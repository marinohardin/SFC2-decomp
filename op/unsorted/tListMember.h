//	tListMember.h: tListMember, the base of an object kept in one of the game's intrusive lists.
// Evidence:  the analysis's class name for the functions at 0x004088c6 to 0x004088e4
//            (original file unknown).

#ifndef tListMember_h
#define tListMember_h 1

class tListMember
{
public:
    // 0x004088c6: resets the vtable to tListMember's (0x0079590c) and returns.
    virtual ~tListMember();
    virtual void vfunction2();
    virtual void vfunction3();
    // HYPOTHESIS: the previous member; tDynamicMemoryFreeList::mGetPreviousListMember
    // (0x00626770) returns it through this slot (offset 0xc).
    virtual tListMember *vfunction4();
    // HYPOTHESIS: the next member; tDynamicMemoryFreeList::mGetNextListMember
    // (0x00626790) returns it through this slot (offset 0x10).
    virtual tListMember *vfunction5();
    // HYPOTHESIS: set the previous member (offset 0x14); used by
    // tDynamicMemoryFreeList::mAddToFrontOfList (0x00626810).
    virtual void vfunction6(tListMember *aMember);
    // HYPOTHESIS: set the next member (offset 0x18).
    virtual void vfunction7(tListMember *aMember);
    // HYPOTHESIS: dumps the member (offset 0x1c); tDynamicMemoryFreeList::mDumpList
    // (0x00626b90) calls it on every member.
    virtual void vfunction8();

    // HYPOTHESIS: the two link pointers, cleared by vfunction2 (offsets 4 and 8).
    tListMember *mPrevious;
    tListMember *mNext;
};

#endif // tListMember_h
