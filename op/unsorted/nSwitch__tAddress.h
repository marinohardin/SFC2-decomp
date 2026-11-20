//	nSwitch__tAddress.h
//
//	A switch address. Only what the rebuilt functions need is declared yet.
//
#if !defined( nSwitch__tAddress_h )
#define nSwitch__tAddress_h

#pragma once

#include "Common/DataStore/Streamable.h"
#include "Common/SmartPtr.h"

namespace nSwitch
{

	// HYPOTHESIS: RTTI names nSwitch::tAddress. The objects the switch sends to are held by
	// smart pointers whose tRefCount sits at +0xc, after a tStreamable (0x0056e124, 0x00565464);
	// that they are addresses is a guess.
	class tAddress : public nDataStore::tStreamable, public tRefCount
	{
	};

};

#endif
