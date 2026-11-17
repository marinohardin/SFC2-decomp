//	tNewsStory.h: tNewsStory, a news story as the server's database holds it.

#if !defined( tNewsStory_h )
#define tNewsStory_h

#pragma once

#include "Common/Database/DatabaseObject.h"

// The class and its base are the exe's RTTI (tNewsStory, nDatabase::tQuery<tNewsStory>,
// nDatabase::tMemberEval<tNewsStory,nDatabase::tID const&,...>). Its own members are not known yet.
class tNewsStory : public nDatabase::tDatabaseObject
{
public:
	// HYPOTHESIS: a non-virtual accessor that returns a tID; 0x0056d874 queries on it.
	const nDatabase::tID& fn_00572b2b() const;
};

#endif
