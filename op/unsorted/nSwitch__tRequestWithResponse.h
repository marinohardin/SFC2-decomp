//	nSwitch__tRequestWithResponse.h
//
//	Name from the exe's RTTI (.?AVtRequestWithResponse@nSwitch@@). Its destructor is
//	0x00716400; the map initialize procedure destroys a tMetaMapSizeRequest local through it.
//
#if !defined( nSwitch__tRequestWithResponse_h )
#define nSwitch__tRequestWithResponse_h

#include "Common/DataStore/Streamable.h"

namespace nSwitch
{

	// HYPOTHESIS: derived from tStreamable (its requests are streamed by the switch); the
	// layout past the base is not recovered.
	class tRequestWithResponse : public nDataStore::tStreamable
	{
	public:
		virtual ~tRequestWithResponse();

	protected:
		int8u field_0c[0xc];
	};

};

#endif
