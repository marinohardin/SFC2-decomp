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
#include <windows.h>
#include "Common/Thread/Optex.h"
#include "unsorted/tHexPoint.h"
#include "unsorted/tMetaMapHex.h"
#include "Common/Thread/SingleAccessWrapper.h"

// tServCharacter::tShipCache and the ship it is made from are declared in the shared header,
// which unsorted/fn_0042C000.cpp also includes.
#include "unsorted/tServCharacter.h"





































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
// 0x0042ac56 calls 0x0042abe4 on the object 0x0042a73d passes, so the flag and map members
// below belong to the same class.
class fn_0042a480_object
{
public:
	bool FUN_0042a403( int bit );
	// HYPOTHESIS: the adjacent byte getter at 0x0042a41a reads the same object.
	bool FUN_0042a41a();
	bool FUN_0042a421( int key0, int key1 );
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
	void FUN_0042a709( int key0, int key1 );
	void FUN_0042a73d();
	// HYPOTHESIS: sets the whole flag word that 0x0042a403 tests.
	void FUN_0042a7b2( unsigned long flags );
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
	int FUN_0042aacb();
	tSingleAccessWrapper< std::vector< tServCharacter::tShipCache > >::iPtr FUN_0042aaeb();
	tSingleAccessWrapper< fn_005395b2_object >::iPtr FUN_0042ab08();
	void FUN_0042ab25( std::vector< db::tID >& ids );
	tHexPoint FUN_0042abe4();
	const tHexPoint& FUN_0042ac36();
	// HYPOTHESIS: the hex points around the one 0x0042abe4 returns, keyed by the two numbers
	// 0x0055f269 and 0x005e8f21 read from each; the map's values are assigned by tHexPoint's
	// operator= (0x005b69b0).
	std::map< std::pair< int, int >, tHexPoint > FUN_0042ac56();
	int FUN_0042ada1();

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
	int field_54;
	int field_58;
	tHexPoint field_5c;
	// HYPOTHESIS: a second hex point, sized by the gap to +0x84; 0x0042ac36 returns it.
	tHexPoint field_70;
	tHexPoint field_84;
	// HYPOTHESIS: a flag word tested by bit index.
	unsigned long field_98;
	// HYPOTHESIS: the byte flag that 0x0042a41a reads at +0x9c.
	bool field_9c;
	// HYPOTHESIS: a map from an 8-byte key to a byte flag; the callee 0x0042b1d3 is its find, the
	// node's value sits at +0x14 and end() is the head pointer at +0xa4. The key type is a guess.
	std::map< std::pair< int, int >, bool > field_a0;
	// HYPOTHESIS: the original's maps are 0x14 bytes (0x0042ac56 copies _Multi at +0x0c), as in the
	// SDK's STL, whose tree also holds a _Nil pointer; this unit's 0x10-byte map leaves this gap.
	byte field_b0[0xb4 - 0xb0];
	// HYPOTHESIS: the wrapped ship cache vector; its Lock is 0x0042c709.
	tSingleAccessWrapper< std::vector< tServCharacter::tShipCache > > field_b4;
	// HYPOTHESIS: tClientCharacter's wrapper at +0xcc of tObject_005391b0, declared only inside
	// unsorted/tClientCharacter.cpp; 0x0042a8b7 locks it and calls 0x005395b2 on the object.
	tSingleAccessWrapper< fn_005395b2_object > field_cc;
	tOptex field_e4;
};

// FUNCTION: STARFLEETOP 0x0042a403
bool fn_0042a480_object::FUN_0042a403( int bit )
{
	return ( field_98 & ( 1 << bit ) ) ? true : false;
}

// FUNCTION: STARFLEETOP 0x0042a41a
bool fn_0042a480_object::FUN_0042a41a()
{
	return field_9c;
}

// FUNCTION: STARFLEETOP 0x0042a421
bool fn_0042a480_object::FUN_0042a421( int key0, int key1 )
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
void fn_0042a480_object::FUN_0042a709( int key0, int key1 )
{
	field_a0[std::pair< int, int >( key0, key1 )] = true;
}

// FUNCTION: STARFLEETOP 0x0042a73d
void fn_0042a480_object::FUN_0042a73d()
{
	// The original's frame is 4 bytes larger (an unused slot between the map and the iterator);
	// its cause is not recovered.
	std::map< std::pair< int, int >, tHexPoint > entries = FUN_0042ac56();

	for( std::map< std::pair< int, int >, tHexPoint >::iterator it = entries.begin(); it != entries.end(); ++it )
		FUN_0042a709( (*it).second.fn_0055f269(), (*it).second.fn_005e8f21() );
}

// FUNCTION: STARFLEETOP 0x0042a7b2
void fn_0042a480_object::FUN_0042a7b2( unsigned long flags )
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

// FUNCTION: STARFLEETOP 0x0042cb29
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

