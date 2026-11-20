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
	// HYPOTHESIS: the original allocates 0x50 bytes (0x00575000's new expression); what sits at
	// 0x48 is not known.
	int8u field_48[0x8];
};

}

#endif
