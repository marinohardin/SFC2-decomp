#if !defined( MemberCallback_h )
#define MemberCallback_h

// tMemberCallback, after the SDK's header of the same name.

#include "Common/Callback.h"

// CLASS PURPOSE: Used to a call a member function when a callback is complete
template< typename tnMemberClass, typename tnResult >
class tMemberCallback :
	public tCallback<tnResult>
{
public:
//Types
	typedef void (tnMemberClass::*iMemberFunction)( tnResult result );

//Setup
	tMemberCallback( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction );
	virtual ~tMemberCallback();

//Operations
	virtual void Callback( tnResult result );
	virtual void operator()( tnResult result );

protected:
	tnMemberClass* fpMemberClass;
	iMemberFunction fpMemberFunction;
};


template< typename tnMemberClass, typename tnResult >
tMemberCallback<tnMemberClass,tnResult>::tMemberCallback<tnMemberClass,tnResult>( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction ) :
	fpMemberClass( pMemberClass ),
	fpMemberFunction( pMemberFunction )
{}

template< typename tnMemberClass, typename tnResult >
tMemberCallback<tnMemberClass,tnResult>::~tMemberCallback<tnMemberClass,tnResult>()
{}

// Helper functions
template< typename tParam, typename tClass >
tMemberCallback< tClass, tParam > CreateCallback( tClass* object, void (tClass::*func)( tParam result ) );

#endif
