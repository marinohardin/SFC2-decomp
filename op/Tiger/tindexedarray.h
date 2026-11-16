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

#endif	// tindexedarray_h
