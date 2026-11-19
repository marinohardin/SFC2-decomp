//	tNotifierAgent.h
//
//	Name from the exe's RTTI. Constructor 0x005cb981, destructor 0x005cba09. Size 0xc from a
//	local in the map initialize procedure.
//
#if !defined( tNotifierAgent_h )
#define tNotifierAgent_h

#include "Common/BaseTypes.h"

class tNotifierAgent
{
public:
	tNotifierAgent();
	virtual ~tNotifierAgent();

	// HYPOTHESIS: both take a value the map initialize procedure reads from its relay.
	void fn_005cc674( int32 param_1 );
	void fn_005cc6db( int32 param_1 );

protected:
	int8u field_04[0x8];
};

#endif
