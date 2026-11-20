#ifndef TNETMSG_KTRANSPORTBOMB_H
#define TNETMSG_KTRANSPORTBOMB_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kTransportBomb: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
