#if !defined( AsyncDatabaseProcedure_GetCharacterShipIDs_h )
#define AsyncDatabaseProcedure_GetCharacterShipIDs_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tGetCharacterShipIDs : public tAsyncDatabaseProcedure
{
public:
	tGetCharacterShipIDs( tAsyncDatabase* pDB );
	virtual ~tGetCharacterShipIDs();

protected:
	void fn_005626f1( const nDataStore::tBuffer* pBuffer );
	// HYPOTHESIS: the original allocates 0x50 bytes (0x00575000's new expression); what sits at
	// 0x48 is not known.
	int8u field_48[0x8];
};

}

#endif
