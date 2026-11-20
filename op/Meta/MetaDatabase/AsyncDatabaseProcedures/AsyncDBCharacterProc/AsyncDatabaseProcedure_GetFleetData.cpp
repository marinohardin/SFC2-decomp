#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_GetFleetData.h"
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

// The original's assertion records the validate on line 30.







// FUNCTION: STARFLEETOP 0x00562910
tGetFleetData::tGetFleetData( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
	validate( pDB );
	fn_00718f10( 0x1d, new tMemberCallback<tGetFleetData,const nDataStore::tBuffer*>( this, fn_005629ca ) );
	fn_00719050();
}
