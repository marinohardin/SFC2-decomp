#include "unsorted/tTransporterHoldItem.h"
#include <new>

// HYPOTHESIS: the library allocator's construct() instantiation for tTransporterHoldItem.
// FUNCTION: STARFLEETOP 0x004282c7
void fn_004282c7(tTransporterHoldItem* _P, const tTransporterHoldItem& _V)
{
	new ((void*)_P) tTransporterHoldItem(_V);
}
