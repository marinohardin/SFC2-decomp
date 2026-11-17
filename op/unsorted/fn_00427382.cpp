// Unit:      unsorted/fn_00427382.cpp
// Functions: 0x00427382 to 0x00427fb2, 37 in op/UNITS.tsv

#include "unsorted/tCurrentTime.h"

// HYPOTHESIS: the compiler's implicit memberwise copy constructor, written out.
// FUNCTION: STARFLEETOP 0x0042739e
tCurrentTime::tCurrentTime( const tCurrentTime& source )
	: nDataStore::tStreamable( source ),
	  field_0xc( source.field_0xc ),
	  field_0x10( source.field_0x10 ),
	  field_0x14( source.field_0x14 ),
	  field_0x18( source.field_0x18 ),
	  field_0x1c( source.field_0x1c ),
	  field_0x20( source.field_0x20 )
{
}
