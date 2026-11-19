//	nSwitch__tResponse.h
//
//	The base of the switch's responses. Only what the rebuilt functions need is declared yet.
//
#if !defined( nSwitch__tResponse_h )
#define nSwitch__tResponse_h

#pragma once

#include "Common/DataStore/Streamable.h"

namespace nSwitch
{

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
