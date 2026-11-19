#define tBmpAsset_out_of_line 1
#include "unsorted/tAsset.h"

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
