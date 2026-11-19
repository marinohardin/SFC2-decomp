#ifndef TNETMSG_KCAPTURESHIP_H
#define TNETMSG_KCAPTURESHIP_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kCaptureShip: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
