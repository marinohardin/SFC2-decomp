#include "unsorted/tNetMsg_kCreateSeeker.h"

extern char g_0083d0ac[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197bf
const tNetMsg::tMapItem *tNetMsg_kCreateSeeker::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d0ac;
}
