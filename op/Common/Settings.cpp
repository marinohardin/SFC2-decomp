#include "Common/Settings.h"
#include "Common/GenericFile.h"

// The two inlined basic_string::_Tidy calls lack the toolchain's _FROZEN test (the library
// difference noted at 0x00708820); the rest of 0x006f6b80 lines up.
// FUNCTION: STARFLEETOP 0x006f6b80
// FUNCTION: STARFLEETOP 0x006f6b60
tSettings::~tSettings()
{
	if( fSection )
	{
		delete fSection;
		fSection = 0;
	}
}
