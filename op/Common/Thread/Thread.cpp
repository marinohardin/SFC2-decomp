// Common/Thread/Thread.cpp

#include <process.h>

#include "Common/Thread/Thread.h"

// The inlined basic_string::_Tidy of the second event's name lacks the toolchain's _FROZEN
// test (the library difference noted at 0x00708820); the rest of 0x006fefd0 lines up.
// FUNCTION: STARFLEETOP 0x006fefd0
tThread::tThread() :
	fWorking( false ),
	fAlive( true ),
	fCallStackOfCallingThread( false )
{
	fThread = _beginthreadex( NULL, 0, ThreadProc, this, 0, &fThreadID );
}

// FUNCTION: STARFLEETOP 0x006ff6f0
bool tThread::Working() const
{
	return fWorking;
}