// FUNCTION: STARFLEETOP 0x0042aacb
int fn_0042a480_object::FUN_0042aacb()
{
	tOptex::tGuard guard( &field_e4 );

	return field_54;
}

// FUNCTION: STARFLEETOP 0x0042aaeb
tSingleAccessWrapper< std::vector< tServCharacter::tShipCache > >::iPtr fn_0042a480_object::FUN_0042aaeb()
{
	return field_b4.Lock();
}

// FUNCTION: STARFLEETOP 0x0042ab08
tSingleAccessWrapper< fn_005395b2_object >::iPtr fn_0042a480_object::FUN_0042ab08()
{
	return field_cc.Lock();
}

// FUNCTION: STARFLEETOP 0x0042ab25
void fn_0042a480_object::FUN_0042ab25( std::vector< db::tID >& ids )
{
	tOptex::tGuard guard( &field_e4 );

	ids.clear();

	tSingleAccessWrapper< std::vector< tServCharacter::tShipCache > >::iPtr caches = FUN_0042aaeb();
	for( std::vector< tServCharacter::tShipCache >::iterator it = (*caches).begin(); it != (*caches).end(); ++it )
		ids.push_back( (*it).FUN_005419ed() );

	// The original sets no unwind state after the access pointer's destructor (as in 0x0042a7bf);
	// its cause is not recovered.
	caches.Done();
}

// FUNCTION: STARFLEETOP 0x0042abe4
tHexPoint fn_0042a480_object::FUN_0042abe4()
{
	// As in 0x0042a92f, the original keeps no unwind state for the returned copy (its flag slot is
	// reused for the guard); this form tracks it, which is the remaining difference.
	tOptex::tGuard guard( &field_e4 );

	return field_5c;
}

// FUNCTION: STARFLEETOP 0x0042ac36
const tHexPoint& fn_0042a480_object::FUN_0042ac36()
{
	tOptex::tGuard guard( &field_e4 );

	return field_70;
}

// FUNCTION: STARFLEETOP 0x0042ac56
std::map< std::pair< int, int >, tHexPoint > fn_0042a480_object::FUN_0042ac56()
{
	tMetaMapHex hex( 0 );
	hex.fn_0053a49c( FUN_0042abe4() );

	std::vector< tHexPoint > points = hex.fn_00539ff1( -1, -1 );
	points.push_back( hex.fn_00539f9c() );

	std::map< std::pair< int, int >, tHexPoint > result;
	for( std::vector< tHexPoint >::iterator it = points.begin(); it != points.end(); ++it )
		result[std::pair< int, int >( (*it).fn_0055f269(), (*it).fn_005e8f21() )] = *it;

	return result;
}

// FUNCTION: STARFLEETOP 0x0042ada1
int fn_0042a480_object::FUN_0042ada1()
{
	tOptex::tGuard guard( &field_e4 );

	return field_58;
}

// HYPOTHESIS: an object holding a pointer at +0x08 that 0x0042b396 takes as its first
// argument; neither the class nor its members are known yet.
class fn_0042adc1_object
{
public:
	void FUN_0042adc1( int value );
	void FUN_0042add2( int value );
	void FUN_0042aff3( int value );

	byte field_00[0x08];
	int field_08;
};

// HYPOTHESIS: argument types are guesses from the one call below; the callee pops its
// arguments, so it is __stdcall.
void __stdcall FUN_0042b396( int target, int flag, int value );

// FUNCTION: STARFLEETOP 0x0042adc1
void fn_0042adc1_object::FUN_0042adc1( int value )
{
	FUN_0042b396( field_08, 1, value );
}

// HYPOTHESIS: argument types follow FUN_0042b396; the callee pops its arguments.
void __stdcall FUN_0042b50c( int target, int flag, int value );

// FUNCTION: STARFLEETOP 0x0042add2
void fn_0042adc1_object::FUN_0042add2( int value )
{
	FUN_0042b50c( field_08, 1, value );
}

// HYPOTHESIS: unsorted/tClientCharacter.cpp still holds its own copy of this class; it should
// include unsorted/tObject_0042ade3.h instead once that unit is open for edits.
#include "unsorted/tObject_0042ade3.h"

// FUNCTION: STARFLEETOP 0x0042ade3
tObject_0042ade3::~tObject_0042ade3()
{
	fn_0042b15f( iterator( *m_04 ), iterator( m_04 ) );
	operator delete( m_04 );
	m_04 = 0;
	m_10 = 0;
	operator delete( m_08 );
	m_08 = 0;
}

// HYPOTHESIS: argument types follow FUN_0042b396; the callee pops its arguments.
void __stdcall FUN_0042baff( int target, int flag, int value );

// HYPOTHESIS: same shape as FUN_0042adc1, so the same object type.
// FUNCTION: STARFLEETOP 0x0042aff3
void fn_0042adc1_object::FUN_0042aff3( int value )
{
	FUN_0042baff( field_08, 1, value );
}
