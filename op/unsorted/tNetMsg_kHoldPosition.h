#ifndef TNETMSG_KHOLDPOSITION_H
#define TNETMSG_KHOLDPOSITION_H

#include "Source/network/NetMsgClass.h"

// HYPOTHESIS: layout beyond the vtable is not known.
struct tNetMsg_kHoldPosition: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
