//	nStoredProcedureArguments__tMetaMapHexRequest.h
//
//	The request for one meta map hex and its response. Only what the rebuilt functions need is
//	declared yet.
//
#if !defined( nStoredProcedureArguments__tMetaMapHexRequest_h )
#define nStoredProcedureArguments__tMetaMapHexRequest_h

#pragma once

#include "unsorted/nSwitch__tRequestWithResponse.h"
#include "unsorted/nSwitch__tResponse.h"
#include "unsorted/tHexPoint.h"
#include "unsorted/tMetaMapHex.h"

namespace nStoredProcedureArguments
{

	// HYPOTHESIS: RTTI names both classes. 0x00565464 holds the request in 0x2c bytes with a
	// tHexPoint at +0x18, and the response in 0xe0 bytes with a tMetaMapHex at +0x10.
	class tMetaMapHexRequest : public nSwitch::tRequestWithResponse
	{
	public:
		class tMetaMapHexResponse : public nSwitch::tResponse
		{
		public:
			tMetaMapHexResponse();

			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

			tMetaMapHex m_10;
		};

		tMetaMapHexRequest();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		tHexPoint m_18;
	};

};

#endif
