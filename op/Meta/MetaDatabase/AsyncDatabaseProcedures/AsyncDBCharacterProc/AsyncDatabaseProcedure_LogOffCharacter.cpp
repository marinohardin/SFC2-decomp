#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_LogOffCharacter.h"
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

// The original's assertion records the validate on line 31.








// FUNCTION: STARFLEETOP 0x00563f83
tLogOffCharacter::tLogOffCharacter( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
	validate( pDB );
	fn_00718f10( 0x10, new tMemberCallback<tLogOffCharacter,const nDataStore::tBuffer*>( this, fn_0056403d ) );
	fn_00719050();
}

// FUNCTION: STARFLEETOP 0x00564016
// FUNCTION: STARFLEETOP 0x00564032
tLogOffCharacter::~tLogOffCharacter()
{
}
