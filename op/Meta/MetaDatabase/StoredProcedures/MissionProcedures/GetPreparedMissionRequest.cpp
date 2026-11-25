#define DATASTORE_OUT_OF_LINE_BUFFER_GET	// MATCH-ONLY: tBuffer::Get is the out-of-line copy in fn_0042A000.cpp (0x0042a31c)
#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Exceptions/Validate.h"
#include "Meta/MetaDatabase/StoredProcedures/MissionProcedures/GetPreparedMissionRequest.h"

using namespace nStoredProcedureArguments;

















// FUNCTION: STARFLEETOP 0x00550d34
void tGetPreparedMissionRequest::vfunction3( nDataStore::tInput& input )
{
	tRequestWithResponse::vfunction3( input );
	int8u c = fObjectVersion;
	fObjectVersion = 0;
	if( c != 0 )
	{
#line 126
		validate( 0 );
	}
	else
	{
		fFlag = ( input >> fKey ).Get() != 0;
	}
}
