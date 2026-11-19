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
