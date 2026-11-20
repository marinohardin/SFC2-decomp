//	nStoredProcedureArguments__tGetCharactersRequest.h
//
//	The request that reads characters, and its response. Only what the rebuilt functions need
//	is declared yet.
//
#if !defined( nStoredProcedureArguments__tGetCharactersRequest_h )
#define nStoredProcedureArguments__tGetCharactersRequest_h

#pragma once

#include <vector>

#include "unsorted/nSwitch__tRequestWithResponse.h"
#include "unsorted/nSwitch__tResponse.h"
#include "unsorted/tServCharacter.h"

namespace nStoredProcedureArguments
{

	// HYPOTHESIS: RTTI names the response; 0x005682b2 holds it in 0x20 bytes with the
	// characters at +0x10 (named by that function's assertion text).
	class tGetCharactersRequest : public nSwitch::tRequestWithResponse
	{
	public:
		class tGetCharactersResponse : public nSwitch::tResponse
		{
		public:
			// HYPOTHESIS: the constructor (0x005536da) takes one argument; its type is unknown.
			tGetCharactersResponse( int32 param_1 );

			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

			// HYPOTHESIS: returns the characters; the exe shares the code with other getters.
			std::vector<tServCharacter>& fn_0058c2e1();

			std::vector<tServCharacter> m_10;
		};
	};

};

#endif
