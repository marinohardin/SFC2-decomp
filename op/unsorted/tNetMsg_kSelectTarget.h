#ifndef TNETMSG_KSELECTTARGET_H
#define TNETMSG_KSELECTTARGET_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kSelectTarget: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
