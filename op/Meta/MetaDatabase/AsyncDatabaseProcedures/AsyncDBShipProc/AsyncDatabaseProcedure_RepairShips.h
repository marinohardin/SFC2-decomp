#if !defined( AsyncDatabaseProcedure_RepairShips_h )
#define AsyncDatabaseProcedure_RepairShips_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"
// What 0x00568b2e uses; included here so that the unit's line records stay those of the
// original.
#include "Common/DataStore/DataStoreInput.h"
#include "Meta/MetaDatabase/StoredProcedures/ShipProcedures/RepairShipsRequest.h"
#include "unsorted/c_00546027.h"
#include "unsorted/fn_005430ad_owner.h"
#include "unsorted/nAsyncDatabaseProcedure__tAsyncDatabase.h"
#include "unsorted/tNotifierAgent.h"
#include "unsorted/tServCharacter.h"
#include "unsorted/tShip.h"

class tShip;

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tRepairShips : public tAsyncDatabaseProcedure
{
public:
	tRepairShips( tAsyncDatabase* pDB );
	virtual ~tRepairShips();

protected:
	void fn_00568b2e( const nDataStore::tBuffer* pBuffer );
	// HYPOTHESIS: applies a number of repair points to a ship.
	void fn_00568f29( tShip* ship, int points );
};

}

#endif
