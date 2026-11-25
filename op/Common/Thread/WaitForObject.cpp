// Common/Thread/WaitForObject.cpp

#include "Common/Thread/WaitForObject.h"

// FUNCTION: STARFLEETOP 0x0070e2a0
bool tWaitForObject::tResult::TimedOut() const
{
	return fKind == 3 ? true : false;
}
