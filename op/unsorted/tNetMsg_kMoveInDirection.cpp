#include "unsorted/tNetMsg_kMoveInDirection.h"

extern char g_0083c844[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x00414b7c
const tNetMsg::tMapItem *tNetMsg_kMoveInDirection::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083c844;
}
