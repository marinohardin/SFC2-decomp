//	nStoredProcedureArguments__tGetNewsRequest.h
//
//	Name from the exe's RTTI (.?AVtGetNewsRequest@nStoredProcedureArguments@@). Size 0x28 from
//	a local in the news flash procedure.
//
#if !defined( nStoredProcedureArguments__tGetNewsRequest_h )
#define nStoredProcedureArguments__tGetNewsRequest_h

#include "Common/DataStore/Streamable.h"
#include "Common/Database/DatabaseID.h"
#include "unsorted/tAddressMgr.h"

namespace nStoredProcedureArguments
{

	class tGetNewsRequest : public nDataStore::tStreamable
	{
	public:
		// HYPOTHESIS: the reply address, a request kind (2 from the news flash) and the ID
		// of the news item asked for.
		tGetNewsRequest( tSmartPtr<tObject_005d3679> replyAddress, int32 param_2, const nDatabase::tID& id );
		virtual ~tGetNewsRequest();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

	protected:
		int8u field_0c[0x1c];
	};

};

#endif
