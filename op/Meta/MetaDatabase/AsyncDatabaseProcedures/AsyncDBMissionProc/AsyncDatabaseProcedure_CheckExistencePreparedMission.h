#if !defined( AsyncDatabaseProcedure_CheckExistencePreparedMission_h )
#define AsyncDatabaseProcedure_CheckExistencePreparedMission_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tCheckExistencePreparedMission : public tAsyncDatabaseProcedure
{
public:
	tCheckExistencePreparedMission( tAsyncDatabase* pDB );
	virtual ~tCheckExistencePreparedMission();

protected:
	void fn_0056751f( const nDataStore::tBuffer* pBuffer );
};

}

#endif
