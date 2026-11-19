//	nStoredProcedureArguments__tNotifyEvent.h
//
//	Name from the exe's RTTI (.?AVtNotifyEvent@nStoredProcedureArguments@@). Size 0x18 from the
//	constructor at 0x0055f1cd (fields at +0x0c, +0x10, +0x14).
//
#if !defined( nStoredProcedureArguments__tNotifyEvent_h )
#define nStoredProcedureArguments__tNotifyEvent_h

#include "Common/DataStore/Streamable.h"

namespace nStoredProcedureArguments
{

	class tNotifyEvent : public nDataStore::tStreamable
	{
	public:
		tNotifyEvent();
		virtual ~tNotifyEvent();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		// HYPOTHESIS: returns the object ID the event is about; the news flash builds a
		// nDatabase::tID from it.
		int32u fn_005e8f25();

	protected:
		int8u field_0c[0x0c];
	};

};

#endif
