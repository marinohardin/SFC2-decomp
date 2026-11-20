#ifndef TNETMSG_KMOVEINDIRECTION_H
#define TNETMSG_KMOVEINDIRECTION_H

#include "Source/network/NetMsgClass.h"

// HYPOTHESIS: a heading and a turn direction (-1, 0 or 1) follow the vtable pointer.
// tAIHelmOfficer.cpp keeps its own reduced declaration of this class.
struct tNetMsg_kMoveInDirection: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
