#include "Common/Exceptions/Validate.h"

// HYPOTHESIS: the 0x64-byte member at +0x10; only its constructor (0x0045e574) is known, and
// the constructor's unwind frame shows it has a destructor.
class tObject_0045e574
{
public:
	tObject_0045e574();
	~tObject_0045e574();
	void fn_004d269b( long arg1, void *a, bool b );
	void fn_004d2797( long arg1, void *a, long arg3 );

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

// HYPOTHESIS: the object the base keeps at +0x74 ("mask"); only 0x004479b4 is called on it.
class fn_004479b4_object
{
public:
	long fn_004479b4( long arg1 );
	void fn_00447926( long arg1, long arg2 );
	long fn_00447a54( long arg1 );
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
	virtual void vfunction13( long delta );

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
