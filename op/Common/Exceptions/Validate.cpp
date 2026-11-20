#include "Common/Exceptions/Validate.h"
#include "Common/Log/LogStream.h"

namespace nException
{

// FUNCTION: STARFLEETOP 0x006fd810
void tValidationFailed::ReportException( const char* caughtDescription ) throw()
{
	try
	{
		ReportHeader( caughtDescription );

		nLog::error << fExpression << " failed the validate in " << fpFileName << "(" << fLineNumber << ")" << nLog::report;

		ReportFooter();
	}
	catch( ... )
	{

	}
}

};
