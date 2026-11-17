// Unit:      unsorted/fn_0042A000.cpp
// Functions: 0x0042a309 to 0x0042aff3, 50 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
#define DATASTORE_OUT_OF_LINE_READ_STRING
#define DATASTORE_OUT_OF_LINE_GET
#define DATASTORE_OUT_OF_LINE_BUFFER_GET
#define DATASTORE_OUT_OF_LINE_READ
#include "Common/DataStore/DataStoreInput.h"

namespace nDataStore {
// MATCH-ONLY: Get is declared inline in Common/DataStore, so the compiler emits its out-of-line
// copy (0x0042a309) only for a use it cannot inline; this pointer is that use.
byte (tInput::*gEmitInputGet)() = &tInput::Get;

// FUNCTION: STARFLEETOP 0x0042a309
byte tInput::Get()
{
	return fBuffer->Get( fGetPos++ );
}

// FUNCTION: STARFLEETOP 0x0042a31c
byte tBuffer::Get( iPos getPos ) const
{
	if( getPos >= Size() )
	{
// MATCH-ONLY: the original compiled this inline body from DataStoreBuffer.cpp, which its header
// includes; the exe's exception carries that file and line. The path names the existing unit
// so the compiler's source listing can open it.
#line 117 "..\\Common\\DataStore\\DataStoreBuffer.cpp"
		throw except::tOutOfBounds( "tBuffer", 0, Size(), getPos, __FILE__, __LINE__ );
#line 31 "fn_0042A000.cpp"
	}

	return fBuffer[getPos];
}

// MATCH-ONLY: as for Get above, this pointer is the use that makes the compiler emit Read's
// out-of-line copy (0x0042a377).
void (tInput::*gEmitInputRead)( byte*, int32 ) = &tInput::Read;

// FUNCTION: STARFLEETOP 0x0042a377
void tInput::Read( byte* data, int32 size )
{
	fBuffer->Read( data, size, fGetPos );

	fGetPos += size;
}

// MATCH-ONLY: as for Get above, this pointer is the use that makes the compiler emit the string
// reader's out-of-line copy (0x0042a397).
tInput& (tInput::*gEmitInputReadString)( std::string& ) = &tInput::operator>>;

// FUNCTION: STARFLEETOP 0x0042a397
tInput& tInput::operator>>( std::string& string )
{
	string.erase();

	int32u size = 0;
	Read( (byte*)&size, sizeof( size ) );

	for( int32u i = 0; i < size; i++ )
	{
		string += Get();
	}

	return *this;
}
}

// HYPOTHESIS: the callee tests the fields at +0x0c and +0x10 against its two arguments and
// returns a bool, like the position classes near it; neither class's name is known yet.
class fn_005b6a5c_object
{
public:
	bool FUN_005b6a5c( int param_1, int param_2 );
};

class fn_0042a3f3_object
{
public:
	void FUN_0042a3f3();

	byte field_00[0x84];
	fn_005b6a5c_object field_84;
};

// FUNCTION: STARFLEETOP 0x0042a3f3
void fn_0042a3f3_object::FUN_0042a3f3()
{
	field_84.FUN_005b6a5c( 0, 0 );
}

#include <map>
#include "unsorted/tMissionCompleteResults.h"

// HYPOTHESIS: a flag word at +0x98 tested by bit index; the class is not known yet.
class fn_0042a403_object
{
public:
	bool FUN_0042a403( int bit );
	// HYPOTHESIS: the adjacent byte getter at 0x0042a41a reads the same object.
	bool FUN_0042a41a();
	bool FUN_0042a421( int key0, int key1 );
	void FUN_0042a709( int key0, int key1 );
	void FUN_0042a73d();
	// HYPOTHESIS: sets the whole flag word that 0x0042a403 tests.
	void FUN_0042a7b2( unsigned long flags );
	// HYPOTHESIS: returns a map by value whose node value sits at +0x0c with an 8-byte key; the
	// mapped type is guessed as the 0x14-byte value whose getters 0x0055f269 and 0x005e8f21 it calls.
	std::map< std::pair< int, int >, tObject_005b69d0 > FUN_0042ac56();

