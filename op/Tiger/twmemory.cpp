//	twmemory.cpp: the Windows memory part.

#include "Tiger/tbasic.h"
#include "Tiger/tpart.h"
#include <windows.h>
#include <stdio.h>

// HYPOTHESIS: tMemoryPart is declared with its part binding (claimed at 0x004133d3) in another
// unit; it is restated here only as far as this unit needs it. Slot 25 (offset 0x64) takes a
// message string.
class tMemoryPart : public tPart { public: GenAbstractBinding( tMemoryPart ); virtual void vfunction6(); virtual void vfunction7(); virtual void vfunction8(); virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11(); virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14(); virtual int vfunction15( long a, long b, char* c ); virtual int vfunction16( long a, long b, char* c ); virtual void vfunction17(); virtual int vfunction18( int handle, long size ); virtual int vfunction19( int handle ); virtual char vfunction20( int handle ); virtual void* vfunction21( int handle ); virtual char vfunction22( int handle ); virtual void vfunction23(); virtual void vfunction24(); virtual void vfunction25( const char* message ); };

extern char g_00c3d5cc;

#include "Tiger/tpaging.h"






























// HYPOTHESIS: one entry of the block table, 0x20 bytes; the word at +0x1c is nonzero while the
// block is allocated.
class twMemoryPart;
class twMemoryBlock
{
	friend class twMemoryPart;
protected:
	unsigned char	mFree	( unsigned char flag );
	short	mCheckForBoundsError	( void );
	void*	mAllocate	( long a, char* b, unsigned long c );
	void*	mReallocate	( long a, unsigned long b );
	void*	mLock	( void );
	unsigned char	mUnlock	( void );
public:
	int				fPad[ 7 ];
	int				fInUse;
};

// HYPOTHESIS: declared only as far as the functions below need; the layout past the part header
// is read from the accesses, and the other virtuals are not recovered.
class twMemoryPart : public tMemoryPart
{
public:
	virtual	void	mDeactivate	( void );
	virtual	void*	mNewBlock	( long a, long b, char* c );
	virtual	void*	mResizeBlock	( void* a, long b );
	virtual	void*	mDuplicateBlock	( void* a );
	virtual	unsigned char	mDelBlock	( void* a );
	virtual	unsigned char	mCheckForBoundsErrors	( void );
	virtual	long	mLargestBlockAvailable	( void );
	virtual	iTruth	mCheckMemory	( void* a, void* b );
	virtual	long	mNewHandle	( long a, long b, char* c );
	virtual	long	mNewHandleZeroIt	( long a, long b, char* c );
	virtual	long	mResizeHandle	( long a, long b );
	virtual	long	mDuplicateHandle	( long a );
	virtual	long	mResizeHandleZeroIt	( long a, long b );
	virtual	unsigned char	mDelHandle	( long a );
	virtual	void*	mLockHandle	( long a );
	virtual	unsigned char	mUnlockHandle	( long a );
	virtual	unsigned char	mRemoveAllLocksOnHandle	( long a );

	tPagingMechanism	fPaging;
	int					fBlockCount;
	int					fAllocCount;
	int					fPad28;
	HGLOBAL				fHandleA;
	HGLOBAL				fHandleB;
	twMemoryBlock*		fMemoryBlocks;

protected:
	unsigned char	mAddMemory	( void );
};

// FUNCTION: STARFLEETOP 0x00637ec0
void	twMemoryPart::mDeactivate( void )
{
	char	message[ 256 ];

	fPaging.mDeactivate();
	if ( fAllocCount > 0 )
	{
		sprintf( message, "You seem to have %ld memory blocks which were not properly deallocated!", fAllocCount );
		BindPart( tMemoryPart )->vfunction25( message );
		if ( fMemoryBlocks == NULL )
		{
			BindPart( tMemoryPart )->vfunction25( "fMemoryBlocks was NULL!" );
			return;
		}
		for ( int i = 0; i < fBlockCount; i++ )
		{
			if ( fMemoryBlocks[ i ].fInUse != 0 )
			{
				fMemoryBlocks[ i ].mFree( 1 );
				fAllocCount--;
			}
		}
	}
	if ( fHandleA != NULL )
	{
		GlobalUnlock( fHandleA );
		if ( GlobalFree( fHandleA ) != NULL )
		{
			FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
			BindPart( tMemoryPart )->vfunction25( message );
			return;
		}
		fHandleA = NULL;
	}
	fHandleA = NULL;
	fMemoryBlocks = NULL;
	if ( fHandleB != NULL )
	{
		GlobalFree( fHandleB );
		fHandleB = NULL;
	}
}

