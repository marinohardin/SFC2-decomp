#include "unsorted/tNetMsg_kCreateESG.h"

extern char g_0083d08c[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197b9
const tNetMsg::tMapItem *tNetMsg_kCreateESG::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d08c;
}
