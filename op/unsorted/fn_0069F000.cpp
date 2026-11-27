#include "Source/entity/entity_physical.h"

// HYPOTHESIS: tD3DViewSurfMgr is declared only inside the d3d view manager sources; this
// reduced view holds the two owned objects released by the destructor (0x10 and 0x14).
// HYPOTHESIS: the owned objects' real class is unknown; the call at vtable offset 8 is
// written against the one declared class with a slot there.
class tD3DViewSurfMgr
{
public:
	virtual ~tD3DViewSurfMgr();

	long field_0x4;
	long field_0x8;
	long field_0xc;

	tDynamicMemoryFreeList *field_0x10;
	tDynamicMemoryFreeList *field_0x14;
};

// HYPOTHESIS: the callee 0x006c9980 is a shared mDump of the free-list family; its declaration
// (tDynamicMemoryFreeListViaHandles::mDump) is missing, so the call is bound to the slot name.
// FUNCTION: STARFLEETOP 0x0069f350
tD3DViewSurfMgr::~tD3DViewSurfMgr()
{
	field_0xc = 0;
	if (field_0x10 != 0)
	{
		field_0x10->vfunction3();
		field_0x10 = 0;
	}
	if (field_0x14 != 0)
	{
		field_0x14->vfunction3();
		field_0x14 = 0;
	}
	((tDynamicMemoryFreeList *)this)->tDynamicMemoryFreeList::vfunction21();
}
