// game/port/types.h: the types game/ source is written in, for both compile modes (DESIGN §7.2,
// CONVENTIONS §4.1). Not original. Included by every unit's header; the native build also
// force-includes it first.
//
//   VC6 SP4 (_MSC_VER 1200)   plain typedefs and macros that expand to the original's spelling, so
//                             the byte check sees exactly the code the original compiled
//   guest mode (SFCOP_GUEST)  clang compiles the same source into the native build, against the
//                             32-bit guest memory of the lifted game (port/guest.h)
//
// The spellings the source uses, and what each is in the two modes:
//
//   int8 .. int32u            fixed-width integers; int32 is `long` under VC6 (the original's
//                             mangled names say so) and 32 bits in guest mode, where `long` is 64
//   ptr32( T )                a pointer stored in guest memory (a member, a global, an element):
//                             `T*` under VC6, a 32-bit guest address in guest mode. A function-like
//                             macro because VC6 has no alias templates
//   SFCOP_VFPTR               first line of a polymorphic root class: nothing under VC6, the 32-bit
//                             vtable pointer in guest mode, where `virtual` is defined away
//   SFCOP_PURE                in place of `= 0` on a pure virtual declaration (guest mode cannot
//                             declare a non-virtual member pure)
#if !defined( SFCOP_PORT_TYPES_H )
#define SFCOP_PORT_TYPES_H

#if defined( _MSC_VER ) && _MSC_VER == 1200

typedef signed char		int8;
typedef unsigned char	int8u;
typedef signed short	int16;
typedef unsigned short	int16u;
typedef signed long		int32;
typedef unsigned long	int32u;

#define ptr32( T )		T*
#define SFCOP_VFPTR
#define SFCOP_PURE		= 0

#elif defined( SFCOP_GUEST )

#include "port/guest.h"

#else
#error "game/ compiles under VC6 SP4 or in guest mode (SFCOP_GUEST); see game/port/types.h"
#endif

#endif
