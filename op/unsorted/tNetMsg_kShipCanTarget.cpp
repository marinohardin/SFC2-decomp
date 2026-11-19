#include "unsorted/tNetMsg_kShipCanTarget.h"

extern char g_0083d020[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x0041979b
const tNetMsg::tMapItem *tNetMsg_kShipCanTarget::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d020;
}
