//	tMissionProfile.h
//
//	A mission profile and the mission entries it holds; the exe's RTTI names both classes.
//	HYPOTHESIS: both stream through the data store (their vfunction2 and vfunction3 sit where
//	tStreamable's StreamOut and StreamIn do); the members are not recovered yet.
//
#if !defined( tMissionProfile_h )
#define tMissionProfile_h

#pragma once

#include "Common/DataStore/Streamable.h"

class tMissionProfile : public nDataStore::tStreamable
{
public:
	class tMissionInfo : public nDataStore::tStreamable
	{
	public:
		virtual void StreamOut( nDataStore::tOutput& output ) const;
		virtual void StreamIn( nDataStore::tInput& input );
	};

	virtual void StreamOut( nDataStore::tOutput& output ) const;
	virtual void StreamIn( nDataStore::tInput& input );
};

#endif
