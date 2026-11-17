// Unit:      Source/app/shellApp.cpp
// Functions: 0x0040c3ba to 0x0040c56e, 5 in op/UNITS.tsv
// Evidence:  assert-path (the original's assertion strings name this file)
// Profile:   game-release (harness/units.json)

#include <windows.h>

#include "Tiger/tpart.h"
#include "Tiger/tbasicfile.h"
#include "dmp3d/common/stream_basicfile.h"
#include "unsorted/tMyApp.h"

void fn_00435c0f();
void fn_004014e1(int, const char *);

// FUNCTION: STARFLEETOP 0x0040c3ba
bool tMyApp::vfunction2()
{
	try
	{
		m_8 = false;
		fn_0069aa30(false);
		fn_00435c0f();
		tPart::mValidateParts();
		tPart::mActivateParts();
		if (!tBasicFile::mInit(true))
		{
#line 149
			throw fn_006a2ec0(__FILE__, __LINE__, "Could not initialize file system");
		}
		fn_0069a9a0("Starfleet Command");
		// HYPOTHESIS: fn_0069aa00 returns the window handle.
		SetFocus((HWND)fn_0069aa00());
		fn_004014e1(fn_0069aa00(), "");
		if (!m_4->vfunction21(this))
		{
#line 177
			throw fn_006a2ec0(__FILE__, __LINE__, "fTest3D.mInit() failed");
		}
		return true;
	}
	// HYPOTHESIS: the four handlers (0x0040c4c2, 0x0040c4fc, 0x0040c520, 0x0040c56e) are
	// reconstructed from the strings beside this function's; their bodies are not verified.
	catch (tPart::tPartStartupException)
	{
		fn_0069aa00();
	}
	catch (tPart::tPartBindingException)
	{
		fn_0069aa00();
	}
	catch (char *string)
	{
		fn_0069aa00();
	}
	catch (fn_006a2ec0_Error error)
	{
		fn_0069aa00();
	}
	return true;
}
