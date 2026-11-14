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
};

#endif // tListMember_h
