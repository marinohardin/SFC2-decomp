#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMissionProc/AsyncDatabaseProcedure_AddPreparedMission.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"

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

// The original's assertion records the validate on line 27.




// FUNCTION: STARFLEETOP 0x005671b6
tAddPreparedMission::tAddPreparedMission( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
	validate( pDB );
	fn_00718f10( 0x33, new tMemberCallback<tAddPreparedMission,const nDataStore::tBuffer*>( this, fn_00567270 ) );
	fn_00719050();
}

tAddPreparedMission::~tAddPreparedMission()
{
}
