// tVisGroupAsset.cpp: the class declaration only; the functions are written one at a time.

#include "unsorted/tVisGroupAsset.h"
#include "Tiger/tStream.h"

// FUNCTION: STARFLEETOP 0x0067b2b0
tStream& tVisGroupAsset::vfunction5( tStream& s )
{
	tAsset::vfunction5( s );
	unsigned long v = field_BC;
	tStream& t = s << v;
	int count = (int)field_5C.size();
	t << (unsigned long)count;
	for ( std::vector<c_0067b2b0>::iterator it = field_5C.begin(); it != field_5C.end(); ++it )
		it->fn_slot0( t );
	// HEADER REQUEST: c_00678500 needs fn_0067f5d0( tStream& ) for the call field_6C.fn_0067f5d0( s ) here.
	return s;
}

// HYPOTHESIS: c_00678500 is still declared as bytes in tAsset.h, so its implicit operator=
// copies 0x50 bytes; the original's is the member-wise one (vector of 0x30-byte entries at +0,
// map keyed by tString at +0x10, int at +0x24, tString at +0x28).
// FUNCTION: STARFLEETOP 0x0067b390
void tVisGroupAsset::vfunction3( const c_00678500& x )
{
	field_6C = x;
}

// FUNCTION: STARFLEETOP 0x0067b580
c_00678500* tVisGroupAsset::vfunction4()
{
	return &field_6C;
}
