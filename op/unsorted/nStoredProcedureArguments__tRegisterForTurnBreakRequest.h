//	nStoredProcedureArguments__tRegisterForTurnBreakRequest.h
//
//	Name from the exe's RTTI (.?AVtRegisterForTurnBreakRequest@nStoredProcedureArguments@@).
//	Its tUniqueName member at +0x18 and the destructor's inlined calls follow the map initialize
//	procedure, which builds one on the stack.
//
#if !defined( nStoredProcedureArguments__tRegisterForTurnBreakRequest_h )
#define nStoredProcedureArguments__tRegisterForTurnBreakRequest_h

#include "unsorted/nSwitch__tRequestWithResponse.h"
#include "unsorted/nSwitch__tUniqueName.h"
#include "unsorted/nSwitch__tResponse.h"
#include "unsorted/tAddressMgr.h"

namespace nStoredProcedureArguments
{

	class tRegisterForTurnBreakRequest : public nSwitch::tRequestWithResponse
	{
	public:
		// HYPOTHESIS: the reply address, as tGetNewsRequest takes it, and a value the map
		// initialize procedure reads from its relay; its meaning is not recovered.
		tRegisterForTurnBreakRequest( tSmartPtr<tObject_005d3679> replyAddress, int32 param_2 );

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		// HYPOTHESIS: nested, as the unit name of its constructor 0x0054f0b9 says.
		class tRegisterForTurnBreakResponse : public nSwitch::tResponse
		{
		public:
			tRegisterForTurnBreakResponse();
			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

		protected:
			// HYPOTHESIS: a streamable member at +0x10 (size 0x30); its type is not recovered.
			class tMember : public nDataStore::tStreamable
			{
			public:
				virtual	void StreamOut( nDataStore::tOutput& output ) const;
				virtual	void StreamIn( nDataStore::tInput& input );

			protected:
				int8u field_0c[0x24];
			};

			tMember field_10;
		};

	protected:
		nSwitch::tUniqueName field_18;
	};

};

#endif
