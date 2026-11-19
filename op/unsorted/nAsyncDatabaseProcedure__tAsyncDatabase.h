//	nAsyncDatabaseProcedure__tAsyncDatabase.h
//
//	The database the async database procedures run against. Only what the rebuilt functions
//	need is declared yet.
//
#if !defined( nAsyncDatabaseProcedure__tAsyncDatabase_h )
#define nAsyncDatabaseProcedure__tAsyncDatabase_h

#pragma once

#include <vector>

#include "Common/Database/DatabaseObjectUpdate.h"

class tHexPoint;
class tMetaMapHex;
class tPoliticalTensionMatrix;
class tPreparedMissions;
class tShip;
class tServCharacter;

namespace nAsyncDatabaseProcedure
{

	// HYPOTHESIS: RTTI names the class. The slots follow tAsyncDatabaseSingle's in
	// Meta/MetaDatabase/AsyncDatabaseProcedures/Single/AsyncDBSingle.cpp, which is taken to
	// implement them; only the slots that rebuilt functions call carry signatures.
	class tAsyncDatabase
	{
	public:
		virtual void vfunction1();
		virtual void vfunction2();
		// HYPOTHESIS: reads characters by ID (0x00568b2e).
		virtual void vfunction3( std::vector<nDatabase::tID> ids, std::vector<tServCharacter>* characters, int lock );
		virtual void vfunction4();
		virtual void vfunction5();
		virtual void vfunction6();
		virtual void vfunction7();
		virtual void vfunction8();
		// HYPOTHESIS: reads ships by ID (0x005682b2); tAsyncDatabaseSingle defines the slot with
		// these arguments.
		virtual void vfunction9( std::vector<nDatabase::tID> ids, std::vector<tShip>* ships, int lock );
		virtual void vfunction10();
		virtual void vfunction11();
		virtual void vfunction12();
		virtual void vfunction13( tHexPoint point, std::vector<tMetaMapHex>* hexes, int lock );
		virtual void vfunction14();
		virtual void vfunction15( nDatabase::tID id, tMetaMapHex* hex, int lock );
		virtual void vfunction16();
		virtual void vfunction17();
		virtual void vfunction18();
		virtual void vfunction19( std::vector<tPoliticalTensionMatrix>* matrices );
		virtual void vfunction20();
		virtual void vfunction21();
		virtual void vfunction22();
		virtual void vfunction23();
		// HYPOTHESIS: reads prepared missions (0x005676f4); tAsyncDatabaseSingle defines the
		// slot with these arguments.
		virtual void vfunction24( nDatabase::tID id, bool byCharacterID, std::vector<tPreparedMissions>* queryResult, int lock );
		virtual void vfunction25();
		virtual void vfunction26();
		virtual void vfunction27();
		virtual void vfunction28( std::vector<tPoliticalTensionMatrix>* matrices );
		virtual void vfunction29();
		virtual void vfunction30();
		virtual void vfunction31( tPoliticalTensionMatrix* matrix );
		virtual void vfunction32();
		virtual void vfunction33();
		virtual void vfunction34();
		virtual void vfunction35();
		virtual void vfunction36();
		virtual void vfunction37();
		// HYPOTHESIS: stores new prepared missions (0x00567270).
		virtual void vfunction38( tPreparedMissions* missions );
		virtual void vfunction39();
		virtual void vfunction40();
		virtual void vfunction41();
		virtual void vfunction42();
		virtual void vfunction43();
		// HYPOTHESIS: writes a ship back (0x00568b2e), taken by value.
		virtual void vfunction44( tShip ship );
		virtual void vfunction45();
		virtual void vfunction46();
		virtual void vfunction47( nDatabase::tDatabaseObjectUpdate update );
		virtual void vfunction48();
		virtual void vfunction49();
		virtual void vfunction50();
		virtual void vfunction51();
		// HYPOTHESIS: kills a ship (0x005682b2); tAsyncDatabaseSingle's slot takes a tShip*.
		virtual void vfunction52( tShip* ship );
		virtual void vfunction53();
		virtual void vfunction54();
		virtual void vfunction55();
		// HYPOTHESIS: removes the prepared missions of a character (0x00567270); the slot
		// tAsyncDatabaseSingle defines at this index takes the same arguments.
		virtual void vfunction56( nDatabase::tID id, bool byCharacterID );
		virtual void vfunction57();
		virtual void vfunction58();
		virtual void vfunction59();
		virtual void vfunction60();
		virtual void vfunction61();
		virtual void vfunction62();
		virtual void vfunction63();
		// HYPOTHESIS: whether prepared missions exist for a character (0x00567270).
		virtual bool vfunction64( nDatabase::tID id, bool byCharacterID );
	};

};

#endif
