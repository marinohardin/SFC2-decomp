#include "unsorted/tAsset.h"
#include "Tiger/tStream.h"

// FUNCTION: STARFLEETOP 0x00678180
tStream& tBoundAsset::vfunction5( tStream& s )
{
	tAsset::vfunction5( s );
	s << (unsigned short)field_5C << (unsigned short)field_5E;
	return s;
}
