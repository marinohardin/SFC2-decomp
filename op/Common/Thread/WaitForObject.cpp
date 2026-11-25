// Common/Thread/WaitForObject.cpp

#include "Common/Thread/WaitForObject.h"

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
