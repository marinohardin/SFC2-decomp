#include "unsorted/tNetMsg_kKillEntity.h"

extern char g_0083bfd4[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x00410a46
const tNetMsg::tMapItem *tNetMsg_kKillEntity::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083bfd4;
}
