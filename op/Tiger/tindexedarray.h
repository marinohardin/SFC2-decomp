//	tindexedarray.h

#ifndef tindexedarray_h
#define tindexedarray_h

#include "Tiger/tpart.h"

// The class name is in the exe's RTTI (tStandardMemoryArray<tAsynchReadInfo>, vtable
// 0x007b7444); the file name and the allocation lines are in the exe's strings.
// HYPOTHESIS: layout from the constructor inlined into tAudioFileInfoPart::mActivate
// (0x00657980): an item block, a count, a grow step of 0x40, a default item copied into every
// new slot, and one byte per slot set to 1. The field names are not recovered. The allocation
// goes through tMemoryPart, which the including unit declares.
template <class T>
class	tStandardMemoryArray
{
	public:
						tStandardMemoryArray	( long aSize );
		virtual			~tStandardMemoryArray	( void );

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

#line 263
	field_08 = (T *) BindPart(tMemoryPart)->mAllocate( aSize * sizeof(T), __LINE__, __FILE__ );
	field_28 = (int8u *) BindPart(tMemoryPart)->mAllocate( aSize, __LINE__, __FILE__ );

	for ( ; field_10 < aSize; field_10++ )
	{
		field_08[field_10] = field_18;
		field_28[field_10] = 1;
	}
}

// HYPOTHESIS: tSafeArray (declared in Tiger/tAudioFile.h) keeps its add here; the allocation
// lines are the original's (0x2c9, 0x2ca).
template <class T>
inline unsigned long tSafeArray<T>::fn_Add( T *aItem )
{
	unsigned long	index = 0;

	if ( field_0c >= field_10 )
	{
		unsigned long	newSize = field_10 + field_14;

		if ( field_10 == 0 )
		{
#line 713
			field_04 = (T *) BindPart(tMemoryPart)->mAllocate( newSize * sizeof(T *), __LINE__, __FILE__ );
			field_08 = (long) BindPart(tMemoryPart)->mAllocate( newSize * sizeof(int16u), __LINE__, __FILE__ );
		}
		else
		{
			field_04 = (T *) BindPart(tMemoryPart)->vfunction8( field_04, newSize * sizeof(T *) );
			// MATCH-ONLY: the original calls the binding out of line here (0x004133d3) after
			// inlining the three before it; a call through a function pointer reproduces that.
			tMemoryPart *(*search)() = &tMemoryPart::Search_tMemoryPart;
			field_08 = (long) search()->vfunction8( (void *) field_08, newSize * sizeof(int16u) );
		}

		for ( ; field_10 < newSize; field_10++ )
		{
			((T **) field_04)[field_10] = NULL;
			((int16u *) field_08)[field_10] = 1;
		}
		index = field_0c;
	}
	else
	{
		if ( ((T **) field_04)[field_0c] == NULL )
			index = field_0c;
		else
		{
			for ( long i = 0; i < (long) field_0c; i++ )
			{
				if ( ((T **) field_04)[i] == NULL )
				{
					index = i;
					i = field_0c;
				}
			}
		}
	}

	((T **) field_04)[index] = aItem;
	field_0c++;
	((int16u *) field_08)[index]++;
	if ( ((int16u *) field_08)[index] == 0 )
		((int16u *) field_08)[index] = 1;

	return ( ((int16u *) field_08)[index] << 16 ) + index;
}

#endif	// tindexedarray_h
