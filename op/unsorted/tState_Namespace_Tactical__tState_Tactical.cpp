// tState_Namespace_Tactical__tState_Tactical.cpp: the tactical (in-mission) game state.
// Evidence:  class (RTTI; original file unknown)
// Profile:   game-release (harness/units.json)

#define tvectormath_spark_members
#include "unsorted/tState_Tactical.h"
#include "unsorted/tApp3dBase.h"
#include "unsorted/tObject_00918888.h"
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

// The type of the mouse at 0x00898c24 is declared in unsorted/tState_Tactical.h.
















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

// MATCH-ONLY: until tMatrix declares these two members (0x0069b450 takes a matrix and is called
// on the matrix being built; 0x0069c7c0 takes three floats), the calls go through these.
void __fastcall fn_0069b450( tMatrix *self, int unused, tMatrix *other );
void __fastcall fn_0069c7c0( tMatrix *self, int unused, float x, float y, float z );


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
// The type of the object at 0x00919b44 is declared in unsorted/tState_Tactical.h.











// HYPOTHESIS: the 0xb0-byte object built by 0x004afd4d (tState_Tactical's field_360, and the
// global at 0x0092f478 that the three matrices at 0x0092f528 to 0x0092f5b8 follow).
class fn_0092f478_object
{
public:
	void fn_004afec0( tPoint3 *point, int kind, tMatrix *matrix );
	void fn_004b9d12( fn_0092f478_object *other );
	unsigned char m_data[0xb0];
};

// HYPOTHESIS: the list head at tState_Tactical's field_1ac.
class fn_004bc3e1_owner
{
public:
	void fn_004bc3e1( int *view );
};

// HYPOTHESIS: the object at tState_Tactical's field_0c.
class fn_004b05f8_owner
{
public:
	void fn_004b05f8( int *view, tPoint3 *point );
};

extern fn_0092f478_object g_0092f478;
extern tMatrix gMatrix_0092f5b8;
extern tMatrix gMatrix_0092f570;
extern tMatrix gMatrix_0092f528;
extern int g_00850bd4;
extern char g_b00850b62;
extern char g_b008930a2;


// MATCH-ONLY: tObject_00918888 declares slots 5 and 6 as void and nullary; the calls here need
// slot 5 to return an int and slot 6 to take a point and return a truth value.
class fn_00918888_slots
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual int vfunction5();
	virtual bool vfunction6( tPoint3 *point );
	virtual void vfunction7();
	virtual int vfunction8();
};

// MATCH-ONLY: fn_00967af4_type declares its slot 1 (offset 4) without the view it takes here.
class fn_00967af4_slots
{
public:
	virtual void vfunction1();
	virtual void vfunction2( int *view );
};

extern fn_00967af4_slots *g_00967af4;

void fn_00509aaf( int *view );
void fn_0050a25d( int *view );
void fn_004ecfb3( int *view );
void fn_00509e09( int *view, tPoint3 point );

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

// FUNCTION: STARFLEETOP 0x004aaff9
// FUNCTION: STARFLEETOP 0x004ab015
tState_Tactical::~tState_Tactical()
{
	// HYPOTHESIS: the object at 0x00919b44 is owned by the state and deleted with it.
	if ( g_00919b44 )
		delete g_00919b44;
}

}	// namespace tState_Namespace_Tactical

// HYPOTHESIS: a member of an int rectangle {left, top, right, bottom}: it grows the first two
// fields and shrinks the last two toward the rectangle given.
// FUNCTION: STARFLEETOP 0x004ab4c3
void __fastcall fn_004ab4c3( int *self, int unused, int *other )
{
	if ( self[0] < other[0] )
		self[0] = other[0];
	if ( self[1] < other[1] )
		self[1] = other[1];
	if ( self[2] > other[2] )
		self[2] = other[2];
	if ( self[3] > other[3] )
		self[3] = other[3];
}

extern float g_007959cc;
extern float g_007961d0;
extern unsigned char g_009186c4;
extern unsigned char g_009186c5;
extern unsigned char g_009186c6;
extern unsigned char g_009186c7;

#define TACT_CLAMP01( x ) ( ( ( ( x ) > 0.0f ? ( x ) : 0.0f ) < g_007959cc ) ? ( ( x ) > 0.0f ? ( x ) : 0.0f ) : 1.0f )

extern unsigned char g_009186c0;
extern unsigned char g_009186c1;
extern unsigned char g_009186c2;
extern unsigned char g_009186c3;

