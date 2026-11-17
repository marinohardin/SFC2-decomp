//	DatabaseObject.h
//
//	The base class of every object stored in the database.
//	Declarations follow the SDK's header; only what the rebuilt functions need is declared yet.
//
#pragma once

#if !defined( DatabaseObject_h )
#define DatabaseObject_h

#include <sstream>

#include "port/types.h"
#include "Common/DynamicAllocator.h"
#include "Common/DataStore/Streamable.h"

namespace nDatabase
{

// CLASS PURPOSE: The base class for any object that will be stored
// in the database
class tDatabaseObject : public tAllocatedClass, public nDataStore::tStreamable
{
public:
//Setup
	tDatabaseObject( iObjectVersion versionNumber = 0 );
	tDatabaseObject( const tDatabaseObject& copy );
	virtual ~tDatabaseObject();
	// HYPOTHESIS: the exe holds this assignment out of line (0x006fa870); derived classes call it.
	tDatabaseObject& operator=( const tDatabaseObject& other );

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

// SQL Operations
	virtual void SQLFillInsertStream( std::stringstream* SQL, bool lastOne = false );
	virtual void SQLFillUpdateStream( std::stringstream* SQL, bool lastOne = false );
	virtual const char* SQLTableName() const;

protected:
//Fields
	// HYPOTHESIS: fAccess, fID, fAlive, fReferenceCount, fLockEvent and fLockID fill
	// 0x14 to 0x48; derived classes' first members sit at 0x48 (tMetaMapDesc).
	char fPad_14[0x34];
};

};
namespace db = nDatabase;

#endif
