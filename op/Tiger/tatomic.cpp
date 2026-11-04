//	tatomic.cpp: tAtomicLock, a one-bit lock taken with an interlocked bit test and set.

#include "Tiger/tcglue.h"

// The unit map gives this unit no header; the class is declared here.
class	tAtomicLock
{
	public:
						tAtomicLock		( void );
		virtual			~tAtomicLock	( void );

				iTruth	mAcquire		( void );
				void	mRelease		( void );

	protected:
		int8u			fReserved[ 28 ];	// HYPOTHESIS: cleared by the constructor, not read in this unit
		int32u			fLock;
};


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
