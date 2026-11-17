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
	// HYPOTHESIS: 0x005e361d hands it its area and the object at +0x17c.
	void fn_005e7539( const tRect *area, fn_005eadc8_class *object );
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
	// HYPOTHESIS: a list of view labels; 0x005e3ad8 clears it (erase 0x0042500f over its +0x4 and
	// +0x8) and pushes "Empires View" and "Cartels View" (push_back 0x00424f16).
	std::vector<tString> *fn_0055e60f();
	void fn_0060a672();
};

fn_0060a54d_class *fn_00607d61();
// HYPOTHESIS: makes the drawing colour from red, green and blue (0x005e374b passes the bytes of
// a 0x00rrggbb value unmasked).
int fn_00607906( int red, int green, int blue );

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

// FUNCTION: STARFLEETOP 0x005e361d
void tMetaMapControl::fn_005e361d( const tRect *area )
{
	if ( field_17c != 0 )
	{
		fn_00607d61()->fn_0060a54d()->fn_005e7539( area, field_17c );
		if ( field_1c4 != 2 )
		{
			c_tScrollBar_FieldA8 *image = field_17c->fn_005eaddc();
			tPoint point = field_17c->fn_005eae6e();
			if ( image != 0 )
			{
				field_14->fn_00675280( image, point.x, point.y, area, 0, 1 );
			}
		}
		if ( !field_17c->fn_005eae11() )
		{
			fn_005e36a3();
		}
	}
}

// FUNCTION: STARFLEETOP 0x005e36a3
void tMetaMapControl::fn_005e36a3()
{
	if ( field_17c != 0 )
	{
		delete field_17c;
		field_17c = 0;
		fn_0045f4c2();
		field_1e8 = true;
	}
}

// FUNCTION: STARFLEETOP 0x005e36cd
void tMetaMapControl::fn_005e36cd( fn_005e36cd_entry *entry, int param_2 )
{
	if ( field_1a4.size() != 0 )
	{
		fn_005e374b( entry, param_2, field_1a4[ entry->field_14 ], -1, -1, -1 );
	}
}

// FUNCTION: STARFLEETOP 0x005e370c
void tMetaMapControl::fn_005e370c( fn_005e36cd_entry *entry, int param_2 )
{
	if ( field_1b4.size() != 0 )
	{
		fn_005e374b( entry, param_2, field_1b4[ entry->field_14 ], -1, -1, -1 );
	}
}

// FUNCTION: STARFLEETOP 0x005e374b
void tMetaMapControl::fn_005e374b( fn_005e36cd_entry *entry, int param_2, c_tScrollBar_FieldA8 *image, int x, int y, unsigned long color )
{
	if ( image != 0 )
	{
		if ( x == -1 && y == -1 )
		{
			x = entry->field_2c;
			y = entry->field_2e;
		}
		if ( color == 0xffffffff )
		{
			color = fn_005e3935( entry );
		}
		fn_005e3a37( entry, &color );
		// HYPOTHESIS: param_2 (the value 0x005e32d7 and its siblings pass down) is the draw area.
		field_14->fn_00675280( image, x, y, (const tRect *)param_2, fn_00607906( color >> 16, color >> 8, color ), 1 );
	}
}

// FUNCTION: STARFLEETOP 0x005e37c4
void tMetaMapControl::fn_005e37c4( fn_005e36cd_entry *entry, int param_2, int param_3 )
{
	fn_005e37d8( entry, param_2, param_3 );
}

// FUNCTION: STARFLEETOP 0x005e37d8
void tMetaMapControl::fn_005e37d8( fn_005e36cd_entry *entry, int param_2, int param_3 )
{
	if ( field_1a4.size() != 0 )
	{
		fn_005e374b( entry, param_2, field_1a4[ entry->field_14 ], entry->field_2c, entry->field_2e, param_3 );
	}
}

// FUNCTION: STARFLEETOP 0x005e381f
void tMetaMapControl::fn_005e381f( fn_005e381f_entry *entry, int param_2 )
{
	if ( field_184.size() != 0 )
	{
		const tPoint &point = entry->fn_005e488e();
		fn_005e374b( entry, param_2, field_184[ 24 ], point.x, point.y, -1 );
		fn_005e374b( entry, param_2, field_184[ entry->field_14 ], entry->field_2c, entry->field_2e, 0 );
	}
}

// FUNCTION: STARFLEETOP 0x005e3891
void tMetaMapControl::fn_005e3891( fn_005e381f_entry *entry, int param_2 )
{
	if ( field_194.size() != 0 )
	{
		const tPoint &point = entry->fn_005e488e();
		fn_005e374b( entry, param_2, field_194[ 24 ], point.x, point.y, -1 );
		fn_005e374b( entry, param_2, field_194[ entry->field_14 ], entry->field_2c, entry->field_2e, 0 );
	}
}

// FUNCTION: STARFLEETOP 0x005e3903
void tMetaMapControl::fn_005e3903( fn_005e381f_entry *entry, int param_2 )
{
	if ( field_1b4.size() != 0 )
	{
		fn_005e374b( entry, param_2, field_1b4[ 0 ], -1, -1, -1 );
	}
}

// HYPOTHESIS: a table of colours 0x005e3935 indexes when the entry's +0x40 flag is clear.
extern unsigned long g_00c39058[];

// FUNCTION: STARFLEETOP 0x005e3935
unsigned long tMetaMapControl::fn_005e3935( fn_005e36cd_entry *entry )
{
	int index = entry->vfunction0( field_1c8 );
	unsigned long color;
	if ( entry->field_40 )
		color = fn_005e397b( fn_005e396d( index ) );
	else
		color = g_00c39058[ index ];
	return color;
}

