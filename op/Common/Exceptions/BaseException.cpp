#include "Common/Exceptions/BaseException.h"
#include "Common/Log/LogStream.h"

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

// FUNCTION: STARFLEETOP 0x006fd380
void tBaseException::ReportHeader( const char* caughtDescription ) throw()
{
	try
	{
		nLog::error << caughtDescription << std::endl
			<< "Exception: " << GetDescription() << " in " << std::endl
			<< GetFile() << "(" << GetLineNumber() << ")" << std::endl << std::endl;
	}
	catch( ... )
	{
	}
}

// FUNCTION: STARFLEETOP 0x006fd590
void tBaseException::ReportException( const char* caughtDescription ) throw()
{
	try
	{
		ReportHeader( caughtDescription );
		ReportFooter();
		nLog::error << nLog::report;
	}
	catch( ... )
	{
	}
}

};
