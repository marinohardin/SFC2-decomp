//	tindexedarray.h

#ifndef tindexedarray_h
#define tindexedarray_h

#include "Tiger/tpart.h"

// The class name is in the exe's RTTI (tStandardMemoryArray<tAsynchReadInfo>, vtable
// 0x007b7444); the file name and the allocation lines are in the exe's strings.
// HYPOTHESIS: layout from the constructor inlined into tAudioFileInfoPart::mActivate
// (0x00657980): an item block, a count, a grow step of 0x40, a default item copied into every
// new slot, and one byte per slot set to 1. The field names are not recovered. The allocation
// goes through tMemoryPart, which the including unit declares. The constructor sizes the
// blocks through the grow function: only that form gives 0x00657980 its exact epilogue.
template <class T>
class	tStandardMemoryArray
{
	public:
						tStandardMemoryArray	( long aSize );
		virtual			~tStandardMemoryArray	( void );

		// HYPOTHESIS: takes a free slot (growing the blocks by the grow step when all are
		// used), marks it used and returns its index. Inlined into
		// tAudioFileInfoPart::mGetNewAsynchReadInfo (0x006580c0); its name is not recovered.
		long			fn_New					( void );
		// HYPOTHESIS: grows both blocks to a size (allocating them when empty) and marks the
		// new slots free; the allocation lines match the constructor's. Name not recovered.
		void			fn_Grow					( long aSize );
		// HYPOTHESIS: true when an index names a slot in use (its byte is 0); the result is
		// kept as a value (sete) before the test. Its name is not recovered.
		iTruth			fn_IsUsed				( long aIndex );
		// HYPOTHESIS: returns the item at a used slot, or 0. Its name is not recovered.
		T				*fn_Get					( long aIndex );
		// HYPOTHESIS: marks a used slot free and releases both blocks when none is left in use.
		// Inlined into tAudioFileInfoPart::mFreeAsynchReadInfo (0x006582c0); name not recovered.
		void			fn_Free					( long aIndex );

		long			field_04;
		T				*field_08;
		long			field_0c;
		long			field_10;
		long			field_14;
		T				field_18;
		int8u			*field_28;
		long			field_2c;
};

template <class T>
inline tStandardMemoryArray<T>::tStandardMemoryArray( long aSize )
{
	field_08 = NULL;
	field_0c = 0;
	field_10 = 0;
	field_14 = 0x40;
	field_2c = -1;
	field_28 = NULL;

	fn_Grow( aSize );
}

template <class T>
inline void tStandardMemoryArray<T>::fn_Grow( long aSize )
{
	if ( field_10 == 0 )
	{
#line 263
		field_08 = (T *) BindPart(tMemoryPart)->mAllocate( aSize * sizeof(T), __LINE__, __FILE__ );
		field_28 = (int8u *) BindPart(tMemoryPart)->mAllocate( aSize, __LINE__, __FILE__ );
	}
	else
	{
		field_08 = (T *) BindPart(tMemoryPart)->vfunction8( field_08, aSize * sizeof(T) );
#line 269
		field_28 = (int8u *) BindPart(tMemoryPart)->mAllocate( aSize, __LINE__, __FILE__ );
	}

	for ( ; field_10 < aSize; field_10++ )
	{
		field_08[field_10] = field_18;
		field_28[field_10] = 1;
	}
}

template <class T>
inline long tStandardMemoryArray<T>::fn_New( void )
{
	long	index = 0;

	if ( field_0c >= field_10 )
	{
		fn_Grow( field_10 + field_14 );
		index = field_0c;
	}
	else
	{
		if ( field_28[field_0c] == 1 )
			index = field_0c;
		else
		{
			for ( long i = 0; i < field_0c; i++ )
			{
				if ( field_28[i] == 1 )
				{
					index = i;
					i = field_0c;
				}
			}
		}
	}

	field_28[index] = 0;
	field_0c++;

	return index;
}

template <class T>
inline iTruth tStandardMemoryArray<T>::fn_IsUsed( long aIndex )
{
	if ( aIndex < field_10 && aIndex >= 0 )
		return field_28[aIndex] == 0;
	return kFalse;
}

template <class T>
inline T *tStandardMemoryArray<T>::fn_Get( long aIndex )
{
	if ( !fn_IsUsed( aIndex ) )
		return NULL;
	return &field_08[aIndex];
}

template <class T>
inline void tStandardMemoryArray<T>::fn_Free( long aIndex )
{
	if ( fn_IsUsed( aIndex ) )
	{
		field_28[aIndex] = 1;
		field_0c--;
		if ( field_0c <= 0 )
		{
			if ( field_08 )
			{
				BindPart(tMemoryPart)->vfunction10( field_08 );
				field_08 = NULL;
			}
			if ( field_28 )
			{
				BindPart(tMemoryPart)->vfunction10( field_28 );
				field_28 = NULL;
			}
			field_0c = 0;
			field_10 = 0;
		}
	}
}

// HYPOTHESIS: tSafeArray (declared in unsorted/tSafeArray.h) keeps its add here; the allocation
// lines are the original's (0x2c9, 0x2ca).
template <class T>
inline unsigned long tSafeArray<T>::fn_Add( T *aItem )
{
	unsigned long	index = 0;

	if ( f0c >= f10 )
	{
		unsigned long	newSize = f10 + f14;

		if ( f10 == 0 )
		{
#line 713
			f04 = (T **) BindPart(tMemoryPart)->mAllocate( newSize * sizeof(T *), __LINE__, __FILE__ );
			f08 = (unsigned short *) BindPart(tMemoryPart)->mAllocate( newSize * sizeof(int16u), __LINE__, __FILE__ );
		}
		else
		{
			f04 = (T **) BindPart(tMemoryPart)->vfunction8( f04, newSize * sizeof(T *) );
			// MATCH-ONLY: the original calls the binding out of line here (0x004133d3) after
			// inlining the three before it; a call through a function pointer reproduces that.
			tMemoryPart *(*search)() = &tMemoryPart::Search_tMemoryPart;
			f08 = (unsigned short *) search()->vfunction8( (void *) f08, newSize * sizeof(int16u) );
		}

		for ( ; f10 < newSize; f10++ )
		{
			((T **) f04)[f10] = NULL;
			((int16u *) f08)[f10] = 1;
		}
		index = f0c;
	}
	else
	{
		if ( ((T **) f04)[f0c] == NULL )
			index = f0c;
		else
		{
			for ( long i = 0; i < (long) f0c; i++ )
			{
				if ( ((T **) f04)[i] == NULL )
				{
					index = i;
					i = f0c;
				}
			}
		}
	}

	((T **) f04)[index] = aItem;
	f0c++;
	((int16u *) f08)[index]++;
	if ( ((int16u *) f08)[index] == 0 )
		((int16u *) f08)[index] = 1;

	return ( ((int16u *) f08)[index] << 16 ) + index;
}

#endif	// tindexedarray_h
