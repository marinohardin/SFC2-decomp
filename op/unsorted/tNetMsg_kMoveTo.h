#ifndef TNETMSG_KMOVETO_H
#define TNETMSG_KMOVETO_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kMoveTo: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
