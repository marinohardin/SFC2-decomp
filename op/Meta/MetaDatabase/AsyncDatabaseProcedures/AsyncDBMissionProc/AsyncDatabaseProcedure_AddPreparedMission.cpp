#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMissionProc/AsyncDatabaseProcedure_AddPreparedMission.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"
#include "Common/DataStore/DataStoreInput.h"
#include "Meta/MetaDatabase/StoredProcedures/MissionProcedures/AddPreparedMissionRequest.h"
#include "unsorted/nAsyncDatabaseProcedure__tAsyncDatabase.h"
#include "unsorted/nSwitch__tAddress.h"
#include "Switch/Switch.h"

// The SDK's template bodies, which Common/Callback.h and Common/MemberCallback.h leave to the
// units that instantiate them.
template< typename tnResult >
tCallback<tnResult>::~tCallback()
{}

template< typename tnMemberClass, typename tnResult >
tMemberCallback<tnMemberClass,tnResult>::tMemberCallback<tnMemberClass,tnResult>( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction ) :
	fpMemberClass( pMemberClass ),
	fpMemberFunction( pMemberFunction )
{}

using namespace nAsyncDatabaseProcedure;

// FUNCTION: STARFLEETOP 0x005671b6
tAddPreparedMission::tAddPreparedMission( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
#line 27
	validate( pDB );
	fn_00718f10( 0x33, new tMemberCallback<tAddPreparedMission,const nDataStore::tBuffer*>( this, fn_00567270 ) );
	fn_00719050();
}

// FUNCTION: STARFLEETOP 0x00567249
// FUNCTION: STARFLEETOP 0x00567265
tAddPreparedMission::~tAddPreparedMission()
{
}

// The SDK's inline destructor of the reader, expanded into the function below as in the
// original; Close itself is called out of line (0x0056027f).
inline nDataStore::tInput::~tInput()
{
	Close( fCleanUpBuffer );
}

// HYPOTHESIS: the callback for the database's answer: reads the missions from the request,
// updates them when they exist and the request allows it, otherwise replaces them, and sends
// the response.
// FUNCTION: STARFLEETOP 0x00567270
void tAddPreparedMission::fn_00567270( const nDataStore::tBuffer* pBuffer )
{
	nDataStore::tInput input( pBuffer );
	nStoredProcedureArguments::tAddPreparedMissionRequest request;
	input >> request;

	fn_00719050();
	nStoredProcedureArguments::tAddPreparedMissionRequest::tAddPreparedMissionResponse response( 0 );
	// The assertion text gives the accessor's name.
#line 49
	validate( request.fPreparedMissions.GetCharacterID().IsValid() );
	if( request.fFlag && fpAsyncDatabase->vfunction64( request.fPreparedMissions.GetCharacterID(), true ) )
	{
		response.fn_00716310( 1 );
	}
	else
	{
		fpAsyncDatabase->vfunction56( request.fPreparedMissions.GetCharacterID(), true );
		fpAsyncDatabase->vfunction38( &request.fPreparedMissions );
		response.fn_00550bdb( request.fPreparedMissions );
		response.fn_00716310( 0 );
	}
	m_1c->fn_00714020( request.vfunction8().Get(), request.vfunction9(), &response );
}
