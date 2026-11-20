#include "unsorted/tNetMsg_kLayMine.h"

extern char g_0083d0f8[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197d1
const tNetMsg::tMapItem *tNetMsg_kLayMine::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d0f8;
}
