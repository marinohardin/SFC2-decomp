#include "unsorted/tListMember.h"

// FUNCTION: STARFLEETOP 0x004088e4
// FUNCTION: STARFLEETOP 0x004088c6
tListMember::~tListMember()
{
}

// FUNCTION: STARFLEETOP 0x004088cd
void tListMember::vfunction2()
{
    mPrevious = 0;
    mNext = 0;
}

// FUNCTION: STARFLEETOP 0x004088d6
tListMember *tListMember::vfunction3()
{
    return mNext;
}

// FUNCTION: STARFLEETOP 0x004088da
void tListMember::vfunction6(tListMember *aMember)
{
    mPrevious = aMember;
}
