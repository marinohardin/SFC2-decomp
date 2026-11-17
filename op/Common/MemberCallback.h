#if !defined( MemberCallback_h )
#define MemberCallback_h

// tMemberCallback, after the SDK's header of the same name.
// HYPOTHESIS: in the SDK it derives from tCallback<tnResult>, which is declared so far only
// inside Common/Thread/ThreadPool.cpp; the base is left out here until it has a header. Its
// only state is the vtable pointer, so the layout is the same.

// CLASS PURPOSE: Used to a call a member function when a callback is complete
template< typename tnMemberClass, typename tnResult >
class tMemberCallback
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

// Helper functions
template< typename tParam, typename tClass >
tMemberCallback< tClass, tParam > CreateCallback( tClass* object, void (tClass::*func)( tParam result ) );

#endif
