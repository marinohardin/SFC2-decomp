#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMapProc/AsyncDatabaseProcedure_UpdateMetaMapHex.h"
#include "Common/DataStore/DataStoreInput.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/nAsyncDatabaseProcedure__tAsyncDatabase.h"
#include "unsorted/nStoredProcedureArguments__tMetaMapHexUpdateRequest.h"
#include "unsorted/nSwitch__tAddress.h"
#include "unsorted/nSwitch__tSwitch.h"

using namespace nAsyncDatabaseProcedure;

// The SDK's inline destructor of the reader, expanded into the function below as in the
// original; Close itself is called out of line (0x0056027f).
inline nDataStore::tInput::~tInput()
{
	Close( fCleanUpBuffer );
}

// HYPOTHESIS: the callback for the database's answer: reads the hex update from the request,
// stores it, reads the updated hex back and sends it in the response.
// FUNCTION: STARFLEETOP 0x00566d8a
void tAsyncDatabaseProcedure_UpdateMetaMapHex::fn_00566d8a( const nDataStore::tBuffer* pBuffer )
{
	nStoredProcedureArguments::tMetaMapHexUpdateRequest request;
	nDataStore::tInput input( pBuffer );
	input >> request;

	nStoredProcedureArguments::tMetaMapHexUpdateRequest::tMetaMapHexUpdateResponse response;
#line 55
	validate( request.GetHexUpdateVector().size() == 1 );
	const nDatabase::tDatabaseObjectUpdate& update = request.GetHexUpdateVector()[0];
	fpAsyncDatabase->vfunction47( update );
	fpAsyncDatabase->vfunction15( update.GetUpdateID(), &response.m_10, true );

	fn_00719050();
	response.fn_00716310( 0 );
	m_1c->fn_00714020( request.vfunction8().Get(), request.vfunction9(), &response );
}
