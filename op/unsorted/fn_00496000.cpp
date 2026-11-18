#include <stdlib.h>
#include "Tiger/tstring.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tReconnector.h"

// HYPOTHESIS: the array elements at +0x28 are tValueI<tString> (0x40 bytes; 0x0043d344 sits
// between that class's functions), declared only in Source/states/SupplyDockItem.cpp; this
// stands in for it until the class moves to a header. 0x0043d344 returns the value by copy.
// The set step (0x0049629b) assigns the tString at +0x18 and notifies with (3, 0), as
// tValueI<tString>::operator= does there.
class fn_0043d344_object : public tValueBase
{
public:
	tString fn_0043d344();
	fn_0043d344_object &operator=( const tString &value );

	tString m18;
};

inline fn_0043d344_object &fn_0043d344_object::operator=( const tString &value )
{
	m18 = value;
	fn_006712c0( 3, 0 );
	return *this;
}

// HYPOTHESIS: the record held at +0x1c0 of the owner of 0x00496253; flag byte at +0x19, six
// values at +0x1bc.
class fn_00496253_record
{
public:
	char m00[0x19];
	unsigned char m19;
	char m1a[0x1bc - 0x1a];
	long m1bc[6];
};

// HYPOTHESIS: the owner of 0x00496214 and 0x00496253 (0x00496253 calls 0x00496214 with this in
// ecx); its class is not known, possibly tEntityConnector, whose functions precede these
// (0x1c4 bytes, constructor 0x004960ba, virtual destructor 0x00496179).
class fn_00496253_object
{
public:
	virtual ~fn_00496253_object();
	tString fn_004961be( long val );
	int fn_00496214( tString s );
	void fn_00496253();
	void fn_0049629b( fn_00496253_record *record );
	fn_00496253_object();
	void fn_0049639c();
	// HYPOTHESIS: called with this in ecx, the value at +0x28 + 0x40 * i and one of the labels
	// "influence1".."influence6"; whether it uses this is not known.
	void fn_00482c00( fn_0043d344_object *value, const char *name );

	char m04[0x28 - 0x04];
	fn_0043d344_object m28[6];
	long m1a8[6];
	fn_00496253_record *m1c0;
};

// A member that ignores this: 0x0049629b calls it with this in ecx (`ret 8`).
// FUNCTION: STARFLEETOP 0x004961be
tString fn_00496253_object::fn_004961be(long val)
{
	tString s;
	return s.mAppendInt32s(val, 1);
}

// FUNCTION: STARFLEETOP 0x00496214
int fn_00496253_object::fn_00496214( tString s )
{
	return atoi(s.mCharStr());
}

// FUNCTION: STARFLEETOP 0x00496253
void fn_00496253_object::fn_00496253()
{
	if( m1c0->m19 & 4 )
	{
		long *value = m1c0->m1bc;
		fn_0043d344_object *item = m28;
		int count = 6;
		do
		{
			*value++ = fn_00496214( item++->fn_0043d344() );
		}
		while( --count );
	}
}

// FUNCTION: STARFLEETOP 0x0049629b
void fn_00496253_object::fn_0049629b( fn_00496253_record *record )
{
	m1c0 = record;
	if( record && ( record->m19 & 4 ) )
	{
		for( int i = 0; i < 6; i++ )
		{
			m1a8[i] = record->m1bc[i];
			m28[i] = fn_004961be( m1a8[i] );
		}
	}
}

extern char g_0084ebf8[];
void fn_004d3592( tReconnector *object, char *value );

// HYPOTHESIS: the class of 0x00496330, which allocates a 0x1c4-byte object (the size of
// fn_00496253_object) with the constructor 0x004960ba.
class fn_00496330_object
{
public:
	fn_00496330_object();
	~fn_00496330_object();
	void fn_00496462( fn_00496253_record *record );
	void fn_0049647e();

	fn_00496253_object *m00;
	// constructed by 0x0047f864 (tReconnector::tReconnector)
	tReconnector m04;
};

// FUNCTION: STARFLEETOP 0x00496330
fn_00496330_object::fn_00496330_object()
{
	fn_004d3592( &m04, g_0084ebf8 );
	m00 = new fn_00496253_object;
	m00->fn_0049639c();
}

// FUNCTION: STARFLEETOP 0x0049639c
void fn_00496253_object::fn_0049639c()
{
	fn_00482c00( &m28[0], "influence1" );
	fn_00482c00( &m28[1], "influence2" );
	fn_00482c00( &m28[2], "influence3" );
	fn_00482c00( &m28[3], "influence4" );
	fn_00482c00( &m28[4], "influence5" );
	fn_00482c00( &m28[5], "influence6" );
}

