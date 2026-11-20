#ifndef TNETMSG_KWARPOUT_H
#define TNETMSG_KWARPOUT_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kWarpOut: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
