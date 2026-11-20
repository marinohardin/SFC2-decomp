//	AddPreparedMissionRequest.h
//
//	The request that adds a character's prepared missions, and its response. Only what the
//	rebuilt functions need is declared yet.
//
#if !defined( AddPreparedMissionRequest_h )
#define AddPreparedMissionRequest_h

#pragma once

#include "unsorted/nSwitch__tRequestWithResponse.h"
#include "unsorted/nSwitch__tResponse.h"
#include "unsorted/tPreparedMissions.h"

namespace nStoredProcedureArguments
{

	// RTTI names both classes. 0x00567270 holds the request in 0xb4 bytes with the missions at
	// +0x18 (named by its assertion text) and a flag at +0xb0, and the response in 0xa8 bytes
	// with missions at +0x10.
	class tAddPreparedMissionRequest : public nSwitch::tRequestWithResponse
	{
	public:
		class tAddPreparedMissionResponse : public nSwitch::tResponse
		{
		public:
			// HYPOTHESIS: the constructor (0x0054f7dd) takes one argument; its type is unknown.
			tAddPreparedMissionResponse( int32 param_1 );

			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

			// HYPOTHESIS: 0x00550bdb takes the missions that were stored.
			void fn_00550bdb( const tPreparedMissions& missions );

			tPreparedMissions m_10;
		};

		tAddPreparedMissionRequest();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );
		virtual void vfunction3( nDataStore::tInput& input );

		tPreparedMissions fPreparedMissions;
		// HYPOTHESIS: set when the missions may already exist and are updated instead of added.
		bool fFlag;
	};

};

#endif
