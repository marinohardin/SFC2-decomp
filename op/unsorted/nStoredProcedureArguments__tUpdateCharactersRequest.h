//	nStoredProcedureArguments__tUpdateCharactersRequest.h
//
//	The request that writes characters back. Only what the rebuilt functions need is declared
//	yet.
//
#if !defined( nStoredProcedureArguments__tUpdateCharactersRequest_h )
#define nStoredProcedureArguments__tUpdateCharactersRequest_h

#pragma once

#include <vector>

#include "Common/SmartPtr.h"
#include "Common/Database/DatabaseObjectUpdate.h"
#include "unsorted/tRequestWithResponse.h"
#include "unsorted/nSwitch__tAddress.h"
#include "unsorted/tServCharacter.h"

namespace nStoredProcedureArguments
{

	// HYPOTHESIS: RTTI names the class; 0x005682b2 holds it in 0x28 bytes with 0x38-byte
	// records (destroyed by 0x0054d244) at +0x18, taken to be updates.
	class tUpdateCharactersRequest : public tRequestWithResponse
	{
	public:
		// HYPOTHESIS: the constructor (0x00553aa5) takes the address the response goes to.
		tUpdateCharactersRequest( tSmartPtr<nSwitch::tAddress> address );

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		// HYPOTHESIS: records the characters to write back.
		void fn_00553ba5( const std::vector<tServCharacter>& characters );

		std::vector<nDatabase::tDatabaseObjectUpdate> m_18;
	};

};

#endif
