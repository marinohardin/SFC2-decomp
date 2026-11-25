// Unit:      unsorted/tMessageDialog.cpp
// Functions: 0x006005aa to 0x00600a4c, 11 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include <string>

#include "unsorted/fn_004D3143.h"

// HYPOTHESIS: the "General_Error" string defined in unsorted/fn_00600208.cpp.
extern std::string g_00c39ea0;

// FUNCTION: STARFLEETOP 0x006006b3
void fn_006006b3()
{
	fn_004d31ed( g_00c39ea0.c_str(), 1, 0, 0x280, 0x1e0 );
	fn_004d3340( g_00c39ea0.c_str() );
}
