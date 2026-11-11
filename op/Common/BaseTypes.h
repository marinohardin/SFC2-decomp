//	BaseTypes.h
//
//	The integer spellings every Common header is written in.
//	Declarations follow the SDK's header. Not declared yet: the 64-bit integer types, which no
//	unit uses yet and which guest mode would need from port/types.h.
//
#if !defined( BaseTypes_h )
#define BaseTypes_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "port/types.h"	// PORT: int8 .. int32u come from port/types.h, 32 bits in both compile modes

typedef	int32			int32s;
typedef	int16			int16s;
typedef	int8			int8s;

typedef int8u			byte;

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )

#endif
