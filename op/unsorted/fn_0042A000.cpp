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

// HYPOTHESIS: a flag word at +0x98 tested by bit index; the class is not known yet.
class fn_0042a403_object
{
public:
	bool FUN_0042a403( int bit );
	// HYPOTHESIS: the adjacent byte getter at 0x0042a41a reads the same object.
	bool FUN_0042a41a();
	bool FUN_0042a421( int key0, int key1 );

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
