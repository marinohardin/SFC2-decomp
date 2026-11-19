#if !defined( AsyncDatabaseProcedure_GetPreparedMission_h )
#define AsyncDatabaseProcedure_GetPreparedMission_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tGetPreparedMission : public tAsyncDatabaseProcedure
{
public:
	tGetPreparedMission( tAsyncDatabase* pDB );
	virtual ~tGetPreparedMission();

protected:
	void fn_005676f4( const nDataStore::tBuffer* pBuffer );
};

}

#endif
