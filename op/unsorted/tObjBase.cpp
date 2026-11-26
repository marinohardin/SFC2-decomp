// tObjBase.cpp: the class declaration only; the functions are written one at a time.

#include "unsorted/tObjBase.h"

// FUNCTION: STARFLEETOP 0x0067cb70
tObjBase::tObjBase()
{
	field_5C = -1;
	field_0C = 0;
	field_08 = 0;
	field_04 = 0;
	field_10 = 0;
	field_14 = 0;
	field_20 = 0;
	field_2C = 0;
	field_28 = 0;
	field_24 = 0;
	field_30 = 0;
	field_3C.y = 0;
	field_3C.x = 0;
	field_34.mSet( 0, 0, 0, 0 );
	field_40 = field_40 & 0xffffffc1 | 1;
	field_44 = -1;
	field_48 = -1;
	field_4C = 0;
	field_54 = 0;
	field_58 = 0;
	field_50 = 0;
}

// FUNCTION: STARFLEETOP 0x0067cbf0
void tObjBase::vfunction32( const tRect *area )
{
	if ( field_0C )
		field_0C->vfunction32( area );
}
