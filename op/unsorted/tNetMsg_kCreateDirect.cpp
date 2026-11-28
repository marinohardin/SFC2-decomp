#include "unsorted/tNetMsg_kCreateDirect.h"

extern char g_0083d06c[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197b3
const tNetMsg::tMapItem *tNetMsg_kCreateDirect::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d06c;
}
