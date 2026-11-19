#ifndef TNETMSG_KSHIPCANTARGET_H
#define TNETMSG_KSHIPCANTARGET_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kShipCanTarget: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
