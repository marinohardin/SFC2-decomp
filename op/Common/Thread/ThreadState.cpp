// Common/Thread/ThreadState.cpp

#include "Common/Thread/ThreadState.h"

// FUNCTION: STARFLEETOP 0x006f5690
tThreadState* tThreadState::GetCurThreadState()
{
	if( fCurThreadState == 0 )
	{
		fCurThreadState = new tThreadState( kUseThreadObject );
	}
	return fCurThreadState;
}
