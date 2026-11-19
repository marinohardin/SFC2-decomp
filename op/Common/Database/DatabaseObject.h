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
#include "Common/Database/DatabaseID.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/ThreadEvent.h"

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

// STL Helper operators for use with sort() and unique() and find_if()
	virtual bool operator< ( const tDatabaseObject& obj );
	virtual bool operator== ( const tDatabaseObject& obj );

// SQL Operations
	virtual void SQLFillInsertStream( std::stringstream* SQL, bool lastOne = false );
	virtual void SQLFillUpdateStream( std::stringstream* SQL, bool lastOne = false );
	virtual const char* SQLTableName() const;
	virtual const char* SQLFieldNames() const;

//Operations
	virtual void MakeReadOnly();
	virtual void SetID( const tID& id );
	// HYPOTHESIS: the SDK's parameter type is tDatabase::iLockID, a typedef of int32u; tDatabase
	// is declared only inside Common/Database/FileDatabaseMgr.cpp.
	virtual void SetLockID( const int32u id );

//Accessors
	virtual const tID&			GetID() const;
	virtual int32u				GetLockID() const;
	virtual bool				IsLocked() const;

	bool IsDead();
	void AddReference();
	void RemoveReference();

protected:
	// HYPOTHESIS: a member at 0x006fc200 that derived classes call on a string before writing
	// it between quotes into the SQL stream; it returns a new string. Not in the SDK header.
	std::string fn_006fc200( std::string text );

//Operations
	virtual void AttachToDB( const tID& id );
	virtual void RemoveFromDB();
	// HYPOTHESIS: the SDK's parameter type is tDatabase::iLockID, see SetLockID.
	virtual bool Lock( int32u lockId );
	virtual void UnLock();

//Fields
	mutable tOptex fAccess;

	tID fID;
	bool fAlive;
	int32u fReferenceCount;

	tThreadEvent*		fLockEvent;
	// HYPOTHESIS: tDatabase::iLockID in the SDK, see SetLockID.
	int32u				fLockID;
};

};
namespace db = nDatabase;

#endif
