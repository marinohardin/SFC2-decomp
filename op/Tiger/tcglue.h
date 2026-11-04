//	tcglue.h: clib, the Tiger library's interface to a few stateless C library routines.

#pragma once

#ifndef	tcglue_h
#define	tcglue_h	1

// The original includes tbasic.h here. The unit map has no place for a header without a unit of
// its own yet, so the declarations this unit needs from it are kept here under its guard.
#ifndef	tbasic_h
#define	tbasic_h	1

typedef	signed long		int32;
typedef	signed long		int32s;
typedef	unsigned long	int32u;

typedef	signed short	int16;
typedef	signed short	int16s;
typedef	unsigned short	int16u;

typedef	signed char		int8;
typedef	signed char		int8s;
typedef unsigned char	int8u;

typedef	int32s			iCount;
typedef	int32s			iIndex;

typedef	int32u			iTag;
typedef	int8u			iTruth;

#define	kFalse	0
#define	kTrue	1

#ifndef NULL
#define NULL    0
#endif

class	GenericException {};
#ifdef assert
#undef assert
#endif
#define	assert(x)	{ if (!(x)) {throw GenericException();} }

#endif	// end tbasic_h block

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
