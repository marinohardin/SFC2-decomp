#include <windows.h>
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

// windows.h defines GetCurrentTime() as GetTickCount(), so this compiles under the
// symbol ?GetTickCount@tDateAndTime@@SA?AV1@XZ, as in the original.
// FUNCTION: STARFLEETOP 0x006f8830
tDateAndTime tDateAndTime::GetCurrentTime()
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

// FUNCTION: STARFLEETOP 0x006f88d0
int32 tDateAndTime::GetYear() const
{
	return GetLocalTm()->tm_year + 1900;
}

// FUNCTION: STARFLEETOP 0x006f88e0
int32 tDateAndTime::GetMonth() const
{
	return GetLocalTm()->tm_mon + 1;
}

// FUNCTION: STARFLEETOP 0x006f88f0
int32 tDateAndTime::GetDay() const
{
	return GetLocalTm()->tm_mday;
}

// FUNCTION: STARFLEETOP 0x006f8900
int32 tDateAndTime::GetHour() const
{
	return GetLocalTm()->tm_hour;
}

// FUNCTION: STARFLEETOP 0x006f8910
int32 tDateAndTime::GetMinute() const
{
	return GetLocalTm()->tm_min;
}

// FUNCTION: STARFLEETOP 0x006f8920
bool tDateAndTime::operator<( const tDateAndTime& time ) const
{
	return ( fTime < time.fTime ) ? true : false;
}
