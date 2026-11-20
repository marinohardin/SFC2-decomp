#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMapProc/AsyncDatabaseProcedure_GetMetaMapHexForUpdate.h"
#include "Common/DataStore/DataStoreInput.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/nAsyncDatabaseProcedure__tAsyncDatabase.h"
#include "unsorted/nStoredProcedureArguments__tMetaMapHexRequest.h"
#include "unsorted/nSwitch__tAddress.h"
#include "Switch/Switch.h"

using namespace nAsyncDatabaseProcedure;

// The SDK's inline destructor of the reader, expanded into the function below as in the
// original; Close itself is called out of line (0x0056027f).
// (DataStoreInput.h defines it.)




// HYPOTHESIS: the callback for the database's answer: reads the hex point from the request,
// fetches that hex locked for update and sends it back in the response.
// FUNCTION: STARFLEETOP 0x00565464
void tAsyncDatabaseProcedure_GetMetaMapHexForUpdate::fn_00565464( const nDataStore::tBuffer* pBuffer )
{
	nStoredProcedureArguments::tMetaMapHexRequest request;
	nDataStore::tInput input( pBuffer );
	input >> request;

	std::vector<tMetaMapHex> hexVector;
	fpAsyncDatabase->vfunction13( request.m_18, &hexVector, false );

	nStoredProcedureArguments::tMetaMapHexRequest::tMetaMapHexResponse response;
	response.fn_00716310( 0 );
	std::vector<tMetaMapHex>::iterator hex = hexVector.begin();
#line 61
	validate( hexVector.size() == 1 );
	response.m_10 = *hex;

	fn_00719050();
	field_1c->fn_00714020( request.vfunction8().Get(), request.vfunction9(), &response );
}
