#include "unsorted/tNetMsg_kSendVoice.h"

extern char g_0083d150[];

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004197f5
const tNetMsg::tMapItem *tNetMsg_kSendVoice::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083d150;
}
