#include "Tiger/tgfsys.h"

// FUNCTION: STARFLEETOP 0x00407c3a
tPath::tPath( const tPath &other ): fn_SessionCookie( other )
{
	fSession = other.fSession;
}

// FUNCTION: STARFLEETOP 0x00407c81
iTruth tSession::mValid( void )
{
	return fCookie != kInvalidCookie;
}