	byte field_00[0x98];
	unsigned long field_98;
	bool field_9c;
	// HYPOTHESIS: a map from an 8-byte key to a byte flag; the callee 0x0042b1d3 is its find, the
	// node's value sits at +0x14 and end() is the head pointer at +0xa4. The key type is a guess.
	std::map< std::pair< int, int >, bool > field_a0;
};

// FUNCTION: STARFLEETOP 0x0042a403
bool fn_0042a403_object::FUN_0042a403( int bit )
{
	return ( field_98 & ( 1 << bit ) ) ? true : false;
}

// FUNCTION: STARFLEETOP 0x0042a41a
bool fn_0042a403_object::FUN_0042a41a()
{
	return field_9c;
}

// FUNCTION: STARFLEETOP 0x0042a421
bool fn_0042a403_object::FUN_0042a421( int key0, int key1 )
{
	std::map< std::pair< int, int >, bool >::iterator it = field_a0.find( std::pair< int, int >( key0, key1 ) );
	return it != field_a0.end() ? (*it).second : false;
}

// HYPOTHESIS: both calls go through the same object to an int getter at 0x0042aa4b; the
// class is not known yet.
class fn_0042a45f_object
{
public:
	bool FUN_0042a45f();
	int FUN_0042aa4b();
};

// FUNCTION: STARFLEETOP 0x0042a45f
bool fn_0042a45f_object::FUN_0042a45f()
{
	if( FUN_0042aa4b() >= 8 && FUN_0042aa4b() <= 15 )
		return true;
	return false;
}

#include <windows.h>
#include "Common/Thread/Optex.h"
#include "unsorted/tHexPoint.h"
#include "Common/Thread/SingleAccessWrapper.h"

// HYPOTHESIS: the 0x434-byte element of the vector 0x0042a7bf copies ship caches from; the
// tShipCache constructor 0x0054185f takes it by reference. Its class is not known yet.
class fn_0054185f_ship
{
public:
	byte field_00[0x434];
};

#include "Common/Database/DatabaseID.h"

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
		tShipCache( const fn_0054185f_ship& ship );
		// HYPOTHESIS: the original declared no destructor; its implicit one (no vtable store)
		// builds byte for byte, but the harness cannot claim a function with no source line, so
		// this declared one (which stores the vtable) stands in for it.
		virtual ~tShipCache();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		db::tID field_0c;
		byte field_1c[0x24 - 0x1c];
		std::string field_24;
		std::string field_34;
		byte field_44[0x4c - 0x44];
	};
};

// MATCH-ONLY: inline in the SDK, as in unsorted/tClientCharacter.cpp, which claims its
// out-of-line copy (0x00429a18); the header cannot define it while that unit does.
inline tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->Leave();
	}
}

// HYPOTHESIS: a 0x14-byte value with no destructor, made by the static 0x006cf310 and turned into
// a string by 0x006cf6a0; neither the class nor its members are known yet.
class fn_006cf310_object
{
public:
	static fn_006cf310_object FUN_006cf310();
	std::string FUN_006cf6a0() const;

	byte field_00[0x14];
};

// HYPOTHESIS: the 0xd0-byte object tClientCharacter wraps at +0xcc (tObject_005391b0 in
// unsorted/tClientCharacter.cpp); only the member 0x0042a8b7 calls is placed, its argument and
// result types are guesses.
class fn_005395b2_object
{
public:
	void FUN_005395b2( int value );
};

