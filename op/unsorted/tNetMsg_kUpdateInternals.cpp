#include "unsorted/tNetMsg_kUpdateInternals.h"

extern char g_0083d160[];

// FUNCTION: STARFLEETOP 0x004197fb
tNetMsg_kUpdateInternals::~tNetMsg_kUpdateInternals()
{
	fStream.fn_0040d692();
}

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x00419809
const tNetMsg::tMapItem *tNetMsg_kUpdateInternals::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d160;
}
