#ifndef TNETMSG_KMOVEINDIRECTION_H
#define TNETMSG_KMOVEINDIRECTION_H

#include "Source/network/NetMsgClass.h"

// HYPOTHESIS: the turn message 0x005169e0 builds on the stack (vftable 0x00796efc): a heading
// and a turn direction (-1, 0 or 1). Its map is at 0x00414b7c.
struct tNetMsg_kMoveInDirection: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	short field_0x4;
	char field_0x6;
};

#endif
