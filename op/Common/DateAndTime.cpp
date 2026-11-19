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

// FUNCTION: STARFLEETOP 0x006f8830
tDateAndTime tDateAndTime::GetTickCount()
{
	return tDateAndTime( time( NULL ) );
}

// FUNCTION: STARFLEETOP 0x006f8860
tm* tDateAndTime::GetLocalTm( tm* pTime ) const
{
	if ( pTime != NULL )
	{
		tm* pLocal = localtime( &fTime );
		if ( pLocal == NULL )
		{
			return NULL;
		}

		*pTime = *pLocal;
		return pTime;
	}

	return localtime( &fTime );
}

// FUNCTION: STARFLEETOP 0x006f88b0
void tDateAndTime::SetToCurrentTime()
{
	fTime = time( NULL );
}
