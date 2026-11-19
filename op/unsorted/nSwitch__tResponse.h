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

		int32	m_0c;
	};

};

#endif
