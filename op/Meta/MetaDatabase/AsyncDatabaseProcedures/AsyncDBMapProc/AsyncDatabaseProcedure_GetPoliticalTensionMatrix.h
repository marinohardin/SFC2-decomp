#if !defined( AsyncDatabaseProcedure_GetPoliticalTensionMatrix_h )
#define AsyncDatabaseProcedure_GetPoliticalTensionMatrix_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the rebuilt functions use.
class tAsyncDatabaseProcedure_GetPoliticalTensionMatrix : public tAsyncDatabaseProcedure
{
public:
	tAsyncDatabaseProcedure_GetPoliticalTensionMatrix( tAsyncDatabase* pDB );
	virtual ~tAsyncDatabaseProcedure_GetPoliticalTensionMatrix();

protected:
	void fn_0056571c( const nDataStore::tBuffer* pBuffer );
};

}

#endif
