// Common/Timer/PerformanceCounter.h

#if !defined( PerformanceCounter_h )
#define PerformanceCounter_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#include "Common/BaseTypes.h"

// HYPOTHESIS: a high resolution timer whose inline members live in Timer/PerformanceCounter.cpp
// (the assertion path inside its constructor, whose text also names fCounterFrequency); the
// class name and the other names are not recovered. The constructor is inline; this header
// includes the file that holds it.
class tPerformanceCounter
{
public:
	tPerformanceCounter();

	// HYPOTHESIS: microseconds since construction; the name is not recovered.
	int32u GetElapsedMicroseconds() const;

protected:
	unsigned __int64 fStart;
	unsigned __int64 fCounterFrequency;
};

inline int32u tPerformanceCounter::GetElapsedMicroseconds() const
{
	LARGE_INTEGER now;
	::QueryPerformanceCounter( &now );
	return (int32u)( ( now.QuadPart - fStart ) / fCounterFrequency );
}

#include "Common/Exceptions/Validate.h"
#include "Common/Timer/PerformanceCounter.cpp"

#endif
