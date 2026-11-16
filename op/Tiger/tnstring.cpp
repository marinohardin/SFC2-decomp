#include <windows.h>
#include <mbctype.h>
#include "Tiger/tstring.h"
#include "Tiger/tcglue.h"

// HYPOTHESIS: a file-scope buffer; the exe keeps 0x1000 bytes of uninitialised data at 0x00c80928.
static char sOSStringBuffer[0x1000];

// FUNCTION: STARFLEETOP 0x0075a5e0
char* tString::mOSString()
{
	clib::memset(sOSStringBuffer, 0, sizeof(sOSStringBuffer));
	UINT codePage = _getmbcp();
	if (codePage)
	{
		if (!WideCharToMultiByte(codePage, 0, mWideStr(), mStrLen(), sOSStringBuffer, sizeof(sOSStringBuffer), NULL, NULL))
		{
			GetLastError();
			return mCharStr();
		}
		return sOSStringBuffer;
	}
	else
		return mCharStr();
}
