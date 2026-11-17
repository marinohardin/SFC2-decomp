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

// HYPOTHESIS: a container guarded by the lock at +0x40; only the members used here are placed.
class fn_005f745f_object
{
public:
	void FUN_005f745f( int value );
	fn_005f74a6_entry FUN_005f74a6();

	byte field_00[0x04];
	fn_005f77c6_object field_04;
	byte field_05[0x40 - 0x05];
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
