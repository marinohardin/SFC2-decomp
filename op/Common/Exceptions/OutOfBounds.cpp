#include "Common/Exceptions/OutOfBounds.h"

namespace nException
{

// FUNCTION: STARFLEETOP 0x006fd240
tOutOfBounds::tOutOfBounds( const char* object, int32 min, int32 max, int32 value, const char* file, int32 lineNumber )
	: tBaseException( "Out of bounds", file, lineNumber )
{
	fObject = object;
	fMin = min;
	fMax = max;
	fValue = value;
}

};
