#include "Switch/AsyncProcedure.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"

class tMetaClientNewsCtlrC;
namespace nAsyncClientNewsProcedure
{

	// Name from the exe's RTTI (.?AVtGetNewsResponse@nAsyncClientNewsProcedure@@).
	class tGetNewsResponse : public nSwitch::tAsyncProcedure
	{
	public:
		// HYPOTHESIS: ctlr (the validate text) is the news controller.
		tGetNewsResponse( tMetaClientNewsCtlrC* ctlr );

		void fn_005dc753( const nDataStore::tBuffer* buffer );

	protected:
		tMetaClientNewsCtlrC* fCtlr;
	};

};

template< typename tnMemberClass, typename tnResult >
tMemberCallback<tnMemberClass,tnResult>::tMemberCallback<tnMemberClass,tnResult>( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction ) :
	fpMemberClass( pMemberClass ),
	fpMemberFunction( pMemberFunction )
{}

using namespace nAsyncClientNewsProcedure;

// FUNCTION: STARFLEETOP 0x005dc698
tGetNewsResponse::tGetNewsResponse( tMetaClientNewsCtlrC* ctlr ) :
	fCtlr( ctlr )
{
	validate( ctlr );
	fn_00718f10( 2, new tMemberCallback<tGetNewsResponse, const nDataStore::tBuffer*>( this, fn_005dc753 ) );
	fn_00719050();
}
