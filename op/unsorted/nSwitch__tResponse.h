//	nSwitch__tResponse.h
//
//	Name from the exe's RTTI (.?AVtResponse@nSwitch@@). Its destructor is 0x00716270; the map
//	initialize procedure's turn break response destroys it through that call.
//
#if !defined( nSwitch__tResponse_h )
#define nSwitch__tResponse_h

#include "Common/DataStore/Streamable.h"

namespace nSwitch
{

	// HYPOTHESIS: derived from tStreamable; the size 0x10 follows the member the turn break
	// response holds at +0x10.
	class tResponse : public nDataStore::tStreamable
	{
	public:
		virtual ~tResponse();

	protected:
		int8u field_0c[4];
	};

};

#endif
