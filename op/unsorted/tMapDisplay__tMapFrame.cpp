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

// FUNCTION: STARFLEETOP 0x0046c436
int tObjBase::vfunction10()
{
	if ( field_0C && field_0C->field_30 == this && !field_30 )
	{
		return 1;
	}
	return 0;
}

// HYPOTHESIS: the data at 0x00c692a8 is a global object of unknown type.
extern char g_00c692a8;

// FUNCTION: STARFLEETOP 0x0046c452
void *tObjBase::vfunction23()
{
	return &g_00c692a8;
}
