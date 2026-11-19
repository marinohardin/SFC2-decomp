#if !defined( AsyncDatabaseProcedure_GetCurrentTurn_h )
#define AsyncDatabaseProcedure_GetCurrentTurn_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class (vtable 0x007b1f34); members are those the constructor uses.
class tGetCurrentTurn : public tAsyncDatabaseProcedure
{
public:
	tGetCurrentTurn( tAsyncDatabase* pDB );
	virtual ~tGetCurrentTurn();

protected:
	void fn_00560116( const nDataStore::tBuffer* pBuffer );
};

}

#endif
