// Common/Thread/WaitForObject.cpp

#include "Common/Thread/WaitForObject.h"

#include <windows.h>

// FUNCTION: STARFLEETOP 0x0070e250
tWaitForObject::tResult::tResult( int32u waitResult )
{
	fKind = 0;
	fIndex = 0;
	if( waitResult == 0x102 )
	{
		fKind = 3;
		return;
	}
	if( waitResult >= 0x80 )
	{
		fKind = 2;
		fIndex = waitResult - 0x80;
		return;
	}
	fKind = 1;
	fIndex = waitResult;
}

// FUNCTION: STARFLEETOP 0x0070e2a0
bool tWaitForObject::tResult::TimedOut() const
{
	return fKind == 3 ? true : false;
}

// FUNCTION: STARFLEETOP 0x0070e2b0
bool tWaitForObject::tResult::Ready() const
{
	return fKind == 1 ? true : false;
}

// FUNCTION: STARFLEETOP 0x0070e2c0
tWaitForObject::tResult tWaitForObject::Wait( void* handle, int32u timeout )
{
	tResult result( WaitForSingleObject( handle, timeout ) );
	return result;
}
