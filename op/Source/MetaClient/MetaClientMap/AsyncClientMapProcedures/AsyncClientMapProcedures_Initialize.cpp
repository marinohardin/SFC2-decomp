#include "Switch/AsyncProcedure.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/tAddressMgr.h"
#include "Switch/Switch.h"
#include "Common/Log/LogStream.h"
#include "unsorted/nStoredProcedureArguments__tMetaMapSizeRequest.h"
class fn_005e5bd7_class;
namespace nAsyncClientMapProcedures
{

	// Name from the exe's RTTI (.?AVtInitialize@nAsyncClientMapProcedures@@).
	class tInitialize : public nSwitch::tAsyncProcedure
	{
	public:
		// The parameter's member name is from the validate( fpMyRelay ) text.
		tInitialize( fn_005e5bd7_class* pMyRelay );
		virtual ~tInitialize();
		// HYPOTHESIS: the callback for message 0 is 0x005dfb86 (its address sits in this unit
		// before the other two; the instruction holding it could not be read).
		void fn_005dfb86( const nDataStore::tBuffer* buffer );
		void fn_005dfc37( const nDataStore::tBuffer* buffer );
		void fn_005dfdab( const nDataStore::tBuffer* buffer );

	protected:
		// Name from the original's validate text.
		tSmartPtr<tObject_005d3679> fpMapRelayAddress;
		fn_005e5bd7_class* fpMyRelay;
	};

};

template< typename tnMemberClass, typename tnResult >
tMemberCallback<tnMemberClass,tnResult>::tMemberCallback<tnMemberClass,tnResult>( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction ) :
	fpMemberClass( pMemberClass ),
	fpMemberFunction( pMemberFunction )
{}

using namespace nAsyncClientMapProcedures;
using namespace nStoredProcedureArguments;

// FUNCTION: STARFLEETOP 0x005dfa31
tInitialize::tInitialize( fn_005e5bd7_class* pMyRelay ) :
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

#include "unsorted/nStoredProcedureArguments__tRegisterForTurnBreakRequest.h"
#include "Common/DataStore/DataStoreInput.h"









// FUNCTION: STARFLEETOP 0x005dfb86
void tInitialize::fn_005dfb86( const nDataStore::tBuffer* buffer )
{
	nLog::info << "MetaClient: kInitialize (Map)" << nLog::report;

	tMetaMapSizeRequest request( fn_00719490( fn_00719580( 1 ) ) );

	fn_00719050();

	validate( fpMapRelayAddress );
	field_1c->fn_00714020( fpMapRelayAddress, 2, &request );
}

#include "unsorted/fn_005e5bd7_class.h"










// FUNCTION: STARFLEETOP 0x005dfc37
void tInitialize::fn_005dfc37( const nDataStore::tBuffer* buffer )
{
	nDataStore::tInput input( buffer );
	tMetaMapSizeRequest::tResponse response;
	input >> response;

	fn_00719050();

	fpMyRelay->fn_005e5bd7( response.field_0c );
	fpMyRelay->fn_005e5bfe( response.field_10 );
	fpMyRelay->fn_005e5ef4();

	tRegisterForTurnBreakRequest request( fn_00719490( fn_00719580( 2 ) ), fpMyRelay->vfunction8( 1 ) );

	validate( tAddressMgr::Instance()->GetClockRelayAddress() );
	field_1c->fn_00714020( tAddressMgr::Instance()->GetClockRelayAddress(), 2, &request );
}
