//	tServCharacter.h
//	The parts of tServCharacter that matched code uses, shared by unsorted/fn_0042A000.cpp and
//	unsorted/fn_0042C000.cpp.
#if !defined( tServCharacter_h )
#define tServCharacter_h

#pragma once

#include "Common/BaseTypes.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Database/DatabaseID.h"

// HYPOTHESIS: the 0x434-byte element of the vector 0x0042a7bf copies ship caches from; the
// tShipCache constructor 0x0054185f takes it by reference. Its class is not known yet.
class fn_0054185f_ship
{
public:
	byte field_00[0x434];
};

// HYPOTHESIS: the class whose RTTI is tServCharacter::tShipCache; only the constructor from a
// ship (0x0054185f) and the destructor (0x0042a863) are known, and the size (0x4c) from the
// stack frame of 0x0042a7bf. The destructor ends in ~tStreamable and destroys an ID at +0x0c
// and strings at +0x24 and +0x34; the other fields are not known yet.
class tServCharacter
{
public:
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
};

#endif
