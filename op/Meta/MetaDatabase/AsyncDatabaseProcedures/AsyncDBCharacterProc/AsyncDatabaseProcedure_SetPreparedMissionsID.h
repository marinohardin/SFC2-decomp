#if !defined( AsyncDatabaseProcedure_SetPreparedMissionsID_h )
#define AsyncDatabaseProcedure_SetPreparedMissionsID_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses. The callback's
// address is taken from the function layout after the constructor, not from a verified relocation.
class tSetPreparedMissionsID : public tAsyncDatabaseProcedure
{
public:
	tSetPreparedMissionsID( tAsyncDatabase* pDB );
	virtual ~tSetPreparedMissionsID();

protected:
	void fn_00564a00( const nDataStore::tBuffer* pBuffer );
};

}

#endif
