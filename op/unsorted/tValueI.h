//	tValueI.h: tValueI<T>, the value object that holds a T and notifies when it is set.
// Evidence:  class (RTTI names tValueI<tString>; original file unknown)

#ifndef tValueI_h
#define tValueI_h 1

#include "unsorted/tValueBase.h"

// RTTI names the class tValueI<tString>; vtable 0x0079b8a4, the value at +0x18.
template< class T >
class tValueI : public tValueBase
{
public:
	// HYPOTHESIS: the set step; the original assigns the value, notifies with (3, 0), and
	// only then destroys the temporary it was given.
	tValueI &operator=( const T &value ) { m18 = value; fn_006712c0( 3, 0 ); return *this; }
	// HYPOTHESIS: a conversion returning a copy of the value (0x0043d344 for tValueI<tString>);
	// the caller copies the returned temporary once more, as a conversion does under VC6.
	operator T();

	T m18;
};

#endif	// tValueI_h
