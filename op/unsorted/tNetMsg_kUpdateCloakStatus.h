#ifndef TNETMSG_KUPDATECLOAKSTATUS_H
#define TNETMSG_KUPDATECLOAKSTATUS_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kUpdateCloakStatus: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
