//	nAsyncDatabaseProcedure__tAsyncDatabase.h
//
//	The database the async database procedures run against. Only what the rebuilt functions
//	need is declared yet.
//
#if !defined( nAsyncDatabaseProcedure__tAsyncDatabase_h )
#define nAsyncDatabaseProcedure__tAsyncDatabase_h

#pragma once

#include <vector>

class tHexPoint;
class tMetaMapHex;

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
	};

};

#endif
