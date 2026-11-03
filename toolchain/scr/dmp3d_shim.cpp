// Stand-ins for the two dmp3D_MSVCR.lib functions every OP API script needs but which the SDK's
// truncated copy of that library lacks (see toolchain/README.md). Reconstructed from the copies
// statically linked into the shipped Assets/Scripts/Gen_Playback.scr (tErrInfo::tErrInfo at
// 0x1001d270, tMatrix::mIdentity at 0x1001d150); verify_shim.py checks that this file compiles to
// the same bytes. Declarations: shared/QSI/Projects/QSIInc/Tiger/tdmpBase.h and tvectormath.h.
#include <stdio.h>
#include <stdarg.h>
#include "tdmpBase.h"
#include "tvectormath.h"

tErrInfo::tErrInfo(const char *pFileName, int lineNum, const char *fmt, ...)
{
	sprintf(fLocale, "Line: %5d, Module: %s", lineNum, pFileName);
	va_list args;
	va_start(args, fmt);
	vsprintf(fMsg, fmt, args);
	va_end(args);
}

void tMatrix::mIdentity(void)
{
	fM[0][0] = 1.0f;	fM[0][1] = 0.0f;	fM[0][2] = 0.0f;	fM[0][3] = 0.0f;
	fM[1][0] = 0.0f;	fM[1][1] = 1.0f;	fM[1][2] = 0.0f;	fM[1][3] = 0.0f;
	fM[2][0] = 0.0f;	fM[2][1] = 0.0f;	fM[2][2] = 1.0f;	fM[2][3] = 0.0f;
	fM[3][0] = 0.0f;	fM[3][1] = 0.0f;	fM[3][2] = 0.0f;	fM[3][3] = 1.0f;
	fIdentity = kTrue;
}
