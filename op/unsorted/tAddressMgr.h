//	tAddressMgr.h
//
//	The manager of the relay addresses the meta client sends to. The class and member names
//	come from the original's validate texts ("tAddressMgr::Instance()->GetNewsRelayAddress()").
//
#if !defined( tAddressMgr_h )
#define tAddressMgr_h

#include "Common/SmartPtr.h"
#include "Common/DataStore/Streamable.h"

// HYPOTHESIS: the reference-counted destination the switch sends to; its tRefCount sits at
// +0xc, after what is taken to be a tStreamable. Its class and name are not known.
// Meta/MetaDatabase/AsyncDatabaseProcedures/Single/AsyncDBSingle.cpp still holds a local
// copy of this declaration and calls 0x005d27f1 as fn_005d27f1.
class tObject_005d3679 : public nDataStore::tStreamable, public tRefCount
{
};

class tAddressMgr
{
public:
	static tAddressMgr* Instance();

	tSmartPtr<tObject_005d3679> GetNewsRelayAddress();
	tSmartPtr<tObject_005d3679> GetMapRelayAddress();
	tSmartPtr<tObject_005d3679> GetClockRelayAddress();
	tSmartPtr<tObject_005d3679> GetCharacterRelayAddress();
};

#endif
