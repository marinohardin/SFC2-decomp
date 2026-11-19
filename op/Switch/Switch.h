//	Switch.h
//
//	The switch that carries messages between the client and the servers.
//	Only what the rebuilt functions need is declared yet.
//
#if !defined( Switch_h )
#define Switch_h

#include "Common/DataStore/DataStorePre.h"

class tObject_005d3679;

namespace nSwitch
{

	// Name from the exe's RTTI (.?AVtSwitch@nSwitch@@).
	// HYPOTHESIS: fn_00713b20 returns the switch (its assertion reads fSwitch); fn_00714020
	// streams an object to the destination. Meta/MetaDatabase/AsyncDatabaseProcedures/Single/
	// AsyncDBSingle.cpp still holds a local copy of this declaration.
	class tSwitch
	{
	public:
		static tSwitch* fn_00713b20();
		void fn_00714020( tObject_005d3679* destination, int flags, const nDataStore::tStreamable* object );
	};

};

#endif
