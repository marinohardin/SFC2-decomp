#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMissionProc/AsyncDatabaseProcedure_CheckExistencePreparedMission.h"
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
// FUNCTION: STARFLEETOP 0x00567465
tCheckExistencePreparedMission::tCheckExistencePreparedMission( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
#line 27
	validate( pDB );
	fn_00718f10( 0x32, new tMemberCallback<tCheckExistencePreparedMission,const nDataStore::tBuffer*>( this, fn_0056751f ) );
	fn_00719050();
}
