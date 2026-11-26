// tState_Namespace_Tactical__tState_Tactical.cpp: the tactical (in-mission) game state.
// Evidence:  class (RTTI; original file unknown)
// Profile:   game-release (harness/units.json)

#include "unsorted/tState_Tactical.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/tNetControl.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tReconnector.h"

// HYPOTHESIS: the objects at 0x00918838 to 0x00918860 each have a slot 1 that releases the
// object and returns the new pointer value.
class fn_00918838_object
{
public:
	virtual void vfunction0();
	virtual fn_00918838_object *vfunction1();
};

// HYPOTHESIS: the mouse at 0x00898c24: slot 9 selects a cursor by name, slot 10 a race's cursor.
class fn_00898c24_object
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9(const char *name);
	virtual void vfunction10(int kind);
};

// HYPOTHESIS: the object at 0x00969088, slot 1 takes nothing.
class fn_00969088_object
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
};

// HYPOTHESIS: the object at 0x0084b47c.
class fn_0084b47c_object
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual bool vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11(int aKind);
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19(int a);
	virtual void vfunction20(double a, int b);
};

// HYPOTHESIS: the view the result of 0x0046aa46 points at; slot 16 sets a rectangle.
class fn_0046aa46_view
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16(int w, int h, int x, int y);
};

extern fn_00898c24_object *g_00898c24;
extern fn_00918838_object *g_00918838;
extern fn_00918838_object *g_0091883c;
extern fn_00918838_object *g_00918840;
extern fn_00918838_object *g_00918844;
extern fn_00918838_object *g_00918848;
extern fn_00918838_object *g_0091884c;
extern fn_00918838_object *g_00918850;
extern fn_00918838_object *g_00918854;
extern fn_00918838_object *g_00918858;
extern fn_00918838_object *g_0091885c;
extern fn_00918838_object *g_00918860;
extern fn_00969088_object *g_00969088;
extern fn_0084b47c_object *g_0084b47c;

unsigned int fn_0046aa46();
void fn_0046aa58();
void fn_0050c6f5();
void fn_004adc89();
void fn_004d3e88();
void fn_004af94b();
void fn_0052ea0a();
bool fn_00434584();

extern "C" unsigned long __stdcall timeGetTime();
extern "C" void __stdcall Sleep( unsigned long ms );

namespace tState_Namespace_Tactical
{

// FUNCTION: STARFLEETOP 0x004ab70a
bool tState_Tactical::vfunction4()
{
	field_470 = 0;
	g_00898c24->vfunction10( gObject_0092deb4->vfunction39() );
	if ( g_00918838 )
		g_00918838 = g_00918838->vfunction1();
	if ( g_0091883c )
		g_0091883c = g_0091883c->vfunction1();
	if ( g_00918840 )
		g_00918840 = g_00918840->vfunction1();
	if ( g_00918844 )
		g_00918844 = g_00918844->vfunction1();
	if ( g_00918848 )
		g_00918848 = g_00918848->vfunction1();
	if ( g_0091884c )
		g_0091884c = g_0091884c->vfunction1();
	if ( g_00918850 )
		g_00918850 = g_00918850->vfunction1();
	if ( g_00918854 )
		g_00918854 = g_00918854->vfunction1();
	if ( g_00918858 )
		g_00918858 = g_00918858->vfunction1();
	if ( g_0091885c )
		g_0091885c = g_0091885c->vfunction1();
	if ( g_00918860 )
		g_00918860 = g_00918860->vfunction1();
	g_00969088->vfunction1();
	g_0084b47c->vfunction11( 0 );
	Sleep( 5000 );
	fn_0050c6f5();
	g_0084b47c->vfunction20( 100.0, 0 );
	unsigned long start = timeGetTime();
	while ( g_0084b47c->vfunction5() && timeGetTime() - start < 5000 )
		Sleep( 1 );
	g_0084b47c->vfunction19( 0x1c );
	fn_004adc89();
	fn_0046aa46_view *view = (fn_0046aa46_view *) fn_0046aa46();
	if ( view )
	{
		int w = field_1e0[2] - field_1e0[0];
		if ( w < 0 )
			w = 0;
		if ( w != 0 )
		{
			int h = field_1e0[3] - field_1e0[1];
			if ( h < 0 )
				h = 0;
			int w2 = field_1e0[2] - field_1e0[0];
			if ( w2 < 0 )
				w2 = 0;
			view->vfunction16( w2, h, field_1e0[0], field_1e0[1] );
		}
	}
	fn_004d3e88();
	// HYPOTHESIS: the two objects are a tValueBase and a tReconnector, destroyed in place.
	tValueBase *a = (tValueBase *) field_244;
	if ( a )
	{
		a->~tValueBase();
		delete a;
		field_244 = 0;
	}
	tReconnector *b = (tReconnector *) field_248;
	if ( b )
	{
		b->~tReconnector();
		delete b;
		field_248 = 0;
	}
	fn_004af94b();
	int race = gObject_0092deb4->vfunction39();
	switch ( race )
	{
	case 0:
	case 16:
		g_00898c24->vfunction9( "starrow" );
		break;
	case 1:
		g_00898c24->vfunction9( "dktagh" );
		break;
	case 2:
		g_00898c24->vfunction9( "rom3" );
		break;
	case 3:
		g_00898c24->vfunction9( "lyran" );
		break;
	case 4:
		g_00898c24->vfunction9( "hydran" );
		break;
	case 5:
		g_00898c24->vfunction9( "gorn" );
		break;
	case 6:
		g_00898c24->vfunction9( "isc" );
		break;
	case 7:
		g_00898c24->vfunction9( "mirak" );
		break;
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		g_00898c24->vfunction9( "orion" );
		break;
	default:
		g_00898c24->vfunction9( "arrow" );
		break;
	}
	fn_0052ea0a();
	if ( fn_00434584() )
	{
		gNetControl.fn_00431821();
		fn_0046aa58();
	}
	return true;
}

}	// namespace tState_Namespace_Tactical
