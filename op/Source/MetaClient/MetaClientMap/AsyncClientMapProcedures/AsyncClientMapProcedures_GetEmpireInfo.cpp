#include "Switch/AsyncProcedure.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"

namespace nAsyncClientMapProcedures
{

	// Name from the exe's RTTI (.?AVtGetEmpireInfo@nAsyncClientMapProcedures@@).
	class tGetEmpireInfo : public nSwitch::tAsyncProcedure
	{
	public:
		// The parameter name is from the validate( relay ) text; its type is not known.
		tGetEmpireInfo( void* relay );

		void fn_005df816( const nDataStore::tBuffer* buffer );
		void fn_005df925( const nDataStore::tBuffer* buffer );

	protected:
		void* fRelay;
		// HYPOTHESIS: set to -1 at construction; meaning not known.
		int32 field_48;
	};

};

template< typename tnMemberClass, typename tnResult >
tMemberCallback<tnMemberClass,tnResult>::tMemberCallback<tnMemberClass,tnResult>( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction ) :
	fpMemberClass( pMemberClass ),
	fpMemberFunction( pMemberFunction )
{}

using namespace nAsyncClientMapProcedures;


// FUNCTION: STARFLEETOP 0x005df72e
tGetEmpireInfo::tGetEmpireInfo( void* relay ) :
	fRelay( relay ),
	field_48( -1 )
{
	validate( relay );
	fn_00718f10( 8, new tMemberCallback<tGetEmpireInfo, const nDataStore::tBuffer*>( this, fn_005df816 ) );
	fn_00718f10( 9, new tMemberCallback<tGetEmpireInfo, const nDataStore::tBuffer*>( this, fn_005df925 ) );
	fn_00719050();
}
