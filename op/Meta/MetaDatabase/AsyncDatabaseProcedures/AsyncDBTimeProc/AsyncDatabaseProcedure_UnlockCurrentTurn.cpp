#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBTimeProc/AsyncDatabaseProcedure_UnlockCurrentTurn.h"
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




// FUNCTION: STARFLEETOP 0x005604f2
tUnlockCurrentTurn::tUnlockCurrentTurn( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
	validate( pDB );
	fn_00718f10( 4, new tMemberCallback<tUnlockCurrentTurn,const nDataStore::tBuffer*>( this, fn_0056058a ) );
	fn_00719050();
}
