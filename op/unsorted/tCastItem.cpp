#include "unsorted/tAsset.h"
#include "Tiger/tStream.h"

// FUNCTION: STARFLEETOP 0x00677d20
tStream& tCastItem::vfunction5( tStream& s )
{
	tAsset::vfunction5( s );
	s << (unsigned long)field_5C << (unsigned long)field_62;
	return s;
}

// FUNCTION: STARFLEETOP 0x00677db0
tStream& tCastItem::vfunction6( tStream& s )
{
	tAsset::vfunction6( s );
	unsigned short* p = (unsigned short*)&field_5C;
	s.fn_slot2( p, 4 );
	if ( s.fSwap )
	{
		unsigned short hi = p[1];
		unsigned short lo = p[0];
		p[0] = (unsigned short)( ( hi >> 8 ) | ( hi << 8 ) );
		p[1] = (unsigned short)( ( lo >> 8 ) | ( lo << 8 ) );
	}
	unsigned short* q = (unsigned short*)&field_62;
	s.fn_slot2( q, 4 );
	if ( s.fSwap )
	{
		unsigned short hi = q[1];
		unsigned short lo = q[0];
		q[0] = (unsigned short)( ( hi >> 8 ) | ( hi << 8 ) );
		q[1] = (unsigned short)( ( lo >> 8 ) | ( lo << 8 ) );
	}
	return s;
}

// HYPOTHESIS: 0x00695180 releases the buffer held in field_62.
void fn_00695180( void *p ) throw();

// FUNCTION: STARFLEETOP 0x00677e30
// FUNCTION: STARFLEETOP 0x00677e50
tCastItem::~tCastItem()
{
	if ( field_66 )
		fn_00695180( (void*)field_66 );
}
