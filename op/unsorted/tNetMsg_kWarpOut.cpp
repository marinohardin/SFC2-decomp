#include "unsorted/tNetMsg_kWarpOut.h"

extern char g_0083d034[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197a7
const tNetMsg::tMapItem *tNetMsg_kWarpOut::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d034;
}
