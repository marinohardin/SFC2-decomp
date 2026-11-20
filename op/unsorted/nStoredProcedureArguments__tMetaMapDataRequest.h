//	nStoredProcedureArguments__tMetaMapDataRequest.h
//
//	Name from the exe's RTTI. Constructor 0x0054c572, destructor 0x0054c5d2. Size 0x1c from a
//	local in the map initialize procedure.
//
#if !defined( nStoredProcedureArguments__tMetaMapDataRequest_h )
#define nStoredProcedureArguments__tMetaMapDataRequest_h

#include "unsorted/nSwitch__tRequestWithResponse.h"
#include "unsorted/tAddressMgr.h"

namespace nStoredProcedureArguments
{

	// HYPOTHESIS: a request with response like its neighbours; the map initialize procedure
	// builds it with an address, 2, -1 and -1, whose meanings are not recovered.
	class tMetaMapDataRequest : public nSwitch::tRequestWithResponse
	{
	public:
		tMetaMapDataRequest( tSmartPtr<tObject_005d3679> address, int32 param_2, int32 param_3, int32 param_4 );
		virtual ~tMetaMapDataRequest();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

	protected:
		int32 field_18;
	};

};

#endif
