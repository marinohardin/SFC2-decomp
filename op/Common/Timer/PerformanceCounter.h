// Common/Timer/PerformanceCounter.h

#if !defined( PerformanceCounter_h )
#define PerformanceCounter_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#include "Common/BaseTypes.h"

// HYPOTHESIS: a high resolution timer whose inline members live in Timer/PerformanceCounter.cpp
// (the assertion path inside its constructor); class and member names are not recovered. The
// constructor is inline; its body is written where it is expanded, so that its check keeps the
// original's line.
class tPerformanceCounter
{
public:
	tPerformanceCounter();

	// HYPOTHESIS: microseconds since construction; the name is not recovered.
	int32u GetElapsedMicroseconds() const;

protected:
	unsigned __int64 fStart;
	unsigned __int64 fTicksPerMicrosecond;
};

inline int32u tPerformanceCounter::GetElapsedMicroseconds() const
{
	LARGE_INTEGER now;
	::QueryPerformanceCounter( &now );
	return (int32u)( ( now.QuadPart - fStart ) / fTicksPerMicrosecond );
}

#endif
