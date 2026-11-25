// tVisGroup.cpp: the class declaration only; the functions are written one at a time.

#include "unsorted/tVisGroup.h"

// FUNCTION: STARFLEETOP 0x0067dd00
bool tVisGroup::vfunction31( int *event )
{
	if ( *event == 0x15 )
	{
		if ( ( (c_tVisGroup_Field68 *)field_68 )->fn_0067f010( this, event + 1 ) )
			return true;
	}
	return fn_0067d1c0( event );
}
