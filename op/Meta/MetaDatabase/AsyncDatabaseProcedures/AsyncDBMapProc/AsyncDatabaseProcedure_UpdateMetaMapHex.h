#if !defined( AsyncDatabaseProcedure_UpdateMetaMapHex_h )
#define AsyncDatabaseProcedure_UpdateMetaMapHex_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the rebuilt functions use.
class tAsyncDatabaseProcedure_UpdateMetaMapHex : public tAsyncDatabaseProcedure
{
public:
	tAsyncDatabaseProcedure_UpdateMetaMapHex( tAsyncDatabase* pDB );
	virtual ~tAsyncDatabaseProcedure_UpdateMetaMapHex();

protected:
	void fn_00566d8a( const nDataStore::tBuffer* pBuffer );
};

}

#endif
