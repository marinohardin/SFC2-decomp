#include "unsorted/tNetMsg_kCaptureShip.h"

extern char g_0083d140[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197e9
const tNetMsg::tMapItem *tNetMsg_kCaptureShip::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d140;
}
