#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBTimeProc/AsyncDatabaseProcedure_GetCurrentTurn.h"
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

#line 24
// FUNCTION: STARFLEETOP 0x0056005c
tGetCurrentTurn::tGetCurrentTurn( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
	validate( pDB );
	fn_00718f10( 2, new tMemberCallback<tGetCurrentTurn,const nDataStore::tBuffer*>( this, fn_00560116 ) );
	fn_00719050();
}
