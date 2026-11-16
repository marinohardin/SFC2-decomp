#include "Common/Exceptions/Validate.h"
class tValueBase;	class fn_004d288f_object;
// HYPOTHESIS: the 0x64-byte member at +0x10 (+0x08 in tSupplyDockShipInfo); it has a destructor.
class tObject_0045e574
{
public:
	tObject_0045e574();
	~tObject_0045e574();
	void fn_004d269b( long arg1, void *a, bool b );
	void fn_004d2797( long arg1, void *a, long arg3 );
	void fn_004d26fb( tValueBase *value, void *a );
	void fn_004d27fa( void *a );
	fn_004d288f_object *fn_004d288f( const void *key );
	char m00[0x64];
};
// HYPOTHESIS: layout from the constructor's stores, 0x7c bytes; "mask" is from the validate text.
class fn_004457b7_object;
class tSupplyDockItem
{
public:
	tSupplyDockItem( long arg1, long arg2, long arg3, long mask, long arg5 );

	virtual long vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5( fn_004457b7_object *target );
	virtual long vfunction6();
	virtual void vfunction7( long *msg );
	virtual void vfunction8( void *a, bool b );
	virtual long vfunction9() { return 0; }
	virtual long vfunction10() { return 0; }
	virtual double vfunction11();
	virtual long vfunction12() { return 0; }

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
// vtable 0x0079fc7c holds 0x0073ea74 (3 bytes, a shared return 0) for vfunction9, 10 and 12.
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
long tSupplyDockItem::vfunction6()
{
	return vfunction12();
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
#include <string>
class fn_00447ce4_object;

// HYPOTHESIS: the object the base keeps at +0x74 ("mask"); only 0x004479b4 is called on it.
class fn_004479b4_object
{
public:
	long fn_004479b4( long arg1 );
	void fn_00447926( long arg1, long arg2 );
	long fn_00447a54( long arg1 );
	fn_00447ce4_object *fn_00447ce4();
	long fn_004477f8( long arg1 );
	long fn_0044780d( long arg1 );
	std::string fn_00447822( long arg1 );
	long fn_004477b1( long arg1 );
	void fn_004477c6( long arg1, long arg2, long arg3 );
	void fn_00447867( long arg1, std::string name );
	long fn_00447415( long arg1, long arg2 );
	void fn_00447531( long arg1, long arg2 );
	// HYPOTHESIS: tSupplyDockShipInfo holds one at +0x6c (0xf0 bytes) and builds it with 0x004455d7,
	// which sits between tShipSystemMask's deleting destructor and its destructor.
	fn_004479b4_object( long arg1, long arg2, long arg3, long arg4 );
	~fn_004479b4_object();
	long fn_004478b5( long arg1 );
	bool fn_00447c61( long arg1 );

	char m00[0xf0];
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
	double fn_004513a9( long arg1 );
	// HYPOTHESIS: looks a name up and writes a long through the pointer; its return is unused.
	void fn_0045147a( const std::string &name, long *value );
	// HYPOTHESIS: a lookup by name with an out value and a flag; fn_0048719b tests its result.
	bool fn_004513d2( const std::string &name, long *value, long flag );
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

#include "unsorted/tValueBase.h"

// HYPOTHESIS: the analysis types this global tValue<long>; its constructor and destructor are external.
class fn_00486500_value : public tValueBase
{
public:
	fn_00486500_value();
	~fn_00486500_value();
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
		void fn_00451904( long count );
		void fn_00451dee( long value );

		char m00[0xc];
		bool m0c;
		bool m0d;
		char m0e;
		char m0f;
		short m10;
		char m12[2];
		short m14;
		// tSupplyDockMissiles is 0x164 bytes (ShipInfo's new), so this is 0xe4.
		char m16[0xce];
	};

	// RTTI names the class; tSupplyDockShipInfo holds one at +0x1c0 (0x60 bytes).
	class tItemRates
	{
	public:
		tItemRates( long arg1 );
		~tItemRates();

		char m00[0x60];
	};
};

// HYPOTHESIS: the object 0x00447ce4 returns when called on the base's +0x74 object.
class fn_00447ce4_object
{
public:
	tStoresState::tMissileInfo *fn_0055e60f();

	// HYPOTHESIS: a signed byte per fighter bay at +0x17a, read by tSupplyDockFighters::vfunction15.
	char m00[0x17a];
	char m17a[4];
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

// FUNCTION: STARFLEETOP 0x00486b8a
void tSupplyDockMissiles::fn_00486b8a( bool up )
{
	short max = m7c->fn_00451c45();
	short cur = m7c->fn_00451a8b();
	if( up && cur < max )
		m7c->fn_00451904( max - cur );
	else
	{
		m7c->fn_00451dee( m7c->m10 + ( up ? 1 : -1 ) );
		vfunction14( m7c->m14 );
	}
	vfunction3();
}

// HYPOTHESIS: vtable 0x0079fd30; four slots of two longs each from the constructor's loop.
// vfunction17 is called directly from the constructor, so its slot is not confirmed; it takes
// the string 0x00447822 returns and its result is multiplied on the FPU stack.
class tSupplyDockFighters : public tSupplyDockItem
{
public:
	tSupplyDockFighters( long arg1, long arg2, long arg3, long mask, long arg5 );

	virtual long vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual long vfunction6();
	virtual void vfunction7( long *msg );
	virtual long vfunction9();
	virtual long vfunction10();
	virtual long vfunction12();
	// HYPOTHESIS: slots 13 and 14 typed as in tSupplyDockShuttles; 15, 16 and 18 take the
	// slot index (vfunction3 calls them at vtable +0x38, +0x3c and +0x44).
	virtual bool vfunction13( long delta );
	virtual void vfunction14( long value );
	virtual long vfunction15( long i );
	virtual long vfunction16( long i );
	virtual double vfunction17( const std::string &name );
	virtual long vfunction18( long i );
	// HYPOTHESIS: virtual in the exe (slot 20); declared plain until slots 13-19 are known.
	void vfunction20( long arg1, long arg2 );
	// HYPOTHESIS: called directly with the slot index; its body is not written yet.
	void fn_00486fac( long i );
	// HYPOTHESIS: each maps a bay's craft name to another name; their bodies are not written yet.
	std::string fn_0048719b( std::string name );
	std::string fn_00487236( std::string name );

	long m7c[4];
	long m8c[4];
};

// HYPOTHESIS: four arrays of four globals passed by value, like g_0084bb4c; their types are not
// known.
extern void *g_0084bb80[4];
extern void *g_0084bb90[4];
extern void *g_0084bba0[4];
extern void *g_0084bbb0[4];

// FUNCTION: STARFLEETOP 0x00486bf2
tSupplyDockFighters::tSupplyDockFighters( long arg1, long arg2, long arg3, long mask, long arg5 )
	: tSupplyDockItem( arg1, arg2, arg3, mask, arg5 )
{
	for( long i = 0; i < 4; i++ )
	{
		m7c[i] = ((fn_004479b4_object *) m74)->fn_004477f8( i );
		m8c[i] = vfunction17( ((fn_004479b4_object *) m74)->fn_00447822( i ) ) * m7c[i];
		if( ((fn_004479b4_object *) m74)->fn_0044780d( i ) )
		{
			m10.fn_004d2797( 0x11027 + i, g_0084bb80[i], 0 );
			m10.fn_004d2797( 0x1102b + i, g_0084bb90[i], 0 );
			m10.fn_004d2797( 0x1102f + i, g_0084bba0[i], 0 );
			m10.fn_004d2797( 0x11033 + i, g_0084bbb0[i], 0 );
		}
	}
}

// FUNCTION: STARFLEETOP 0x00486cfc
long tSupplyDockFighters::vfunction1()
{
	long result = tSupplyDockItem::vfunction1();
	for( long i = 0; i < 4; i++ )
	{
		if( ((fn_004479b4_object *) m74)->fn_0044780d( i ) )
			fn_00486fac( i );
	}
	vfunction3();
	return result;
}

// FUNCTION: STARFLEETOP 0x00486d32
void tSupplyDockFighters::vfunction20( long arg1, long arg2 )
{
	((fn_004479b4_object *) m74)->fn_004477c6( arg2, arg1, ((fn_004479b4_object *) m74)->fn_004477b1( arg2 ) );
}

// HYPOTHESIS: a free cdecl function of a name, the counterpart of fn_004d3306; other units pass
// it a name string.
void fn_004d3340( long arg1 );
// The four fighter bay names, "CC_FighterBay1" first.
extern const char *g_0084bbc0[];

// FUNCTION: STARFLEETOP 0x00486d56
void tSupplyDockFighters::vfunction2()
{
	for( int i = 3; i > -1; i-- )
	{
		fn_004d3340( (long) g_0084bbc0[i] );
		fn_004d3306( (long) g_0084bbc0[i] );
	}
}

// FUNCTION: STARFLEETOP 0x00486d80
void tSupplyDockFighters::vfunction3()
{
	tString text;

	for( long i = 0; i < 4; i++ )
	{
		if( ((fn_004479b4_object *) m74)->fn_0044780d( i ) )
		{
			text = "";
			text.mAppendInt32s( vfunction16( i ) );
			fn_004d33d4( 0x11017 + i, text );
			text = "";
			text.mAppendInt32s( vfunction17( ((fn_004479b4_object *) m74)->fn_00447822( i ) ) );
			fn_004d33d4( 0x1101b + i, text );
			text = "";
			text.mAppendInt32s( vfunction18( i ) );
			fn_004d33d4( 0x1101f + i, text );

			vfunction8( g_0084bb80[i], vfunction16( i ) > 0 );
			vfunction8( g_0084bb90[i], vfunction16( i ) < vfunction15( i ) );
			vfunction8( g_0084bba0[i], true );
			vfunction8( g_0084bbb0[i], true );
			fn_004d33d4( 0x11023 + i, ((fn_004479b4_object *) m74)->fn_00447822( i ) );
		}
	}
}

// FUNCTION: STARFLEETOP 0x00486f6d
long tSupplyDockFighters::vfunction6()
{
	long total = 0;
	for( long i = 0; i < 4; i++ )
		total += vfunction18( i );
	return total;
}

// HYPOTHESIS: the argument's class is not known; only 0x004457b7 is called on it, with the
// object 0x00447ce4 returns.
class fn_004457b7_object
{
public:
	void fn_004457b7( fn_00447ce4_object *object );
};

// FUNCTION: STARFLEETOP 0x00486f8c
void tSupplyDockItem::vfunction5( fn_004457b7_object *target )
{
	target->fn_004457b7( ((fn_004479b4_object *) m74)->fn_00447ce4() );
	vfunction6();
}

// FUNCTION: STARFLEETOP 0x00486fac
void tSupplyDockFighters::fn_00486fac( long i )
{
	fn_004d3278( (long) g_0084bbc0[i], (long) g_0084bbc0[i], 0, m04, m0c, 800, 600 );
}

// FUNCTION: STARFLEETOP 0x00486fd6
long tSupplyDockFighters::vfunction15( long i )
{
	return ((fn_004479b4_object *) m74)->fn_00447ce4()->m17a[i];
}

// FUNCTION: STARFLEETOP 0x00486fed
long tSupplyDockFighters::vfunction16( long i )
{
	return ((fn_004479b4_object *) m74)->fn_004477f8( i );
}

// FUNCTION: STARFLEETOP 0x00486ffc
double tSupplyDockFighters::vfunction17( const std::string &name )
{
	long value = 0;
	((fn_0045139e_object *) m78)->fn_0045147a( name, &value );
	return value;
}

// FUNCTION: STARFLEETOP 0x0048701a
long tSupplyDockFighters::vfunction18( long i )
{
	long total = vfunction16( i ) * vfunction17( ((fn_004479b4_object *) m74)->fn_00447822( i ) );
	long result = total - m8c[i];
	if( result < 0 )
		result = 0;
	return result;
}

// FUNCTION: STARFLEETOP 0x00487091
void tSupplyDockFighters::vfunction7( long *msg )
{
	long i;
	long count;

	switch( *msg )
	{
	case 0x11027:
	case 0x11028:
	case 0x11029:
	case 0x1102a:
		i = *msg - 0x11027;
		count = ((fn_004479b4_object *) m74)->fn_004477f8( i ) - 1;
		if( count < 0 )
			count = 0;
		((fn_004479b4_object *) m74)->fn_004477c6( i, count, ((fn_004479b4_object *) m74)->fn_004477b1( i ) );
		break;
	case 0x1102b:
	case 0x1102c:
	case 0x1102d:
	case 0x1102e:
		i = *msg - 0x1102b;
		count = ((fn_004479b4_object *) m74)->fn_004477f8( i ) + 1;
		if( count > vfunction15( i ) )
			count = vfunction15( i );
		((fn_004479b4_object *) m74)->fn_004477c6( i, count, ((fn_004479b4_object *) m74)->fn_004477b1( i ) );
		break;
	case 0x1102f:
	case 0x11030:
	case 0x11031:
	case 0x11032:
		i = *msg - 0x1102f;
		((fn_004479b4_object *) m74)->fn_00447867( i, fn_00487236( ((fn_004479b4_object *) m74)->fn_00447822( i ) ) );
		break;
	case 0x11033:
	case 0x11034:
	case 0x11035:
	case 0x11036:
		i = *msg - 0x11033;
		((fn_004479b4_object *) m74)->fn_00447867( i, fn_0048719b( ((fn_004479b4_object *) m74)->fn_00447822( i ) ) );
		break;
	}
	vfunction3();
	vfunction6();
}

// FUNCTION: STARFLEETOP 0x0048719b
std::string tSupplyDockFighters::fn_0048719b( std::string name )
{
	long next;
	if( ((fn_0045139e_object *) m78)->fn_004513d2( name, &next, 1 ) )
		return name;
	return "ShipNotFound!";
}

// FUNCTION: STARFLEETOP 0x00487236
std::string tSupplyDockFighters::fn_00487236( std::string name )
{
	long next;
	if( ((fn_0045139e_object *) m78)->fn_004513d2( name, &next, -1 ) )
		return name;
	return "ShipNotFound!";
}

class tSupplyDockMisc : public tSupplyDockItem
{
public:
	tSupplyDockMisc( long arg1, long arg2, long arg3, long mask, long arg5 );

	// HYPOTHESIS: called directly with the index; their bodies are not written yet.
	long fn_004876ae( long i );
	long fn_004876bf( long i );
	double fn_004876d0( long i );
	long fn_004876df( long i );
	// HYPOTHESIS: a member taking the index and the count saved at construction.
	void fn_00487757( long i, long count );
	// HYPOTHESIS: steps item i by delta; false when the result leaves 0..fn_004876ae( i ).
	bool fn_0048771a( long i, long delta );

	virtual long vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual long vfunction6();
	virtual void vfunction7( long *msg );

	long m7c[3];
};

// HYPOTHESIS: globals passed by value, like g_0084bb4c; their types are not known.
extern void *g_0084bbd0;
extern void *g_0084bbd4;
extern void *g_0084bbd8;
extern void *g_0084bbdc;
extern void *g_0084bbe0;
extern void *g_0084bbe4;

// HYPOTHESIS: vtable 0x0079fd84.
// FUNCTION: STARFLEETOP 0x004872d2
tSupplyDockMisc::tSupplyDockMisc( long arg1, long arg2, long arg3, long mask, long arg5 )
	: tSupplyDockItem( arg1, arg2, arg3, mask, arg5 )
{
	m10.fn_004d2797( 0x1104a, g_0084bbd0, 0 );
	m10.fn_004d2797( 0x1104b, g_0084bbd4, 0 );
	m10.fn_004d2797( 0x11050, g_0084bbd8, 0 );
	m10.fn_004d2797( 0x11051, g_0084bbdc, 0 );
	m10.fn_004d2797( 0x11056, g_0084bbe0, 0 );
	m10.fn_004d2797( 0x11057, g_0084bbe4, 0 );
	for( long i = 0; i < 3; i++ )
		m7c[i] = fn_004876bf( i );
}

// The three misc item names, "CC_Marines" first, and their area names, "Misc_AREA_0" first.
extern const char *g_0084bbe8[];
extern const char *g_0084bbf4[];

// FUNCTION: STARFLEETOP 0x004873a2
long tSupplyDockMisc::vfunction1()
{
	long result = tSupplyDockItem::vfunction1();
	for( long i = 0; i < 3; i++ )
		fn_004d3278( (long) g_0084bbe8[i], (long) g_0084bbf4[i], result, m04, m0c, 800, 600 );
	vfunction3();
	return result;
}

// FUNCTION: STARFLEETOP 0x004873ea
void tSupplyDockMisc::vfunction2()
{
	tSupplyDockItem::vfunction2();
	for( long i = 0; i < 3; i++ )
		fn_004d3306( (long) g_0084bbe8[i] );
}

// HYPOTHESIS: the four text ids per misc item, at a fixed data address.
extern const long g_0079fc48[3][4];

// FUNCTION: STARFLEETOP 0x0048740a
void tSupplyDockMisc::vfunction3()
{
	tString text;

	for( long i = 0; i < 3; i++ )
	{
		text = "";
		text.mAppendInt32s( fn_004876bf( i ) );
		fn_004d33d4( g_0079fc48[i][0], text );
		text = "";
		text.mAppendInt32s( fn_004876ae( i ) );
		fn_004d33d4( g_0079fc48[i][1], text );
		text = "";
		text.mAppendInt32s( fn_004876d0( i ) );
		fn_004d33d4( g_0079fc48[i][2], text );
		text = "";
		text.mAppendInt32s( fn_004876df( i ) );
		fn_004d33d4( g_0079fc48[i][3], text );
	}

	vfunction8( g_0084bbd0, fn_004876bf( 0 ) > 0 );
	vfunction8( g_0084bbd4, fn_004876bf( 0 ) < fn_004876ae( 0 ) );
	vfunction8( g_0084bbd8, fn_004876bf( 1 ) > 0 );
	vfunction8( g_0084bbdc, fn_004876bf( 1 ) < fn_004876ae( 1 ) );
	vfunction8( g_0084bbe0, fn_004876bf( 2 ) > 0 );
	vfunction8( g_0084bbe4, fn_004876bf( 2 ) < fn_004876ae( 2 ) );
}

// FUNCTION: STARFLEETOP 0x0048761d
void tSupplyDockMisc::vfunction4()
{
	for( long i = 0; i < 3; i++ )
		fn_00487757( i, m7c[i] );
}

// FUNCTION: STARFLEETOP 0x0048763e
long tSupplyDockMisc::vfunction6()
{
	long total = 0;
	for( long i = 0; i < 3; i++ )
		total += fn_004876df( i );
	return total;
}

// FUNCTION: STARFLEETOP 0x0048765d
void tSupplyDockMisc::vfunction7( long *msg )
{
	switch( *msg )
	{
	case 0x1104a:
		fn_0048771a( 0, -1 );
		break;
	case 0x1104b:
		fn_0048771a( 0, 1 );
		break;
	case 0x11050:
		fn_0048771a( 1, -1 );
		break;
	case 0x11051:
		fn_0048771a( 1, 1 );
		break;
	case 0x11056:
		fn_0048771a( 2, -1 );
		break;
	case 0x11057:
		fn_0048771a( 2, 1 );
		break;
	}
	vfunction6();
}

// FUNCTION: STARFLEETOP 0x004876ae
long tSupplyDockMisc::fn_004876ae( long i )
{
	return ((fn_004479b4_object *) m74)->fn_00447415( i, 2 );
}

// FUNCTION: STARFLEETOP 0x004876bf
long tSupplyDockMisc::fn_004876bf( long i )
{
	return ((fn_004479b4_object *) m74)->fn_00447415( i, 0 );
}

// FUNCTION: STARFLEETOP 0x004876d0
double tSupplyDockMisc::fn_004876d0( long i )
{
	return ((fn_0045139e_object *) m78)->fn_004513a9( i );
}

// FUNCTION: STARFLEETOP 0x004876df
long tSupplyDockMisc::fn_004876df( long i )
{
	long cost = ( fn_004876bf( i ) - m7c[i] ) * fn_004876d0( i );
	if ( cost < 0 )
		cost = 0;
	return cost;
}

// FUNCTION: STARFLEETOP 0x0048771a
bool tSupplyDockMisc::fn_0048771a( long i, long delta )
{
	long value = fn_004876bf( i ) + delta;
	if( value > fn_004876ae( i ) || value < 0 )
		return false;
	fn_00487757( i, value );
	return true;
}

// FUNCTION: STARFLEETOP 0x00487757
void tSupplyDockMisc::fn_00487757( long i, long count )
{
	if( count != fn_004876bf( i ) )
	{
		((fn_004479b4_object *) m74)->fn_00447531( i, count );
		vfunction3();
	}
}

#include <sstream>

// HYPOTHESIS: the object tObject_0045e574::fn_004d288f returns; tPanelMgr.cpp declares the
// same class, which belongs in a shared header.
class fn_004d288f_object
{
public:
	void fn_004d2573( std::string text );
};

// RTTI names the class tValueI<tString>; vtable 0x0079b8a4, the value at +0x18.
template< class T >
class tValueI : public tValueBase
{
public:
	// HYPOTHESIS: the set step; the original assigns the value, notifies with (3, 0), and
	// only then destroys the temporary it was given.
	tValueI &operator=( const T &value ) { m18 = value; fn_006712c0( 3, 0 ); return *this; }

	T m18;
};

// HYPOTHESIS: vtable 0x0079fdc0 has one slot (0x00487f84); layout from the constructor's stores.
class tSupplyDockShipInfo
{
public:
	// The validate text names the limit tSupplyDockShipInfo::kMaxDisplayableOwnedShips.
	enum { kMaxDisplayableOwnedShips = 3 };

	tSupplyDockShipInfo( long arg1, long arg2, long arg3, long shipIndex, long arg5, long arg6, long arg7, const char *arg8, const char *arg9, long arg10, long arg11, long arg12, long arg13 );

	virtual void vfunction1();
	void fn_00487df7( long arg1 );
	void fn_00487e4c( long arg1 );

	long m04;
	tObject_0045e574 m08;
	fn_004479b4_object m6c;
	tSupplyDockItem *m15c[4];
	long m16c;
	long m170;
	long m174;
	tValueI< tString > m178;
	long m1b8;
	long m1bc;
	tStoresState::tItemRates m1c0;
	long m220;
	long m224;
};

// HYPOTHESIS: per-ship names and control ids indexed by the ship index; their types are not known.
extern const char *g_0084bb1c[3];
extern void *g_0084bb28[3];
extern void *g_0084bb34[3];
extern void *g_0084bb40[3];
extern void *g_0084bc00[12];
// HYPOTHESIS: the names the four item panels are built with.
extern const char *g_0084bc30;
extern const char *g_0084bc34;
extern const char *g_0084bc38;
extern const char *g_0084bc3c;

// FUNCTION: STARFLEETOP 0x00487784
tSupplyDockShipInfo::tSupplyDockShipInfo( long arg1, long arg2, long arg3, long shipIndex, long arg5, long arg6, long arg7, const char *arg8, const char *arg9, long arg10, long arg11, long arg12, long arg13 )
	: m6c( arg5, arg6, arg7, arg12 ), m16c( arg1 ), m170( arg3 ), m174( shipIndex ), m1b8( arg13 ), m1c0( arg11 ), m220( 0 )
{
	std::stringstream stream;

#line 1285
	validate( shipIndex < tSupplyDockShipInfo::kMaxDisplayableOwnedShips );

	long index = m174;
	m08.fn_004d2797( index + 0x11004, g_0084bb28[index], 0 );
	m08.fn_004d2797( m174 + 0x1100a, g_0084bb34[m174], 0 );
	m08.fn_004d2797( m174 + 0x1100d, g_0084bb40[m174], 0 );
	// MATCH-ONLY: the original shifts the index register in place (x4 for the first table,
	// x4 again here); a [3][4] table read with the plain index gives a single shl by 4.
	index *= 4;
	m08.fn_004d26fb( &m178, g_0084bc00[index + 0] );
	m08.fn_004d27fa( g_0084bc00[index + 1] );
	m08.fn_004d27fa( g_0084bc00[index + 2] );
	m08.fn_004d27fa( g_0084bc00[index + 3] );
	fn_004d3278( (long) g_0084bb1c[m174], (long) g_0084bb1c[m174], 0, m16c, arg2, 800, 600 );

	m178 = arg8;

	stream.str( "" );
	stream << arg9;
	m08.fn_004d288f( g_0084bc00[index + 1] )->fn_004d2573( stream.str() );

	for( long i = 0; i < 4; i++ )
		m15c[i] = NULL;
	if( m6c.fn_004478b5( 0 ) )
		m15c[0] = new tSupplyDockShuttles( (long) g_0084bc30, m16c, arg2, (long) &m6c, (long) &m1c0 );
	if( m6c.fn_00447c61( 1 ) )
		m15c[1] = new tSupplyDockMissiles( (long) g_0084bc34, m16c, arg2, (long) &m6c, (long) &m1c0 );
	if( m6c.fn_00447c61( 2 ) )
		m15c[2] = new tSupplyDockFighters( (long) g_0084bc38, m16c, arg2, (long) &m6c, (long) &m1c0 );
	if( m6c.fn_00447c61( 4 ) )
		m15c[3] = new tSupplyDockMisc( (long) g_0084bc3c, m16c, arg2, (long) &m6c, (long) &m1c0 );

	m224 = -1;
	fn_00487df7( arg10 );
	fn_00487e4c( 0 );
}
