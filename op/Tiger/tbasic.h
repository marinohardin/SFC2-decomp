//	tbasic.h: the integer and truth types, and the assert, every Tiger header is written in.
//
//	Declarations follow the SDK's header. Not declared yet: the byte-order helpers and the tPoint
//	and tRect structs, whose inline functions no unit here uses yet, and the 64-bit integer
//	types, which no unit uses yet and which guest mode would need from port/types.h.

#pragma once

#ifndef	tbasic_h
#define	tbasic_h	1

#include "port/types.h"	// PORT: int8 .. int32u come from port/types.h, 32 bits in both compile modes

typedef	int32			int32s;
typedef	int16			int16s;
typedef	int8			int8s;

typedef	int32s			iCount;
typedef	int32s			iIndex;

typedef	int32u			iTag;
typedef	int8u			iTruth;

typedef	double			iTime;

typedef	int16u			iWideChar;

#define	kFalse	0
#define	kTrue	1

#ifndef	nil
	#define	nil ((void*)0L)
#endif

#ifndef NULL
#define NULL    0
#endif

class	GenericException {};
#ifdef assert
#undef assert
#endif
#define	assert(x)	{ if (!(x)) {throw GenericException();} }

#endif	// end tbasic_h block
