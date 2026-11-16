// tSafeArray.h: Tiger's handle-indexed array template (name and members from the SDK library
// symbols of its tClassTrackingInfo, tBasicAudioFile and tChannelGroup instantiations).

#ifndef TSAFEARRAY_H
#define TSAFEARRAY_H

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

	protected:
		unsigned long	mGetIndexFromRealIndex	( unsigned long );
		void			mAddMoreRoom			( long );

		T				**f04;
		unsigned short	*f08;
		long			f0c;
		unsigned long	f10;
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

#endif // TSAFEARRAY_H
