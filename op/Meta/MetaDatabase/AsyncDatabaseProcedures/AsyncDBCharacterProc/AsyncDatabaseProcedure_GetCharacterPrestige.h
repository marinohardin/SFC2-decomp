#if !defined( AsyncDatabaseProcedure_GetCharacterPrestige_h )
#define AsyncDatabaseProcedure_GetCharacterPrestige_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses.
class tGetCharacterPrestige : public tAsyncDatabaseProcedure
{
public:
	tGetCharacterPrestige( tAsyncDatabase* pDB );
	virtual ~tGetCharacterPrestige();

protected:
	void fn_00561d18( const nDataStore::tBuffer* pBuffer );
	// HYPOTHESIS: the original allocates 0x50 bytes (0x00575000's new expression); what sits at
	// 0x48 is not known.
	int8u field_48[0x8];
};

}

#endif
