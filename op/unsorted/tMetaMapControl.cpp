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
	void fn_005e6aeb( int param_1, std::vector<fn_005e381f_entry> &list );
	void fn_005e6dea( int param_1, std::vector<fn_005e36cd_entry> &list );
};

// HYPOTHESIS: the object fn_005e488e returns; 0x005e326c hands it the map view state.
class fn_004cf594_class
{
public:
	void fn_004cf594( int param_1 );
	void fn_004c2288();
};

class fn_0060a54d_class
{
public:
	fn_005e5e24_class *fn_0060a54d();
	fn_004cf594_class *fn_005e488e();
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

// FUNCTION: STARFLEETOP 0x005e31be
void tMetaMapControl::fn_005e31be()
{
	field_180->fn_005e2539();
	if ( field_17c != 0 )
	{
		field_17c->fn_005eadcd();
	}
}

// FUNCTION: STARFLEETOP 0x005e31dd
void tMetaMapControl::fn_005e31dd( int param_1 )
{
	int state = field_1c4;
	if ( state == 2 )
	{
		if ( param_1 == 1 )
		{
			field_1c4 = 1;
			if ( field_17c != 0 )
			{
				field_17c->fn_005eae34();
			}
		}
	}
	else if ( state == 1 )
	{
		if ( param_1 == -1 )
		{
			field_1c4 = 2;
			if ( field_17c != 0 )
			{
				field_17c->fn_005eae4a();
			}
		}
		if ( param_1 == 1 )
		{
			field_1c4 = 0;
		}
	}
	else if ( state == 0 )
	{
		if ( param_1 == -1 )
		{
			field_1c4 = 1;
		}
	}
	fn_00607d61()->fn_0060a54d()->fn_005e5e24( field_1c4 );
}

// FUNCTION: STARFLEETOP 0x005e326c
void tMetaMapControl::fn_005e326c( int param_1 )
{
	field_1c8 = param_1;
	fn_004cf594_class *view = fn_00607d61()->fn_005e488e();
	if ( field_1c8 == 0 )
	{
		g_p008cc580->vfunction6( "Meta", "EmpireMapView", 1 );
		view->fn_004cf594( 0 );
	}
	else
	{
		g_p008cc580->vfunction6( "Meta", "EmpireMapView", 0 );
		view->fn_004cf594( 1 );
	}
	view->fn_004c2288();
	fn_0045f4c2();
}

// FUNCTION: STARFLEETOP 0x005e32d7
void tMetaMapControl::fn_005e32d7( int param_1 )
{
	std::vector<fn_005e381f_entry> list1;
	fn_00607d61()->fn_0060a54d()->fn_005e6aeb( param_1, list1 );
	std::vector<fn_005e381f_entry>::iterator it1;
	for ( it1 = list1.begin(); it1 != list1.end(); it1++ )
	{
		fn_005e381f( it1, param_1 );
	}

	std::vector<fn_005e36cd_entry> list2;
	fn_00607d61()->fn_0060a54d()->fn_005e6dea( param_1, list2 );
	std::vector<fn_005e36cd_entry>::iterator it2;
	for ( it2 = list2.begin(); it2 != list2.end(); it2++ )
	{
		if ( !( it2->field_4c & 0x1 ) )
			fn_005e36cd( it2, param_1 );
	}
	for ( it2 = list2.begin(); it2 != list2.end(); it2++ )
	{
		if ( it2->field_4c & 0x1 )
			fn_005e36cd( it2, param_1 );
	}
	for ( it2 = list2.begin(); it2 != list2.end(); it2++ )
	{
		if ( ( it2->field_4c & 0x80 ) || ( it2->field_4c & 0x300 ) )
			fn_005e37c4( it2, param_1, it2->field_50 );
	}
}

// FUNCTION: STARFLEETOP 0x005e33f4
void tMetaMapControl::fn_005e33f4( int param_1 )
{
	std::vector<fn_005e381f_entry> list1;
	fn_00607d61()->fn_0060a54d()->fn_005e6aeb( param_1, list1 );
	std::vector<fn_005e381f_entry>::iterator it1;
	for ( it1 = list1.begin(); it1 != list1.end(); it1++ )
	{
		fn_005e3891( it1, param_1 );
	}

	std::vector<fn_005e36cd_entry> list2;
	fn_00607d61()->fn_0060a54d()->fn_005e6dea( param_1, list2 );
	std::vector<fn_005e36cd_entry>::iterator it2;
	for ( it2 = list2.begin(); it2 != list2.end(); it2++ )
	{
		if ( !( it2->field_4c & 0x1 ) )
			fn_005e36cd( it2, param_1 );
	}
	for ( it2 = list2.begin(); it2 != list2.end(); it2++ )
	{
		if ( it2->field_4c & 0x1 )
			fn_005e36cd( it2, param_1 );
	}
	for ( it2 = list2.begin(); it2 != list2.end(); it2++ )
	{
		if ( ( it2->field_4c & 0x80 ) || ( it2->field_4c & 0x300 ) )
			fn_005e37d8( it2, param_1, it2->field_50 );
	}
}

// FUNCTION: STARFLEETOP 0x005e3511
void tMetaMapControl::fn_005e3511( int param_1 )
{
	std::vector<fn_005e381f_entry> list1;
	fn_00607d61()->fn_0060a54d()->fn_005e6aeb( param_1, list1 );
	std::vector<fn_005e381f_entry>::iterator it1;
	for ( it1 = list1.begin(); it1 != list1.end(); it1++ )
	{
		fn_005e3903( it1, param_1 );
	}

	std::vector<fn_005e36cd_entry> list2;
	fn_00607d61()->fn_0060a54d()->fn_005e6dea( param_1, list2 );
	std::vector<fn_005e36cd_entry>::iterator it2;
	for ( it2 = list2.begin(); it2 != list2.end(); it2++ )
	{
		if ( !( it2->field_4c & 0x1 ) )
			fn_005e370c( it2, param_1 );
	}
	for ( it2 = list2.begin(); it2 != list2.end(); it2++ )
	{
		if ( it2->field_4c & 0x1 )
			fn_005e36cd( it2, param_1 );
	}
	for ( it2 = list2.begin(); it2 != list2.end(); it2++ )
	{
		if ( it2->field_14 == 5 )
			fn_005e37d8( it2, param_1, it2->field_50 );
	}
}
