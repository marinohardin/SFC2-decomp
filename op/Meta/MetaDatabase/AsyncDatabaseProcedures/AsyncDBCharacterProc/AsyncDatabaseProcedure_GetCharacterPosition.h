#if !defined( AsyncDatabaseProcedure_GetCharacterPosition_h )
#define AsyncDatabaseProcedure_GetCharacterPosition_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tGetCharacterPosition : public tAsyncDatabaseProcedure
{
public:
	tGetCharacterPosition( tAsyncDatabase* pDB );
	virtual ~tGetCharacterPosition();

protected:
	void fn_005619d8( const nDataStore::tBuffer* pBuffer );
};

}

#endif
