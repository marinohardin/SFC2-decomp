#include "unsorted/tNetMsg_kHoldPosition.h"

extern char g_0083c83c[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x00414b76
const tNetMsg::tMapItem *tNetMsg_kHoldPosition::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083c83c;
}
