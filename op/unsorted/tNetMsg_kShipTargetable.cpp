#include "unsorted/tNetMsg_kShipTargetable.h"

extern char g_0083d018[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x00419795
const tNetMsg::tMapItem *tNetMsg_kShipTargetable::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d018;
}
