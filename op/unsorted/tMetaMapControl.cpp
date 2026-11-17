// Unit:      unsorted/tMetaMapControl.cpp
// Evidence:  class (RTTI; original file unknown)

#include "unsorted/tMetaMapControl.h"
#include "unsorted/tCompassRose.h"
#include "unsorted/tApplication.h"

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

// HYPOTHESIS: the object returned by fn_00607d61 (tPanelMgr.cpp declares the same getter for its
// own use); fn_0060a54d on it returns the object whose fn_005e5e24 takes the new value.
class fn_005e5e24_class
{
public:
	void fn_005e5e24( int value );
};

class fn_0060a54d_class
{
public:
	fn_005e5e24_class *fn_0060a54d();
};

fn_0060a54d_class *fn_00607d61();

// FUNCTION: STARFLEETOP 0x005e3148
void tMetaMapControl::fn_005e3148( int param_1 )
{
	field_1c4 = param_1;
	fn_00607d61()->fn_0060a54d()->fn_005e5e24( param_1 );
	fn_005e3b86();
	field_180->fn_005e2501();
	if ( g_p008cc580->vfunction5( "Meta", "EmpireMapView", 0 ) == 1 )
		fn_005e326c( false );
	else
		fn_005e326c( true );
	fn_0045f4c2();
	if ( field_17c != 0 )
	{
		field_17c->fn_005eadc8();
	}
}
