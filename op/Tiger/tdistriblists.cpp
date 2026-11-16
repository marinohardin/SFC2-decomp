//	tdistriblists.cpp: tDynamicMemoryFreeList, the list whose members link themselves.
// Evidence:  sdk-member (an SDK library object compiled from this file has these functions)

#include "Source/entity/entity_physical.h"
#include "unsorted/tListMember.h"

// FUNCTION: STARFLEETOP 0x00626770
tListMember *tDynamicMemoryFreeList::mGetPreviousListMember(tListMember *aMember)
{
	if (aMember == 0)
		return 0;
	return aMember->vfunction4();
}

// FUNCTION: STARFLEETOP 0x00626790
tListMember *tDynamicMemoryFreeList::mGetNextListMember(tListMember *aMember)
{
	if (aMember == 0)
		return 0;
	return aMember->vfunction5();
}

// FUNCTION: STARFLEETOP 0x006267b0
tListMember *tDynamicMemoryFreeList::mGetPreviousCircularListMember(tListMember *aMember)
{
	if (aMember == 0)
		return 0;
	tListMember *previous = aMember->vfunction4();
	if (previous == 0)
		previous = vfunction4();
	return previous;
}

// FUNCTION: STARFLEETOP 0x006267e0
tListMember *tDynamicMemoryFreeList::mGetNextCircularListMember(tListMember *aMember)
{
	if (aMember == 0)
		return 0;
	tListMember *next = aMember->vfunction5();
	if (next == 0)
		// HYPOTHESIS: vfunction3 returns the first member as a tListMember.
		next = (tListMember *)vfunction3();
	return next;
}
