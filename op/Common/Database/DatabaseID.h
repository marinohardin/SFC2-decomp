//	DatabaseID.h
//
//	The identifier of an object in the database.
//	Declarations follow the SDK's header; only what the rebuilt functions need is declared yet.
//
#pragma once

#if !defined( DatabaseID_h )
#define DatabaseID_h

#include "port/types.h"
#include "Common/DataStore/Streamable.h"

namespace nDatabase
{

// CLASS PURPOSE: An identifier for an object in a database
class tID : public nDataStore::tStreamable
{
public:
	// Setup
	tID();
	tID( int32u objectID );
	virtual ~tID();

	// Operations
	bool IsValid() const;

	int32u	GetObjectID() const;

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	bool operator==( const tID& compare ) const;
	bool operator!=( const tID& compare ) const;
	bool operator<( const tID& compare ) const;

protected:
	// Fields
	int32u fObjectID;
};

};
namespace db = nDatabase;

#endif
