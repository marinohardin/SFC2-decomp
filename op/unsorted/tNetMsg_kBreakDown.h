#ifndef TNETMSG_KBREAKDOWN_H
#define TNETMSG_KBREAKDOWN_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kBreakDown: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
