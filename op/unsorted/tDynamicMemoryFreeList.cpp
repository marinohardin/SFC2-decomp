#include "Source/entity/entity_physical.h"

// FUNCTION: STARFLEETOP 0x00407cf1
tDynamicMemoryFreeList::~tDynamicMemoryFreeList()
{
	mClearList();
}

// FUNCTION: STARFLEETOP 0x00407cfc
void tDynamicMemoryFreeList::vfunction2()
{
	field_0xc = 0;
	field_0x4 = 0;
	field_0x8 = 0;
}

// FUNCTION: STARFLEETOP 0x00407d08
void *tDynamicMemoryFreeList::vfunction3()
{
	return field_0x4;
}

// FUNCTION: STARFLEETOP 0x00407d0c
bool tDynamicMemoryFreeList::vfunction16()
{
	bool empty = field_0xc == 0;
	return empty;	// HYPOTHESIS: the list is empty
}
