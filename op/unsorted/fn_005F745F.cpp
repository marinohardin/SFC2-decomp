// Unit:      unsorted/fn_005F745F.cpp
// Functions: 0x005f745f to 0x005f7fd4, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
#include <string>
#include "Common/Thread/Optex.h"

// MATCH-ONLY: inline in the SDK, as in unsorted/tClientCharacter.cpp, which claims its
// out-of-line copy (0x00429a18); the header cannot define it while that unit does.
inline tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->Leave();
	}
}

// HYPOTHESIS: the object at +0x04 of fn_005f745f_object; 0x005f77c6 takes one argument whose
// type is not known.
// HYPOTHESIS: the queued entry: 0x18 bytes on the stack, its copy constructor is 0x005f7526
// and its destructor inlines to the string's _Tidy (0x005da20a); +0x10 and +0x14 are not known.
struct fn_005f74a6_entry
{
	fn_005f74a6_entry( const fn_005f74a6_entry& other );

	std::string field_00;
	int field_10;
	int field_14;
};

// HYPOTHESIS: a queue of entries: 0x005f77a7 reads the front, 0x005f7897 drops it.
class fn_005f77c6_object
{
public:
	void FUN_005f77c6( int value );
	fn_005f74a6_entry& FUN_005f77a7();
	void FUN_005f7897();
};

class fn_005f7961_object;

// HYPOTHESIS: the queued value: one string, copied by a copy constructor the original calls
// out of line (0x0055fe12); its destructor inlines to _Tidy (0x005da20a).
struct fn_0055fe12_value
{
	fn_0055fe12_value( const fn_0055fe12_value& other );

	std::string field_00;
	int field_10;
};

// HYPOTHESIS: a position in fn_005f7961_object: the node at +0x0c and the owner; 0x005f7db6
// reads the value it points at.
class fn_005f7db6_iterator
{
public:
	fn_005f7db6_iterator( void* node, fn_005f7961_object* owner )
		: fNode( node ), fOwner( owner )
	{
	}
	fn_0055fe12_value& FUN_005f7db6();

	void* fNode;
	fn_005f7961_object* fOwner;
};

// HYPOTHESIS: the object at +0x18 of fn_005f745f_object, a queue of strings; 0x005f7961 takes
// one argument whose type is not known, 0x005f7902 drops the front.
class fn_005f7961_object
{
public:
	void FUN_005f7961( int value );
	void FUN_005f7902();

	byte field_00[0x0c];
	void* field_0c;
};

// HYPOTHESIS: a container guarded by the lock at +0x40; only the members used here are placed.
class fn_005f745f_object
{
public:
	void FUN_005f745f( int value );
	fn_005f74a6_entry FUN_005f74a6();
	void FUN_005f755c( int value );
	fn_0055fe12_value FUN_005f75a3();

	byte field_00[0x04];
	fn_005f77c6_object field_04;
	byte field_05[0x18 - 0x05];
	fn_005f7961_object field_18;
	byte field_28[0x40 - 0x28];
	tOptex field_40;
};

// FUNCTION: STARFLEETOP 0x005f745f
void fn_005f745f_object::FUN_005f745f( int value )
{
	tOptex::tGuard guard( &field_40 );

	field_04.FUN_005f77c6( value );

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x005f74a6
fn_005f74a6_entry fn_005f745f_object::FUN_005f74a6()
{
	tOptex::tGuard guard( &field_40 );

	fn_005f74a6_entry value( field_04.FUN_005f77a7() );
	field_04.FUN_005f7897();

	return value;
}

// HYPOTHESIS: the member-wise copy the compiler would generate, written out to be claimed.
// FUNCTION: STARFLEETOP 0x005f7526
fn_005f74a6_entry::fn_005f74a6_entry( const fn_005f74a6_entry& other )
	: field_00( other.field_00 ), field_10( other.field_10 ), field_14( other.field_14 )
{
}

// FUNCTION: STARFLEETOP 0x005f755c
void fn_005f745f_object::FUN_005f755c( int value )
{
	tOptex::tGuard guard( &field_40 );

	field_18.FUN_005f7961( value );

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x005f75a3
fn_0055fe12_value fn_005f745f_object::FUN_005f75a3()
{
	tOptex::tGuard guard( &field_40 );

	fn_005f7db6_iterator front( field_18.field_0c, &field_18 );
	fn_0055fe12_value value( front.FUN_005f7db6() );
	field_18.FUN_005f7902();

	return value;
}
