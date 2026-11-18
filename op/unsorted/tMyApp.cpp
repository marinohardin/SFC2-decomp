// Unit:      unsorted/tMyApp.cpp
// Functions: 0x0040c5e6 to 0x0040c724, 10 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include "Tiger/tpart.h"
#include "unsorted/tApplication.h"

#include <string.h>

#include "Tiger/tbasicfile.h"

#include "unsorted/tMyApp.h"

// HYPOTHESIS: a free (or static) function returning a flag; vfunction15 calls it without a this.
bool fn_0069ac10();

// FUNCTION: STARFLEETOP 0x0040c5e6
bool tMyApp::vfunction3()
{
	m_4->vfunction12();
	tBasicFile::mDone();
	if (!g_00898c35) {
		g_00898c35 = true;
		tPart::mDeactivateParts();
	}
	g_00898c34 = true;
	return true;
}

// FUNCTION: STARFLEETOP 0x0040c612
bool tMyApp::vfunction25()
{
	g_p008cc580->vfunction2("SFC.INI");
	return !g_p008cc580->vfunction5("Debug", "CatchAll", 1);
}

// FUNCTION: STARFLEETOP 0x0040c63f
bool tMyApp::vfunction24()
{
	if (g_00898c34) {
		return false;
	}
	return m_4->vfunction13() ? true : false;
}

// FUNCTION: STARFLEETOP 0x0040c659
bool tMyApp::vfunction15(int param_1)
{
	if (!g_00898c34) {
		m_4->vfunction15(param_1);
		if (param_1 == 0x115 && fn_0069ac10()) {
			m_8 ^= 1;
			fn_0069aa30(m_8);
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0040c699
bool tMyApp::vfunction16(int param_1)
{
	if (!g_00898c34) {
		m_4->vfunction16(param_1);
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0040c6b3
bool tMyApp::vfunction18(int param_1, int param_2, int param_3, int param_4)
{
	if (g_00898c34) {
		return false;
	}
	m_4->vfunction17(param_1, param_2, param_3, param_4);
	return true;
}

// FUNCTION: STARFLEETOP 0x0040c6dd
bool tMyApp::vfunction19(char *param_1)
{
	if (g_00898c34) {
		return false;
	}
	// HYPOTHESIS: the cdecl call to 0x00729723 is the CRT strlen (one argument, unsigned test).
	if (strlen(param_1) > 0) {
		m_4->vfunction22(param_1);
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x0040c70d
void tMyApp::vfunction21(int param_1)
{
	m_4->vfunction14(param_1);
}

// FUNCTION: STARFLEETOP 0x0040c71c
void tMyApp::vfunction22()
{
	m_4->vfunction18();
}

// FUNCTION: STARFLEETOP 0x0040c724
void tMyApp::vfunction23()
{
	m_4->vfunction19();
}
