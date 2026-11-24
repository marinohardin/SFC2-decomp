#define tBmpAsset_out_of_line 1
#include "unsorted/tAsset.h"
#include "Tiger/tStream.h"

// FUNCTION: STARFLEETOP 0x00678ae0
tBmpAsset::tBmpAsset()
{
	field_6A = 0xff;
	field_6C = 0xff;
	field_6B = 0;
}

// FUNCTION: STARFLEETOP 0x00678cc0
// FUNCTION: STARFLEETOP 0x00678b10
tBmpAsset::~tBmpAsset()
{
}

// FUNCTION: STARFLEETOP 0x00678b80
tStream& tBmpAsset::vfunction5( tStream& s )
{
	// MATCH-ONLY: the original expands tCastItem::vfunction5 (0x00677d20) here; the profile does
	// not inline it across units, so its body is written out.
	tAsset::vfunction5( s );
	s << (unsigned long)field_5C << (unsigned long)field_62;
	// MATCH-ONLY: the second byte goes to the stream the first write returns; the shared
	// operator<<( char ) returns *this, so the chain is written through slot 3.
	tStream* t;
	{
		char c = field_6A;
		t = &s.fn_slot3( &c, 1 );
	}
	{
		char c = field_6B;
		t->fn_slot3( &c, 1 );
	}
	return s;
}

void fn_00676a00( unsigned short* p );

// FUNCTION: STARFLEETOP 0x00678c40
tStream& tBmpAsset::vfunction6( tStream& s )
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
		fn_00676a00( q );
	s.fn_slot2( &field_6A, 1 ).fn_slot2( &field_6B, 1 );
	return s;
}
