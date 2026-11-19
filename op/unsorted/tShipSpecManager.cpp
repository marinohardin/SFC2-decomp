#include "Source/entity/entity_physical.h"

// HYPOTHESIS: tShipSpecManager derives from tDynamicMemoryFreeList (its vftable layout is the base's).
class tShipSpecManager : public tDynamicMemoryFreeList
{
public:
	virtual ~tShipSpecManager();
};

// FUNCTION: STARFLEETOP 0x00407d36
tShipSpecManager::~tShipSpecManager()
{
}
