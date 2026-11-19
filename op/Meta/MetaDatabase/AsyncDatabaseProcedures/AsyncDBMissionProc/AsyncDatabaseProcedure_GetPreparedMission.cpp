#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMissionProc/AsyncDatabaseProcedure_GetPreparedMission.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/nAsyncDatabaseProcedure__tAsyncDatabase.h"

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

// The analysis names this address as the destructor; the body is the constructor.
// FUNCTION: STARFLEETOP 0x0056763a
tGetPreparedMission::tGetPreparedMission( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
#line 27
	validate( pDB );
	fn_00718f10( 0x35, new tMemberCallback<tGetPreparedMission,const nDataStore::tBuffer*>( this, fn_005676f4 ) );
	fn_00719050();
}

// FUNCTION: STARFLEETOP 0x005676cd
// FUNCTION: STARFLEETOP 0x005676e9
tGetPreparedMission::~tGetPreparedMission()
{
}

#include "Common/DataStore/DataStoreInput.h"
#include "Meta/MetaDatabase/StoredProcedures/MissionProcedures/GetPreparedMissionRequest.h"
#include "unsorted/nSwitch__tAddress.h"
#include "unsorted/nSwitch__tSwitch.h"

// The SDK's inline destructor of the reader, expanded into the function below as in the
// original; Close itself is called out of line (0x0056027f).
inline nDataStore::tInput::~tInput()
{
	Close( fCleanUpBuffer );
}

// HYPOTHESIS: the callback for the database's answer: reads the character's prepared missions
// and sends them back in the response, or a failure when there are none.
// FUNCTION: STARFLEETOP 0x005676f4
void tGetPreparedMission::fn_005676f4( const nDataStore::tBuffer* pBuffer )
{
	nDataStore::tInput input( pBuffer );
	nStoredProcedureArguments::tGetPreparedMissionRequest request;
	input >> request;

	fn_00719050();
	nStoredProcedureArguments::tGetPreparedMissionRequest::tGetPreparedMissionResponse response( 0 );
	std::vector<tPreparedMissions> missions;
	fpAsyncDatabase->vfunction24( request.fKey, true, &missions, !request.fFlag );
#line 53
	validate( missions.size() <= 1 );
	if( missions.size() > 0 )
	{
		response.fn_00716310( 0 );
		response.fn_00550bdb( missions[0] );
	}
	else
	{
		response.fn_00716310( 1 );
	}
	m_1c->fn_00714020( request.vfunction8().Get(), request.vfunction9(), &response );
}


