// Common/Log/LogStream.cpp

#include "Common/BaseTypes.h"
#include "Common/Thread/ThreadState.h"

#include "Common/Log/LogStream.h"

namespace nLog
{

// FUNCTION: STARFLEETOP 0x006fe4f0
tLogStream::tStreamBuf::tStreamBuf( tLog* eventLog )
	: fLog( eventLog )
	, fEventType( tLog::kErrorEvent )
	, fLockType( false )
	, fAccess( tThreadState::GetCurThreadState()->DefaultThreadObjectUse() )
{}

};
