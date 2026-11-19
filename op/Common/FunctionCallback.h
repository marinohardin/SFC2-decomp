// Common/FunctionCallback.h

#if !defined( FunctionCallback_h )
#define FunctionCallback_h

#include "Common/BaseTypes.h"
#include "Common/Callback.h"

// CLASS PURPOSE: Used to a call a static function when a callback is complete
template< typename tnResult >
class tFunctionCallback : public tCallback< tnResult >
{
public:
//Types
	typedef void (*iFunction)( tnResult result );

//Setup
	tFunctionCallback( iFunction function );
	virtual ~tFunctionCallback();

//Operations
	virtual void Callback( tnResult result );

protected:
	iFunction fpFunction;
};

template< typename tnResult >
tFunctionCallback<tnResult>::tFunctionCallback( iFunction function ) :
	fpFunction( function )
{}

#endif // !defined( FunctionCallback_h )
