#ifndef TNETMSG_KSHIPTARGETABLE_H
#define TNETMSG_KSHIPTARGETABLE_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kShipTargetable: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
