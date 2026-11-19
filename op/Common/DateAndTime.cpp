#include <time.h>
#include "Common/DateAndTime.h"

// FUNCTION: STARFLEETOP 0x006f87d0
tDateAndTime::tDateAndTime()
{
	fTime = time( NULL );
}
