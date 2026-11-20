#ifndef TNETMSG_KLAYMINE_H
#define TNETMSG_KLAYMINE_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kLayMine: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
