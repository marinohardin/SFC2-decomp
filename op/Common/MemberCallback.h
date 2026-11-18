#if !defined( MemberCallback_h )
#define MemberCallback_h

#include "Common/BaseTypes.h"
#include "Common/Callback.h"

// CLASS PURPOSE: Used to a call a member function when a callback is complete
template< typename tnMemberClass, typename tnResult >
class tMemberCallback
	: public tCallback<tnResult>
{
public:
	typedef void (tnMemberClass::*iMemberFunction)( tnResult result );

	tMemberCallback( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction );
	virtual ~tMemberCallback();

	virtual void Callback( tnResult result );
	virtual void operator()( tnResult result );

protected:
	tnMemberClass* fpMemberClass;
	iMemberFunction fpMemberFunction;
};

#endif
