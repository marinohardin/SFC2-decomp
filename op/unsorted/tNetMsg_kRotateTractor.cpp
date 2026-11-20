#include "unsorted/tNetMsg_kRotateTractor.h"

extern char g_0083d148[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197ef
const tNetMsg::tMapItem *tNetMsg_kRotateTractor::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d148;
}
