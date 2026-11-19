#if !defined( AsyncDatabaseProcedure_AddPreparedMission_h )
#define AsyncDatabaseProcedure_AddPreparedMission_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tAddPreparedMission : public tAsyncDatabaseProcedure
{
public:
	tAddPreparedMission( tAsyncDatabase* pDB );
	virtual ~tAddPreparedMission();

protected:
	void fn_00567270( const nDataStore::tBuffer* pBuffer );
};

}

#endif
