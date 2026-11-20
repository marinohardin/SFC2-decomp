#if !defined( AsyncDatabaseProcedure_LogOffCharacter_h )
#define AsyncDatabaseProcedure_LogOffCharacter_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tLogOffCharacter : public tAsyncDatabaseProcedure
{
public:
	tLogOffCharacter( tAsyncDatabase* pDB );
	virtual ~tLogOffCharacter();

protected:
	void fn_0056403d( const nDataStore::tBuffer* pBuffer );
};

}

#endif
