#ifndef TNETMSG_KADJUSTSPEED_H
#define TNETMSG_KADJUSTSPEED_H

#include "Source/network/NetMsgClass.h"

// HYPOTHESIS: one float (a speed) follows the vtable pointer.
// tAIHelmOfficer.cpp builds it on the stack (vftable 0x007971a4); its map is at 0x0041978f.
struct tNetMsg_kAdjustSpeed: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	float field_0x4;
};

#endif
