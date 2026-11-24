#include "Source/entity/entity_physical.h"

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

// MATCH-ONLY: the original's instance of the destructor (0x00407cf1, with its deleting destructor
// 0x00407d14) is the compiler's copy of the inline destructor in Source/entity/entity_physical.h;
// with inline expansion off, an object and its destruction make this unit emit it. Not a function
// of the original.
#pragma inline_depth( 0 )
void fn_emit_tDynamicMemoryFreeList()
{
	tDynamicMemoryFreeList list;
}
#pragma inline_depth()
