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
};

#endif // tListMember_h
