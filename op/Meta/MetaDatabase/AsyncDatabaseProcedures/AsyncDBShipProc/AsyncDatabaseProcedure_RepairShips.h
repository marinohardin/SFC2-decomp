#if !defined( AsyncDatabaseProcedure_RepairShips_h )
#define AsyncDatabaseProcedure_RepairShips_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

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
};

}

#endif
