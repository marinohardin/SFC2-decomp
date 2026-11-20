#include "unsorted/tNetMsg_kCorrectPos.h"

extern char g_0083d03c[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197ad
const tNetMsg::tMapItem *tNetMsg_kCorrectPos::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d03c;
}
