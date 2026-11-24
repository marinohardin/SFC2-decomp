#include "Tiger/tgfsys.h"

// FUNCTION: STARFLEETOP 0x00407c81
iTruth tSession::mValid( void )
{
	return fCookie != kInvalidCookie;
}

// MATCH-ONLY: the original's instances of tPath's copy constructor and destructor (0x00407c3a,
// 0x00407c8b) and of the cookie's copy constructor (0x00407c5b) are the compiler's copies of the
// inline functions in Tiger/tgfsys.h; with inline expansion off, a copy and its destruction make
// this unit emit them. Not a function of the original.
#pragma inline_depth( 0 )
void fn_emit_tPath_copies( tPath &path )
{
	tPath copy( path );
}
#pragma inline_depth()
