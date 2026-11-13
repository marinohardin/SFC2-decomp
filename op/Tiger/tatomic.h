//	tatomic.h: tAtomicLock, a one-bit lock taken with an interlocked bit test and set.

#ifndef tatomic_h
#define tatomic_h

#include "Tiger/tcglue.h"

class	tAtomicLock
{
	public:
						tAtomicLock		( void );
		virtual			~tAtomicLock	( void );

				iTruth	mAcquire		( void );
				void	mRelease		( void );

	protected:
		// HYPOTHESIS: tgSoundPart reads the lock word of the locks it holds.
		friend class	tgSoundPart;

		int8u			fReserved[ 28 ];	// HYPOTHESIS: cleared by the constructor, not read in tatomic.cpp
		int32u			fLock;
};

#endif
