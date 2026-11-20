#ifndef TNETMSG_KUPDATEINTERNALS_H
#define TNETMSG_KUPDATEINTERNALS_H

#include "Source/network/NetMsgClass.h"
#include "unsorted/sfcstreams.h"

struct tNetMsg_kUpdateInternals: public tNetMsg
{
	virtual ~tNetMsg_kUpdateInternals();
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	// HYPOTHESIS: the stream member at +8 is torn down by tSFCStream::fn_0040d692
	int fField04;
	tSFCStream fStream;
};

#endif
