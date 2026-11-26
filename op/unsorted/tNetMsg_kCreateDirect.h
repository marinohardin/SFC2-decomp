#ifndef TNETMSG_KCREATEDIRECT_H
#define TNETMSG_KCREATEDIRECT_H

#include "Source/network/NetMsgClass.h"

// The vtable is at 0x00797204 (mGetMap 0x004197b3, then tNetMsg's two stream functions). The
// fields are read from the message's map at 0x0083d06c, whose entries are (size, offset):
// 4@4, 4@8, 1@12, 1@13, 1@14, 4@16, 1@20. What each one means is not known.
struct tNetMsg_kCreateDirect: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	int field_0x4;
	int field_0x8;
	unsigned char field_0xc;
	unsigned char field_0xd;
	unsigned char field_0xe;
	int field_0x10;
	unsigned char field_0x14;
};

// The map ends at 0x14 + 1; the next 4-byte boundary is the object's size.
typedef char tNetMsg_kCreateDirect_size_is_0x18[sizeof(tNetMsg_kCreateDirect) == 0x18 ? 1 : -1];

#endif
