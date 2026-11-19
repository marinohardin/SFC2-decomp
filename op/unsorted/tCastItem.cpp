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

// FUNCTION: STARFLEETOP 0x00677e30
tCastItem::~tCastItem()
{
}
