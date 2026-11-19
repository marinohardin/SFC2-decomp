#ifndef TNETMSG_KADJUSTSPEED_H
#define TNETMSG_KADJUSTSPEED_H

#include "Source/network/NetMsgClass.h"

// HYPOTHESIS: one float (a speed) follows the vtable pointer.
// tAIHelmOfficer.cpp keeps its own reduced declaration of this class.
struct tNetMsg_kAdjustSpeed: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
