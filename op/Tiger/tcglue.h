//	tcglue.h: clib, the Tiger library's interface to a few stateless C library routines.

#pragma once

#ifndef	tcglue_h
#define	tcglue_h	1

#include "Tiger/tbasic.h"

class	clib
{
	public:

		typedef unsigned size_t;

		static	void	memset( void *destPtr, int8u value, iCount count );
		static  void	*memcpy( void *dest, const void *src, size_t count );
		static  int		memcmp( void *dest, const void *src, size_t count );
		static	void	strcpy( char *dest, const char *source);
		static	iCount	strlen( const char *string );

};

#endif
