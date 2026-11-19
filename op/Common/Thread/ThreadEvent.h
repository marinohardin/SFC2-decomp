// Common/Thread/ThreadEvent.h

#if !defined( ThreadEvent_h )
#define ThreadEvent_h

#include "Common/BaseTypes.h"

#include <string>

#include "Common/Thread/ThreadState.h"

// CLASS PURPOSE: Used for thread syncronizing.
class tThreadEvent
{
public:
// Setup
	tThreadEvent( bool initialState = false, bool manualReset = false, const std::string& name = "", tThreadState::eUse use = tThreadState::kUseThreadObject );
	tThreadEvent( const tThreadEvent& copy );
	tThreadEvent& operator=( const tThreadEvent& copy );
	virtual ~tThreadEvent();

// Operations
	// The SDK's conversion to the event handle (operator void*) is left out until a rebuilt
	// function needs it; tools/manifest.py cannot read its declaration.
	void Signal();
	void Reset();

protected:
// Fields
	void* fHandleToEvent;
	tThreadState::eUse fUse;
};

#endif
