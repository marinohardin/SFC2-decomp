// tSafeArray.h: Tiger's handle-indexed array template (name and members from the SDK library
// symbols of its tClassTrackingInfo, tBasicAudioFile and tChannelGroup instantiations).

#ifndef TSAFEARRAY_H
#define TSAFEARRAY_H

#include "unsorted/tMemoryPart.h"

// HYPOTHESIS: the layout follows the reads in tAudioPriorityManagerPart::mAreWeAllowedToPlay
// (0x00661fa0): the item table, a 16-bit use count per slot (the high half of a handle), the
// number of items and the table's size. Member names are not known.
template < class T >
class tSafeArray
{
	public:
		virtual		~tSafeArray( void );

		T *				mGetItemAtThisIndex		( unsigned long );
		void			mRemoveItemAtThisIndex	( unsigned long );
		unsigned long	mGetNextItemIndex		( unsigned long );
		unsigned char	mIsThisIndexValid		( unsigned long );

		// HYPOTHESIS: the three helpers below are inlined into mAreWeAllowedToPlay (0x00661fa0)
		// and have no symbol; their names are this project's own. mDropItem does what
		// mRemoveItemAtThisIndex (0x00663210) does, but calls the validity check out of line.
		inline	unsigned long	mFirstItemIndex	( void );
		inline	unsigned long	mAddItem		( T * );
		inline	void			mDropItem		( unsigned long );
		// HYPOTHESIS: mWeHaveStoppedPlaying (0x006624b0) inlines a validity check, a lookup and a
		// removal; the names are this project's own.
		inline	unsigned char	mIsValid		( unsigned long );
		inline	T *				mPeekItem		( unsigned long );
		inline	void			mClearItem		( unsigned long );
		// HYPOTHESIS: tAudioPriorityManagerPart::mClose (0x00662540) inlines the release of every
		// item, when the array owns them, and of both tables; the name is this project's own.
		inline	void			mDeleteAll		( void );

	protected:
		unsigned long	mGetIndexFromRealIndex	( unsigned long );
		void			mAddMoreRoom			( long );

		T				**f04;
		unsigned short	*f08;
		// HYPOTHESIS: the count is unsigned and the size signed, from the compares in
		// mClose (0x00662540).
		unsigned long	f0c;
		long			f10;
		// HYPOTHESIS: the field at +0x14 is not known; the flag at +0x18 says the array owns its
		// items and deletes them (mClose at 0x00662540 compares it with 1).
		long			f14;
		unsigned char	f18;
};

template < class T >
inline unsigned long tSafeArray< T >::mFirstItemIndex( void )
{
	if ( f0c )
	{
		for ( unsigned long i = 0; i < f10; i++ )
		{
			if ( f04[i] )
				return mGetIndexFromRealIndex( i );
		}
	}
	return 0;
}

template < class T >
inline unsigned long tSafeArray< T >::mAddItem( T *item )
{
	unsigned long index = 0;
	if ( f0c >= f10 )
	{
		mAddMoreRoom( 0 );
		index = f0c;
	}
	else if ( f04[f0c] == NULL )
		index = f0c;
	else
	{
		for ( long i = 0; i < f0c; i++ )
		{
			if ( f04[i] == NULL )
			{
				index = i;
				i = f0c;
			}
		}
	}
	f04[index] = item;
	f0c++;
	f08[index]++;
	if ( f08[index] == 0 )
		f08[index] = 1;
	return ( f08[index] << 16 ) + index;
}

template < class T >
inline void tSafeArray< T >::mDropItem( unsigned long index )
{
	if ( mIsThisIndexValid( index ) )
	{
		f04[index & 0xffff] = NULL;
		f0c--;
	}
}

template < class T >
inline unsigned char tSafeArray< T >::mIsValid( unsigned long index )
{
	unsigned long realIndex = index & 0xffff;
	if ( realIndex < f10 && (unsigned short)( index >> 16 ) == f08[realIndex] && f04[realIndex] != NULL )
		return true;
	return false;
}

template < class T >
inline T *tSafeArray< T >::mPeekItem( unsigned long index )
{
	if ( mIsValid( index ) )
		return f04[index & 0xffff];
	return NULL;
}

template < class T >
inline void tSafeArray< T >::mClearItem( unsigned long index )
{
	if ( mIsValid( index ) )
	{
		f04[index & 0xffff] = NULL;
		f0c--;
	}
}

template < class T >
inline void tSafeArray< T >::mDeleteAll( void )
{
	if ( f18 == true )
	{
		for ( long i = 0; f0c > 0 && i < f10; i++ )
		{
			if ( f04[i] )
			{
				delete f04[i];
				f04[i] = NULL;
				f0c--;
			}
		}
	}
	if ( f04 )
	{
		BindPart( tMemoryPart )->vfunction10( f04 );
		f04 = NULL;
	}
	if ( f08 )
	{
		BindPart( tMemoryPart )->vfunction10( f08 );
		f08 = NULL;
	}
	f0c = 0;
	f10 = 0;
}

#endif // TSAFEARRAY_H
