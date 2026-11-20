#if !defined( AsyncDatabaseProcedure_GetMetaMapHexForUpdate_h )
#define AsyncDatabaseProcedure_GetMetaMapHexForUpdate_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the rebuilt functions use.
class tAsyncDatabaseProcedure_GetMetaMapHexForUpdate : public tAsyncDatabaseProcedure
{
public:
	tAsyncDatabaseProcedure_GetMetaMapHexForUpdate( tAsyncDatabase* pDB );
	virtual ~tAsyncDatabaseProcedure_GetMetaMapHexForUpdate();

protected:
	void fn_00565464( const nDataStore::tBuffer* pBuffer );
};

}

#endif
