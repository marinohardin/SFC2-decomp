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

// The class is declared in a header, which the logon information procedure also includes.
#include "unsorted/fn_0042a480_object.h"

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

// Both calls go through this object to its int getter at 0x0042aa4b, so 0x0042a45f is a
// member of the same class.






// FUNCTION: STARFLEETOP 0x0042a45f
bool fn_0042a480_object::FUN_0042a45f()
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

// MATCH-ONLY: the class's implicit destructor (0x0042a863, unclaimed), written out for its deleting one.
// FUNCTION: STARFLEETOP 0x0042cb29
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

// Unclaimed: 0x0042adc1 is the library's vector<unsigned char>::push_back, as its callers 0x00538d00 and 0x00544db7 name it.
void fn_0042adc1_object::FUN_0042adc1( int value )
{
	FUN_0042b396( field_08, 1, value );
}

// HYPOTHESIS: argument types follow FUN_0042b396; the callee pops its arguments.
void __stdcall FUN_0042b50c( int target, int flag, int value );

// Unclaimed: 0x0042add2 is the library's vector<tID>::push_back, which develop's callers name under two classes.
void fn_0042adc1_object::FUN_0042add2( int value )
{
	FUN_0042b50c( field_08, 1, value );
}

// The class is shared with unsorted/tClientCharacter.cpp, which includes the same header.

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
// Unclaimed: 0x0042aff3 is the library's vector<tShipCache>::push_back, as 0x0042c863 names it.
void fn_0042adc1_object::FUN_0042aff3( int value )
{
	FUN_0042baff( field_08, 1, value );
}
