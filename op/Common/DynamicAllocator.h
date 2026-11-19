//	DynamicAllocator.h
//
//	Classes that can be made by type at run time: every tClassAllocator<> registers itself with
//	the tDynamicAllocator singleton, which hands out allocator IDs once the list is locked.
//	Declarations follow the SDK's header; only what this unit's functions need is declared yet.
//
#pragma once

#if !defined( DynamicAllocator_h )
#define DynamicAllocator_h

#include <typeinfo>
#include <vector>

#include "port/types.h"
#include "Common/CleanupGlobalPointer.h"

typedef int32 iAllocatorID;
const int8 kInvalidAllocatorID = -1;

// VTABLE: STARFLEETOP 0x007bb2f8
class tAllocatedClass
{
public:
	tAllocatedClass();
	virtual ~tAllocatedClass();

	void mSetAllocatorID( iAllocatorID allocID );
	virtual iAllocatorID mGetAllocatorID() const;

	virtual int32 operator==( const type_info& type );

protected:
	iAllocatorID fAllocID;
};

// VTABLE: STARFLEETOP 0x007bb2e8
class tAllocator
{
public:
	virtual tAllocatedClass* mAllocate() = 0;

protected:
	virtual ~tAllocator();

	friend class tDynamicAllocator;
	virtual void mAssignAllocID( iAllocatorID allocID ) = 0;
};

class tDynamicAllocator
{
public:
	static tDynamicAllocator* Instance();
	void LockDownAllocator();

	void mAddToAllocList( const type_info& type, tAllocator* alloc );

	tAllocatedClass* mAllocateClass( iAllocatorID typeAllocID );

	iAllocatorID mGetAllocatorID( const type_info& type );
	iAllocatorID mGetAllocatorID( const char* raw_name );

private:
	class tAllocatorInfo
	{
	public:
		int32	operator<( const tAllocatorInfo& allocInfo ) const;
		int32	operator==( const type_info& type ) const;
		int32	operator==( const char* raw_name ) const;

		const type_info* fTypeID;
		tAllocator* fAllocator;
	};

	typedef std::vector< tAllocatorInfo > tAllocatorList;

	tDynamicAllocator();
	~tDynamicAllocator();

	friend class tCleanupGlobalPointer< tDynamicAllocator >;
	static tCleanupGlobalPointer< tDynamicAllocator > fCleanupSingletion;
	static tDynamicAllocator* fpDynamicAllocator;

	bool fAllocatorListLocked;
	tAllocatorList* fAllocatorList;
};

template< class tClass >
class tClassAllocator : public tAllocator
{
public:
	tClassAllocator();

	// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no source line,
	// so it is written out as a member: destroy, then delete when bit 0 of the flag is set.
	// The class declares no destructor of its own (as in the SDK); the implicit one is called.
	tClassAllocator* scalar_deleting_destructor( unsigned char flags );

	virtual tAllocatedClass* mAllocate();
	static iAllocatorID mGetAllocatorID();

protected:
	virtual void mAssignAllocID( iAllocatorID allocID );

	static iAllocatorID fAllocID;
};

#endif
