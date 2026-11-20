#include "Source/entity/entity_physical.h"
#include "Common/Object.h"

// HYPOTHESIS: tShipSpecManager derives from tDynamicMemoryFreeList (its vftable layout is the base's).
class tShipSpecManager : public tDynamicMemoryFreeList
{
public:
	virtual ~tShipSpecManager();

	long field_0x10;
	long field_0x14;
	// HYPOTHESIS: an owned object with a virtual destructor; the type is unknown, tObject stands in.
	tObject *field_0x18;
};

// FUNCTION: STARFLEETOP 0x00407d36
// FUNCTION: STARFLEETOP 0x00407d52
tShipSpecManager::~tShipSpecManager()
{
	delete field_0x18;
}
