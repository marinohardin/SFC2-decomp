#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"
#include "Common/DataStore/DataStoreBuffer.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tTransactionUpdateStores : public tAsyncDatabaseProcedure
{
public:
	tTransactionUpdateStores( tAsyncDatabase* db );
	virtual ~tTransactionUpdateStores();

protected:
	void fn_00569c83( const nDataStore::tBuffer* pBuffer );
};

}

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
// FUNCTION: STARFLEETOP 0x00569bc9
tTransactionUpdateStores::tTransactionUpdateStores( tAsyncDatabase* db ) : tAsyncDatabaseProcedure( db )
{
#line 36
	validate( db );
	fn_00718f10( 10, new tMemberCallback<tTransactionUpdateStores,const nDataStore::tBuffer*>( this, fn_00569c83 ) );
	fn_00719050();
}

// FUNCTION: STARFLEETOP 0x00569c5c
// FUNCTION: STARFLEETOP 0x00569c78
#line 48
tTransactionUpdateStores::~tTransactionUpdateStores()
{
}
