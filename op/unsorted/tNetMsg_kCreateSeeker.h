//	tNetMsg_kCreateSeeker.h: tNetMsg_kCreateSeeker, the network message that creates a seeker.
// Evidence:  class (RTTI; vtable 0x00797164) and the message map at 0x0083d0ac

#ifndef tNetMsg_kCreateSeeker_h
#define tNetMsg_kCreateSeeker_h 1

#include "Source/network/NetMsgClass.h"

// The vtable has three slots: the map getter (0x004197bf, which returns 0x0083d0ac) and the two
// stream slots it shares with every message (0x00434cff, 0x00434d0e). The map lists seven
// fields: 4 bytes at +4 and +8, one byte each at +0xc, +0xd and +0xe, two bytes at +0x10, one
// byte at +0x12. The functions that send it build it as a 0x14-byte local; every one stores
// +4 (an entity ID), +8 (the next player ID, from 0x004239d6 on the entity manager at
// 0x008e1060) and the bytes at +0xc, +0xd (0xff) and +0xe, and calls 0x0043405f through the
// net controller at 0x008f5b98 with 0x0b.
// Several of the functions that the symbol table lists under this class's destructor (0x00439ed0,
// 0x0043a0db, 0x004503ae and others) are methods of the classes that send the message; they
// only have a local of this type.
struct tNetMsg_kCreateSeeker: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	int field_04;
	int field_08;
	// HYPOTHESIS: the command byte; the senders store 0x12, 0x32 and 0x33 here.
	unsigned char field_0c;
	unsigned char field_0d;
	unsigned char field_0e;
	short field_10;
	unsigned char field_12;
};

typedef char tNetMsg_kCreateSeeker_size_check[ sizeof( tNetMsg_kCreateSeeker ) == 0x14 ? 1 : -1 ];

#endif	// tNetMsg_kCreateSeeker_h
