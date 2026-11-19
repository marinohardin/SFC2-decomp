// Common/Log/LogStream.h

#if !defined( EventLogStream_h )
#define EventLogStream_h

#include <ostream>

namespace nLog
{

// CLASS PURPOSE: Used to log events using a stream
// HYPOTHESIS: reduced to its base; the SDK's stream buffer member waits for tLog.
class tLogStream : public std::ostream
{
};


// Event log stream manipulators
// WARNING!! You must stream in report when you are done with your msg!!!!!!!
// If you don't the message stream will be locked forever!!!!!
std::ostream& __cdecl report( std::ostream& stream );

extern tLogStream out;
extern tLogStream warning;
extern tLogStream error;
extern tLogStream info;
extern tLogStream debug;
};

#endif
