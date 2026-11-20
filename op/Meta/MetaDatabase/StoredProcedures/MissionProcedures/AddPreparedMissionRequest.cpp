#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Exceptions/Validate.h"
#include "Meta/MetaDatabase/StoredProcedures/MissionProcedures/AddPreparedMissionRequest.h"

using namespace nStoredProcedureArguments;

// FUNCTION: STARFLEETOP 0x0054fa6d
void tAddPreparedMissionRequest::vfunction3( nDataStore::tInput& input )
{
	nSwitch::tRequestWithResponse::vfunction3( input );
	char c = fObjectVersion;
	fObjectVersion = 0;
	if( c != 0 )
	{
#line 128
		validate( 0 );
	}
	else
	{
		fFlag = ( input >> fPreparedMissions ).Get() != 0;
	}
}