// FUNCTION: STARFLEETOP 0x0049640b
fn_00496330_object::~fn_00496330_object()
{
	if( m00 )
	{
		delete m00;
		m00 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00496462
void fn_00496330_object::fn_00496462( fn_00496253_record *record )
{
	m00->fn_0049629b( record );
	m04.fn_00671880( m00 );
}

// FUNCTION: STARFLEETOP 0x0049647e
void fn_00496330_object::fn_0049647e()
{
	m00->fn_00496253();
}

// HYPOTHESIS: a free function at 0x0069b0e0 returning a float on the FPU stack (a random value).
float fn_0069b0e0();

// HYPOTHESIS: a record initialised from two draws of fn_0069b0e0; 0x0049670b keeps the
// fractional part of m00 + m04 * (0x0092f61c - 0x0091a22c) in m08 (a phase).
class fn_004966e7_object
{
public:
	fn_004966e7_object();
	void fn_0049670b();

	float m00;
	float m04;
	float m08;
};

// FUNCTION: STARFLEETOP 0x004966e7
fn_004966e7_object::fn_004966e7_object()
{
	m00 = fn_0069b0e0();
	float r = fn_0069b0e0() * 0.25f;
	m04 = r + r - 0.25f;
}

extern float gObject_0092f61c;
extern float g_0091a22c;

// FUNCTION: STARFLEETOP 0x0049670b
void fn_004966e7_object::fn_0049670b()
{
	float phase = ( gObject_0092f61c - g_0091a22c ) * m04 + m00;
	float f = phase - (int)phase;
	m08 = f;
	if( f < 0.0f )
		m08 = f + 1.0f;
}

// HYPOTHESIS: a record with a byte at +0x00, a float at +0x04 and two draws of fn_0069b0e0;
// m08 is moved away from zero by 0.35.
class fn_0049674e_object
{
public:
	fn_0049674e_object();
	void fn_004967a5();

	unsigned char m00;
	float m04;
	float m08;
	float m0c;
};

// HYPOTHESIS: two float globals read at run time; their difference is about 0.025 (Ghidra's
// 0.025000006) and 0x007a08e8 holds 0.35, so 0x007a08e4 is likely 0.375 (a maximum and a minimum).
extern float g_007a08e4;
extern float g_007a08e8;

// FUNCTION: STARFLEETOP 0x0049674e
fn_0049674e_object::fn_0049674e_object()
{
	m00 = 0;
	m04 = 0.0f;
	float range = g_007a08e4 - g_007a08e8;
	m0c = fn_0069b0e0();
	float r = fn_0069b0e0() * range;
	m08 = r + r - range;
	if( m08 > 0.0f )
		m08 += g_007a08e8;
	else
		m08 -= g_007a08e8;
}

// HYPOTHESIS: 0x007a08ec holds 5.0 (a period in seconds); 0x0085366c is the previous frame's time.
extern float g_007a08ec;
extern float g_0085366c;

// FUNCTION: STARFLEETOP 0x004967a5
void fn_0049674e_object::fn_004967a5()
{
	if( gObject_0092f61c - m04 > g_007a08ec )
	{
		m04 = gObject_0092f61c - fn_0069b0e0() * g_007a08ec;
		m00 = !m00;
	}
	if( !m00 )
	{
		float range = g_007a08e4 - g_007a08e8;
		m0c += ( gObject_0092f61c - g_0085366c ) * m08;
		if( m0c < 0.0f )
		{
			m0c = 0.0f;
			m08 = fn_0069b0e0() * range;
			m08 += g_007a08e8;
		}
		else if( m0c > 1.0f )
		{
			m0c = 1.0f;
			m08 = -( fn_0069b0e0() * range );
			m08 -= g_007a08e8;
		}
	}
}

// HYPOTHESIS: the object held by the global at 0x008cc620; its third virtual loads a texture
// by file name and returns a handle.
class fn_008cc620_object
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual long vfunction2( const char *name, long a, long b, long c );
};

extern fn_008cc620_object *g_008cc620;
extern char g_009172dc;
// HYPOTHESIS: three targeting reticle texture handles.
extern long g_0084ec54[3];

// FUNCTION: STARFLEETOP 0x00496853
void fn_00496853()
{
	if( !g_009172dc )
	{
		g_0084ec54[0] = g_008cc620->vfunction2( "assets/textures/TargetingRedicles.bmp", 1, 0, 0 );
		g_0084ec54[1] = g_008cc620->vfunction2( "assets/textures/TargetingRedicles-noise1.bmp", 1, 0, 0 );
		g_0084ec54[2] = g_008cc620->vfunction2( "assets/textures/TargetingRedicles-noise2.bmp", 1, 0, 0 );
	}
	g_009172dc = 1;
}
