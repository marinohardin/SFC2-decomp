//	DynamicAllocator.cpp
//
//	Three of the unit's functions so far, claimed for the guest-mode spike (DESIGN §12.4). The rest
//	of the unit (0x006fc490 to 0x006fcb40, game/UNITS.tsv) is still lifted.
//

#include "Common/DynamicAllocator.h"

#include <string.h>

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
