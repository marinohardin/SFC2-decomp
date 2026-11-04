//	DynamicAllocator.h
//
//	Classes that can be made by type at run time: every tClassAllocator<> registers itself with
//	the tDynamicAllocator singleton, which hands out allocator IDs once the list is locked.
//	Declarations follow the SDK's header; only what this unit's functions need is declared yet.
//
#pragma once

#if !defined( DynamicAllocator_h )
#define DynamicAllocator_h

// Host headers first: in guest mode port/types.h defines macros a host header must not see.
#include <typeinfo>
#include <vector>

#include "port/types.h"

typedef int32 iAllocatorID;
const int8 kInvalidAllocatorID = -1;

// VTABLE: STARFLEETOP 0x007bb2f8
class tAllocatedClass
{
	SFCOP_VFPTR // PORT: pointer size: the guest object's vtable pointer is 32 bits
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
	SFCOP_VFPTR // PORT: pointer size: the guest object's vtable pointer is 32 bits
public:
	virtual tAllocatedClass* mAllocate() SFCOP_PURE; // PORT: guest mode: the member is not virtual there, so it cannot be pure

protected:
	virtual ~tAllocator();

	friend class tDynamicAllocator;
	virtual void mAssignAllocID( iAllocatorID allocID ) SFCOP_PURE; // PORT: guest mode: the member is not virtual there, so it cannot be pure
};

class tDynamicAllocator
{
public:
	static tDynamicAllocator* Instance();
	void LockDownAllocator();

	iAllocatorID mGetAllocatorID( const type_info& type );
	iAllocatorID mGetAllocatorID( const char* raw_name );

private:
	class tAllocatorInfo
	{
	public:
		int32	operator<( const tAllocatorInfo& allocInfo ) const;
		int32	operator==( const type_info& type ) const;
		int32	operator==( const char* raw_name ) const;

		ptr32( const type_info ) fTypeID;
		ptr32( tAllocator ) fAllocator;
	};

	typedef std::vector< tAllocatorInfo > tAllocatorList;

	tDynamicAllocator();
	~tDynamicAllocator();

	bool fAllocatorListLocked;
	ptr32( tAllocatorList ) fAllocatorList;
};

#endif
