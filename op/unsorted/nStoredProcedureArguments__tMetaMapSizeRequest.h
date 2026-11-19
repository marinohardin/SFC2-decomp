//	nStoredProcedureArguments__tMetaMapSizeRequest.h
//
//	Name from the exe's RTTI (.?AVtMetaMapSizeRequest@nStoredProcedureArguments@@). Size 0x18
//	from a local in the map initialize procedure.
//
#if !defined( nStoredProcedureArguments__tMetaMapSizeRequest_h )
#define nStoredProcedureArguments__tMetaMapSizeRequest_h

#include "unsorted/nSwitch__tRequestWithResponse.h"
#include "unsorted/tAddressMgr.h"

namespace nStoredProcedureArguments
{

	class tMetaMapSizeRequest : public nSwitch::tRequestWithResponse
	{
	public:
		// HYPOTHESIS: the reply address, as tGetNewsRequest takes it.
		tMetaMapSizeRequest( tSmartPtr<tObject_005d3679> replyAddress );

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );
	};

};

#endif
