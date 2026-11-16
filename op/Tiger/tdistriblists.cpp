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
