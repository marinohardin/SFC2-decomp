#include "unsorted/tNetMsg_kTransportBomb.h"

extern char g_0083d114[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197dd
const tNetMsg::tMapItem *tNetMsg_kTransportBomb::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d114;
}
