#include "Common/Exceptions/OutOfBounds.h"

// FUNCTION: STARFLEETOP 0x0042838f
nException::tOutOfBounds::tOutOfBounds( const tOutOfBounds& other )
	: tBaseException( other ), fObject( other.fObject ), fMin( other.fMin ), fMax( other.fMax ), fValue( other.fValue )
{
}
