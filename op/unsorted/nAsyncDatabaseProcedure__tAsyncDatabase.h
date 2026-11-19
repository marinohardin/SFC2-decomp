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
		virtual void vfunction3();
		virtual void vfunction4();
		virtual void vfunction5();
		virtual void vfunction6();
		virtual void vfunction7();
		virtual void vfunction8();
		virtual void vfunction9();
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
		virtual void vfunction24();
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
		virtual void vfunction38();
		virtual void vfunction39();
		virtual void vfunction40();
		virtual void vfunction41();
		virtual void vfunction42();
		virtual void vfunction43();
		virtual void vfunction44();
		virtual void vfunction45();
		virtual void vfunction46();
		virtual void vfunction47( nDatabase::tDatabaseObjectUpdate update );
	};

};

#endif