// HYPOTHESIS: this is tClientCharacter (the string at +0x0c and the lock at +0xe4 match its
// layout), declared only inside unsorted/tClientCharacter.cpp; only the members used here are
// placed. 0x0042a535 and 0x0042a5ad assign strings at +0x1c and +0x30 under the same lock.
class fn_0042a480_object
{
public:
	void FUN_0042a480();
	void FUN_0042a4f7( std::string value );
	void FUN_0042a535( const std::string& value );
	bool FUN_0042a587( int value );
	void FUN_0042a5ad( const std::string& value );
	void FUN_0042a5ff( int value );
	void FUN_0042a623( int value );
	void FUN_0042a647( int value );
	void FUN_0042a66b( int value );
	void FUN_0042a68f( tHexPoint value );
	void FUN_0042a6ea( const tHexPoint& value );
	void FUN_0042a6fc( bool value );
	void FUN_0042a7bf( const std::vector< fn_0054185f_ship >& ships );
	void FUN_0042a8b7( int value );
	std::string FUN_0042a92f();
	const std::string& FUN_0042a99a();
	db::tID FUN_0042a9ba();
	const std::string& FUN_0042aa0b();
	int FUN_0042aa2b();
	int FUN_0042aa4b();
	int FUN_0042aa6b();
	int FUN_0042aa8b();
	int FUN_0042aaab();

	byte field_00[0x0c];
	std::string field_0c;
	std::string field_1c;
	int field_2c;
	std::string field_30;
	int field_40;
	int field_44;
	int field_48;
	int field_4c;
	int field_50;
	byte field_54[0x58 - 0x54];
	int field_58;
	tHexPoint field_5c;
	byte field_70[0x84 - 0x70];
	tHexPoint field_84;
	byte field_98[0x9c - 0x98];
	// HYPOTHESIS: the byte flag that 0x0042a41a reads at +0x9c.
	bool field_9c;
	byte field_9d[0xb4 - 0x9d];
	// HYPOTHESIS: the wrapped ship cache vector; its Lock is 0x0042c709.
	tSingleAccessWrapper< std::vector< tServCharacter::tShipCache > > field_b4;
	// HYPOTHESIS: tClientCharacter's wrapper at +0xcc of tObject_005391b0, declared only inside
	// unsorted/tClientCharacter.cpp; 0x0042a8b7 locks it and calls 0x005395b2 on the object.
	tSingleAccessWrapper< fn_005395b2_object > field_cc;
	tOptex field_e4;
};

