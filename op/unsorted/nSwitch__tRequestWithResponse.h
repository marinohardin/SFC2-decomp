//	nSwitch__tRequestWithResponse.h
//
//	Name from the exe's RTTI (.?AVtRequestWithResponse@nSwitch@@). The base of the stored
//	procedure requests that expect a response. Its destructor is 0x00716400; the map
//	initialize procedure destroys a tMetaMapSizeRequest local through it.
//	HYPOTHESIS: the layout is reconstructed from the accesses of the derived requests' vfunction3
//	(0x0054fa6d, 0x0054ffe5); tAsyncDatabaseProcedure_GetMetaMapHexForUpdate (0x00565464)
//	streams a request in through the tStreamable input operator with no adjustment, so
//	tStreamable is the base at offset 0; the byte the derived vfunction3 read and reset at +8
//	is its fObjectVersion.
//
#if !defined( nSwitch__tRequestWithResponse_h )
#define nSwitch__tRequestWithResponse_h

#include "Common/BaseTypes.h"
#include "Common/DataStore/Streamable.h"
#include "Common/SmartPtr.h"

namespace nSwitch
{

	class tAddress;
	class tResponse;

	class tRequestWithResponse : public nDataStore::tStreamable
	{
	public:
		virtual ~tRequestWithResponse();
		virtual void vfunction3( nDataStore::tInput& input );
		// HYPOTHESIS: virtual slots 8 and 9, which 0x00565464 calls statically on a local
		// request. The first returns, held by a smart pointer, the address the response goes
		// to; the second returns the flags nSwitch::tSwitch::fn_00714020 sends the response
		// with.
		tSmartPtr<nSwitch::tAddress> vfunction8();
		int vfunction9();

	protected:
		int8u field_0c[0xc];
	};

};

#endif
