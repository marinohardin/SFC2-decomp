#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMissionProc/AsyncDatabaseProcedure_UpdatePreparedMission.h"
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

// The analysis names this address as the destructor; the body is the constructor.
// FUNCTION: STARFLEETOP 0x00567ad2
tUpdatePreparedMission::tUpdatePreparedMission( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
#line 27
	validate( pDB );
	fn_00718f10( 0x34, new tMemberCallback<tUpdatePreparedMission,const nDataStore::tBuffer*>( this, fn_00567b8c ) );
	fn_00719050();
}
