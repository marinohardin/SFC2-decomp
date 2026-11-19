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
		// HYPOTHESIS: the constructor at 0x0055f1cd fills the three fields; the news panel
		// passes ( 3, 0, -1 ). Parameter types are guesses.
		tNotifyEvent( int32 field0c, int32 field10, int32 field14 );
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
