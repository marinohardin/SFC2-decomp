#include <string>
#include "unsorted/tMedalsPanelCreator.h"

extern "C" int atexit( void ( *func )( void ) );

void fn_004bd431();

// HYPOTHESIS: arranges at exit the cleanup at 0x004bd431 (Ghidra labels it tApp3dBase::vfunction5, unconfirmed).
// FUNCTION: STARFLEETOP 0x00601141
void fn_00601141()
{
	atexit( fn_004bd431 );
}

// HYPOTHESIS: arranges at exit the same cleanup at 0x004bd431 as the function above.
// FUNCTION: STARFLEETOP 0x0060114d
void fn_0060114d()
{
	atexit( fn_004bd431 );
}

// HYPOTHESIS: arranges the same cleanup at 0x004bd431 as the two functions above.
// FUNCTION: STARFLEETOP 0x00601159
void fn_00601159()
{
	atexit( fn_004bd431 );
}

// HYPOTHESIS: the original passes a temporary std::string to the base constructor; blocked on tMetaClientCreator.h (HEADER REQUEST).
// FUNCTION: STARFLEETOP 0x00601165
tMedalsPanelCreator::tMedalsPanelCreator()
{
	// HYPOTHESIS: the original builds this temporary and hands it to the base constructor.
	std::string name( "" );
}
