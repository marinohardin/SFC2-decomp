#include "Common/DataStore/Streamable.h"

namespace nDataStore
{

// FUNCTION: STARFLEETOP 0x006f8cd0
// FUNCTION: STARFLEETOP 0x006f8cb0
tStreamable::tCRCFailed::~tCRCFailed()
{
}

// FUNCTION: STARFLEETOP 0x006f8d50
void tStreamable::tCRCFailed::ReportException( const char* caughtDescription )
{
	tBaseException::ReportException( caughtDescription );
}

// FUNCTION: STARFLEETOP 0x006f8d70
tStreamable::iObjectIDInfoLookup tStreamable::fObjectIDInfoLookup;

};
