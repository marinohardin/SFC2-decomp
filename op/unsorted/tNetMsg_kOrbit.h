#ifndef TNETMSG_KORBIT_H
#define TNETMSG_KORBIT_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kOrbit: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
