#if !defined( AsyncDatabaseProcedure_GetLoggedOnCharacterIDs_h )
#define AsyncDatabaseProcedure_GetLoggedOnCharacterIDs_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tGetLoggedOnCharacterIDs : public tAsyncDatabaseProcedure
{
public:
	tGetLoggedOnCharacterIDs( tAsyncDatabase* pDB );
	virtual ~tGetLoggedOnCharacterIDs();

protected:
	void fn_00562fc3( const nDataStore::tBuffer* pBuffer );
};

}

#endif
