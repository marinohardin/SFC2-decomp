//	tServCharacter.h: tServCharacter, a character as the server's database holds it.

#if !defined( tServCharacter_h )
#define tServCharacter_h

#pragma once

#include "Common/Database/DatabaseObject.h"
#include "Common/BaseTypes.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Database/DatabaseID.h"

#include "unsorted/tHexPoint.h"

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

	// The overrides in the vtable at 0x007b0b54 (0x00543c70, 0x00544150, 0x00544719,
	// 0x0054471f) and in tStreamable's at 0x007b0b44 (0x0054258b is StreamIn, 0x0054231a
	// StreamOut; 0x00544b2a is the adjusting stub for the destructor).
	virtual void SQLFillInsertStream( std::stringstream* SQL, bool lastOne = false );
	virtual void SQLFillUpdateStream( std::stringstream* SQL, bool lastOne = false );
	virtual const char* SQLTableName() const;
	virtual const char* SQLFieldNames() const;
	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	// HYPOTHESIS: a flag 0x0056c8d6 queries on; it tests the member at +0x14c.
	bool fn_00543445() const;
	// HYPOTHESIS: two flags 0x0056c95c tests before it keeps a character.
	bool fn_005433dd() const;
	bool fn_0054340c() const;
	// HYPOTHESIS: three more flags 0x0056cb3b queries on.
	bool fn_005433e8() const;
	bool fn_005433f4() const;
	bool fn_00543400() const;

	// The record is 0x160 bytes (the stride 0x0056c95c walks a vector with, and the last member
	// the constructor 0x00541e73 builds ends there). That constructor is the copy constructor: it
	// builds a default object and calls 0x00541f81, the assignment operator, which copies the
	// other record's members (neither is declared here: declaring a constructor would drop the
	// implicit default one the units that vector these records rely on). Members are placed by
	// the constructor and the destructor (0x00541d4f). The vtables are 0x007b0b54 (+0) and
	// 0x007b0b44 (+8, tStreamable's); slots 5 to 8 (0x00543c70, 0x00544150, 0x00544719,
	// 0x0054471f) are the SQL stream and name functions tDatabaseObject declares, slots 1 and 2
	// of the second table (0x0054231a, 0x0054258b) its StreamOut and StreamIn.
	std::string field_48;
	long field_58;
	// HYPOTHESIS: a tSingleAccessWrapper of a vector (constructed by 0x005447ff, with the lock
	// at +4 destroyed by 0x004296c8); the member at +0xf0 is the same kind over the ship caches.
	char field_5c[0x18];
	std::string field_74;
	std::string field_84;
	int field_94;
	tHexPoint field_98;
	tHexPoint field_ac;
	db::tID field_c0;
	// HYPOTHESIS: a vector of tIDs (the destructor 0x0053d691): its begin and end are at +0xd4
	// and +0xd8.
	char field_d0[0x10];
	std::string field_e0;
	// A tSingleAccessWrapper of the ship caches vector (destroyed by 0x0042cc05, and 0x0042aaeb
	// locks it).
	char field_f0[0x18];
	unsigned int field_108;
	unsigned int field_10c;
	unsigned int field_110;
	// Bit flags; the one-bit tests are 0x005433dd, 0x005433e8, 0x005433f4 and 0x00543400.
	unsigned int field_114;
	float field_118;
	tHexPoint field_11c;
	int field_130;
	long field_134;
	int field_138;
	int field_13c;
	int field_140;
	long field_144;
	long field_148;
	tHexPoint field_14c;
};

typedef char tServCharacter_size_check[ sizeof( tServCharacter ) == 0x160 ? 1 : -1 ];

#endif
