#if !defined( Callback_h )
#define Callback_h

#include "Common/BaseTypes.h"

#include <list>
#include <vector>

// CLASS PURPOSE: A generic interface to callbacks
// HYPOTHESIS: Common/Thread/ThreadPool.cpp still carries its own copy of this class.
template< typename tnResult >
class tCallback
{
public:
// MATCH-ONLY: the SDK's header also declares the types iPtrList and iPtrVector here. No unit
// uses them, and with them the compiler does not finish unsorted/fn_005DE000.cpp and
// unsorted/fn_005FB000.cpp, which instantiate this class for reference results.

//Setup
	virtual ~tCallback();

//Operations
	virtual void Callback( tnResult result ) = 0;
};

#endif
