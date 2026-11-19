//	tPreparedMissions.h: tPreparedMissions, a character's prepared missions as the server's database holds them.

#if !defined( tPreparedMissions_h )
#define tPreparedMissions_h

#pragma once

#include "Common/Database/DatabaseObject.h"

// The class and its base are the exe's RTTI (tPreparedMissions, nDatabase::tQuery<tPreparedMissions>,
// nDatabase::tMemberEval<tPreparedMissions,nDatabase::tID,...>). 0x0056d9d3 steps a vector of them
// by 0x98 bytes. Its own members are not known yet.
class tPreparedMissions : public nDatabase::tDatabaseObject
{
public:
	// HYPOTHESIS: its allocator news 0x98 bytes and constructs it out of line.
	tPreparedMissions();
	// 0x00567270 calls the destructor out of line (0x0053ebd1).
	virtual ~tPreparedMissions();
	// The name is 0x00567270's assertion text; 0x0056d9d3 queries on it.
	nDatabase::tID GetCharacterID() const;
	// HYPOTHESIS: a non-virtual accessor that returns a tID; 0x0056d9d3 queries on it. The exe
	// shares its 5-byte body with tNewsStory's accessor at the same address.
	const nDatabase::tID& fn_00572b2b() const;

	char fPad48[0x98 - 0x48];
};

#endif
