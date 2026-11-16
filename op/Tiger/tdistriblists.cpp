//	tdistriblists.cpp: tDynamicMemoryFreeList, the list whose members link themselves.
// Evidence:  sdk-member (an SDK library object compiled from this file has these functions)

#include "Source/entity/entity_physical.h"
#include "unsorted/tListMember.h"
#include "Tiger/tSndMemMgr.h"

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

// FUNCTION: STARFLEETOP 0x00626810
unsigned char tDynamicMemoryFreeList::mAddToFrontOfList(tListMember *aMember)
{
	if (aMember == 0 || aMember->vfunction4() != 0 || field_0x4 == aMember)
		return 0;
	aMember->vfunction6(0);
	aMember->vfunction7(field_0x4);
	if (field_0x4 != 0)
		field_0x4->vfunction6(aMember);
	else
		field_0x8 = aMember;
	field_0x4 = aMember;
	field_0xc++;
	return 1;
}

// FUNCTION: STARFLEETOP 0x00626880
unsigned char tDynamicMemoryFreeList::mAddToBackOfList(tListMember *aMember)
{
	if (aMember == 0 || aMember->vfunction4() != 0 || field_0x4 == aMember)
		return 0;
	aMember->vfunction7(0);
	aMember->vfunction6(field_0x8);
	if (field_0x8 != 0)
		field_0x8->vfunction7(aMember);
	else
		field_0x4 = aMember;
	field_0x8 = aMember;
	field_0xc++;
	return 1;
}

// FUNCTION: STARFLEETOP 0x006268f0
unsigned char tDynamicMemoryFreeList::mInsertAfterListMember(tListMember *aListMember, tListMember *aMember)
{
	if (aMember == 0 || aMember->vfunction4() != 0 || field_0x4 == aMember)
		return 0;
	if (aListMember == 0)
		return mAddToFrontOfList(aMember);
	if (field_0x8 == aListMember)
		return mAddToBackOfList(aMember);
	tListMember *next = aListMember->vfunction5();
	next->vfunction6(aMember);
	aMember->vfunction6(aListMember);
	aListMember->vfunction7(aMember);
	aMember->vfunction7(next);
	field_0xc++;
	return 1;
}

// FUNCTION: STARFLEETOP 0x00626980
unsigned char tDynamicMemoryFreeList::mInsertBeforeListMember(tListMember *aMember, tListMember *aListMember)
{
	if (aMember == 0 || aMember->vfunction4() != 0 || field_0x4 == aMember)
		return 0;
	if (aListMember == 0)
		return mAddToBackOfList(aMember);
	if (field_0x4 == aListMember)
		return mAddToFrontOfList(aMember);
	tListMember *previous = aListMember->vfunction4();
	previous->vfunction7(aMember);
	aMember->vfunction7(aListMember);
	aListMember->vfunction6(aMember);
	aMember->vfunction6(previous);
	field_0xc++;
	return 1;
}

// FUNCTION: STARFLEETOP 0x00626a10
unsigned char tDynamicMemoryFreeList::mRemoveFromList(tListMember *aMember)
{
	if (aMember == 0)
		return 0;
	if (field_0x4 == aMember)
		field_0x4 = aMember->vfunction5();
	else if (aMember->vfunction4() == 0)
		return 0;
	if (field_0x8 == aMember)
		field_0x8 = aMember->vfunction4();
	tListMember *previous = aMember->vfunction4();
	tListMember *next = aMember->vfunction5();
	if (previous != 0)
		previous->vfunction7(next);
	if (next != 0)
		next->vfunction6(previous);
	// HYPOTHESIS: slot 2 (offset 8) clears the member's links.
	aMember->vfunction3();
	field_0xc--;
	return 1;
}

// FUNCTION: STARFLEETOP 0x00626aa0
unsigned char tDynamicMemoryFreeList::mIsListMember(tListMember *aMember)
{
	for (tListMember *member = field_0x4; member != 0; member = member->vfunction5())
		if (member == aMember)
			return 1;
	return 0;
}

// FUNCTION: STARFLEETOP 0x00626ad0
long tDynamicMemoryFreeList::mGetZeroIndexedListMemberPosition(tListMember *aMember)
{
	long position = 0;
	for (tListMember *member = field_0x4; member != 0; member = member->vfunction5())
	{
		if (member == aMember)
			return position;
		position++;
	}
	return -1;
}

// FUNCTION: STARFLEETOP 0x00626b00
tListMember *tDynamicMemoryFreeList::mGetListMemberAtThisIndex(long index)
{
	tListMember *member = field_0x4;
	long position = 0;
	while (member != 0 && position != index)
	{
		position++;
		member = member->vfunction5();
	}
	return member;
}

// FUNCTION: STARFLEETOP 0x00626b30
void tDynamicMemoryFreeList::mClearList()
{
	while (field_0x4 != 0)
		mRemoveFromList(field_0x4);
}

// FUNCTION: STARFLEETOP 0x00626b50
void tDynamicMemoryFreeList::mDeleteListMembers()
{
	// HYPOTHESIS: vfunction3 returns the first member as a tListMember.
	tListMember *member = (tListMember *)vfunction3();
	while (member != 0)
	{
		mRemoveFromList(member);
		delete member;
		member = (tListMember *)vfunction3();
	}
}

// FUNCTION: STARFLEETOP 0x00626b90
void tDynamicMemoryFreeList::mDumpList()
{
	vfunction21();
	for (tListMember *member = field_0x4; member != 0; member = member->vfunction5())
		member->vfunction8();
}

// FUNCTION: STARFLEETOP 0x00626bc0
long tDynamicMemoryFreeListViaHandles::mGetPreviousListMemberHandle(tListMemberWithHandle *aMember)
{
	if (aMember == 0)
		return -1;
	return aMember->vfunc_0c();
}
