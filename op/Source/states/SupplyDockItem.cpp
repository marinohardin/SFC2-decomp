#include "Common/Exceptions/Validate.h"
class fn_00486500_value;
// HYPOTHESIS: the 0x64-byte member at +0x10; only its constructor (0x0045e574) is known, and
// the constructor's unwind frame shows it has a destructor.
class tObject_0045e574
{
public:
	tObject_0045e574();
	~tObject_0045e574();
	void fn_004d269b( long arg1, void *a, bool b );
	void fn_004d2797( long arg1, void *a, long arg3 );
	void fn_004d26fb( fn_00486500_value *value, void *a );
	char m00[0x64];
};
// HYPOTHESIS: layout from the constructor's stores; 0x7c bytes per the RTTI recovery. The
// fifth argument's name comes from the original's validate text ("mask").
class tSupplyDockItem
{
public:
	tSupplyDockItem( long arg1, long arg2, long arg3, long mask, long arg5 );

	virtual long vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7( long *msg );
	virtual void vfunction8( void *a, bool b );
	virtual long vfunction9() = 0;
	virtual long vfunction10() = 0;
	virtual double vfunction11();
	virtual long vfunction12() = 0;

	long m04;
	long m08;
	long m0c;
	tObject_0045e574 m10;
	long m74;
	long m78;
};

// HYPOTHESIS: a free cdecl function of seven arguments; its unit is not written yet.
long fn_004d3278( long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7 );
// HYPOTHESIS: a free cdecl function of one argument; other units pass it a name string.
void fn_004d3306( long arg1 );
// HYPOTHESIS: vfunction9, 10 and 12 are pure; vtable 0x0079fc7c holds 0x0073ea74 in those slots.
// The analysis names this address ~tSupplyDockItem; the code is the constructor.
// FUNCTION: STARFLEETOP 0x0048612c
tSupplyDockItem::tSupplyDockItem( long arg1, long arg2, long arg3, long mask, long arg5 )
	: m04( arg1 ), m08( arg2 ), m0c( arg3 ), m74( mask ), m78( arg5 )
{
	// The original's validate sits on line 54 (push 0x36).
	validate( mask );
}

// FUNCTION: STARFLEETOP 0x004861aa
long tSupplyDockItem::vfunction1()
{
	long result = 0;
	if( m04 )
		result = fn_004d3278( m04, m04, 0, m08, m0c, 800, 600 );
	vfunction3();
	return result;
}

// FUNCTION: STARFLEETOP 0x004861e0
void tSupplyDockItem::vfunction2()
{
	if( m04 )
		fn_004d3306( m04 );
}

// FUNCTION: STARFLEETOP 0x004861ef
void tSupplyDockItem::vfunction6()
{
	vfunction12();
}

// HYPOTHESIS: m10 looks like the 0x64-byte class tPanelMgr holds at +0x100 (declared only in
// tPanelMgr.cpp), whose 0x004d269b there takes two pointers and a flag.
// FUNCTION: STARFLEETOP 0x004861f4
void tSupplyDockItem::vfunction8( void *a, bool b )
{
	m10.fn_004d269b( m04, a, b );
}

// FUNCTION: STARFLEETOP 0x0048620a
double tSupplyDockItem::vfunction11()
{
	return 0.0;
}

class fn_00447ce4_object;

// HYPOTHESIS: the object the base keeps at +0x74 ("mask"); only 0x004479b4 is called on it.
class fn_004479b4_object
{
public:
	long fn_004479b4( long arg1 );
	void fn_00447926( long arg1, long arg2 );
	long fn_00447a54( long arg1 );
	fn_00447ce4_object *fn_00447ce4();
};

// HYPOTHESIS: vtable 0x0079fcb8; one member past the base's 0x7c bytes.
class tSupplyDockShuttles : public tSupplyDockItem
{
public:
	tSupplyDockShuttles( long arg1, long arg2, long arg3, long mask, long arg5 );

	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction7( long *msg );
	virtual long vfunction9();
	virtual long vfunction10();
	virtual double vfunction11();
	virtual long vfunction12();
	virtual bool vfunction13( long delta );
	virtual void vfunction14( long value );

