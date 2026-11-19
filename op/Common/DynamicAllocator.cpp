//	DynamicAllocator.cpp
//
//	Three of the unit's functions so far. The native build lifts this unit's VC6 object (DESIGN
//	§7.2, tools/objlift/); the rest of the unit (0x006fc490 to 0x006fcb40, game/UNITS.tsv) is
//	still the exe's lifted code.
//

#include "Common/DynamicAllocator.h"

#include <string.h>
// FUNCTION: STARFLEETOP 0x006fc490
tAllocatedClass::tAllocatedClass() : fAllocID( -1 ) {}
// FUNCTION: STARFLEETOP 0x006fc4a0
// FUNCTION: STARFLEETOP 0x006fc4c0
tAllocatedClass::~tAllocatedClass() {}

// FUNCTION: STARFLEETOP 0x006fc4d0
int32 tAllocatedClass::operator==( const type_info& type )
{
	int32 valid = ( typeid( *this ) == type );
	if( !valid )
	{
		valid = typeid( this ) == type;
	}

	return valid;
}

// FUNCTION: STARFLEETOP 0x006fc500
int32 tDynamicAllocator::tAllocatorInfo::operator<( const tAllocatorInfo& allocInfo ) const
{
	if( fTypeID )
	{
		if( allocInfo.fTypeID )
		{
			return strcmp( fTypeID->raw_name(), allocInfo.fTypeID->raw_name() ) <= 0;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

// FUNCTION: STARFLEETOP 0x006fc580
int32 tDynamicAllocator::tAllocatorInfo::operator==( const type_info& type ) const
{
	if( fTypeID )
	{
		return *fTypeID == type;
	}
	else
	{
		return false;
	}
}

// FUNCTION: STARFLEETOP 0x006fc5a0
int32 tDynamicAllocator::tAllocatorInfo::operator==( const char* raw_name ) const
{
	if( fTypeID )
	{
		if( strcmp( fTypeID->raw_name(), raw_name ) == 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

tDynamicAllocator* tDynamicAllocator::fpDynamicAllocator = 0;

// FUNCTION: STARFLEETOP 0x006fc640
// FUNCTION: STARFLEETOP 0x006fc630
// FUNCTION: STARFLEETOP 0x006fc610
tCleanupGlobalPointer< tDynamicAllocator > tDynamicAllocator::fCleanupSingletion( &tDynamicAllocator::fpDynamicAllocator );

// FUNCTION: STARFLEETOP 0x006fc750
void tDynamicAllocator::LockDownAllocator()
{
	if( fAllocatorListLocked )
	{
		return;
	}

	fAllocatorListLocked = true;

	for( int32u i = 0; i < fAllocatorList->size(); i++ )
	{
		( *fAllocatorList )[ i ].fAllocator->mAssignAllocID( i );
	}
}
