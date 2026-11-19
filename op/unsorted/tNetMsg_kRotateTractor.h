#ifndef TNETMSG_KROTATETRACTOR_H
#define TNETMSG_KROTATETRACTOR_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kRotateTractor: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
