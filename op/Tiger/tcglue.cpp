//	tcglue.cpp: clib, the C library routines behind the Tiger library's glue.

#include "Tiger/tcglue.h"

#include <string.h>


// FUNCTION: STARFLEETOP 0x0062ddb0
void	clib::memset( void *destPtr, int8u value, iCount count )
{
	::memset( destPtr, value, count );
}

// FUNCTION: STARFLEETOP 0x0062dde0
void	clib::strcpy( char *dest, const char *source )
{
	::strcpy( dest, source );
}

// FUNCTION: STARFLEETOP 0x0062de10
void	*clib::memcpy( void *dest, const void *src, size_t count )
{
	return ::memcpy( dest, src, count );
}
