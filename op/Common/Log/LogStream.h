#if !defined( EventLogStream_h )
#define EventLogStream_h

#include "Common/BaseTypes.h"

#include <ostream>

namespace nLog
{

// HYPOTHESIS: the SDK's tLogStream also holds a tStreamBuf member (an optex, two strings, the
// event type and the log); it is left out until a matched function needs the layout.
class tLogStream : public std::ostream
{
};


// Event log stream manipulators
std::ostream& __cdecl report( std::ostream& stream );
std::ostream& __cdecl set_warning( std::ostream& stream );
std::ostream& __cdecl set_error( std::ostream& stream );
std::ostream& __cdecl set_info( std::ostream& stream );
std::ostream& __cdecl set_audit_success( std::ostream& stream );
std::ostream& __cdecl set_audit_fail( std::ostream& stream );

extern tLogStream out;
extern tLogStream warning;
extern tLogStream error;
extern tLogStream info;
extern tLogStream debug;
};

namespace logmsg = nLog;
#endif
