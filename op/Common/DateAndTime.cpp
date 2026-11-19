#include <time.h>
#include "Common/DateAndTime.h"

// FUNCTION: STARFLEETOP 0x006f87d0
tDateAndTime::tDateAndTime()
{
	fTime = time( NULL );
}

// FUNCTION: STARFLEETOP 0x006f8800
tDateAndTime::tDateAndTime( time_t time )
{
	fTime = time;
}

// FUNCTION: STARFLEETOP 0x006f8820
const tDateAndTime& tDateAndTime::operator=( time_t timeSrc )
{
	fTime = timeSrc;
	return *this;
}
