#if !defined( AsyncDatabaseProcedure_UpdatePreparedMission_h )
#define AsyncDatabaseProcedure_UpdatePreparedMission_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tUpdatePreparedMission : public tAsyncDatabaseProcedure
{
public:
	tUpdatePreparedMission( tAsyncDatabase* pDB );
	virtual ~tUpdatePreparedMission();

protected:
	void fn_00567b8c( const nDataStore::tBuffer* pBuffer );
};

}

#endif