#define TACT_CLAMP01G( x ) ( ( ( ( x ) > zero ? ( x ) : 0.0f ) < g_007959cc ) ? ( ( x ) > zero ? ( x ) : 0.0f ) : 1.0f )

// HYPOTHESIS: sets a global colour from three floats in 0..1 (red, green, blue).
// FUNCTION: STARFLEETOP 0x004ab4f3
void __cdecl fn_004ab4f3( float *rgb )
{
	g_009186c6 = (unsigned char)( TACT_CLAMP01( rgb[0] ) * g_007961d0 );
	g_009186c5 = (unsigned char)( TACT_CLAMP01( rgb[1] ) * g_007961d0 );
	g_009186c4 = (unsigned char)( TACT_CLAMP01( rgb[2] ) * g_007961d0 );
	g_009186c7 = 0xff;
}

// FUNCTION: STARFLEETOP 0x004ab5c6
void __cdecl fn_004ab5c6( float *rgb )
{
	float zero = 0.0f;
	g_009186c2 = (unsigned char)( TACT_CLAMP01G( rgb[0] ) * g_007961d0 );
	g_009186c1 = (unsigned char)( TACT_CLAMP01G( rgb[1] ) * g_007961d0 );
	g_009186c0 = (unsigned char)( TACT_CLAMP01G( rgb[2] ) * g_007961d0 );
	g_009186c3 = 0xff;
}

// FUNCTION: STARFLEETOP 0x004ab699
void __cdecl fn_004ab699( unsigned char *rgba )
{
	rgba[2] = g_009186c2;
	rgba[1] = g_009186c1;
	unsigned char r = g_009186c0;
	rgba[3] = 0xff;
	rgba[0] = r;
}

// HYPOTHESIS: the object at 0x009199c8, whose slot 9 takes a mode.
class fn_009199c8_object
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
	virtual void vfunction9( int mode );
};

extern fn_009199c8_object *g_009199c8;
extern int g_009186bc;

// FUNCTION: STARFLEETOP 0x004ab6bc
void __cdecl fn_004ab6bc( int kind )
{
	switch ( kind )
	{
	case 0:
		g_009186bc = 0;
		g_009199c8->vfunction9( 0 );
		break;
	case 1:
		g_009186bc = 1;
		g_009199c8->vfunction9( 1 );
		break;
	case 2:
		g_009186bc = 2;
		g_009199c8->vfunction9( 3 );
		break;
	}
}

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

// FUNCTION: STARFLEETOP 0x004aba3c
void __cdecl fn_004aba3c( char value )
{
	g_b00850b62 = value;
}

// FUNCTION: STARFLEETOP 0x004aba46
void __cdecl fn_004aba46()
{
	fn_004aba3c( 0 );
}

#include "unsorted/tObject_0090b730.h"

// HYPOTHESIS: pointers to the names of the states, held in variables.
extern const char *g_00850bd0;
extern const char *g_00850bc4;
extern int gValue_0090b670;

