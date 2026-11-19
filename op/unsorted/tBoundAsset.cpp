#include "unsorted/tAsset.h"
#include "Tiger/tStream.h"

// FUNCTION: STARFLEETOP 0x00678180
tStream& tBoundAsset::vfunction5( tStream& s )
{
	tAsset::vfunction5( s );
	s << (unsigned short)field_5C << (unsigned short)field_5E;
	return s;
}

// FUNCTION: STARFLEETOP 0x006781f0
tStream& tBoundAsset::vfunction6( tStream& s )
{
	tAsset::vfunction6( s );
	unsigned short* p = (unsigned short*)&field_5C;
	s.fn_slot2( p, 2 );
	if ( s.fSwap )
		*p = (unsigned short)( ( *p >> 8 ) | ( *p << 8 ) );
	unsigned short* q = (unsigned short*)&field_5E;
	s.fn_slot2( q, 2 );
	if ( s.fSwap )
		*q = (unsigned short)( ( *q >> 8 ) | ( *q << 8 ) );
	return s;
}
