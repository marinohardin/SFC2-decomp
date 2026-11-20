#if !defined( AsyncDatabaseProcedure_TransactionUpdateStores_h )
#define AsyncDatabaseProcedure_TransactionUpdateStores_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"
#include "Common/DataStore/DataStoreBuffer.h"
// What 0x00569c83 uses; included here so that the unit's line records stay those of the
// original.
#include "Common/DataStore/DataStoreInput.h"
#include "Meta/MetaDatabase/StoredProcedures/ShipProcedures/TransactionUpdateStoresRequest.h"
#include "unsorted/c_004280f1.h"
#include "unsorted/c_00546027.h"
#include "unsorted/fn_005430ad_owner.h"
#include "unsorted/nAsyncDatabaseProcedure__tAsyncDatabase.h"
#include "unsorted/nSwitch__tAddress.h"
#include "Switch/Switch.h"
#include "unsorted/tNotifierAgent.h"
#include "unsorted/tServCharacter.h"
#include "unsorted/tShip.h"

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

#endif
