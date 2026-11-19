#if !defined( AsyncDatabaseProcedure_LockCurrentTurn_h )
#define AsyncDatabaseProcedure_LockCurrentTurn_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tLockCurrentTurn : public tAsyncDatabaseProcedure
{
public:
	tLockCurrentTurn( tAsyncDatabase* pDB );
	virtual ~tLockCurrentTurn();

protected:
	void fn_005603a4( const nDataStore::tBuffer* pBuffer );
};

}

#endif
