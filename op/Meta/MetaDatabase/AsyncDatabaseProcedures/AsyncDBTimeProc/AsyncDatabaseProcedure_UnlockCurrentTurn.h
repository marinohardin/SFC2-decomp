#if !defined( AsyncDatabaseProcedure_UnlockCurrentTurn_h )
#define AsyncDatabaseProcedure_UnlockCurrentTurn_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tUnlockCurrentTurn : public tAsyncDatabaseProcedure
{
public:
	tUnlockCurrentTurn( tAsyncDatabase* pDB );
	virtual ~tUnlockCurrentTurn();

protected:
	void fn_0056058a( const nDataStore::tBuffer* pBuffer );
};

}

#endif
