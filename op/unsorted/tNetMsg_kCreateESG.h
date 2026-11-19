#ifndef TNETMSG_KCREATEESG_H
#define TNETMSG_KCREATEESG_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kCreateESG: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
