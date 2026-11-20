//	tAddressMgr.h
//
//	The manager of the relay addresses the meta client sends to. The class and member names
//	come from the original's validate texts ("tAddressMgr::Instance()->GetNewsRelayAddress()").
//
#if !defined( tAddressMgr_h )
#define tAddressMgr_h

#include "Common/SmartPtr.h"
#include "Common/DataStore/Streamable.h"
#include "unsorted/nSwitch__tAddress.h"

// HYPOTHESIS: the reference-counted destination the switch sends to is nSwitch::tAddress
// (unsorted/nSwitch__tAddress.h); tObject_005d3679 is the name earlier units gave it.
// Meta/MetaDatabase/AsyncDatabaseProcedures/Single/AsyncDBSingle.cpp includes this header
// and calls 0x005d27f1 as fn_005d27f1.
typedef nSwitch::tAddress tObject_005d3679;

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
