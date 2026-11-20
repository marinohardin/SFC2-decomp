// Common/CommandLineArgs.cpp

#include <windows.h>
#include <string>

#include "Common/CommandLineArgs.h"

tCommandLineArgs::iAutoPtr tCommandLineArgs::g_00c7f8b8;

// FUNCTION: STARFLEETOP 0x006fdb80
tCommandLineArgs *tCommandLineArgs::Instance(void)
{
	if (g_00c7f8b8.get() == 0)
	{
		g_00c7f8b8 = iAutoPtr(new tCommandLineArgs);
	}

	return g_00c7f8b8.get();
}

// FUNCTION: STARFLEETOP 0x006fdc50
tCommandLineArgs::tCommandLineArgs()
	: m14(GetCommandLineA())
{
}

// MATCH-ONLY: the destructor (0x006fdd40) is implicit: its body is the member destructors. Its
// inlined basic_string::_Tidy calls lack the toolchain's _FROZEN test (0x00708820), so it is unclaimed.
// FUNCTION: STARFLEETOP 0x006fdd20
tCommandLineArgs::~tCommandLineArgs()
{
}
