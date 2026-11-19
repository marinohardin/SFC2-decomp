#if !defined( AsyncProcedure_h )
#define AsyncProcedure_h

#include "Common/BaseTypes.h"
#include "Common/Callback.h"

namespace nDataStore
{
	class tBuffer;
};

namespace nSwitch
{

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

	protected:
		int8u field_04[0x40];
	};

};

#endif