// FUNCTION: STARFLEETOP 0x00638120
unsigned char	twMemoryPart::mAddMemory( void )
{
	char	message[ 256 ];

	if ( fHandleA == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to add memory to a Null Handle!" );
		return 0;
	}
	GlobalUnlock( fHandleA );
	int newCount = fBlockCount + 0x100;
	HGLOBAL handle = GlobalReAlloc( fHandleA, newCount * 0x20, GMEM_MOVEABLE );
	if ( handle == NULL )
	{
		FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	fHandleA = handle;
	fPad28 = fBlockCount;
	fBlockCount = newCount;
	fMemoryBlocks = (twMemoryBlock*)GlobalLock( handle );
	if ( fMemoryBlocks == NULL )
	{
		FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	for ( int i = fPad28; i < fBlockCount; i++ )
	{
		twMemoryBlock*	block = &fMemoryBlocks[ i ];
		block->fPad[ 0 ] = 0;
		block->fPad[ 1 ] = 0;
		block->fPad[ 2 ] = 0;
		block->fPad[ 3 ] = 0;
		block->fPad[ 4 ] = 0;
		block->fInUse = 0;
		block->fPad[ 5 ] = 0;
		block->fPad[ 6 ] = 0;
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x006383a0
void*	twMemoryPart::mNewBlock( long a, long b, char* c )
{
	// HYPOTHESIS: g_00c3d5cc is a global flag that disables block allocation while set.
	if ( g_00c3d5cc == 0 )
	{
		void*	block = fPaging.mNewBlock( a, b, c );
		if ( block != NULL )
		{
			return block;
		}
		int	handle = vfunction16( a, b, c );
		if ( handle == 0 )
		{
			BindPart( tMemoryPart )->vfunction25( "Couldn't Allocate a New Handle!" );
			return 0;
		}
		block = vfunction21( handle );
		if ( block != NULL )
		{
			return block;
		}
		BindPart( tMemoryPart )->vfunction25( "Failed to Lock a Handle!" );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00638500
void*	twMemoryPart::mResizeBlock( void* a, long b )
{
	if ( fPaging.mIsThisAPagingPointer( a ) == 1 )
	{
		return fPaging.mResizeBlock( a, b );
	}
	if ( a == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "Oops -- we were passed a Null pointer!" );
		return 0;
	}
	int	handle = 0;
	if ( fMemoryBlocks != NULL )
	{
		int	i = 0;
		for ( ; i < fBlockCount; i++ )
		{
			if ( (void*)fMemoryBlocks[ i ].fPad[ 3 ] == a )
			{
				handle = i + 1;
				break;
			}
		}
	}
	if ( handle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Couldn't find the Handle for the pointer!" );
		return 0;
	}
	if ( vfunction22( handle ) == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Trouble Unlocking a Handle!" );
		return 0;
	}
	int	newHandle = vfunction18( handle, b );
	if ( newHandle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Couldn't Resize the Handle!" );
		return 0;
	}
	void*	block = vfunction21( newHandle );
	if ( block != NULL )
	{
		return block;
	}
	BindPart( tMemoryPart )->vfunction25( "Failed to Lock a Handle!" );
	return 0;
}

// FUNCTION: STARFLEETOP 0x00638830
void*	twMemoryPart::mDuplicateBlock( void* a )
{
	if ( fPaging.mIsThisAPagingPointer( a ) == 1 )
	{
		return fPaging.mDuplicateBlock( a );
	}
	if ( a == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "Oops -- we were passed a Null pointer!" );
		return 0;
	}
	int	handle = 0;
	if ( fMemoryBlocks != NULL )
	{
		int	i = 0;
		for ( ; i < fBlockCount; i++ )
		{
			if ( (void*)fMemoryBlocks[ i ].fPad[ 3 ] == a )
			{
				handle = i + 1;
				break;
			}
		}
	}
	if ( handle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Couldn't find the Handle for the pointer!" );
		return 0;
	}
	// HYPOTHESIS: the original locks the source handle, not the one vfunction19 returns.
	if ( vfunction19( handle ) == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Couldn't Duplicate the Handle!" );
		return 0;
	}
	void*	block = vfunction21( handle );
	if ( block != NULL )
	{
		return block;
	}
	BindPart( tMemoryPart )->vfunction25( "Failed to Lock a Handle!" );
	return 0;
}

// FUNCTION: STARFLEETOP 0x00638ac0
unsigned char	twMemoryPart::mDelBlock( void* a )
{
	if ( fPaging.mIsThisAPagingPointer( a ) == 1 )
	{
		return fPaging.mDelBlock( a );
	}
	if ( a == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "Oops -- we were passed a Null pointer!" );
		return 0;
	}
	if ( fMemoryBlocks == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to Delete a Block when the MemoryPart is not Valid!" );
		return 0;
	}
	int	handle = 0;
	int	i = 0;
	for ( ; i < fBlockCount; i++ )
	{
		if ( (void*)fMemoryBlocks[ i ].fPad[ 3 ] == a )
		{
			handle = i + 1;
			break;
		}
	}
	if ( handle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Couldn't find the Handle for the pointer!" );
		return 0;
	}
	if ( vfunction22( handle ) == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Trouble Unlocking a Handle!" );
		return 0;
	}
	if ( vfunction20( handle ) == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Couldn't Delete the Handle!" );
		return 0;
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x00638de0
unsigned char	twMemoryPart::mCheckForBoundsErrors( void )
{
	unsigned char	failed = 0;

	for ( int i = 0; i < fBlockCount; i++ )
	{
		if ( fMemoryBlocks[ i ].fInUse != 0 && fMemoryBlocks[ i ].mCheckForBoundsError() != 0 )
		{
			failed = 1;
		}
	}
	if ( fPaging.mCheckForBoundsErrors() == 1 )
	{
		return 1;
	}
	return failed;
}

// FUNCTION: STARFLEETOP 0x00638e30
long	twMemoryPart::mNewHandle( long a, long b, char* c )
{
	int	result = fPaging.mNewHandle( a, b, c );
	if ( result > 0 )
	{
		return result;
	}
	if ( fAllocCount == fBlockCount && mAddMemory() == 0 )
	{
		char	message[ 256 ];
		sprintf( message, "Could not add more memory blocks!" );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	if ( a < 0 )
	{
		char	message[ 256 ];
		sprintf( message, "You are trying to allocate a block of %ld size!", a );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	if ( fMemoryBlocks == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "fMemoryBlocks was NULL!" );
		return 0;
	}
	int	start = fPad28;
	if ( fMemoryBlocks[ fPad28 ].fInUse != 0 )
	{
		fPad28++;
		if ( fPad28 == fBlockCount )
		{
			fPad28 = 0;
		}
		while ( fMemoryBlocks[ fPad28 ].fInUse != 0 && fPad28 != start )
		{
			fPad28 = fPad28 + 1;
			if ( fPad28 == fBlockCount )
			{
				fPad28 = 0;
			}
		}
		if ( fPad28 == start )
		{
			char	message[ 256 ];
			sprintf( message, "Internal Error finding available block!" );
			BindPart( tMemoryPart )->vfunction25( message );
			return 0;
		}
	}
	if ( fMemoryBlocks[ fPad28 ].mAllocate( a, c, 0 ) == NULL )
	{
		if ( fHandleB != NULL )
		{
			GlobalFree( fHandleB );
			fHandleB = NULL;
			BindPart( tMemoryPart )->vfunction25( "Running Out Of Memory!" );
		}
		if ( fMemoryBlocks[ fPad28 ].mAllocate( a, 0, 0 ) == NULL )
		{
			char	message[ 256 ];
			sprintf( message, "Could not allocate %ld bytes!", a );
			BindPart( tMemoryPart )->vfunction25( message );
			return 0;
		}
	}
	fMemoryBlocks[ fPad28 ].fPad[ 6 ] = b;
	fAllocCount++;
	fPad28++;
	int	handle = fPad28;
	if ( handle == fBlockCount )
	{
		fPad28 = 0;
	}
	return handle;
}

// FUNCTION: STARFLEETOP 0x00639310
long	twMemoryPart::mNewHandleZeroIt( long a, long b, char* c )
{
	int	result = fPaging.mNewHandleZeroIt( a, b, c );
	if ( result > 0 )
	{
		return result;
	}
	if ( fAllocCount == fBlockCount && mAddMemory() == 0 )
	{
		char	message[ 256 ];
		sprintf( message, "Could not add more memory blocks!" );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	if ( a < 0 )
	{
		char	message[ 256 ];
		sprintf( message, "You are trying to allocate a block of %ld size!", a );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	if ( fMemoryBlocks == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "fMemoryBlocks was NULL!" );
		return 0;
	}
	int	start = fPad28;
	if ( fMemoryBlocks[ fPad28 ].fInUse != 0 )
	{
		fPad28++;
		if ( fPad28 == fBlockCount )
		{
			fPad28 = 0;
		}
		while ( fMemoryBlocks[ fPad28 ].fInUse != 0 && fPad28 != start )
		{
			fPad28 = fPad28 + 1;
			if ( fPad28 == fBlockCount )
			{
				fPad28 = 0;
			}
		}
		if ( fPad28 == start )
		{
			char	message[ 256 ];
			sprintf( message, "Internal Error finding available block!" );
			BindPart( tMemoryPart )->vfunction25( message );
			return 0;
		}
	}
	if ( fMemoryBlocks[ fPad28 ].mAllocate( a, c, 0x40 ) == NULL )
	{
		if ( fHandleB != NULL )
		{
			GlobalFree( fHandleB );
			fHandleB = NULL;
		}
		BindPart( tMemoryPart )->vfunction25( "Running Out Of Memory!" );
		if ( fMemoryBlocks[ fPad28 ].mAllocate( a, 0, 0x40 ) == NULL )
		{
			char	message[ 256 ];
			sprintf( message, "Could not allocate %ld bytes!", a );
			BindPart( tMemoryPart )->vfunction25( message );
			return 0;
		}
	}
	fMemoryBlocks[ fPad28 ].fPad[ 6 ] = b;
	fAllocCount++;
	fPad28++;
	int	handle = fPad28;
	if ( handle == fBlockCount )
	{
		fPad28 = 0;
	}
	return handle;
}

// FUNCTION: STARFLEETOP 0x006397f0
long	twMemoryPart::mResizeHandle( long a, long b )
{
	if ( a >= 0x10000 )
	{
		return fPaging.mResizeHandle( a, b );
	}
	if ( b < 0 )
	{
		char	message[ 256 ];
		sprintf( message, "You are trying to ReAllocate a block to %ld size!", b );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	if ( fMemoryBlocks == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "fMemoryBlocks was NULL!" );
		return 0;
	}
	if ( fMemoryBlocks[ a - 1 ].mReallocate( b, 0 ) == NULL )
	{
		if ( fHandleB != NULL )
		{
			GlobalFree( fHandleB );
			fHandleB = NULL;
		}
		BindPart( tMemoryPart )->vfunction25( "Running Out Of Memory!" );
		if ( fMemoryBlocks[ a - 1 ].mReallocate( b, 0 ) == NULL )
		{
			char	message[ 256 ];
			sprintf( message, "Could not Reallocate to %ld bytes!", b );
			BindPart( tMemoryPart )->vfunction25( message );
			return 0;
		}
	}
	return a;
}

// FUNCTION: STARFLEETOP 0x00639b00
long	twMemoryPart::mDuplicateHandle( long a )
{
	void*	dst;
	void*	src;
	if ( a >= 0x10000 )
	{
		return fPaging.mDuplicateHandle( a );
	}
	if ( fMemoryBlocks == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "fMemoryBlocks was NULL!" );
		return 0;
	}
	if ( fMemoryBlocks[ a - 1 ].fPad[ 1 ] <= 0 || a > fBlockCount )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to Duplicate an invalid Handle!" );
		return 0;
	}
	int	newHandle = vfunction15( fMemoryBlocks[ a - 1 ].fPad[ 0 ] << 2, 0x2e9, __FILE__ );
	if ( newHandle == 0 )
	{
		char	message[ 256 ];
		sprintf( message, "Could not Duplicate to %ld bytes!", fMemoryBlocks[ a - 1 ].fPad[ 0 ] << 2 );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	dst = vfunction21( newHandle );
	src = vfunction21( a );
	memcpy( dst, src, fMemoryBlocks[ a - 1 ].fPad[ 0 ] << 2 );


	vfunction22( newHandle );
	vfunction22( a );
	return newHandle;
}

// FUNCTION: STARFLEETOP 0x00639d90
long	twMemoryPart::mResizeHandleZeroIt( long a, long b )
{
	if ( a >= 0x10000 )
	{
		return fPaging.mResizeHandleZeroIt( a, b );
	}
	if ( b < 0 )
	{
		char	message[ 256 ];
		sprintf( message, "You are trying to ReAllocate a block to %ld size!", b );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	if ( fMemoryBlocks == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "fMemoryBlocks was NULL!" );
		return 0;
	}
	if ( fMemoryBlocks[ a - 1 ].mReallocate( b, 0x40 ) == NULL )
	{
		if ( fHandleB != NULL )
		{
			GlobalFree( fHandleB );
			fHandleB = NULL;
		}
		BindPart( tMemoryPart )->vfunction25( "Running Out Of Memory!" );
		if ( fMemoryBlocks[ a - 1 ].mReallocate( b, 0x40 ) == NULL )
		{
			char	message[ 256 ];
			sprintf( message, "Could not Reallocate to %ld bytes!", b );
			BindPart( tMemoryPart )->vfunction25( message );
			return 0;
		}
	}
	return a;
}

// FUNCTION: STARFLEETOP 0x0063a0a0
unsigned char	twMemoryPart::mDelHandle( long a )
{
	char	message[ 256 ];

	if ( a >= 0x10000 )
	{
		return fPaging.mDelHandle( a );
	}
	if ( fHandleA == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to Delete -- MemoryManager is not Active" );
		return 0;
	}
	if ( a > 0 && a <= fBlockCount )
	{
		if ( fMemoryBlocks == NULL )
		{
			BindPart( tMemoryPart )->vfunction25( "fMemoryBlocks was NULL!" );
			return 0;
		}
		if ( fMemoryBlocks[ a - 1 ].fInUse != 0 )
		{
			while ( fMemoryBlocks[ a - 1 ].fPad[ 2 ] > 0 )
			{
				if ( vfunction22( a ) != 1 )
				{
					return 0;
				}
				BindPart( tMemoryPart )->vfunction25( "You are attempting to Delete A Locked Handle -- I'm removing 1 Lock!" );
			}
			fMemoryBlocks[ a - 1 ].mFree( 0 );
			fAllocCount--;
			return 1;
		}
		sprintf( message, "You are trying to delete a Handle that was already deleted!" );
		BindPart( tMemoryPart )->vfunction25( message );
		return 1;
	}
	sprintf( message, "You have an Invalid Handle!" );
	BindPart( tMemoryPart )->vfunction25( message );
	return 0;
}

// FUNCTION: STARFLEETOP 0x0063a440
void*	twMemoryPart::mLockHandle( long a )
{
	char	message[ 256 ];

	if ( a >= 0x10000 )
	{
		return fPaging.mLockHandle( a );
	}
	if ( a < 1 || a > fBlockCount )
	{
		sprintf( message, "You are trying to lock a Handle that is out of range!" );
		BindPart( tMemoryPart )->vfunction25( message );
	}
	return fMemoryBlocks[ a - 1 ].mLock();
}

// HYPOTHESIS: the unlock counterpart of mLock (0x0063b6f0); the library has no copy of it, so it
// was inline and exists only inside mUnlockHandle. The name follows mLock.
inline unsigned char	twMemoryBlock::mUnlock( void )
{
	if ( fInUse == 0 )
	{
		char	message[ 256 ];
		sprintf( message, "You are trying to unlock a handle that doesn't exist." );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	if ( fPad[ 2 ] <= 0 )
	{
		char	message[ 256 ];
		sprintf( message, "You are trying to unlock a handle that isn't locked." );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	if ( fPad[ 2 ] > 1 )
	{
		fPad[ 2 ]--;
		return 1;
	}
	GlobalUnlock( (HGLOBAL) fInUse );
	fPad[ 2 ] = 0;
	fPad[ 3 ] = 0;
	fPad[ 4 ] = 0;
	fPad[ 5 ] = 0;
	return 1;
}

// FUNCTION: STARFLEETOP 0x0063a540
unsigned char	twMemoryPart::mUnlockHandle( long a )
{
	if ( a >= 0x10000 )
	{
		return fPaging.vfunction12( a );
	}
	if ( a < 1 || a > fBlockCount )
	{
		char	message[ 256 ];
		sprintf( message, "You are trying to unlock a Handle that is out of range!" );
		BindPart( tMemoryPart )->vfunction25( message );
	}
	return fMemoryBlocks[ a - 1 ].mUnlock();
}

// FUNCTION: STARFLEETOP 0x0063a7b0
unsigned char	twMemoryPart::mRemoveAllLocksOnHandle( long a )
{
	if ( a >= 0x10000 )
	{
		return fPaging.vfunction13( a );
	}
	if ( a < 1 || a > fBlockCount )
	{
		char	message[ 256 ];
		sprintf( message, "You are trying to unlock a Handle that is out of range!" );
		BindPart( tMemoryPart )->vfunction25( message );
	}
	long	index = a - 1;
	while ( fMemoryBlocks[ index ].fPad[ 2 ] > 0 )
	{
		if ( vfunction22( a ) != 1 )
		{
			return 0;
		}
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x0063a8d0
long	twMemoryPart::mLargestBlockAvailable( void )
{
	MEMORYSTATUS	status;
	status.dwLength = sizeof( MEMORYSTATUS );
	status.dwAvailPhys = 0;
	GlobalMemoryStatus( &status );
	return status.dwAvailPhys;
}

// FUNCTION: STARFLEETOP 0x0063a900
iTruth	twMemoryPart::mCheckMemory( void*, void* )
{
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x0063b360
unsigned char	twMemoryBlock::mFree( unsigned char flag )
{
	char	message[ 256 ];
	char	report[ 256 ];

	if ( fInUse != 0 )
	{
		if ( flag == 1 )
		{
			if ( fPad[ 5 ] == 0 )
			{
				char*	p = (char*) GlobalLock( (HGLOBAL) fInUse );
				if ( p == NULL )
				{
					FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
					BindPart( tMemoryPart )->vfunction25( message );
					return 0;
				}
				fPad[ 5 ] = (int) ( p + fPad[ 0 ] * 4 + 16 );
			}
			if ( fPad[ 5 ] != 0 )
			{
				sprintf( report, "Attempting to Free a block of memory of size %ld. %s %ld", fPad[ 0 ] << 2, fPad[ 5 ], fPad[ 6 ] );
			}
			else
			{
				sprintf( report, "Attempting to Free a block of memory of size %ld. %ld", fPad[ 0 ] << 2, fPad[ 6 ] );
			}
			BindPart( tMemoryPart )->vfunction25( report );
			if ( fPad[ 3 ] == 0 )
			{
				GlobalUnlock( (HGLOBAL) fInUse );
				fPad[ 5 ] = 0;
			}
		}
		if ( fPad[ 2 ] > 0 )
		{
			sprintf( message, "While freeing a block of memory of size %ld, we encountered %ld outstanding locks. Allocated:%s %ld.", fPad[ 0 ] << 2, fPad[ 2 ], fPad[ 5 ], fPad[ 6 ] );
			BindPart( tMemoryPart )->vfunction25( message );
		}
		while ( fPad[ 2 ] > 0 )
		{
			GlobalUnlock( (HGLOBAL) fInUse );
			fPad[ 2 ]--;
		}
		if ( GlobalFree( (HGLOBAL) fInUse ) != NULL )
		{
			FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
			BindPart( tMemoryPart )->vfunction25( message );
			fPad[ 0 ] = 0;
			fPad[ 1 ] = 0;
			fPad[ 2 ] = 0;
			fPad[ 3 ] = 0;
			fPad[ 4 ] = 0;
			fInUse = 0;
			fPad[ 5 ] = 0;
			fPad[ 6 ] = 0;
			return 0;
		}
	}
	fPad[ 0 ] = 0;
	fPad[ 1 ] = 0;
	fPad[ 2 ] = 0;
	fPad[ 3 ] = 0;
	fPad[ 4 ] = 0;
	fInUse = 0;
	fPad[ 5 ] = 0;
	fPad[ 6 ] = 0;
	return 1;
}

// FUNCTION: STARFLEETOP 0x0063a910
short	twMemoryBlock::mCheckForBoundsError( void )
{
	char	message[ 256 ];
	short	error = 0;
	int*	cursor;

	if ( fInUse == 0 )
	{
		return 0;
	}
	if ( fPad[ 3 ] == 0 )
	{
		cursor = (int*) GlobalLock( (HGLOBAL) fInUse );
		if ( cursor == NULL )
		{
			FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
			BindPart( tMemoryPart )->vfunction25( message );
			return 0;
		}
	}
	else
	{
		cursor = (int*) fPad[ 3 ] - 2;
	}

	int		frontBad = 0;
	for ( int i = 2; i != 0; i-- )
	{
		if ( *cursor++ != 0x43434343 )
		{
			frontBad = 1;
		}
	}
	int		backBad = 0;
	cursor = cursor + fPad[ 0 ];
	for ( int j = 2; j != 0; j-- )
	{
		if ( *cursor++ != 0x43434343 )
		{
			backBad = 1;
		}
	}
	if ( (short) frontBad == 1 )
	{
		error = 2;
	}
	if ( (short) backBad == 1 )
	{
		if ( (short) error == 2 )
		{
			error = 1;
		}
		else
		{
			error = 3;
		}
	}
	if ( (short) error == 1 )
	{
		fPad[ 5 ] = (int) cursor;
		sprintf( message, "Bounds error -- front and back -- noticed while Freeing a block of size %ld Allocated:%s %ld.", fPad[ 0 ] << 2, cursor, fPad[ 6 ] );
		BindPart( tMemoryPart )->vfunction25( message );
	}
	else if ( (short) error == 2 )
	{
		fPad[ 5 ] = (int) cursor;
		sprintf( message, "Bounds error -- Front overwritten -- noticed while Freeing a block of size %ld. Allocated:%s %ld.", fPad[ 0 ] << 2, cursor, fPad[ 6 ] );
		BindPart( tMemoryPart )->vfunction25( message );
	}
	else if ( (short) error == 3 )
	{
		fPad[ 5 ] = (int) cursor;
		sprintf( message, "Bounds error -- Far end overwritten -- noticed while Freeing a block of size %ld. Allocated:%s %ld.", fPad[ 0 ] << 2, cursor, fPad[ 6 ] );
		BindPart( tMemoryPart )->vfunction25( message );
	}
	if ( fPad[ 3 ] == 0 )
	{
		GlobalUnlock( (HGLOBAL) fInUse );
		fPad[ 5 ] = 0;
	}
	return (short) error;
}

// FUNCTION: STARFLEETOP 0x0063aca0
void*	twMemoryBlock::mAllocate( long a, char* b, unsigned long c )
{
	fPad[ 0 ] = ( a + 3 ) >> 2;
	if ( b != NULL )
	{
		fPad[ 1 ] = strlen( b ) + 1 + 16 + fPad[ 0 ] * 4;
	}
	else
	{
		fPad[ 1 ] = fPad[ 0 ] * 4 + 17;
	}
	fPad[ 2 ] = 0;
	fPad[ 3 ] = 0;
	fPad[ 4 ] = 0;
	fInUse = (int) GlobalAlloc( c | GMEM_MOVEABLE, fPad[ 1 ] );
	if ( fInUse == NULL )
	{
		char	message[ 256 ];
		FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
		BindPart( tMemoryPart )->vfunction25( message );
	}
	else
	{
		char*	p = (char*) GlobalLock( (HGLOBAL) fInUse );
		if ( p == NULL )
		{
			char	message[ 256 ];
			FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
			BindPart( tMemoryPart )->vfunction25( message );
		}
		else
		{
			memset( p, 'C', 8 );
			p += 8;
			// HYPOTHESIS: the back guard's form is a guess; a plain memset and step folds the
			// first step into the address and scores lower.
			p = (char*) memset( p + fPad[ 0 ] * 4, 'C', 8 ) + 8;
			fPad[ 5 ] = (int) p;
			if ( b != NULL )
			{
				strcpy( (char*) fPad[ 5 ], b );
			}
			else
			{
				*(char*) fPad[ 5 ] = 0;
			}
			GlobalUnlock( (HGLOBAL) fInUse );
		}
	}
	return (void*) fInUse;
}

// FUNCTION: STARFLEETOP 0x0063aee0
void*	twMemoryBlock::mReallocate( long a, unsigned long b )
{
	mCheckForBoundsError();
	if ( fPad[ 2 ] != 0 )
	{
		char	message[ 256 ];
		sprintf( message, "Trying to ReAllocate a Locked Handle!" );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	int		oldWords = fPad[ 0 ];
	fPad[ 3 ] = 0;
	fPad[ 4 ] = 0;
	int		newWords = ( a + 3 ) >> 2;
	int		oldTotal = fPad[ 1 ];
	int		newTotal = oldTotal - oldWords * 4;
	int		moveSize = newTotal - 8;
	newTotal += newWords * 4;
	int		oldBytes = oldWords * 4;
	char*	p;

	if ( newTotal > oldTotal )
	{
		HGLOBAL	handle = GlobalReAlloc( (HGLOBAL) fInUse, newTotal, b | GMEM_MOVEABLE );
		if ( handle == NULL )
		{
			char	message[ 256 ];
			FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
			BindPart( tMemoryPart )->vfunction25( message );
			return 0;
		}
		fPad[ 0 ] = newWords;
		fPad[ 1 ] = newTotal;
		fInUse = (int) handle;
		p = (char*) GlobalLock( handle );
		if ( p == NULL )
		{
			char	message[ 256 ];
			FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
			BindPart( tMemoryPart )->vfunction25( message );
			return (void*) fInUse;
		}
		memmove( p + newWords * 4 + 8, p + oldBytes + 8, moveSize );
		GlobalUnlock( (HGLOBAL) fInUse );
		return (void*) fInUse;
	}
	p = (char*) GlobalLock( (HGLOBAL) fInUse );
	if ( p == NULL )
	{
		char	message[ 256 ];
		FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
		BindPart( tMemoryPart )->vfunction25( message );
		return (void*) fInUse;
	}
	memmove( p + newWords * 4 + 8, p + oldBytes + 8, moveSize );
	GlobalUnlock( (HGLOBAL) fInUse );
	HGLOBAL	handle = GlobalReAlloc( (HGLOBAL) fInUse, newTotal, b | GMEM_MOVEABLE );
	if ( handle == NULL )
	{
		char	message[ 256 ];
		FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	fPad[ 0 ] = newWords;
	fPad[ 1 ] = newTotal;
	fInUse = (int) handle;
	return (void*) handle;
}

// FUNCTION: STARFLEETOP 0x0063b6f0
void*	twMemoryBlock::mLock( void )
{
	if ( fInUse == 0 )
	{
		char	message[ 256 ];
		sprintf( message, "You are trying to lock a handle that doesn't exist." );
		BindPart( tMemoryPart )->vfunction25( message );
		return NULL;
	}
	if ( fPad[ 2 ] > 0 )
	{
		fPad[ 2 ]++;
		return (void*) fPad[ 3 ];
	}
	char*	p = (char*) GlobalLock( (HGLOBAL) fInUse );
	if ( p == NULL )
	{
		char	message[ 256 ];
		FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0x400, message, 256, NULL );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	p += 8;
	fPad[ 3 ] = (int) p;
	fPad[ 2 ] = 1;
	p += fPad[ 0 ] * 4;
	fPad[ 4 ] = (int) p;
	p += 8;
	fPad[ 5 ] = (int) p;
	return (void*) fPad[ 3 ];
}
