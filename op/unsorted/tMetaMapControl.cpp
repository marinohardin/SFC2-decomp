// Unit:      unsorted/tMetaMapControl.cpp
// Evidence:  class (RTTI; original file unknown)

#include "unsorted/tMetaMapControl.h"
#include "unsorted/tCompassRose.h"

// FUNCTION: STARFLEETOP 0x005e2f95
tMetaMapControl::tMetaMapControl()
	: field_17c( 0 ), field_1c4( 1 )
{
	tObjBase::vfunction1( 1 );
	field_180 = new tCompassRose( this );
	field_1d4 = 0;
	field_1d8 = 0;
	field_1e4 = 0;
	field_1e8 = false;
}

// FUNCTION: STARFLEETOP 0x005e3090
// FUNCTION: STARFLEETOP 0x005e30ac
tMetaMapControl::~tMetaMapControl()
{
	fn_005e36a3();
	delete field_180;
	field_180 = 0;
}
