#if !defined( AsyncDatabaseProcedure_GetCharacterOpenBidShipIDs_h )
#define AsyncDatabaseProcedure_GetCharacterOpenBidShipIDs_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tGetCharacterOpenBidShipIDs : public tAsyncDatabaseProcedure
{
public:
	tGetCharacterOpenBidShipIDs( tAsyncDatabase* pDB );
	virtual ~tGetCharacterOpenBidShipIDs();

protected:
	void fn_0056172a( const nDataStore::tBuffer* pBuffer );
};

}

#endif
