// Unit:      unsorted/fn_00687000.cpp
// Functions: 0x00687010 to 0x00687240, 9 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "unsorted/tNotifiee.h"
#include "Tiger/tstring.h"

// HYPOTHESIS: tActionUser (RTTI) derives from tNotifiee and holds a tString at +8; it has no
// shared header yet, so the declaration is local until one exists.
class tActionUser : public tNotifiee
{
public:
	tActionUser();
	virtual ~tActionUser();

	tString field_0x8;
};

// HYPOTHESIS: the symbol table names this tNotifiee::~tNotifiee, but the body is a
// constructor of tActionUser with the tNotifiee base constructor inlined.
// FUNCTION: STARFLEETOP 0x00687060
tActionUser::tActionUser() : field_0x8()
{
}
