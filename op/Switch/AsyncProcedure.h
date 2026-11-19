#if !defined( AsyncProcedure_h )
#define AsyncProcedure_h

#include "Common/BaseTypes.h"
#include "Common/Callback.h"
#include "unsorted/tAddressMgr.h"

namespace nDataStore
{
	class tBuffer;
};

namespace nSwitch
{

	class tSwitch;

	// Name from the exe's RTTI (.?AVtAsyncProcedure@nSwitch@@). Layout to 0x44 from the
	// derived procedures, which store their first member there.
	class tAsyncProcedure
	{
	public:
		tAsyncProcedure();
		virtual ~tAsyncProcedure();

		// HYPOTHESIS: registers a callback for a message kind; called with 2 by the client
		// news procedures.
		void fn_00718f10( int32 param_1, tCallback<const nDataStore::tBuffer*>* param_2 );
		void fn_00719050();
		// HYPOTHESIS: the map initialize procedure passes fn_00719580( 1 ) to fn_00719490 and
		// builds a reply address from the result; return types are not recovered.
		int32 fn_00719580( int32 param_1 );
		const tSmartPtr<tObject_005d3679>& fn_00719490( int32 param_1 );

	protected:
		int8u field_04[0x18];
		// HYPOTHESIS: the switch the procedure sends through (the map initialize procedure
		// calls fn_00714020 on it).
		tSwitch* field_1c;
		int8u field_20[0x1c];
		// HYPOTHESIS: the procedure's own address, handed to the requests it sends (the news
		// flash passes it by value to tGetNewsRequest).
		tSmartPtr<tObject_005d3679> field_3c;
		int8u field_40[0x4];
	};

};

#endif
