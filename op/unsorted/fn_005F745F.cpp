// Unit:      unsorted/fn_005F745F.cpp
// Functions: 0x005f745f to 0x005f7fd4, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
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
class fn_005f77c6_object
{
public:
	void FUN_005f77c6( int value );
};

// HYPOTHESIS: a container guarded by the lock at +0x40; only the members used here are placed.
class fn_005f745f_object
{
public:
	void FUN_005f745f( int value );

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