namespace tState_Namespace_Tactical
{

// FUNCTION: STARFLEETOP 0x004aba4f
void tState_Tactical::fn_004aba4f()
{
	const char *name;
	if ( gObject_0092deb4->vfunction57() )
	{
		int state = gObject_0090b730.fn_0060ca4f();
		if ( state != 0 )
		{
			if ( state <= 0 )
				return;
			if ( state > 5 )
				return;
			field_468 = g_00850bc4;
			name = g_00850bc4;
		}
		else
		{
			field_468 = g_00850bd0;
			return;
		}
	}
	else
	{
		int state = gObject_0090b730.fn_0060ca4f();
		if ( state != 0 )
		{
			if ( state > 0 && state <= 5 )
				field_468 = g_00850bc4;
		}
		else
		{
			if ( gValue_0090b670 != 2 )
				field_468 = g_00850bd0;
			else
				field_468 = "FilmRoom";
		}
		name = field_468;
	}
	field_46c = name;
}

// FUNCTION: STARFLEETOP 0x004adc4c
char *tState_Tactical::vfunction13()
{
	return "tactical";
}

// FUNCTION: STARFLEETOP 0x004adc52
unsigned char tState_Tactical::fn_004adc52()
{
	return field_48e;
}

// FUNCTION: STARFLEETOP 0x004adc6d
// FUNCTION: STARFLEETOP 0x004adc63
tState_Tactical g_tState_Tactical;

// HYPOTHESIS: a free accessor for the one tactical state; whether it was a static member is not known.
// FUNCTION: STARFLEETOP 0x004adc83
tState_Tactical *fn_004adc83()
{
	return &g_tState_Tactical;
}

// HYPOTHESIS: an object of unknown type at 0x008515c8; the slot returns only its address.
extern int g_008515c8;

// FUNCTION: STARFLEETOP 0x004af807
void *tState_Tactical::vfunction19()
{
	return &g_008515c8;
}

// FUNCTION: STARFLEETOP 0x004b6647
void tState_Tactical::vfunction10( int param_1 )
{
	g_00919f00.fn_004b6642();
}

// FUNCTION: STARFLEETOP 0x004b9a3a
void tState_Tactical::vfunction9( int *param_1 )
{
	char haveTarget;
	tPoint3 target;
	if ( param_1 )
	{
		// HYPOTHESIS: the counts of the seven growable arrays (+4 of each {data, count, capacity}).
		*(int *)&field_410.m_data[4] = 0;
		*(int *)&field_41c.m_data[4] = 0;
		*(int *)&field_428.m_data[4] = 0;
		*(int *)&field_434.m_data[4] = 0;
		*(int *)&field_440.m_data[4] = 0;
		*(int *)&field_44c.m_data[4] = 0;
		*(int *)&field_458.m_data[4] = 0;
		// HYPOTHESIS: the argument is the view (slot 55 returns its matrix, slot 60 its projection).
		tView3D *view = (tView3D *)param_1;
		tMatrix *matrix = view->vfunction56();
		field_294 = *matrix;
		gMatrix_0092f5b8 = *matrix;
		float halfWidth = (float)( ( field_1f0[2] - field_1f0[0] ) >= 0 ? ( field_1f0[2] - field_1f0[0] ) : 0 ) * 0.5f;
		float halfHeight = (float)( ( field_1f0[3] - field_1f0[1] ) >= 0 ? ( field_1f0[3] - field_1f0[1] ) : 0 ) * 0.5f;
		{
			tMatrix screen;
			screen.mTranslate( (float)field_1f0[0] + halfWidth, (float)field_1f0[1] + halfHeight, 0.0f );
			fn_0069c7c0( &screen, 0, halfWidth, -halfHeight, 1.0f );
			fn_0069b450( &screen, 0, view->vfunction61() );


			field_31c = screen;
			screen.mMult( field_2d8, field_294 );
			gMatrix_0092f570 = field_2d8;
			gMatrix_0092f528 = field_31c;
		}
		g_0092f478.fn_004b9d12( (fn_0092f478_object *)field_360 );
		haveTarget = ( (fn_00918888_slots *)gObject_00918888 )->vfunction6( &target );
		int kind = ( (fn_00918888_slots *)gObject_00918888 )->vfunction5();
		( (fn_0092f478_object *)field_360 )->fn_004afec0( &target, kind, &field_294 );
		if ( g_00850bd4 != 0 && !g_b008930a2 )
		{
			fn_00509aaf( param_1 );
			( (fn_004bc3e1_owner *)&field_1ac )->fn_004bc3e1( param_1 );
			fn_0050a25d( param_1 );
			if ( haveTarget )
			{
				if ( g_b00850b62 && gObject_00918888->vfunction8() != 5 )
					( (fn_004b05f8_owner *)&field_0c )->fn_004b05f8( param_1, &target );
				if ( gObject_00918888->vfunction8() != 5 )
					fn_00509e09( param_1, target );
			}
			tGameState::vfunction3();
			fn_004ba05b( param_1 );
			fn_004ba5ea( param_1 );
			// HYPOTHESIS: the settings block the view's slot 32 takes; only these fields are set
			// (the block shares the matrix's stack slot, so it is at least 0x44 bytes).
			struct
			{
				int unused;
				int type;
				float f1;
				float f2;
				float f3;
				float pad[3];
				float f4;
				float f5;
				float f6;
				float f7;
				float tail[5];
			} light;
			light.f4 = 1.0f;
			light.type = 0;
			light.f3 = -1.0f;
			light.f5 = 1.0f;
			light.f6 = 1.0f;
			light.f1 = 0.0f;
			light.f2 = 0.0f;
			light.f7 = ( view->vfunction10() & 0x20 ) ? 1.5f : 4.0f;
			view->vfunction33( 0, (void *)&light.type );
			view->vfunction39( 0, true );
			if ( (unsigned int)view->vfunction32() > 1 )
				view->vfunction39( 1, false );
			fn_004ba7c7( param_1 );
			fn_004ecfb3( param_1 );
			g_00967af4->vfunction2( param_1 );
		}
		fn_004ba9eb( param_1 );
	}
}

}	// namespace tState_Namespace_Tactical
