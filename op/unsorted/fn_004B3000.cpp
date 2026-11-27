#include "unsorted/tState_Tactical.h"
#include "unsorted/tObject_0092deb4.h"

// HYPOTHESIS: an input event record: a kind at +0x00, flags at +0x14 and two longs at +0x18
// and +0x1c; the kind is rewritten in place. 0x004bc55a takes the two longs and fills a
// 12-byte buffer on the caller's stack.

namespace tState_Namespace_Tactical
{

// FUNCTION: STARFLEETOP 0x004b35db
void tState_Tactical::fn_004b35db( long *event )
{
	switch ( event[0] )
	{
	case 2:
		if ( event[5] & 1 )
			event[0] = 0x10025;
		else
			event[0] = ( event[5] & 2 ) ? 0x10024 : 0x10023;
		break;
	case 6:
		event[0] = 0x10027;
		if ( event[5] & 1 )
		{
			char buffer[12];
			fn_004bc55a( event[6], event[7], buffer );
		}
		break;
	}
}

// FUNCTION: STARFLEETOP 0x004b3758
void tState_Tactical::fn_004b3758( long *event )
{
	switch ( event[0] )
	{
	case 2:
		event[0] = 0x10028;
		field_470 = 0;
		g_00898c24->vfunction10( gObject_0092deb4->vfunction39() );
		break;
	}
}

}	// namespace tState_Namespace_Tactical
