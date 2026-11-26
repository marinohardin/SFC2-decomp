#include "unsorted/tVisObject.h"

// FUNCTION: STARFLEETOP 0x0046c408
void tObjBase::vfunction1( unsigned int n )
{
	field_40 = ( ( n & 1 ) << 3 ) | ( field_40 & ~8 );
	if ( ( field_40 & 8 ) && field_0C )
	{
		field_0C->vfunction1( ( field_40 << 28 ) >> 31 );
	}
}
