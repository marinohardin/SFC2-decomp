#if !defined( AsyncDatabaseProcedure_SetPreparedMissionsID_h )
#define AsyncDatabaseProcedure_SetPreparedMissionsID_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those the constructor uses. The callback's
// address is taken from the function layout after the constructor, not from a verified relocation.
class tSetPreparedMissionsID : public tAsyncDatabaseProcedure
{
public:
	tSetPreparedMissionsID( tAsyncDatabase* pDB );
	virtual ~tSetPreparedMissionsID();

protected:
	void fn_00564a00( const nDataStore::tBuffer* pBuffer );
	// HYPOTHESIS: the original allocates 0x50 bytes (0x00575000's new expression); what sits at
	// 0x48 is not known.
	int8u field_48[0x8];
};

}

#endif