	long m7c;
};

// HYPOTHESIS: two globals passed by value; their types are not known.
extern void *g_0084bb4c;
extern void *g_0084bb50;

// FUNCTION: STARFLEETOP 0x00486211
tSupplyDockShuttles::tSupplyDockShuttles( long arg1, long arg2, long arg3, long mask, long arg5 )
	: tSupplyDockItem( arg1, arg2, arg3, mask, arg5 ), m7c( 0 )
{
	m10.fn_004d2797( 0x11014, g_0084bb4c, 0 );
	m10.fn_004d2797( 0x11015, g_0084bb50, 0 );
	// HYPOTHESIS: the base stores the "mask" argument as a long; here it is used as an object.
	m7c = ((fn_004479b4_object *) m74)->fn_004479b4( 0 );
}

#include "Tiger/tstring.h"

// HYPOTHESIS: a free cdecl function taking a string id and a tString by value; its unit is
// not written yet.
void fn_004d33d4( long id, tString text );

// FUNCTION: STARFLEETOP 0x00486296
void tSupplyDockShuttles::vfunction3()
{
	tString text;

	text = "";
	text.mAppendInt32s( vfunction10() );
	fn_004d33d4( 0x11010, text );
	text = "";
	text.mAppendInt32s( vfunction9() );
	fn_004d33d4( 0x11011, text );
	text = "";
	text.mAppendInt32s( vfunction11() );
	fn_004d33d4( 0x11012, text );
	text = "";
	text.mAppendInt32s( vfunction12() );
	fn_004d33d4( 0x11013, text );

	vfunction8( g_0084bb4c, vfunction10() > 0 );
	vfunction8( g_0084bb50, vfunction10() < vfunction9() );
}

// FUNCTION: STARFLEETOP 0x004863f5
void tSupplyDockShuttles::vfunction4()
{
	long value = m7c;
	if( value != ((fn_004479b4_object *) m74)->fn_004479b4( 0 ) )
	{
		((fn_004479b4_object *) m74)->fn_00447926( 0, value );
		vfunction3();
	}
}

// HYPOTHESIS: the argument points at a message whose first long is one of the ids the
// constructor registered on m10.
// FUNCTION: STARFLEETOP 0x0048641f
void tSupplyDockShuttles::vfunction7( long *msg )
{
	switch( *msg )
	{
	case 0x11014:
		vfunction13( -1 );
		break;
	case 0x11015:
		vfunction13( 1 );
		break;
	}
	vfunction6();
}

// FUNCTION: STARFLEETOP 0x0048644c
long tSupplyDockShuttles::vfunction9()
{
	return ((fn_004479b4_object *) m74)->fn_00447a54( 0 );
}

// FUNCTION: STARFLEETOP 0x00486457
long tSupplyDockShuttles::vfunction10()
{
	return ((fn_004479b4_object *) m74)->fn_004479b4( 0 );
}

// HYPOTHESIS: the object the base keeps at +0x78 (the constructor's fifth argument); only
// 0x0045139e is called on it, and it returns on the FPU stack.
class fn_0045139e_object
{
public:
	double fn_0045139e( long arg1 );
};

// FUNCTION: STARFLEETOP 0x00486462
double tSupplyDockShuttles::vfunction11()
{
	return ((fn_0045139e_object *) m78)->fn_0045139e( 3 );
}

// FUNCTION: STARFLEETOP 0x0048646d
long tSupplyDockShuttles::vfunction12()
{
	long result = ( vfunction10() - m7c ) * vfunction11();
	if( result < 0 )
		result = 0;
	return result;
}

// FUNCTION: STARFLEETOP 0x0048649e
bool tSupplyDockShuttles::vfunction13( long delta )
{
	long value = vfunction10() + delta;
	if( value > vfunction9() || value < 0 )
		return false;
	vfunction14( value );
	return true;
}

