#if !defined( AsyncDatabaseProcedure_GetFleetData_h )
#define AsyncDatabaseProcedure_GetFleetData_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tGetFleetData : public tAsyncDatabaseProcedure
{
public:
	tGetFleetData( tAsyncDatabase* pDB );
	virtual ~tGetFleetData();

protected:
	void fn_005629ca( const nDataStore::tBuffer* pBuffer );
};

}

#endif
