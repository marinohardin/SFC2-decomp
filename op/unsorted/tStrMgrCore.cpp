#include "Tiger/tgStrMgr.h"

// FUNCTION: STARFLEETOP 0x00424b90
// HYPOTHESIS: the parameter is the byte flag at +0x40 (the throw-on-miss flag), and the
// counter at +0x3c starts at 1.
tStrMgrCore::tStrMgrCore( unsigned char throwOnMiss )
{
	fn_00627200_throwOnMiss = throwOnMiss;
	fn_00627180_count = 1;
}
