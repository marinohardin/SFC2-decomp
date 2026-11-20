#if !defined( AsyncDatabaseProcedure_ClearPreparedMissionsID_h )
#define AsyncDatabaseProcedure_ClearPreparedMissionsID_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses. The callback's
// address is taken from the function layout after the constructor, not from a verified relocation.
class tClearPreparedMissionsID : public tAsyncDatabaseProcedure
{
public:
	tClearPreparedMissionsID( tAsyncDatabase* pDB );
	virtual ~tClearPreparedMissionsID();

protected:
	void fn_00560779( const nDataStore::tBuffer* pBuffer );
};

}

#endif
