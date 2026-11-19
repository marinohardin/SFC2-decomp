//	tRequestWithResponse.h
//
//	The base of the stored procedure requests that expect a response.
//	HYPOTHESIS: the layout is reconstructed from the accesses of the derived requests' vfunction3
//	(0x0054fa6d, 0x0054ffe5); the exe names the class nSwitch::tRequestWithResponse.
//
#if !defined( tRequestWithResponse_h )
#define tRequestWithResponse_h

#pragma once

#include "Common/BaseTypes.h"

namespace nDataStore
{
	class tInput;
}

class tRequestWithResponse
{
public:
	virtual void v0();
	virtual void v1();
	virtual void v2();
	virtual void vfunction3( nDataStore::tInput& input );
	char pad4[4];
	int8u fFlag8;
	char pad[0x13 - 4];
};

#endif
