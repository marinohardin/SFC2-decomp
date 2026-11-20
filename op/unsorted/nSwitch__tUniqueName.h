//	nSwitch__tUniqueName.h
//
//	Name from the exe's RTTI (.?AVtUniqueName@nSwitch@@), a tStreamable. Size 0x34 from the
//	request that holds one at +0x18 in the map initialize procedure's stack frame.
//
#if !defined( nSwitch__tUniqueName_h )
#define nSwitch__tUniqueName_h

#include "Common/DataStore/Streamable.h"

namespace nSwitch
{

	class tUniqueName : public nDataStore::tStreamable
	{
	public:
		virtual ~tUniqueName();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

	protected:
		// HYPOTHESIS: members not recovered.
		int8u field_0c[0x28];
	};

};

#endif
