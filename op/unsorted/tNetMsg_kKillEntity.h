#ifndef TNETMSG_KKILLENTITY_H
#define TNETMSG_KKILLENTITY_H

#include "Source/network/NetMsgClass.h"

// HYPOTHESIS: one one-byte field, set to 1 by 0x004139ec; its meaning is not known.
struct tNetMsg_kKillEntity: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
	bool field_0x4;
};

#endif
