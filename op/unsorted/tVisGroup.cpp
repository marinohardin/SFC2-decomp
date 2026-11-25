// tVisGroup.cpp: the class declaration only; the functions are written one at a time.

#include "unsorted/tVisGroup.h"

// FUNCTION: STARFLEETOP 0x0067da80
tVisGroup::tVisGroup()
{
	field_40 |= 8;
}

// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no source line of
// its own; it is written out as a member that calls the destructor, then deletes when bit 0
// of the flag is set.
// FUNCTION: STARFLEETOP 0x0067dba0
tVisGroup *tVisGroup::scalar_deleting_destructor( unsigned char flags )
{
	tVisGroup::~tVisGroup();
	if ( flags & 1 )
		::operator delete( this );
	return this;
}

// FUNCTION: STARFLEETOP 0x0067dc30
tVisGroup::~tVisGroup()
{
}

// FUNCTION: STARFLEETOP 0x0067dcf0
void tVisGroup::vfunction40()
{
	field_68.fn_0067f090( this, 0, 0 );
}

// FUNCTION: STARFLEETOP 0x0067dd00
bool tVisGroup::vfunction31( int *event )
{
	if ( *event == 0x15 )
	{
		if ( field_68.fn_0067f010( this, event + 1 ) )
			return true;
	}
	return fn_0067d1c0( event );
}