// HYPOTHESIS: a table of three-byte colours (red, green, blue) 0x005e396d indexes.
extern unsigned char g_007b5038[][ 3 ];

// FUNCTION: STARFLEETOP 0x005e396d
unsigned char *tMetaMapControl::fn_005e396d( int param_1 )
{
	return g_007b5038[ param_1 ];
}

// HYPOTHESIS: a brightness scale 0x005e397b applies to each colour channel.
extern float g_007b5098;

// FUNCTION: STARFLEETOP 0x005e397b
unsigned long tMetaMapControl::fn_005e397b( unsigned char *param_1 )
{
	unsigned long r = param_1[ 0 ];
	unsigned long g = param_1[ 1 ];
	unsigned long b = param_1[ 2 ];
	if ( r > 230 || g > 230 || b > 230 )
	{
		r += 64;
		g += 64;
		b += 64;
	}
	if ( r < 5 && g < 5 && b < 5 )
	{
		r = g = b = 128;
	}
	unsigned long red = (unsigned long)( r * g_007b5098 );
	unsigned long rr = red > 255 ? 255 : red;
	unsigned long green = (unsigned long)( g * g_007b5098 );
	unsigned long gg = green > 255 ? 255 : green;
	unsigned long blue = (unsigned long)( b * g_007b5098 );
	unsigned long bb = blue > 255 ? 255 : blue;
	return (unsigned char)rr | ( (unsigned short)(unsigned char)gg << 8 ) | ( (unsigned long)(unsigned char)bb << 16 );
}

// HYPOTHESIS: the scale 0x005e3a37 applies to each channel when the entry's +0x44 is 2.
extern float g_007b509c;

// FUNCTION: STARFLEETOP 0x005e3a37
void tMetaMapControl::fn_005e3a37( fn_005e36cd_entry *entry, unsigned long *color )
{
	if ( color )
	{
		if ( *color == 0 )
			*color = 0xffffff;
		unsigned long c = *color;
		unsigned long b = ( c >> 16 ) & 0xff;
		unsigned long g = ( c >> 8 ) & 0xff;
		unsigned long r = c & 0xff;
		if ( entry->field_44 == 2 )
		{
			b = (unsigned long)( b * g_007b509c );
			g = (unsigned long)( g * g_007b509c );
			r = (unsigned long)( r * g_007b509c );
		}
		*color = (unsigned char)b | ( (unsigned short)(unsigned char)g << 8 ) | ( (unsigned long)(unsigned char)r << 16 );
	}
}

// FUNCTION: STARFLEETOP 0x005e3ad8
void tMetaMapControl::fn_005e3ad8()
{
	std::vector<tString> *list = fn_00607d61()->fn_0055e60f();
	list->clear();
	list->push_back( "Empires View" );
	list->push_back( "Cartels View" );
	fn_005e4455();
}

// FUNCTION: STARFLEETOP 0x005e3b6e
void tMetaMapControl::fn_005e3b6e()
{
	fn_00607d61()->fn_0060a672();
	fn_005e4455();
}

// FUNCTION: STARFLEETOP 0x005e3b86
void tMetaMapControl::fn_005e3b86()
{
	fn_005e3ba5();
	fn_005e3d73();
	fn_005e3e48();
	fn_005e3f1d();
}

// Defined in unsorted/fn_005E2501.cpp.
extern std::string g_00c39048;
extern std::string g_00c39038;
extern std::string g_00c39028;
extern std::string g_00c39018;
extern std::string g_00c39008;
extern std::string g_00c38ff8;
extern std::string g_00c38fe8;
extern std::string g_00c38fd8;
extern std::string g_00c38fc8;
extern std::string g_00c38fb8;

// FUNCTION: STARFLEETOP 0x005e3ba5
void tMetaMapControl::fn_005e3ba5()
{
	field_1a4.assign( 0x18, 0 );
	field_1a4[ 1 ] = fn_005e3f82( g_00c39038, 0 );
	field_1a4[ 2 ] = fn_005e3f82( g_00c39028, 0 );
	field_1a4[ 0 ] = fn_005e3f82( g_00c39018, 0 );
	field_1a4[ 6 ] = fn_005e3f82( g_00c39048, 0 );
	field_1a4[ 7 ] = fn_005e3f82( g_00c39048, 1 );
	field_1a4[ 8 ] = fn_005e3f82( g_00c39048, 2 );
	field_1a4[ 9 ] = fn_005e3f82( g_00c39048, 3 );
	field_1a4[ 10 ] = fn_005e3f82( g_00c39048, 4 );
	field_1a4[ 11 ] = fn_005e3f82( g_00c39048, 5 );
	field_1a4[ 12 ] = fn_005e3f82( g_00c39048, 6 );
	field_1a4[ 13 ] = fn_005e3f82( g_00c39048, 7 );
	field_1a4[ 14 ] = fn_005e3f82( g_00c39048, 8 );
	field_1a4[ 15 ] = fn_005e3f82( g_00c39008, 0 );
	field_1a4[ 16 ] = fn_005e3f82( g_00c38ff8, 0 );
	field_1a4[ 17 ] = fn_005e3f82( g_00c38fe8, 0 );
	field_1a4[ 3 ] = fn_005e3f82( g_00c38fd8, 0 );
	field_1a4[ 4 ] = fn_005e3f82( g_00c38fc8, 0 );
	field_1a4[ 5 ] = fn_005e3f82( g_00c38fb8, 0 );
}
