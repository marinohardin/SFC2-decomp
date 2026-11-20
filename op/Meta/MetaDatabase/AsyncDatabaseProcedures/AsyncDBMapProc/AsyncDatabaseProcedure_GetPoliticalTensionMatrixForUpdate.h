#if !defined( AsyncDatabaseProcedure_GetPoliticalTensionMatrixForUpdate_h )
#define AsyncDatabaseProcedure_GetPoliticalTensionMatrixForUpdate_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the rebuilt functions use.
class tGetPoliticalTensionMatrixForUpdate : public tAsyncDatabaseProcedure
{
public:
	tGetPoliticalTensionMatrixForUpdate( tAsyncDatabase* pDB );
	virtual ~tGetPoliticalTensionMatrixForUpdate();

protected:
	void fn_00565993( const nDataStore::tBuffer* pBuffer );
};

}

#endif
