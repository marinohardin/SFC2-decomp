//	Database.h: nDatabase::tDatabase, the in-memory database of tDatabaseObjects.
//
//	Declarations follow the SDK's header; only what the rebuilt functions need is declared yet.

#if !defined( Database_h )
#define Database_h

#pragma once

#include <string>

#include "Common/BaseTypes.h"
#include "Common/DynamicAllocator.h"
#include "Common/Thread/Optex.h"

namespace nDatabase
{

class tDatabaseObject;
class tDatabaseObjectUpdate;

// HYPOTHESIS: minimal declarations; the constructors are those called at 0x007036b2 and
// 0x0070b30e.
class tDatabaseStorage
{
};

class tStreamDatabaseStorage : public tDatabaseStorage
{
public:
	tStreamDatabaseStorage( const std::string& name );

	// HYPOTHESIS: members unknown. The original allocates 0x40 bytes for this class (New and
	// Load); padding it to that size made objdiff time out on New, so the size is not
	// modelled yet.
};

// HYPOTHESIS: the class has a vtable of its own, with the destructor at slot 0 (symbol at
// 0x00703cb0); the other slots follow the stand-ins that call them: RemoveObjectFromDB is
// slot 2, UnLock( tDatabaseObject* ) slot 6 and UpdateObject slot 7. Slots 1, 3 to 5, 8 and 9
// are not named yet; slot 8 returns a long (read at 0x005715f0). The members are those the
// constructor (0x00703940) and the rebuilt member templates read: the optex at +0x4 and the
// storage at +0x40; the rest is padding up to the size New allocates (0x68).
class tDatabase
{
public:
	tDatabase( tDatabaseStorage* storage );
	virtual ~tDatabase();

	virtual void vfunction1();
	virtual void RemoveObjectFromDB( tDatabaseObject* pObject );
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void UnLock( tDatabaseObject* pObject );
	virtual void UpdateObject( const tDatabaseObjectUpdate& update, bool unlock = true );
	virtual long vfunction8( long param_1 );
	virtual void vfunction9();

protected:
	void AddObjectToDBIndex( tDatabaseObject* pActualObject, iAllocatorID allocID );

	tOptex					fAccess;
	char					fPad_after_access[0x40 - 4 - sizeof( tOptex )];
	tDatabaseStorage*		fDatabaseStorage;
	char					fPad_after_storage[0x68 - 0x44];
};

typedef char tDatabase_size_is_0x68[sizeof( tDatabase ) == 0x68 ? 1 : -1];

}

#endif
