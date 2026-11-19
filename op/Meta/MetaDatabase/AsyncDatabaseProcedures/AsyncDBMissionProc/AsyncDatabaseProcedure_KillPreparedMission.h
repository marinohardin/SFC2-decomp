#if !defined( AsyncDatabaseProcedure_KillPreparedMission_h )
#define AsyncDatabaseProcedure_KillPreparedMission_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tKillPreparedMission : public tAsyncDatabaseProcedure
{
public:
	tKillPreparedMission( tAsyncDatabase* pDB );
	virtual ~tKillPreparedMission();

protected:
	void fn_005679ae( const nDataStore::tBuffer* pBuffer );
};

}

#endif
