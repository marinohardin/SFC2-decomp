#include "unsorted/tNetMsg_kAdjustSpeed.h"

extern char g_0083d010[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x0041978f
const tNetMsg::tMapItem *tNetMsg_kAdjustSpeed::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d010;
}
