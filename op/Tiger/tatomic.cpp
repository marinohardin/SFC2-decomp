//	tatomic.cpp: tAtomicLock, a one-bit lock taken with an interlocked bit test and set.

#include "Tiger/tatomic.h"

// MATCH-ONLY: tatomic.h now declares tAtomicLock; these lines are kept so line numbers stay.















// Returns the previous value of bit 0 of *lockPtr after setting it, in eax.
static inline int	atomicTestAndSet( int32u *lockPtr )
{
	__asm
	{
		push	ecx
		mov		eax, 0
		mov		ecx, lockPtr
		lock bts	word ptr [ecx], 0
		jnc		wasClear
		mov		eax, 1
	wasClear:
		pop		ecx
	}
}


// FUNCTION: STARFLEETOP 0x00628e90
tAtomicLock::tAtomicLock( void )
{
	clib::memset( fReserved, 0, sizeof(fReserved) );
	fLock = 0;
}

// FUNCTION: STARFLEETOP 0x00628ee0
tAtomicLock::~tAtomicLock( void )
{
}

// FUNCTION: STARFLEETOP 0x00628ef0
iTruth	tAtomicLock::mAcquire( void )
{
	iTruth	acquired = !atomicTestAndSet( &fLock );
	return acquired;
}

// FUNCTION: STARFLEETOP 0x00628f20
void	tAtomicLock::mRelease( void )
{
	fLock = 0;
}