// FUNCTION: STARFLEETOP 0x0042a480
void fn_0042a480_object::FUN_0042a480()
{
	tOptex::tGuard guard( &field_e4 );

	fn_006cf310_object value = fn_006cf310_object::FUN_006cf310();
	field_0c = value.FUN_006cf6a0();

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x0042a4f7
void fn_0042a480_object::FUN_0042a4f7( std::string value )
{
	field_0c = value;
}

// FUNCTION: STARFLEETOP 0x0042a535
void fn_0042a480_object::FUN_0042a535( const std::string& value )
{
	tOptex::tGuard guard( &field_e4 );

	field_1c = value;

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x0042a587
bool fn_0042a480_object::FUN_0042a587( int value )
{
	field_e4.Enter();
	field_2c = value;
	field_e4.Leave();
	return true;
}

// FUNCTION: STARFLEETOP 0x0042a5ad
void fn_0042a480_object::FUN_0042a5ad( const std::string& value )
{
	tOptex::tGuard guard( &field_e4 );

	field_30 = value;

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x0042a5ff
void fn_0042a480_object::FUN_0042a5ff( int value )
{
	field_e4.Enter();
	field_40 = value;
	field_e4.Leave();
}

// FUNCTION: STARFLEETOP 0x0042a623
void fn_0042a480_object::FUN_0042a623( int value )
{
	field_e4.Enter();
	field_48 = value;
	field_e4.Leave();
}

// FUNCTION: STARFLEETOP 0x0042a647
void fn_0042a480_object::FUN_0042a647( int value )
{
	field_e4.Enter();
	field_50 = value;
	field_e4.Leave();
}

// FUNCTION: STARFLEETOP 0x0042a66b
void fn_0042a480_object::FUN_0042a66b( int value )
{
	field_e4.Enter();
	field_58 = value;
	field_e4.Leave();
}

// FUNCTION: STARFLEETOP 0x0042a68f
void fn_0042a480_object::FUN_0042a68f( tHexPoint value )
{
	tOptex::tGuard guard( &field_e4 );

	field_5c = value;

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x0042a6ea
void fn_0042a480_object::FUN_0042a6ea( const tHexPoint& value )
{
	field_84 = value;
}

// FUNCTION: STARFLEETOP 0x0042a6fc
void fn_0042a480_object::FUN_0042a6fc( bool value )
{
	field_9c = value;
}

// FUNCTION: STARFLEETOP 0x0042a709
void fn_0042a403_object::FUN_0042a709( int key0, int key1 )
{
	field_a0[std::pair< int, int >( key0, key1 )] = true;
}

// FUNCTION: STARFLEETOP 0x0042a73d
void fn_0042a403_object::FUN_0042a73d()
{
	// The original's frame is 4 bytes larger (an unused slot between the map and the iterator);
	// its cause is not recovered.
	std::map< std::pair< int, int >, tObject_005b69d0 > entries = FUN_0042ac56();

	for( std::map< std::pair< int, int >, tObject_005b69d0 >::iterator it = entries.begin(); it != entries.end(); ++it )
		FUN_0042a709( (*it).second.fn_0055f269(), (*it).second.fn_005e8f21() );
}

// FUNCTION: STARFLEETOP 0x0042a7b2
void fn_0042a403_object::FUN_0042a7b2( unsigned long flags )
{
	field_98 = flags;
}

// FUNCTION: STARFLEETOP 0x0042a7bf
void fn_0042a480_object::FUN_0042a7bf( const std::vector< fn_0054185f_ship >& ships )
{
	tSingleAccessWrapper< std::vector< tServCharacter::tShipCache > >::iPtr caches = field_b4.Lock();

	(*caches).clear();
	for( std::vector< fn_0054185f_ship >::const_iterator it = ships.begin(); it != ships.end(); ++it )
		(*caches).push_back( tServCharacter::tShipCache( *it ) );

	caches.Done();
}

// FUNCTION: STARFLEETOP 0x0042a863
tServCharacter::tShipCache::~tShipCache()
{
}

// FUNCTION: STARFLEETOP 0x0042a8b7
void fn_0042a480_object::FUN_0042a8b7( int value )
{
	tOptex::tGuard guard( &field_e4 );

	(*field_cc.Lock()).FUN_005395b2( value );

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x0042a92f
std::string fn_0042a480_object::FUN_0042a92f()
{
	// The original keeps no unwind state for the returned copy (its flag slot is reused for the
	// guard); this form tracks it, which is the remaining difference.
	tOptex::tGuard guard( &field_e4 );

	return field_0c;
}

// FUNCTION: STARFLEETOP 0x0042a99a
const std::string& fn_0042a480_object::FUN_0042a99a()
{
	tOptex::tGuard guard( &field_e4 );

	return field_1c;
}

// FUNCTION: STARFLEETOP 0x0042a9ba
db::tID fn_0042a480_object::FUN_0042a9ba()
{
	// As in 0x0042a92f, the original keeps no unwind state for the returned tID and its frame is
	// 8 bytes smaller; this form tracks it, which is the remaining difference.
	tOptex::tGuard guard( &field_e4 );

	return field_2c;
}

// FUNCTION: STARFLEETOP 0x0042aa0b
const std::string& fn_0042a480_object::FUN_0042aa0b()
{
	tOptex::tGuard guard( &field_e4 );

	return field_30;
}

// FUNCTION: STARFLEETOP 0x0042aa2b
int fn_0042a480_object::FUN_0042aa2b()
{
	tOptex::tGuard guard( &field_e4 );

	return field_40;
}

// FUNCTION: STARFLEETOP 0x0042aa4b
int fn_0042a480_object::FUN_0042aa4b()
{
	tOptex::tGuard guard( &field_e4 );

	return field_44;
}

// FUNCTION: STARFLEETOP 0x0042aa6b
int fn_0042a480_object::FUN_0042aa6b()
{
	tOptex::tGuard guard( &field_e4 );

	return field_48;
}

// FUNCTION: STARFLEETOP 0x0042aa8b
int fn_0042a480_object::FUN_0042aa8b()
{
	tOptex::tGuard guard( &field_e4 );

	return field_4c;
}

// FUNCTION: STARFLEETOP 0x0042aaab
int fn_0042a480_object::FUN_0042aaab()
{
	tOptex::tGuard guard( &field_e4 );

	return field_50;
}
