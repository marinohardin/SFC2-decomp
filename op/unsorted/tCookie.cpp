#include "Tiger/tgfsys.h"

// FUNCTION: STARFLEETOP 0x00407c17
iTruth tCookie::operator==( const tCookie &other ) const
{
	return !( *this != other );
}
