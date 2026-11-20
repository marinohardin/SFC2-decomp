//	nStoredProcedureArguments__tKillShipsRequest.h
//
//	The request that kills ships, and its response. Only what the rebuilt functions need is
//	declared yet.
//
#if !defined( nStoredProcedureArguments__tKillShipsRequest_h )
#define nStoredProcedureArguments__tKillShipsRequest_h

#pragma once

#include <vector>

#include "Common/Database/DatabaseID.h"
#include "unsorted/tRequestWithResponse.h"
#include "unsorted/nSwitch__tResponse.h"

namespace nStoredProcedureArguments
{

	// RTTI names both classes. 0x005682b2 holds the response in 0x20 bytes with ship IDs at
	// +0x10; the request sits in nAsyncDatabaseProcedure::tKillShips at +0x48.
	class tKillShipsRequest : public tRequestWithResponse
	{
	public:
		class tKillShipsResponse : public nSwitch::tResponse
		{
		public:
			// HYPOTHESIS: the constructor (0x0055919d) is given the request it answers.
			tKillShipsResponse( const tKillShipsRequest* pRequest );

			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

			// HYPOTHESIS: returns the IDs of the ships killed; the exe shares the code with other
			// getters.
			std::vector<nDatabase::tID>& fn_0058c2e1();

			std::vector<nDatabase::tID> m_10;
		};

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		// HYPOTHESIS: returns the IDs of the ships to kill.
		const std::vector<nDatabase::tID>& fn_005596e9() const;

		std::vector<nDatabase::tID> m_18;
		// HYPOTHESIS: the rest up to the next member of tKillShips (+0x80) is not known.
		char m_28[0x38 - 0x28];
	};

};

#endif
