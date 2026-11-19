#if !defined( AsyncDatabaseProcedure_GetCharacterRank_h )
#define AsyncDatabaseProcedure_GetCharacterRank_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tGetCharacterRank : public tAsyncDatabaseProcedure
{
public:
	tGetCharacterRank( tAsyncDatabase* pDB );
	virtual ~tGetCharacterRank();

protected:
	void fn_00561f77( const nDataStore::tBuffer* pBuffer );
};

}

#endif
