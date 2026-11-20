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
		// HYPOTHESIS: the constructor (0x00716230) takes one argument it does not use; its
		// type is unknown.
		tResponse( int32 );
		virtual ~tResponse();

		// HYPOTHESIS: 0x00716310, called with 0 on a new response by 0x00565464; what it sets
		// is not known.
		void fn_00716310( int32 param_1 );

		int32	m_0c;
	};

};

#endif