// FUNCTION: STARFLEETOP 0x004864cf
void tSupplyDockShuttles::vfunction14( long value )
{
	if( value != vfunction10() )
	{
		((fn_004479b4_object *) m74)->fn_00447926( 0, value );
		vfunction3();
	}
}

// HYPOTHESIS: the analysis types this global tValue<long>; its constructor and destructor are external.
class fn_00486500_value
{
public:
	fn_00486500_value();
	~fn_00486500_value();
	// HYPOTHESIS: 0x006712c0 is the function tAICaptain.cpp declares on its tObject_006712c0,
	// which also sets the integer at +0x18 first.
	void fn_006712c0( long arg1, long arg2 );
	long m00;
	char m04[0x14];
	long m18;
};

// FUNCTION: STARFLEETOP 0x0048650a
// FUNCTION: STARFLEETOP 0x00486500
fn_00486500_value g_009157c8;

// FUNCTION: STARFLEETOP 0x00486534
// FUNCTION: STARFLEETOP 0x0048652a
fn_00486500_value g_009157a8;

// FUNCTION: STARFLEETOP 0x0048655e
// FUNCTION: STARFLEETOP 0x00486554
fn_00486500_value g_00915788;

// HYPOTHESIS: RTTI names the class tStoresState::tMissileInfo; its constructor (0x004514fd)
// and 0x0045154e are used here, and the unwind frame shows a destructor. 0x0045154e is
// called both as m80.fn( m7c ) and m7c->fn( &m80 ), so 0x0055e60f returns one too; the
// constructor below sets its bytes at +0xc and +0xd to 1.
class tStoresState
{
public:
	class tMissileInfo
	{
	public:
		tMissileInfo();
		~tMissileInfo();
		void fn_0045154e( tMissileInfo *info );
		long fn_004517e1();
		void fn_0045189f( long count );
		long fn_00451a8b();
		double fn_00451aa6( long arg5 );
		double fn_00451aef( long arg5 );
		long fn_00451c03();
		long fn_00451c18();
		short fn_00451c2d();
		short fn_00451c45();
		void fn_00451d07( bool value );
		void fn_00451dcd( long value );

		char m00[0xc];
		bool m0c;
		bool m0d;
		char m0e;
		char m0f;
	};
};

// HYPOTHESIS: the object 0x00447ce4 returns when called on the base's +0x74 object.
class fn_00447ce4_object
{
public:
	tStoresState::tMissileInfo *fn_0055e60f();
};

// HYPOTHESIS: vtable 0x0079fcf4; vfunction9, 10 and 12 are pure in the base, so this class
// overrides them.
class tSupplyDockMissiles : public tSupplyDockItem
{
public:
	tSupplyDockMissiles( long arg1, long arg2, long arg3, long mask, long arg5 );

	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction7( long *msg );
	virtual long vfunction9();
	virtual long vfunction10();
	virtual double vfunction11();
	virtual long vfunction12();
	virtual bool vfunction13( long delta );
	virtual void vfunction14( long value );
	void fn_00486aa1( bool up );
	void fn_00486ad5( bool up );
	void fn_00486b8a( bool up );

	tStoresState::tMissileInfo *m7c;
	tStoresState::tMissileInfo m80;
};

// HYPOTHESIS: globals passed by value, like g_0084bb4c and g_0084bb50; their types are not known.
extern void *g_0084bb54;
extern void *g_0084bb58;
extern void *g_0084bb5c;
extern void *g_0084bb60;
extern void *g_0084bb64;
extern void *g_0084bb68;
extern void *g_0084bb6c;
extern void *g_0084bb70;
extern void *g_0084bb74;
extern void *g_0084bb78;
extern void *g_0084bb7c;

