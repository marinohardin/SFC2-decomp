#ifndef TNETMSG_KKILLENTITY_H
#define TNETMSG_KKILLENTITY_H

#include "Source/network/NetMsgClass.h"

// HYPOTHESIS: one one-byte field, set to 1 by 0x004139ec; its meaning is not known.
// Source/entity/entity_physical.cpp still holds its own copy of this declaration; it should
// include this header instead.
struct tNetMsg_kKillEntity: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
	bool field_0x4;
};

#endif
