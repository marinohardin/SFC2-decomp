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
// Types
	typedef std::list<tCallback*> iPtrList;
	typedef std::vector<tCallback*> iPtrVector;

//Setup
	virtual ~tCallback();

//Operations
	virtual void Callback( tnResult result ) = 0;
};

#endif