// FUNCTION: STARFLEETOP 0x00486574
tSupplyDockMissiles::tSupplyDockMissiles( long arg1, long arg2, long arg3, long mask, long arg5 )
	: tSupplyDockItem( arg1, arg2, arg3, mask, arg5 )
{
	m10.fn_004d2797( 0x1103b, g_0084bb54, 0 );
	m10.fn_004d2797( 0x1103c, g_0084bb58, 0 );
	m10.fn_004d2797( 0x1103d, g_0084bb5c, 0 );
	m10.fn_004d2797( 0x1103e, g_0084bb60, 0 );
	m10.fn_004d2797( 0x1103f, g_0084bb64, 0 );
	m10.fn_004d2797( 0x11040, g_0084bb68, 0 );
	m10.fn_004d2797( 0x11041, g_0084bb6c, 0 );
	m10.fn_004d2797( 0x11042, g_0084bb70, 0 );
	m10.fn_004d26fb( &g_009157c8, g_0084bb74 );
	m10.fn_004d26fb( &g_009157a8, g_0084bb78 );
	m10.fn_004d26fb( &g_00915788, g_0084bb7c );
	m7c = ((fn_004479b4_object *) m74)->fn_00447ce4()->fn_0055e60f();
	m7c->m0c = true;
	m7c->m0d = true;
	m80.fn_0045154e( m7c );
}

#include "unsorted/tObject_0090b730.h"

// HYPOTHESIS: a free cdecl function of a name and the base's +0x04; its unit is not written yet.
void fn_004d346f( const char *name, long arg2 );
// HYPOTHESIS: two names at fixed data addresses; their text is not known here.
extern const char g_0084bff4[];
extern const char g_0084bfe4[];

// FUNCTION: STARFLEETOP 0x004866c4
void tSupplyDockMissiles::vfunction3()
{
	tString text;

	text = "";
	text.mAppendInt32s( vfunction10() );
	fn_004d33d4( 0x11037, text );
	text = "";
	text.mAppendInt32s( vfunction9() );
	fn_004d33d4( 0x11038, text );
	text = "";
	text.mAppendInt32s( vfunction11() );
	fn_004d33d4( 0x11039, text );
	text = "";
	text.mAppendInt32s( vfunction12() );
	fn_004d33d4( 0x1103a, text );

	vfunction8( g_0084bb54, vfunction10() > 0 );
	vfunction8( g_0084bb58, vfunction10() < vfunction9() );
	vfunction8( g_0084bb5c, m7c->fn_00451c03() != 0 );
	vfunction8( g_0084bb60, m7c->fn_00451c03() != 1 );
	vfunction8( g_0084bb64, m7c->fn_00451c18() > 0 );

	long state = gObject_0090b730.fn_0060ca4f();
	if( state != 5 && state != 4 )
		vfunction8( g_0084bb68, m7c->fn_00451c18() < gObject_0090b730.fn_0060ca9e() );
	else
	{
		long level = gObject_0090b730.fn_005c7fb3();
		if( level < 4 )
			vfunction8( g_0084bb68, m7c->fn_00451c18() < 0 );
		else if( level < 0x11 )
			vfunction8( g_0084bb68, m7c->fn_00451c18() < 1 );
		else
			vfunction8( g_0084bb68, m7c->fn_00451c18() < 2 );
	}

	vfunction8( g_0084bb6c, m7c->fn_00451c2d() > 1 );
	vfunction8( g_0084bb70, m7c->fn_00451c2d() < 4 );

	g_009157c8.m18 = m7c->fn_00451c03();
	g_009157c8.fn_006712c0( 3, 0 );
	g_009157a8.m18 = m7c->fn_00451c18();
	g_009157a8.fn_006712c0( 3, 0 );
	g_00915788.m18 = m7c->fn_00451c2d() - 1;
	g_00915788.fn_006712c0( 3, 0 );

	fn_004d346f( g_0084bff4, m04 );
	fn_004d346f( g_0084bfe4, m04 );
}

