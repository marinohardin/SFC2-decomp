//	tServCharacter.h: tServCharacter, a character as the server's database holds it.

#if !defined( tServCharacter_h )
#define tServCharacter_h

#pragma once

#include "Common/Database/DatabaseObject.h"

// The base class is the exe's RTTI. Its own members are not known yet.
class tServCharacter : public nDatabase::tDatabaseObject
{
public:
	// HYPOTHESIS: a flag 0x0056c8d6 queries on; it tests the member at +0x14c.
	bool fn_00543445() const;
};

#endif
