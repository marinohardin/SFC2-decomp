#include "Common/Exceptions/BaseException.h"

namespace nException
{

// FUNCTION: STARFLEETOP 0x006fd340
tBaseException::tBaseException( const char* pDescription, const char* pFile, int32 lineNumber ) throw()
	: std::exception( pDescription ), fpFileName( pFile ), fLineNumber( lineNumber )
{
}

// FUNCTION: STARFLEETOP 0x006fd370
tBaseException::~tBaseException() throw()
{
}

};
