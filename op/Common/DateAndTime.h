//	DateAndTime.h
//
//	A point in time, held as a time_t, that streams through a data store.
//	Declarations follow the SDK's header.
//
#if !defined( DateAndTime_h )
#define DateAndTime_h

#pragma once

#include "Common/BaseTypes.h"
#include "Common/DataStore/Streamable.h"

#include <time.h>
#include <iosfwd>

class tDateAndTime : public nDataStore::tStreamable
{
public:
	enum eDaylightSavingMode
	{
		kStandardTime,
		kDaylightSavingTime,
		kAutomatic
	};

	static tDateAndTime GetCurrentTime();
	static tDateAndTime GetTickCount();

	tDateAndTime();
	tDateAndTime( time_t time );
	tDateAndTime( int32 year, int32 month, int32 day, int32 hour, int32 min, int32 sec, eDaylightSavingMode daylightSaving = kAutomatic );

	const tDateAndTime& operator=( time_t timeSrc );

	tm* GetGmtTm( tm* pTime = NULL ) const;
	tm* GetLocalTm( tm* pTime = NULL ) const;
	time_t GetTime() const;

	int32 GetYear() const;
	int32 GetMonth() const;
	int32 GetDay() const;
	int32 GetHour() const;
	int32 GetMinute() const;
	int32 GetSecond() const;
	int32 GetDayOfWeek() const;

	void SetToCurrentTime();

	bool operator==( const tDateAndTime& time ) const;
	bool operator<( const tDateAndTime& time ) const;

	void StreamOut( nDataStore::tOutput& output ) const;
	void StreamIn( nDataStore::tInput& input );

	friend std::ostream& operator<<( std::ostream& output, const tDateAndTime& dateAndTime );

protected:
	int ConvertDaylightSavingModeToTMValue( eDaylightSavingMode mode );

	time_t fTime;
};

#endif
