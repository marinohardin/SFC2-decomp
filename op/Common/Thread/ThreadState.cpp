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

// FUNCTION: STARFLEETOP 0x006f5700
void tThreadState::SetCurThreadState( tThreadState* state )
{
	if( fCurThreadState != 0 )
	{
		delete fCurThreadState;
	}
	fCurThreadState = state;
}
