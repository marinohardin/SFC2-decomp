//	nStoredProcedureArguments__tGetClientCharacterRequest.h
//
//	Name from the exe's RTTI (.?AVtGetClientCharacterRequest@nStoredProcedureArguments@@).
//	Constructor 0x00553e1e. The layout follows the logon information procedure, which builds
//	one on the stack and destroys it inline: a member at +0x18 (destructor 0x0053d691), then
//	the base.
//
#if !defined( nStoredProcedureArguments__tGetClientCharacterRequest_h )
#define nStoredProcedureArguments__tGetClientCharacterRequest_h

#include "unsorted/nSwitch__tRequestWithResponse.h"
#include "unsorted/tAddressMgr.h"
#include "Common/Database/DatabaseID.h"

namespace nStoredProcedureArguments
{

	class tGetClientCharacterRequest : public nSwitch::tRequestWithResponse
	{
	public:
		// HYPOTHESIS: the reply address, as tRegisterForTurnBreakRequest takes it, and the
		// character's database identifier.
		tGetClientCharacterRequest( tSmartPtr<tObject_005d3679> replyAddress, const nDatabase::tID& characterID );

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

	protected:
		// HYPOTHESIS: a 0x14-byte streamable member at +0x18 with destructor 0x0053d691; its
		// type is not recovered.
		class tMember : public nDataStore::tStreamable
		{
		public:
			virtual ~tMember();
			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

		protected:
			int8u field_0c[0x8];
		};

		tMember field_18;
	};

};

#endif
