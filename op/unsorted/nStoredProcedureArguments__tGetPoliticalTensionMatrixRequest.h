//	nStoredProcedureArguments__tGetPoliticalTensionMatrixRequest.h
//
//	The request for the political tension matrix and its response. Only what the rebuilt
//	functions need is declared yet.
//
#if !defined( nStoredProcedureArguments__tGetPoliticalTensionMatrixRequest_h )
#define nStoredProcedureArguments__tGetPoliticalTensionMatrixRequest_h

#pragma once

#include "unsorted/tRequestWithResponse.h"
#include "unsorted/nSwitch__tResponse.h"
#include "unsorted/tPoliticalTensionMatrix.h"

namespace nStoredProcedureArguments
{

	// HYPOTHESIS: RTTI names both classes. 0x0056571c holds the request in 0x18 bytes and
	// destroys it as a tRequestWithResponse, and the response in 0x70 bytes with a
	// tPoliticalTensionMatrix at +0x10.
	class tGetPoliticalTensionMatrixRequest : public tRequestWithResponse
	{
	public:
		class tGetPoliticalTensionMatrixResponse : public nSwitch::tResponse
		{
		public:
			tGetPoliticalTensionMatrixResponse();
			virtual ~tGetPoliticalTensionMatrixResponse();

			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

			tPoliticalTensionMatrix m_10;
		};

		tGetPoliticalTensionMatrixRequest();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );
	};

};

#endif
