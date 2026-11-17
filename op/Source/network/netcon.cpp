// Unit:      Source/network/netcon.cpp
// Functions: 0x0043405f to 0x00434195, 2 in op/UNITS.tsv
// Evidence:  assert-path (the original's assertion strings name this file)
// Profile:   game-release (harness/units.json)

#include "Source/network/NetMsgClass.h"
#include "dmp3d/common/stream_basicfile.h"
#include "unsorted/tNetControl.h"

// The header streamed in front of every queued message (RTTI .?AUtNetMsg_Header@@, vtable
// 0x0079867c). HYPOTHESIS: field names; types are from mQueueMsg's stores.
struct tNetMsg_Header: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	double field_8;
	unsigned char field_10;
	int field_14;
};

// HYPOTHESIS: the 8-byte entries at 0x008f5b48, one per player, the first used when
// fn_004326a4 returns 8.
struct c_008f5b48_entry
{
	int field_0;
	int field_4;
};
extern c_008f5b48_entry g_008f5b48[];

// HYPOTHESIS: the object fn_004308b2 builds from a streamed buffer; fn_0043080d sends it.
class c_004308b2
{
public:
	void fn_0043080d( c_008f5b48_entry *entry );
};
c_004308b2 *fn_004308b2( int size, unsigned char *buffer );

// HYPOTHESIS: the current network time.
unsigned long fn_00432316();

// FUNCTION: STARFLEETOP 0x0043405f
void tNetControl::mQueueMsg(tNetMsg *aMsg, int aCommand, int aDestination, double aTime)
{
	unsigned char buffer[ 0x440 ];
	tNetMsg_Header header;

	if( aTime == -1.0 )
		header.field_8 = fn_00432316();
	else
		header.field_8 = fn_00432316() + aTime;
	header.field_10 = aCommand;
	header.field_14 = aDestination;

	unsigned char *p = buffer + header.mStreamTo( buffer );
	if( aMsg )
		p += aMsg->mStreamTo( p );
	int size = p - buffer;
	if( size > 0x400 )
	{
#line 3504
		throw fn_006a2ec0( __FILE__, __LINE__, "tNetControl::mQueueMsg(): Message buffer overrun\n" );
#line 63
	}

	if( fn_004326a4() != 8 )
	{
		// HYPOTHESIS: c_0043127f_node is declared only in tNetControl.cpp; its next pointer
		// is at +0x0 and its player number (field_68) at +0x68.
		for( int *node = (int *)field_8CB8; node; node = (int *)node[ 0 ] )
		{
			c_004308b2 *msg = fn_004308b2( size, buffer );
			if( msg )
			{
				int player = node[ 0x68 / 4 ] - 1;
				if( player >= 0 )
					msg->fn_0043080d( &g_008f5b48[ player ] );
			}
		}
	}
	else
	{
		c_004308b2 *msg = fn_004308b2( size, buffer );
		if( msg )
			msg->fn_0043080d( &g_008f5b48[ 0 ] );
	}
}

// FUNCTION: STARFLEETOP 0x00434195
void tNetControl::mQueueMsg(tNetMsg *aMsg, int aCommand, int aPlayer, int aDestination, double aTime)
{
	unsigned char buffer[ 0x440 ];
	tNetMsg_Header header;

	if( aTime == -1.0 )
		header.field_8 = fn_00432316();
	else
		header.field_8 = fn_00432316() + aTime;
	header.field_10 = aCommand;
	header.field_14 = aDestination;

	unsigned char *p = buffer + header.mStreamTo( buffer );
	if( aMsg )
		p += aMsg->mStreamTo( p );
	int size = p - buffer;
	if( size > 0x400 )
	{
#line 3588
		throw fn_006a2ec0( __FILE__, __LINE__, "tNetControl::mQueueMsg(): Message buffer overrun\n" );
#line 110
	}

	c_004308b2 *msg = fn_004308b2( size, buffer );
	if( msg )
		msg->fn_0043080d( &g_008f5b48[ aPlayer ] );
}
