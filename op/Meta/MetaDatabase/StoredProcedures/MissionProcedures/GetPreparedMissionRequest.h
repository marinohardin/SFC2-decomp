//	GetPreparedMissionRequest.h
//
//	The request that reads a character's prepared missions, and its response. Only what the
//	rebuilt functions need is declared yet.
//
#if !defined( GetPreparedMissionRequest_h )
#define GetPreparedMissionRequest_h

#pragma once

#include "Common/Database/DatabaseID.h"
#include "unsorted/tRequestWithResponse.h"
#include "unsorted/nSwitch__tResponse.h"
#include "unsorted/tPreparedMissions.h"

namespace nStoredProcedureArguments
{

	// RTTI names both classes. 0x005676f4 holds the request in 0x2c bytes with an ID at +0x18
	// (copied and destroyed as a nDatabase::tID) and a flag at +0x28, and the response in 0xa8
	// bytes with missions at +0x10.
	class tGetPreparedMissionRequest : public tRequestWithResponse
	{
	public:
		class tGetPreparedMissionResponse : public nSwitch::tResponse
		{
		public:
			// HYPOTHESIS: the constructor (0x00550b4e) takes one argument; its type is unknown.
			tGetPreparedMissionResponse( int32 param_1 );

			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

			// HYPOTHESIS: 0x00550bdb takes the missions that were read.
			void fn_00550bdb( const tPreparedMissions& missions );

			tPreparedMissions m_10;
		};

		tGetPreparedMissionRequest();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );
		virtual void vfunction3( nDataStore::tInput& input );

		// HYPOTHESIS: the character whose missions are read.
		nDatabase::tID fKey;
		// HYPOTHESIS: when clear, the database reads the missions with a lock.
		bool fFlag;
	};

};

#endif
