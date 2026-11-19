#include "unsorted/tAsset.h"
#include "Tiger/tStream.h"

// FUNCTION: STARFLEETOP 0x00677d20
tStream& tCastItem::vfunction5( tStream& s )
{
	tAsset::vfunction5( s );
	s << (unsigned long)field_5C << (unsigned long)field_62;
	return s;
}
