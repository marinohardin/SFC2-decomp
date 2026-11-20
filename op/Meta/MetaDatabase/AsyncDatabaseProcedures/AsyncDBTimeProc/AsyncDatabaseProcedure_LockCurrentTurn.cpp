#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBTimeProc/AsyncDatabaseProcedure_LockCurrentTurn.h"
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

// The original's assertion records the validate on line 35.












// FUNCTION: STARFLEETOP 0x005602ea
tLockCurrentTurn::tLockCurrentTurn( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
	validate( pDB );
	fn_00718f10( 3, new tMemberCallback<tLockCurrentTurn,const nDataStore::tBuffer*>( this, fn_005603a4 ) );
	fn_00719050();
}
