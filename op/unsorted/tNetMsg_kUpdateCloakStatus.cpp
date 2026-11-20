#include "unsorted/tNetMsg_kUpdateCloakStatus.h"

extern char g_0083d16c[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x0041980f
const tNetMsg::tMapItem *tNetMsg_kUpdateCloakStatus::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d16c;
}
