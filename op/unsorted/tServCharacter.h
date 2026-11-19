//	tServCharacter.h: tServCharacter, a character as the server's database holds it.

#if !defined( tServCharacter_h )
#define tServCharacter_h

#pragma once

#include "Common/Database/DatabaseObject.h"
#include "Common/BaseTypes.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Database/DatabaseID.h"

#include <string>

// HYPOTHESIS: the 0x434-byte element of the vector 0x0042a7bf copies ship caches from; the
// tShipCache constructor 0x0054185f takes it by reference. Its class is not known yet.
class fn_0054185f_ship
{
public:
	byte field_00[0x434];
};

// The base class is the exe's RTTI. Its own members are not known yet.
class tServCharacter : public nDatabase::tDatabaseObject
{
public:
	// HYPOTHESIS: the class whose RTTI is tServCharacter::tShipCache; only the constructor from
	// a ship (0x0054185f) and the destructor (0x0042a863) are known, and the size (0x4c) from
	// the stack frame of 0x0042a7bf. The destructor ends in ~tStreamable and destroys an ID at
	// +0x0c and strings at +0x24 and +0x34; the other fields are not known yet.
	class tShipCache : public nDataStore::tStreamable
	{
	public:
		tShipCache();
		tShipCache( const fn_0054185f_ship& ship );
		tShipCache( const tShipCache& other );
		// HYPOTHESIS: the original declared no destructor; its implicit one (no vtable store)
		// builds byte for byte, but the harness cannot claim a function with no source line, so
		// this declared one (which stores the vtable) stands in for it.
		virtual ~tShipCache();

		tShipCache& operator=( const tShipCache& other );

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		// HYPOTHESIS: a getter returning a tID by value; 0x0042ab25 collects it from each cache.
		db::tID FUN_005419ed() const;

		db::tID field_0c;
		int field_1c;
		int field_20;
		std::string field_24;
		std::string field_34;
		int field_44;
		int field_48;
	};

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
