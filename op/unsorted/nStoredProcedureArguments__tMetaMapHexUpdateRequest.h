//	nStoredProcedureArguments__tMetaMapHexUpdateRequest.h
//
//	The request that stores an update of a meta map hex, and its response. Only what the
//	rebuilt functions need is declared yet.
//
#if !defined( nStoredProcedureArguments__tMetaMapHexUpdateRequest_h )
#define nStoredProcedureArguments__tMetaMapHexUpdateRequest_h

#pragma once

#include <vector>

#include "unsorted/tRequestWithResponse.h"
#include "unsorted/nSwitch__tResponse.h"
#include "unsorted/tMetaMapHex.h"
#include "Common/Database/DatabaseObjectUpdate.h"

namespace nStoredProcedureArguments
{

	// HYPOTHESIS: RTTI names both classes. 0x00566d8a holds the request in 0x28 bytes with a
	// vector of 0x38-byte updates at +0x18 (destroyed by 0x0054d244, read through GetHexUpdateVector),
	// and the response in 0xe0 bytes with a tMetaMapHex at +0x10.
	class tMetaMapHexUpdateRequest : public tRequestWithResponse
	{
	public:
		class tMetaMapHexUpdateResponse : public nSwitch::tResponse
		{
		public:
			tMetaMapHexUpdateResponse();

			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

			tMetaMapHex m_10;
		};

		tMetaMapHexUpdateRequest();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		// The name is the one 0x00566d8a's assertion text gives (0x0055e877).
		const std::vector<nDatabase::tDatabaseObjectUpdate>& GetHexUpdateVector() const;

		std::vector<nDatabase::tDatabaseObjectUpdate> m_18;
	};

};

#endif
