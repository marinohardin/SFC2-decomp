// Common/Timer/PerformanceCounter.inl
//
// HYPOTHESIS: the inline members of tPerformanceCounter, included by PerformanceCounter.h; the
// constructor's check names this file and line 44.
//
// MATCH-ONLY: the original's check holds this file's name relative to its project folder; the
// directive below gives that name, which an include path from the op/ root cannot.
#line 9 ".\\Timer\\PerformanceCounter.cpp"

























inline tPerformanceCounter::tPerformanceCounter()
	: fStart( 0 )
{



	unsigned __int64 frequency;
	::QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>( &frequency ) );
	fCounterFrequency = frequency / 1000000;

	validate( fCounterFrequency != 0 );

	LARGE_INTEGER now;
	::QueryPerformanceCounter( &now );
	fStart = now.QuadPart;
}
