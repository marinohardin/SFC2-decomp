//	DatabaseObjectUpdate.h
//
//	A recorded change to a database object.
//	The members are the SDK library's symbols; only what the rebuilt functions need is declared yet.
//
#if !defined( DatabaseObjectUpdate_h )
#define DatabaseObjectUpdate_h

#pragma once

#include "Common/DataStore/Streamable.h"
#include "Common/Database/DatabaseID.h"

namespace nDatabase
{

class tDatabaseObject;

// HYPOTHESIS: the base is a guess from the virtual StreamOut and StreamIn.
class tDatabaseObjectUpdate : public nDataStore::tStreamable
{
public:
//Setup
	tDatabaseObjectUpdate();
	tDatabaseObjectUpdate( const tDatabaseObject& object );
	virtual ~tDatabaseObjectUpdate();

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

//Operations
	virtual void RecordUpdate( const tDatabaseObject& object );
	virtual void CommitUpdate( tDatabaseObject* pObject ) const;
	virtual void CommitCopy( tDatabaseObject* pObject ) const;

//Accessors
	const tID& GetUpdateID() const;

protected:
//Fields
	// HYPOTHESIS: the object is 0x38 bytes (0x0056dfad holds one in a 0x38-byte frame slot);
	// its fields are not known.
	char fPad_0c[0x38 - 0x0c];
};

};

#endif
