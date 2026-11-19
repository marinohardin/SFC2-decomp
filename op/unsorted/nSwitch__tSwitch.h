//	nSwitch__tSwitch.h
//
//	The switch. Only what the rebuilt functions need is declared yet.
//
#if !defined( nSwitch__tSwitch_h )
#define nSwitch__tSwitch_h

#pragma once

#include "Common/DataStore/DataStorePre.h"

namespace nSwitch
{

	class tAddress;

	// HYPOTHESIS: fn_00713b20 returns the switch (its assertion reads fSwitch); fn_00714020
	// streams an object to the destination with the given flags.
	class tSwitch
	{
	public:
		static tSwitch* fn_00713b20();
		void fn_00714020( tAddress* destination, int flags, const nDataStore::tStreamable* object );
	};

};

#endif
