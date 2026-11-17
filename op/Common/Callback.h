#if !defined( Callback_h )
#define Callback_h

// tCallback, after the SDK's header of the same name.
// HYPOTHESIS: Common/Thread/ThreadPool.cpp still holds its own copy of this class; that unit
// should include this header instead once its lane can take the edit.

// CLASS PURPOSE: A generic interface to callbacks
template< typename tnResult >
class tCallback
{
public:
//Setup
	virtual ~tCallback();

//Operations
	virtual void Callback( tnResult result ) = 0;
};

#endif