// FUNCTION: STARFLEETOP 0x004869a1
void tSupplyDockMissiles::vfunction4()
{
	m7c->fn_0045154e( &m80 );
	vfunction3();
}

// HYPOTHESIS: the three helpers take a direction flag; the ids are the button pairs the
// constructor registered on m10.
// FUNCTION: STARFLEETOP 0x004869bc
void tSupplyDockMissiles::vfunction7( long *msg )
{
	switch( *msg )
	{
	case 0x1103d:
		fn_00486aa1( false );
		break;
	case 0x1103e:
		fn_00486aa1( true );
		break;
	case 0x1103f:
		fn_00486ad5( false );
		break;
	case 0x11040:
		fn_00486ad5( true );
		break;
	case 0x11041:
		fn_00486b8a( false );
		break;
	case 0x11042:
		fn_00486b8a( true );
		break;
	}
	vfunction6();
}

// FUNCTION: STARFLEETOP 0x00486a11
long tSupplyDockMissiles::vfunction9()
{
	return m7c->fn_00451c45();
}

// FUNCTION: STARFLEETOP 0x00486a1d
long tSupplyDockMissiles::vfunction10()
{
	return m7c->fn_00451a8b();
}

// FUNCTION: STARFLEETOP 0x00486a25
double tSupplyDockMissiles::vfunction11()
{
	return m7c->fn_00451aa6( m78 );
}

// FUNCTION: STARFLEETOP 0x00486a31
long tSupplyDockMissiles::vfunction12()
{
	long diff = (long) m7c->fn_00451aef( m78 ) - (long) m80.fn_00451aef( m78 );
	if( diff < 0 )
		diff = 0;
	return diff;
}

// FUNCTION: STARFLEETOP 0x00486a63
bool tSupplyDockMissiles::vfunction13( long delta )
{
	vfunction14( delta );
	return true;
}

// FUNCTION: STARFLEETOP 0x00486a71
void tSupplyDockMissiles::vfunction14( long value )
{
	long count = m7c->fn_004517e1();
	m7c->fn_0045189f( ( value < 0 ? -1 : 1 ) * count );
	vfunction3();
}

// HYPOTHESIS: m7c's byte at +0xe is a two-state setting (0 or 1) that the flag steps
// up or down; 0x00451d07 sets it.
// FUNCTION: STARFLEETOP 0x00486aa1
void tSupplyDockMissiles::fn_00486aa1( bool up )
{
	switch( m7c->m0e )
	{
	case 0:
		if( up )
			m7c->fn_00451d07( true );
		break;
	case 1:
		if( !up )
			m7c->fn_00451d07( false );
		break;
	}
	vfunction3();
}

// HYPOTHESIS: m7c's byte at +0xf is a three-state setting (0..2) that the flag steps up or
// down; 0x00451dcd sets it, and the mission state then clamps it against the stock.
// FUNCTION: STARFLEETOP 0x00486ad5
void tSupplyDockMissiles::fn_00486ad5( bool up )
{
	switch( m7c->m0f )
	{
	case 0:
		if( up )
			m7c->fn_00451dcd( 1 );
		break;
	case 1:
		if( up )
			m7c->fn_00451dcd( 2 );
		else
			m7c->fn_00451dcd( 0 );
		break;
	case 2:
		if( !up )
			m7c->fn_00451dcd( 1 );
		break;
	}

	long state = gObject_0090b730.fn_0060ca4f();
	if( state == 5 || state == 4 )
	{
		long level = gObject_0090b730.fn_005c7fb3();
		if( level < 4 && m7c->fn_00451c18() > 0 )
			m7c->fn_00451dcd( 0 );
		else if( level < 0x11 && m7c->fn_00451c18() > 1 )
			m7c->fn_00451dcd( 1 );
	}
	else if( m7c->fn_00451c18() > gObject_0090b730.fn_0060ca9e() )
		m7c->fn_00451dcd( gObject_0090b730.fn_0060ca9e() );
	vfunction3();
}
