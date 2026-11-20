//	nStoredProcedureArguments__tGetFleetDataRequest.h
//
//	Name from the exe's RTTI (.?AVtGetFleetDataRequest@nStoredProcedureArguments@@).
//	Constructor 0x0055477f. The layout follows the fleet icon update procedure, which builds
//	one on the stack and destroys it inline: a nDatabase::tID at +0x18 (destructor 0x006fa430),
//	then the base.
//
#if !defined( nStoredProcedureArguments__tGetFleetDataRequest_h )
#define nStoredProcedureArguments__tGetFleetDataRequest_h

#include "unsorted/nSwitch__tRequestWithResponse.h"
#include "unsorted/tAddressMgr.h"
#include "Common/Database/DatabaseID.h"

namespace nStoredProcedureArguments
{

	class tGetFleetDataRequest : public nSwitch::tRequestWithResponse
	{
	public:
		// HYPOTHESIS: the reply address and a database identifier, as
		// tGetClientCharacterRequest takes them.
		tGetFleetDataRequest( tSmartPtr<tObject_005d3679> replyAddress, const nDatabase::tID& characterID );

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

	protected:
		// HYPOTHESIS: the identifier the constructor is given.
		nDatabase::tID field_18;
	};

};

#endif
