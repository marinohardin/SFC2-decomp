#include "Source/entity/entity_physical.h"

// FUNCTION: STARFLEETOP 0x00407cf1
tDynamicMemoryFreeList::~tDynamicMemoryFreeList()
{
	mClearList();
}
