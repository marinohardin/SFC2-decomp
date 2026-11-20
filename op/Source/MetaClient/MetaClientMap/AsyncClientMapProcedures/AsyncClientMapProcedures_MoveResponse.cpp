#include "Switch/AsyncProcedure.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"
class fn_005e5bd7_class;
namespace nAsyncClientMapProcedures
{

	// Name from the exe's RTTI (.?AVtMoveResponse@nAsyncClientMapProcedures@@).
	class tMoveResponse : public nSwitch::tAsyncProcedure
	{
	public:
		// Parameter name from the validate text. HYPOTHESIS: the relay tInitialize takes.
		tMoveResponse( fn_005e5bd7_class* relay );
		// HYPOTHESIS: the callback registered for message 4 (the RTTI names a
		// tMemberCallback<tMoveResponse,const nDataStore::tBuffer*>).
		void fn_005e0349( const nDataStore::tBuffer* buffer );
	protected:
		fn_005e5bd7_class* field_44;
	};

};

template< typename tnMemberClass, typename tnResult >
tMemberCallback<tnMemberClass,tnResult>::tMemberCallback<tnMemberClass,tnResult>( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction ) :
	fpMemberClass( pMemberClass ),
	fpMemberFunction( pMemberFunction )
{}

using namespace nAsyncClientMapProcedures;
// FUNCTION: STARFLEETOP 0x005e028e
tMoveResponse::tMoveResponse( fn_005e5bd7_class* relay ) :
	field_44( relay )
{
	validate( relay );
	fn_00718f10( 4, new tMemberCallback<tMoveResponse, const nDataStore::tBuffer*>( this, fn_005e0349 ) );
	fn_00719050();
}
