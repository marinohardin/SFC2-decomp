#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMapProc/AsyncDatabaseProcedure_GetPoliticalTensionMatrix.h"
#include "Common/DataStore/DataStoreInput.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/nAsyncDatabaseProcedure__tAsyncDatabase.h"
#include "unsorted/nStoredProcedureArguments__tGetPoliticalTensionMatrixRequest.h"
#include "unsorted/nSwitch__tAddress.h"
#include "Switch/Switch.h"

using namespace nAsyncDatabaseProcedure;

// The SDK's inline destructor of the reader, expanded into the function below as in the
// original; Close itself is called out of line (0x0056027f).
// (DataStoreInput.h defines it.)




// HYPOTHESIS: the callback for the database's answer: reads the matrix in from the database,
// or makes a new one there when there is none, and sends it back in the response.
// FUNCTION: STARFLEETOP 0x0056571c
void tAsyncDatabaseProcedure_GetPoliticalTensionMatrix::fn_0056571c( const nDataStore::tBuffer* pBuffer )
{
	nStoredProcedureArguments::tGetPoliticalTensionMatrixRequest request;
	nStoredProcedureArguments::tGetPoliticalTensionMatrixRequest::tGetPoliticalTensionMatrixResponse response;
	nDataStore::tInput input( pBuffer );
	input >> request;

	std::vector<tPoliticalTensionMatrix> matrixVector;
	fpAsyncDatabase->vfunction19( &matrixVector );

	int resultSize = matrixVector.size();
#line 57
	validate( resultSize <= 1 );
	if( resultSize > 0 )
	{
		response.m_10 = matrixVector[0];
	}
	else
	{
		tPoliticalTensionMatrix matrix;
		fpAsyncDatabase->vfunction31( &matrix );
		response.m_10 = matrix;
	}

	fn_00719050();
	m_1c->fn_00714020( request.vfunction8().Get(), request.vfunction9(), &response );
}
