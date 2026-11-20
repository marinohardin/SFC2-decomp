#define tgfsys_copies_out_of_line
#include "Tiger/tgfsys.h"

// FUNCTION: STARFLEETOP 0x00407c17
iTruth tCookie::operator==( const tCookie &other ) const
{
	return !( *this != other );
}

// FUNCTION: STARFLEETOP 0x00407c29
iTruth tCookie::operator<( const tCookie &other ) const
{
	return fCookie < other.fCookie;
}

// FUNCTION: STARFLEETOP 0x00407c5b
fn_SessionCookie::fn_SessionCookie( const fn_SessionCookie &other )
{
	fCookie = other.fCookie;
	fSession = other.fSession;
}
