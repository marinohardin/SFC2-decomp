#ifndef TNETMSG_KSENDVOICE_H
#define TNETMSG_KSENDVOICE_H

#include "Source/network/NetMsgClass.h"

struct tNetMsg_kSendVoice: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
};

#endif
