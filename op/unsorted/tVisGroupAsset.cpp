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
