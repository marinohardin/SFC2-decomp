#include "unsorted/tNetMsg_kSelectTarget.h"

extern char g_0083d10c[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197d7
const tNetMsg::tMapItem *tNetMsg_kSelectTarget::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d10c;
}
