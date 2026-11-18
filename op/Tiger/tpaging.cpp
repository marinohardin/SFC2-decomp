//	tpaging.cpp: the paging mechanism of the memory part.

#include "Tiger/tbasic.h"
#include "Tiger/tpart.h"
#include <stdio.h>
#include <string.h>

// HYPOTHESIS: tMemoryPart is declared with its part binding in another unit; slot 25 (offset
// 0x64) takes a message string.
class tMemoryPart : public tPart { public: GenAbstractBinding( tMemoryPart ); virtual void vfunction6(); virtual void* vfunction7( long a, long b, char* c ); virtual void* vfunction8( void* a, long b ); virtual void vfunction9(); virtual unsigned char vfunction10( void* a ); virtual void vfunction11(); virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14(); virtual int vfunction15( long a, long b, char* c ); virtual int vfunction16( long a, long b, char* c ); virtual void vfunction17(); virtual int vfunction18( int handle, long size ); virtual int vfunction19( int handle ); virtual char vfunction20( int handle ); virtual void* vfunction21( int handle ); virtual char vfunction22( int handle ); virtual void vfunction23(); virtual void vfunction24(); virtual void vfunction25( const char* message ); };

#include "Tiger/tpaging.h"

// FUNCTION: STARFLEETOP 0x0063b8a0
unsigned char	tPagingMechanism::mAddPage( void )
{
	if ( fTableSize == 0 )
	{
		fTableSize = 0x101;
		fTable = (char**)BindPart( tMemoryPart )->vfunction7( fTableSize * 4, 0, " " );
		fLastPage = NULL;
		if ( fTable == NULL )
		{
			char	message[ 256 ];
			sprintf( message, "trouble allocating initial Page Block of Size %ld", fTableSize * 4 );
			BindPart( tMemoryPart )->vfunction25( message );
			return 0;
		}
		for ( int i = 0; i < fTableSize; i++ )
		{
			fTable[ i ] = NULL;
		}
	}
	else if ( fPageCount == fTableSize )
	{
		int	i = fTableSize;
		fTableSize += 8;
		char**	table = (char**)BindPart( tMemoryPart )->vfunction8( fTable, fTableSize * 4 );
		if ( table == NULL )
		{
			char	message[ 256 ];
			sprintf( message, "trouble reallocating initial Page Block of Size %ld", fTableSize * 4 );
			BindPart( tMemoryPart )->vfunction25( message );
			return 0;
		}
		fTable = table;
		for ( ; i < fTableSize; i++ )
		{
			fTable[ i ] = NULL;
		}
	}

	int	index = fPageCount;
	if ( fTable[ index ] != NULL )
	{
		int	i = 0;
		for ( ; i < fPageCount; i++ )
		{
			if ( fTable[ i ] == NULL )
			{
				index = i;
				i = fPageCount + 1;
			}
		}
		if ( i == fPageCount )
		{
			BindPart( tMemoryPart )->vfunction25( "We were expecting a NULL page and we did not find one!" );
			return 0;
		}
	}

	fTable[ index ] = (char*)BindPart( tMemoryPart )->vfunction7( 0xfe04, 0, " " );
	if ( fTable[ index ] == NULL )
	{
		char	message[ 256 ];
		sprintf( message, "trouble allocating Page %ld", index );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	// HYPOTHESIS: the page ends in a table of 0xfc shorts at +0xfc00; both ends start at 0xfc.
	short*	sizes = (short*)( fTable[ index ] + 0xfc00 );
	sizes[ 0 ] = 0xfc;
	sizes[ 0xfb ] = 0xfc;
	*(int*)( fTable[ index ] + 0xfe00 ) = index;
	*(char**)( fTable[ index ] + 0xfdf8 ) = NULL;
	*(char**)( fTable[ index ] + 0xfdfc ) = fLastPage;
	if ( fLastPage != NULL )
	{
		*(char**)( fLastPage + 0xfdf8 ) = fTable[ index ];
	}
	fLastPage = fTable[ index ];
	fPageCount++;
	return 1;
}

// HYPOTHESIS: a page is 0xfc entries of 0x100 bytes, a table of 0xfc shorts at +0xfc00 and a
// trailer; only the parts mClosePage reads are declared.
class tPagingBlock
{
public:
	unsigned char	mCheckForBoundsError	( long index );
	char	fEntries[ 0xfc00 ];
	unsigned short	fSizes[ 0xfc ];
	tPagingBlock*	fNewer;
	tPagingBlock*	fOlder;
	long	fIndex;
	// HYPOTHESIS: the exe symbol marks it protected; the paging mechanism is a friend.
	friend class tPagingMechanism;
protected:
	long	mGetIndexWhereToAddThisMuchSpace	( long size );
};

// FUNCTION: STARFLEETOP 0x0063be00
void	tPagingMechanism::mClosePage( tPagingBlock* page )
{
	int		i;

	for ( i = 0; i < 0xfc; )
	{
		char*	entry = page->fEntries + i * 0x100;
		if ( *(short*)( entry + 10 ) == 0 )
		{
			i += page->fSizes[ i ];
		}
		else
		{
			char	name[ 256 ];
			char	message[ 256 ];
			page->mCheckForBoundsError( i );
			int		words = *(int*)entry;
			char*	file = entry + words * 4 + 0x1c;
			int		j;
			for ( j = 0; j < 0xff; j++ )
			{
				name[ j ] = entry[ 0x14 + j * 2 ];
			}
			name[ j ] = 0;
			sprintf( message, "Closing page %ld, found memory(if tString=%s) of size %ld created in %s at line %ld with %ld open locks!",
				page->fIndex, name, words * 4, file, (int)*(short*)( entry + 4 ), (int)*(short*)( entry + 8 ) );
			BindPart( tMemoryPart )->vfunction25( message );
			i += *(unsigned short*)( entry + 10 );
		}
	}

	if ( page->fNewer == NULL )
	{
		fLastPage = (char*)page->fOlder;
	}
	else
	{
		page->fNewer->fOlder = page->fOlder;
	}
	if ( page->fOlder != NULL )
	{
		page->fOlder->fNewer = page->fNewer;
	}
	long	index = page->fIndex;
	BindPart( tMemoryPart )->vfunction10( page );
	fTable[ index ] = NULL;
	fPageCount--;
	if ( fPageCount == 0 && fTable != NULL )
	{
		BindPart( tMemoryPart )->vfunction10( fTable );
		fTableSize = 0;
		fTable = NULL;
	}
}

// FUNCTION: STARFLEETOP 0x0063c0b0
void	tPagingMechanism::mDeactivate( void )
{
	while ( fLastPage != NULL )
	{
		mClosePage( (tPagingBlock*)fLastPage );
	}
}

// FUNCTION: STARFLEETOP 0x0063c0d0
long	tPagingBlock::mGetIndexWhereToAddThisMuchSpace( long size )
{
	long	index = 0;
	if ( *(unsigned short*)( fEntries + 10 ) > 0 )
	{
		index = fSizes[ 0 ];
	}
	// HYPOTHESIS: the search ends on a flag rather than a break.
	bool	found = false;
	while ( !found && index < 0xfc )
	{
		if ( fSizes[ index ] >= size )
		{
			found = true;
		}
		else
		{
			index += fSizes[ index ];
			if ( index >= 0xfc )
			{
				return -1;
			}
			index += fSizes[ index ];
		}
	}
	if ( !found )
	{
		return -1;
	}
	long	before = 0;
	if ( index > 0 )
	{
		before = fSizes[ index - 1 ];
	}
	long	avail = fSizes[ index ];
	if ( avail > size )
	{
		long	rest = avail - size;
		fSizes[ index + size ] = (unsigned short)rest;
		fSizes[ index + avail - 1 ] = (unsigned short)rest;
		if ( index > 0 )
		{
			fSizes[ index - 1 ] = 0;
			fSizes[ index ] = 0;
			fSizes[ index + size - 1 ] = (unsigned short)( size + before );
			fSizes[ index - before ] = (unsigned short)( size + before );
		}
		else
		{
			fSizes[ index ] = (unsigned short)size;
			fSizes[ index + size - 1 ] = (unsigned short)size;
		}
	}
	else
	{
		long	after = 0;
		if ( index + avail < 0xfc )
		{
			after = fSizes[ index + avail ];
		}
		if ( index == 0 )
		{
			long	next = fSizes[ size ];
			fSizes[ size - 1 ] = 0;
			fSizes[ size ] = 0;
			fSizes[ 0 ] = (unsigned short)( next + size );
			fSizes[ next + size - 1 ] = (unsigned short)( next + size );
		}
		else if ( index == 0xfc - size )
		{
			fSizes[ index - 1 ] = 0;
			fSizes[ index ] = 0;
			fSizes[ size + index - 1 ] = (unsigned short)( before + size );
			fSizes[ index - before ] = (unsigned short)( before + size );
		}
		else
		{
			fSizes[ index ] = 0;
			fSizes[ index + size - 1 ] = 0;
			fSizes[ index + size ] = 0;
			fSizes[ index - 1 ] = 0;
			fSizes[ index - before ] = (unsigned short)( after + before + size );
			fSizes[ after + index + size - 1 ] = (unsigned short)( after + before + size );
		}
	}
	return index;
}

// FUNCTION: STARFLEETOP 0x0063c270
long	tPagingMechanism::mNewHandle( long size, long line, char* file )
{
	long	words = ( size + 3 ) >> 2;
	long	blocks = 1;
	long	total = strlen( file ) + 1 + 0x1c + words * 4;
	tPagingBlock*	page;
	long	index;
	while ( total > 0x100 && blocks <= 4 )
	{
		total -= 0x100;
		blocks++;
	}
	if ( blocks > 4 )
	{
		return 0;
	}

	if ( fTable == NULL )
	{
		mAddPage();
		if ( fTable == NULL )
		{
			BindPart( tMemoryPart )->vfunction25( "Paging is having trouble adding pages" );
			return 0;
		}
	}

	long	pageIndex;
	page = (tPagingBlock*)fLastPage;
	for ( ;; )
	{
		if ( page == NULL )
		{
			if ( !mAddPage() )
			{
				BindPart( tMemoryPart )->vfunction25( "Paging is having trouble adding pages" );
				return 0;
			}
			page = (tPagingBlock*)fLastPage;
		}
		pageIndex = page->fIndex;
		index = page->mGetIndexWhereToAddThisMuchSpace( blocks );
		if ( index != -1 )
		{
			break;
		}
		page = page->fOlder;
	}
	char*	entry = page->fEntries + index * 0x100;
	*(short*)( entry + 10 ) = (short)blocks;
	*(short*)( entry + 4 ) = (short)line;
	*(int*)entry = words;
	int		length = strlen( file ) + 1;
	*(short*)( entry + 8 ) = 0;
	*(short*)( entry + 6 ) = (short)length;
	memset( entry + 0xc, 'C', 8 );
	char*	tail = entry + 0x14 + words * 4;
	memset( tail, 'C', 8 );
	memcpy( tail + 8, file, length );
	return ( ( pageIndex + 1 ) << 16 ) + index;
}

// FUNCTION: STARFLEETOP 0x0063c480
int	tPagingMechanism::mNewHandleZeroIt( long a, long b, char* c )
{
	return mNewHandle( a, b, c );
}

// FUNCTION: STARFLEETOP 0x0063c4a0
void*	tPagingMechanism::mNewBlock( long size, long line, char* file )
{
	long	words = ( size + 3 ) >> 2;
	long	blocks = 1;
	long	total = strlen( file ) + 1 + 0x1c + words * 4;
	tPagingBlock*	page;
	long	index;
	while ( total > 0x100 && blocks <= 4 )
	{
		total -= 0x100;
		blocks++;
	}
	if ( blocks > 4 )
	{
		return 0;
	}

	if ( fTable == NULL )
	{
		mAddPage();
		if ( fTable == NULL )
		{
			BindPart( tMemoryPart )->vfunction25( "Paging is having trouble adding pages" );
			return 0;
		}
	}

	page = (tPagingBlock*)fLastPage;
	for ( ;; )
	{
		if ( page == NULL )
		{
			if ( !mAddPage() )
			{
				BindPart( tMemoryPart )->vfunction25( "Paging is having trouble adding pages" );
				return 0;
			}
			page = (tPagingBlock*)fLastPage;
		}
		index = page->mGetIndexWhereToAddThisMuchSpace( blocks );
		if ( index != -1 )
		{
			break;
		}
		page = page->fOlder;
	}
	char*	entry = page->fEntries + index * 0x100;
	*(short*)( entry + 4 ) = (short)line;
	*(int*)entry = words;
	*(short*)( entry + 10 ) = (short)blocks;
	int		length = strlen( file ) + 1;
	*(short*)( entry + 6 ) = (short)length;
	*(short*)( entry + 8 ) = 1;
	memset( entry + 0xc, 'C', 8 );
	char*	tail = entry + 0x14 + words * 4;
	memset( tail, 'C', 8 );
	memcpy( tail + 8, file, length );
	return entry + 0x14;
}

// FUNCTION: STARFLEETOP 0x0063c6d0
unsigned char	tPagingMechanism::mGetPtrsFromHandle( long handle, tPagingBlock*& page, unsigned char*& entry, long& index )
{
	if ( handle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to access an invalid page handle" );
		return 0;
	}
	if ( fTable == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to access an invalid page handle when there are no pages" );
		return 0;
	}
	long	pageIndex = ( handle >> 16 ) - 1;
	if ( pageIndex < 0 || pageIndex >= fTableSize )
	{
		char	message[ 256 ];
		sprintf( message, "Trying to access an invalid page handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	page = (tPagingBlock*)fTable[ pageIndex ];
	if ( page == NULL )
	{
		char	message[ 256 ];
		sprintf( message, "Trying to access an invalid page handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	index = handle - ( pageIndex << 16 ) - 0x10000;
	if ( index >= 0 && index < 0xfc )
	{
		entry = (unsigned char*)page + index * 0x100;
		return 1;
	}
	else
	{
		char	message[ 256 ];
		sprintf( message, "Trying to access an invalid page handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
}

// FUNCTION: STARFLEETOP 0x0063ca50
long	tPagingMechanism::mGetHandleFromPointer( void* pointer )
{
	if ( pointer == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to get a Handle for a NULL pointer" );
	}
	else if ( fTable != NULL && fLastPage != NULL )
	{
		for ( tPagingBlock* page = (tPagingBlock*)fLastPage; page != NULL; page = page->fOlder )
		{
			if ( (char*)pointer >= (char*)page && (char*)pointer < (char*)page + 0xfe04 )
			{
				if ( (char*)page <= (char*)pointer )
				{
					long	index = ( (char*)pointer - (char*)page ) >> 8;
					if ( index < 0xfc )
					{
						return ( page->fIndex + 1 ) * 0x10000 + index;
					}
				}
				else
				{
					BindPart( tMemoryPart )->vfunction25( "Invalid pointer!" );
					return 0;
				}
				BindPart( tMemoryPart )->vfunction25( "Invalid pointer!" );
				return 0;
			}
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x0063cc40
void*	tPagingMechanism::mLockHandle( long handle )
{
	tPagingBlock*	page;
	unsigned char*	entry;
	long	index;
	if ( !mGetPtrsFromHandle( handle, page, entry, index ) )
	{
		char	message[ 256 ];
		sprintf( message, "Unable to Lock Handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return NULL;
	}
	*(short*)( entry + 8 ) = *(short*)( entry + 8 ) + 1;
	return entry + 0x14;
}

// FUNCTION: STARFLEETOP 0x0063cd30
unsigned char	tPagingMechanism::vfunction12( long handle )
{
	tPagingBlock*	page;
	unsigned char*	entry;
	long	index;
	if ( !mGetPtrsFromHandle( handle, page, entry, index ) )
	{
		char	message[ 256 ];
		sprintf( message, "Unable to Lock Handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	*(short*)( entry + 8 ) = *(short*)( entry + 8 ) - 1;
	return 1;
}

// FUNCTION: STARFLEETOP 0x0063ce10
unsigned char	tPagingMechanism::vfunction13( long handle )
{
	tPagingBlock*	page;
	unsigned char*	entry;
	long	index;
	if ( !mGetPtrsFromHandle( handle, page, entry, index ) )
	{
		char	message[ 256 ];
		sprintf( message, "Unable to Lock Handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	*(short*)( entry + 8 ) = 0;
	return 1;
}

// FUNCTION: STARFLEETOP 0x0063d0c0
unsigned char	tPagingMechanism::mIsThisAPagingPointer( void* a )
{
	tPagingBlock*	page = (tPagingBlock*)fLastPage;
	while ( page != 0 )
	{
		if ( a >= (void*)page && a < (void*)( page + 1 ) )
		{
			return 1;
		}
		page = page->fOlder;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x0063d0f0
unsigned char	tPagingMechanism::mCheckForBoundsErrors( void )
{
	unsigned char	result = 0;

	if ( fTable == NULL )
	{
		return 0;
	}
	for ( tPagingBlock* page = (tPagingBlock*)fLastPage; page != 0; page = page->fOlder )
	{
		int	i = 0;
		do
		{
			char*	entry = page->fEntries + i * 0x100;
			if ( *(unsigned short*)( entry + 10 ) > 0 )
			{
				if ( page->mCheckForBoundsError( i ) == 1 )
				{
					result = 1;
				}
				i += *(unsigned short*)( entry + 10 );
			}
			else
			{
				i += page->fSizes[ i ];
			}
		} while ( i < 0xfc );
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x0063cf00
unsigned char	tPagingBlock::mCheckForBoundsError( long index )
{
	char*	entry = fEntries + index * 0x100;
	// HYPOTHESIS: an entry holds its size in words at +0, a line at +4, two guard words at
	// +0xc, the data, two more guard words and then the allocating file's name.
	int*	guard = (int*)( entry + 0xc );
	int*	front = guard;
	int		i;

	for ( i = 0; i < 2; i++ )
	{
		if ( *front++ != 0x43434343 )
		{
			char	message[ 256 ];
			sprintf( message, "Bounds error -- Front overwritten -- in a block of size %ld. Allocated:%s %ld.",
				*(int*)entry * 4, (char*)guard + *(int*)entry * 4 + 0x10, (int)*(short*)( entry + 4 ) );
			BindPart( tMemoryPart )->vfunction25( message );
			return 1;
		}
	}
	front += *(int*)entry;
	int*	back = front;
	for ( i = 0; i < 2; i++ )
	{
		if ( *front++ != 0x43434343 )
		{
			char	message[ 256 ];
			// HYPOTHESIS: the name is read from the first back guard word, whichever word failed.
			sprintf( message, "Bounds error -- Back End overwritten -- in a block of size %ld. Allocated:%s %ld.",
				*(int*)entry * 4, (char*)( back + 2 ), (int)*(short*)( entry + 4 ) );
			BindPart( tMemoryPart )->vfunction25( message );
			return 1;
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x0063d160
unsigned char	tPagingMechanism::mDelBlock( void* a )
{
	long	handle = mGetHandleFromPointer( a );
	if ( handle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to delete a nonexistant paging block" );
		return 0;
	}
	// HYPOTHESIS: the lock removal is the virtual the original reaches at offset 0x30.
	if ( vfunction12( handle ) == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Couldn't remove locks while trying to delete a nonexistant paging block" );
	}
	return mDelHandle( handle );
}

// FUNCTION: STARFLEETOP 0x0063d2b0
unsigned char	tPagingMechanism::mDelHandle( long handle )
{
	tPagingBlock*	page;
	unsigned char*	entry;
	long	index;
	char	message[ 256 ];
	long	c;
	long	f;
	long	start;
	long	i;
	long	e;
	long	g;

	if ( !mGetPtrsFromHandle( handle, page, entry, index ) )
	{
		sprintf( message, "Unable to Delete Handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	long	n = *(unsigned short*)( entry + 10 );
	if ( n == 0 )
	{
		sprintf( message, "You are deleting a Handle for the 2nd time: %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 1;
	}
	short*	locks = (short*)( entry + 8 );
	while ( *locks > 0 )
	{
		if ( vfunction12( handle ) != 1 )
		{
			return 0;
		}
		BindPart( tMemoryPart )->vfunction25( "You are attempting to Delete A Locked Handle -- I'm removing 1 Lock!" );
	}
	page->mCheckForBoundsError( index );
	long	a = page->fSizes[ index ];
	long	end = n + index;
	long	b = page->fSizes[ end - 1 ];
	bool	flag = false;
	if ( n == 1 && a > 1 )
	{
		if ( index + 1 < 0xfc )
		{
			flag = *(short*)( entry + 0x10a ) == 0;
		}
		else
		{
			flag = true;
		}
	}
	if ( a > 0 && !flag )
	{
		if ( a == n )
		{
			if ( index == 0 )
			{
				c = page->fSizes[ n ];
				page->fSizes[ n - 1 ] = 0;
				page->fSizes[ n ] = 0;
				page->fSizes[ 0 ] = (unsigned short)( c + n );
				page->fSizes[ c + n - 1 ] = (unsigned short)( c + n );
			}
			else if ( end >= 0xfc )
			{
				long	d = page->fSizes[ index - 1 ];
				page->fSizes[ index ] = 0;
				page->fSizes[ index - 1 ] = 0;
				page->fSizes[ index + n - 1 ] = (unsigned short)( d + n );
				page->fSizes[ index - d ] = (unsigned short)( d + n );
			}
			else
			{
				long	d = page->fSizes[ index - 1 ];
				f = page->fSizes[ end ];
				page->fSizes[ index ] = 0;
				page->fSizes[ index + n - 1 ] = 0;
				page->fSizes[ index - 1 ] = 0;
				page->fSizes[ index + n ] = 0;
				page->fSizes[ index - d ] = (unsigned short)( f + d + n );
				page->fSizes[ index + f + n - 1 ] = (unsigned short)( f + d + n );
			}
		}
		else
		{
			if ( index == 0 )
			{
				page->fSizes[ 0 ] = n;
				page->fSizes[ n - 1 ] = n;
				g = a - n;
				page->fSizes[ n ] = (unsigned short)g;
				page->fSizes[ a - 1 ] = (unsigned short)g;
			}
			else
			{
				long	d = page->fSizes[ index - 1 ];
				page->fSizes[ index ] = 0;
				page->fSizes[ index - 1 ] = 0;
				page->fSizes[ index - d ] = (unsigned short)( d + n );
				page->fSizes[ index + n - 1 ] = (unsigned short)( d + n );
				page->fSizes[ index + n ] = (unsigned short)( a - n );
				page->fSizes[ index + a - 1 ] = (unsigned short)( a - n );
			}
		}
	}
	else if ( b > 0 )
	{
		if ( end >= 0xfc )
		{
			page->fSizes[ index ] = n;
			page->fSizes[ index + n - 1 ] = n;
			g = b - n;
			page->fSizes[ index - 1 ] = (unsigned short)g;
			i = index + n - b;
			page->fSizes[ i ] = (unsigned short)g;
		}
		else
		{
			long	c = page->fSizes[ end ];
			page->fSizes[ end - 1 ] = 0;
			page->fSizes[ index + n ] = 0;
			page->fSizes[ index - 1 ] = (unsigned short)( b - n );
			start = index + n - b;
			page->fSizes[ start ] = (unsigned short)( b - n );
			page->fSizes[ index ] = (unsigned short)( c + n );
			page->fSizes[ index + n + c - 1 ] = (unsigned short)( c + n );
		}
	}
	else
	{
		start = 0;
		i = page->fSizes[ 0 ];
		if ( i <= index )
		{
			do
			{
				start = i;
				i += page->fSizes[ i ];
			} while ( i <= index );
		}
		f = page->fSizes[ start ];
		e = index - start;
		page->fSizes[ index - 1 ] = (unsigned short)e;
		page->fSizes[ start ] = (unsigned short)e;
		page->fSizes[ index ] = n;
		page->fSizes[ index + n - 1 ] = n;
		g = f - e - n;
		page->fSizes[ index + n ] = (unsigned short)g;
		page->fSizes[ start + f - 1 ] = (unsigned short)g;
	}
	memset( entry, 0, n * 0x100 );
	if ( page->fSizes[ 0 ] == 0xfc )
	{
		mClosePage( page );
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x0063d8c0
long	tPagingMechanism::mResizeHandle( long handle, long size )
{
	long	index;
	long	words;
	long	slots;
	long	n;
	long	b;
	char*	name;
	long	newEnd;

	if ( handle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to Reaize an invalid page handle" );
		return 0;
	}
	if ( fTable == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to resize an invalid page handle" );
		return 0;
	}
	long	pageIndex = ( handle >> 16 ) - 1;
	if ( pageIndex < 0 || pageIndex >= fTableSize )
	{
		char	message[ 256 ];
		sprintf( message, "Trying to resize an invalid page handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	tPagingBlock*	page = (tPagingBlock*)fTable[ pageIndex ];
	index = handle - ( pageIndex << 16 ) - 0x10000;
	if ( index < 0 || index >= 0xfc )
	{
		char	message[ 256 ];
		sprintf( message, "Trying to Resize an invalid page handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	char*	entry = (char*)page + index * 0x100;
	n = *(unsigned short*)( entry + 10 );
	if ( n <= 0 )
	{
		char	message[ 256 ];
		sprintf( message, "Trying to Resize an invalid page handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	page->mCheckForBoundsError( index );
	short*	locks = (short*)( entry + 8 );
	while ( *locks > 0 )
	{
		if ( vfunction12( handle ) != 1 )
		{
			return 0;
		}
		BindPart( tMemoryPart )->vfunction25( "You are attempting to Resize A Locked Handle -- I'm removing 1 Lock!" );
	}

	name = entry + *(int*)entry * 4 + 0x1c;
	words = ( size + 3 ) >> 2;
	long	need = strlen( name ) + words * 4 + 0x1d;
	for ( slots = 1; need > 0x100 && slots <= 4; slots++ )
	{
		need -= 0x100;
	}

	if ( slots == n )
	{
		if ( words < *(int*)entry )
		{
			memmove( entry + 0x14 + words * 4, entry + 0x14 + *(int*)entry * 4, *(short*)( entry + 6 ) + 8 );
			memset( entry + *(short*)( entry + 6 ) + words * 4 + 0x1c, 0, ( *(int*)entry - words ) * 4 );
		}
		else if ( words > *(int*)entry )
		{
			memmove( entry + 0x14 + words * 4, entry + 0x14 + *(int*)entry * 4, *(short*)( entry + 6 ) + 8 );
			memset( entry + 0x14 + *(int*)entry * 4, 0, ( words - *(int*)entry ) * 4 );
		}
		*(int*)entry = words;
		return handle;
	}

	long	end = n + index;
	b = page->fSizes[ end - 1 ];
	if ( slots < n )
	{
		memmove( entry + 0x14 + words * 4, entry + 0x14 + *(int*)entry * 4, *(short*)( entry + 6 ) + 8 );
		// HYPOTHESIS: the original clears from the word b into the block, not from the new end.
		memset( entry + *(short*)( entry + 6 ) + b * 4 + 0x1c, 0, ( *(int*)entry - words ) * 4 );
		if ( b > 0 )
		{
			long	d = page->fSizes[ end - 1 ];
			if ( end < 0xfc )
			{
				long	c = page->fSizes[ end ];
				page->fSizes[ end - 1 ] = 0;
				page->fSizes[ end ] = 0;
				page->fSizes[ n - d + index ] = (unsigned short)( slots - n + d );
				page->fSizes[ index + slots - 1 ] = (unsigned short)( slots - n + d );
				page->fSizes[ index + slots ] = (unsigned short)( c - slots + n );
				page->fSizes[ c + n + index - 1 ] = (unsigned short)( c - slots + n );
				*(int*)entry = words;
				*(unsigned short*)( entry + 10 ) = (unsigned short)slots;
				return handle;
			}
			page->fSizes[ n - d + index ] = (unsigned short)( slots - n + d );
			page->fSizes[ index + slots - 1 ] = (unsigned short)( slots - n + d );
			page->fSizes[ index + slots ] = (unsigned short)( n - slots );
			page->fSizes[ end - 1 ] = (unsigned short)( n - slots );
			*(int*)entry = words;
			*(unsigned short*)( entry + 10 ) = (unsigned short)slots;
			return handle;
		}
		long	start = 0;
		long	i = page->fSizes[ 0 ];
		if ( i <= index )
		{
			do
			{
				start = i;
				i += page->fSizes[ i ];
			} while ( i <= index );
		}
		long	f = page->fSizes[ start ];
		page->fSizes[ slots + index - 1 ] = (unsigned short)( slots - start + index );
		page->fSizes[ start ] = (unsigned short)( slots - start + index );
		page->fSizes[ slots + index ] = (unsigned short)( n - slots );
		page->fSizes[ end - 1 ] = (unsigned short)( n - slots );
		page->fSizes[ end ] = (unsigned short)( f - ( slots - start + index ) - ( n - slots ) );
		page->fSizes[ start + f - 1 ] = (unsigned short)( f - ( slots - start + index ) - ( n - slots ) );
		*(int*)entry = words;
		*(unsigned short*)( entry + 10 ) = (unsigned short)slots;
		return handle;
	}

	newEnd = index + slots;
	if ( newEnd < 0xfc && slots <= 4 )
	{
		long	grow = slots - n;
		if ( b > 0 )
		{
			if ( page->fSizes[ end ] > grow && *(unsigned short*)( entry + 0x10a ) == 0 )
			{
				memmove( entry + 0x14 + words * 4, entry + 0x14 + *(int*)entry * 4, *(short*)( entry + 6 ) + 8 );
				memset( entry + 0x14 + *(int*)entry * 4, 0, ( words - *(int*)entry ) * 4 );
				long	d = page->fSizes[ end - 1 ];
				long	c = page->fSizes[ end ];
				page->fSizes[ end - 1 ] = 0;
				page->fSizes[ end ] = 0;
				page->fSizes[ n - d + index ] = (unsigned short)( grow + d );
				page->fSizes[ newEnd - 1 ] = (unsigned short)( grow + d );
				page->fSizes[ newEnd ] = (unsigned short)( c - grow );
				page->fSizes[ c + n + index - 1 ] = (unsigned short)( c - grow );
				*(int*)entry = words;
				*(unsigned short*)( entry + 10 ) = (unsigned short)slots;
				return handle;
			}
			else if ( page->fSizes[ end ] == grow && *(unsigned short*)( entry + 0x10a ) == 0 )
			{
				memmove( entry + 0x14 + words * 4, entry + 0x14 + *(int*)entry * 4, *(short*)( entry + 6 ) + 8 );
				memset( entry + 0x14 + *(int*)entry * 4, 0, ( words - *(int*)entry ) * 4 );
				long	c = page->fSizes[ end ];
				long	d = page->fSizes[ end - 1 ];
				// HYPOTHESIS: the next block is found by adding c and index to the new end.
				long	next = newEnd + c + index;
				if ( next < 0xfc )
				{
					long	e = page->fSizes[ next ];
					page->fSizes[ end - 1 ] = 0;
					page->fSizes[ end ] = 0;
					page->fSizes[ next ] = 0;
					page->fSizes[ next - 1 ] = 0;
					page->fSizes[ n - d + index ] = (unsigned short)( c + e + d );
					page->fSizes[ c + e + n + index - 1 ] = (unsigned short)( c + e + d );
				}
				else
				{
					page->fSizes[ end - 1 ] = 0;
					page->fSizes[ end ] = 0;
					page->fSizes[ n - d + index ] = (unsigned short)( c + d );
					page->fSizes[ next - 1 ] = (unsigned short)( c + d );
				}
				*(int*)entry = words;
				*(unsigned short*)( entry + 10 ) = (unsigned short)slots;
				return handle;
			}
		}
	}

	long	newHandle = BindPart( tMemoryPart )->vfunction16( size, *(short*)( entry + 4 ), name );
	void*	newPtr = BindPart( tMemoryPart )->vfunction21( newHandle );
	if ( newPtr )
	{
		memcpy( newPtr, entry + 0x14, *(int*)entry * 4 );
	}
	BindPart( tMemoryPart )->vfunction20( handle );
	BindPart( tMemoryPart )->vfunction22( newHandle );
	return newHandle;
}

// FUNCTION: STARFLEETOP 0x0063e450
long	tPagingMechanism::mDuplicateHandle( long handle )
{
	if ( handle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to duplicate an invalid page handle" );
		return 0;
	}
	if ( fTable == NULL )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to duplicate an invalid page handle" );
		return 0;
	}
	long	pageIndex = ( handle >> 16 ) - 1;
	if ( pageIndex < 0 || pageIndex >= fTableSize )
	{
		char	message[ 256 ];
		sprintf( message, "Trying to duplicate an invalid page handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	tPagingBlock*	page = (tPagingBlock*)fTable[ pageIndex ];
	long	index = handle - ( pageIndex << 16 ) - 0x10000;
	if ( index < 0 || index >= 0xfc )
	{
		char	message[ 256 ];
		sprintf( message, "Trying to duplicate an invalid page handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	char*	entry = (char*)page + index * 0x100;
	if ( *(unsigned short*)( entry + 10 ) <= 0 )
	{
		char	message[ 256 ];
		sprintf( message, "Trying to duplicate an invalid page handle %ld", handle );
		BindPart( tMemoryPart )->vfunction25( message );
		return 0;
	}
	page->mCheckForBoundsError( index );
	long	newHandle = mNewHandleZeroIt( *(int*)entry * 4, 0x4ef, __FILE__ );
	if ( newHandle == 0 )
	{
		return 0;
	}
	void*	newPtr = BindPart( tMemoryPart )->vfunction21( newHandle );
	void*	oldPtr = BindPart( tMemoryPart )->vfunction21( handle );
	if ( newPtr && oldPtr )
	{
		memcpy( newPtr, oldPtr, *(int*)entry * 4 );
	}
	BindPart( tMemoryPart )->vfunction22( handle );
	BindPart( tMemoryPart )->vfunction22( newHandle );
	return newHandle;
}

// FUNCTION: STARFLEETOP 0x0063e9a0
long	tPagingMechanism::mResizeHandleZeroIt( long a, long b )
{
	return mResizeHandle( a, b );
}

// FUNCTION: STARFLEETOP 0x0063e9c0
void*	tPagingMechanism::mResizeBlock( void* a, long b )
{
	long	handle = mGetHandleFromPointer( a );
	if ( handle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to resize a nonexistant paging block" );
		return 0;
	}
	if ( vfunction12( handle ) == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Couldn't remove locks while trying to resize a nonexistant paging block" );
	}
	long	newHandle = mResizeHandle( handle, b );
	if ( newHandle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Couldn't resize a paging block" );
		return 0;
	}
	return BindPart( tMemoryPart )->vfunction21( newHandle );
}

// FUNCTION: STARFLEETOP 0x0063ec10
void*	tPagingMechanism::mDuplicateBlock( void* a )
{
	long	handle = mGetHandleFromPointer( a );
	if ( handle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Trying to duplicate a nonexistant paging block" );
		return 0;
	}
	long	newHandle = mDuplicateHandle( handle );
	if ( newHandle == 0 )
	{
		BindPart( tMemoryPart )->vfunction25( "Couldn't duplicate a paging block" );
		return 0;
	}
	return BindPart( tMemoryPart )->vfunction21( newHandle );
}
