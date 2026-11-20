#if !defined( AsyncDatabaseProcedure_KillShips_h )
#define AsyncDatabaseProcedure_KillShips_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"
#include "unsorted/nStoredProcedureArguments__tKillShipsRequest.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: the object at +0x80 that 0x005682b2 hands its response to (0x00568538); its
// class is not known.
class tObject_00568538
{
public:
	void fn_00568538( const nStoredProcedureArguments::tKillShipsRequest::tKillShipsResponse& response );
};

// HYPOTHESIS: RTTI names the class; members are those 0x005682b2 uses.
class tKillShips : public tAsyncDatabaseProcedure
{
public:
	virtual ~tKillShips();

protected:
	void fn_005682b2( const nDataStore::tBuffer* pBuffer );

	nStoredProcedureArguments::tKillShipsRequest fRequest;
	tObject_00568538 m_80;
};

}

#endif
