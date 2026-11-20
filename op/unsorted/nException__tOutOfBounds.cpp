#include "Common/Exceptions/OutOfBounds.h"

namespace nException
{

// FUNCTION: STARFLEETOP 0x0042838f
tOutOfBounds::tOutOfBounds( const tOutOfBounds& other )
	: tBaseException( other ), fObject( other.fObject ), fMin( other.fMin ), fMax( other.fMax ), fValue( other.fValue )
{
}

}
