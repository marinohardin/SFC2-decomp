#if !defined( AsyncDBProcedure_h )
#define AsyncDBProcedure_h

#include "Switch/AsyncProcedure.h"

namespace nAsyncDatabaseProcedure
{

class tAsyncDatabase;

// HYPOTHESIS: RTTI names the class; its constructor at 0x00574499 asserts fpAsyncDatabase.
class tAsyncDatabaseProcedure : public nSwitch::tAsyncProcedure
{
public:
	tAsyncDatabaseProcedure( tAsyncDatabase* pAsyncDatabase );
	virtual ~tAsyncDatabaseProcedure();

protected:
	tAsyncDatabase* fpAsyncDatabase;
};

}

#endif
