//	tServCharacter.h: tServCharacter, a character as the server's database holds it.

#if !defined( tServCharacter_h )
#define tServCharacter_h

#pragma once

#include "Common/Database/DatabaseObject.h"

// The base class is the exe's RTTI. Its own members are not known yet.
class tServCharacter : public nDatabase::tDatabaseObject
{
public:
	// HYPOTHESIS: the exe holds the destructor out of line (0x00541d4f); 0x0056e006 calls it.
	virtual ~tServCharacter();

	// HYPOTHESIS: a flag 0x0056c8d6 queries on; it tests the member at +0x14c.
	bool fn_00543445() const;
	// HYPOTHESIS: two flags 0x0056c95c tests before it keeps a character.
	bool fn_005433dd() const;
	bool fn_0054340c() const;
	// HYPOTHESIS: three more flags 0x0056cb3b queries on.
	bool fn_005433e8() const;
	bool fn_005433f4() const;
	bool fn_00543400() const;

	// HYPOTHESIS: the record is 0x160 bytes (the stride 0x0056c95c walks a vector with).
	char fPad_48[0x160 - 0x48];
};

#endif
