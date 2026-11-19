#include "Switch/AsyncProcedure.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/tAddressMgr.h"

namespace nAsyncClientMapProcedures
{

	// Name from the exe's RTTI (.?AVtInitialize@nAsyncClientMapProcedures@@).
	class tInitialize : public nSwitch::tAsyncProcedure
	{
	public:
		// The parameter's member name is from the validate( fpMyRelay ) text; its type is not known.
		tInitialize( void* pMyRelay );
		virtual ~tInitialize();
		// HYPOTHESIS: the callback for message 0 is 0x005dfb86 (its address sits in this unit
		// before the other two; the instruction holding it could not be read).
		void fn_005dfb86( const nDataStore::tBuffer* buffer );
		void fn_005dfc37( const nDataStore::tBuffer* buffer );
		void fn_005dfdab( const nDataStore::tBuffer* buffer );

	protected:
		// Name from the original's validate text.
		tSmartPtr<tObject_005d3679> fpMapRelayAddress;
		void* fpMyRelay;
	};

};

template< typename tnMemberClass, typename tnResult >
tMemberCallback<tnMemberClass,tnResult>::tMemberCallback<tnMemberClass,tnResult>( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction ) :
	fpMemberClass( pMemberClass ),
	fpMemberFunction( pMemberFunction )
{}

using namespace nAsyncClientMapProcedures;





// FUNCTION: STARFLEETOP 0x005dfa31
tInitialize::tInitialize( void* pMyRelay ) :
	fpMapRelayAddress( tAddressMgr::Instance()->GetMapRelayAddress() ),
	fpMyRelay( pMyRelay )
{
	validate( fpMyRelay );
	fn_00718f10( 0, new tMemberCallback<tInitialize, const nDataStore::tBuffer*>( this, fn_005dfb86 ) );
	fn_00718f10( 1, new tMemberCallback<tInitialize, const nDataStore::tBuffer*>( this, fn_005dfc37 ) );
	fn_00718f10( 2, new tMemberCallback<tInitialize, const nDataStore::tBuffer*>( this, fn_005dfdab ) );
	fn_00719050();
}

// FUNCTION: STARFLEETOP 0x005dfb28
// FUNCTION: STARFLEETOP 0x005dfb44
tInitialize::~tInitialize()
{
}
