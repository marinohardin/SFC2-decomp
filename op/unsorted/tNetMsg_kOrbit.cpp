#include "unsorted/tNetMsg_kOrbit.h"

extern char g_0083c858[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x00414b88
const tNetMsg::tMapItem *tNetMsg_kOrbit::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083c858;
}
