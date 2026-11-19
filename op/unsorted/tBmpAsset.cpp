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

// HYPOTHESIS: 0x00695180 releases the buffer held in field_66; 0x00678b10 is a second out-of-line
// copy of the tCastItem destructor body, emitted for the tBmpAsset unit.
void fn_00695180( void *p ) throw();

// FUNCTION: STARFLEETOP 0x00678b10
tCastItem::~tCastItem()
{
	if ( field_66 )
		fn_00695180( (void*)field_66 );
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
