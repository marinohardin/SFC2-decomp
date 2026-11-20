#ifndef TNETMSG_KCORRECTPOS_H
#define TNETMSG_KCORRECTPOS_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kCorrectPos: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
