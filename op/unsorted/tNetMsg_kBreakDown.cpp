#include "unsorted/tNetMsg_kBreakDown.h"

extern char g_0083d028[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197a1
const tNetMsg::tMapItem *tNetMsg_kBreakDown::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d028;
}
