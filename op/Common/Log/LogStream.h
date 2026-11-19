//	LogStream.h
//
//	The log streams and their manipulators.
//	Declarations follow the SDK's header; only what the rebuilt functions need is declared yet.
//	Common/Thread/ThreadPool.cpp still holds a local copy of these declarations.
//
#if !defined( EventLogStream_h )
#define EventLogStream_h

#include "Common/BaseTypes.h"

#include <ostream>

namespace nLog
{

// CLASS PURPOSE: Used to log events using a stream
class tLogStream : public std::ostream
{
};


// Event log stream manipulators
// WARNING!! You must stream in report when you are done with your msg!!!!!!!
// If you don't the message stream will be locked forever!!!!!
std::ostream& __cdecl report( std::ostream& stream );

extern tLogStream warning;
extern tLogStream info;
};

#endif
