#include "unsorted/tNetMsg_kMoveTo.h"

extern char g_0083c850[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x00414b82
const tNetMsg::tMapItem *tNetMsg_kMoveTo::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083c850;
}
