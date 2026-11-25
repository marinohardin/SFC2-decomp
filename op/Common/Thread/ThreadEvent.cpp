// Common/Thread/ThreadEvent.cpp

#include "Common/Thread/ThreadEvent.h"

extern "C" __declspec(dllimport) int __stdcall CloseHandle( void* );

// FUNCTION: STARFLEETOP 0x0070e4e0
// FUNCTION: STARFLEETOP 0x0070e500
tThreadEvent::~tThreadEvent()
{
	if( fUse )
	{
		CloseHandle( fHandleToEvent );
	}
}
