// Unit:      unsorted/tState_Skirmish.cpp
// Functions: 0x0047f95d to 0x004801a5, 10 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include "unsorted/tState_Skirmish.h"
#include "unsorted/c_0046aa10_msg.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/tValueBase.h"
#include "unsorted/fn_004D3143.h"
#include "unsorted/tObject_0090b730.h"

// HYPOTHESIS: partial. The original also loops i = 0..2 over a tShipSystemMask copied from slot 85
// of *0x0092deb4 and handed to 0x00482f3b on the object at this+0x1154; the copy constructor
// 0x004456a1 and that slot's signature are not declared in headers this lane may edit.
// FUNCTION: STARFLEETOP 0x0047feb1
void tState_Skirmish::fn_0047feb1()
{
	gObject_0092deb4->vfunction86();
	gObject_0092deb4->vfunction109();
}

// FUNCTION: STARFLEETOP 0x0047ff4e
void tState_Skirmish::fn_0047ff4e()
{
	int *p = (int *)((char *)this + 0x11d8);
	for (int n = 5; n != 0; n--)
	{
		p[15] = 1;
		p[0] = 0;
		p[5] = 2;
		p[10] = 0;
		p[-5] = 0;
		p++;
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
			fn_00482ed3(i, j, -1);
	}
	fn_00482ed3(0, 0, 0);
	field_0x10 = 0;
	gObject_0092deb4->vfunction38();
	fn_00482ed3(1, 0, 1);
	*(int *)((char *)this + 0x11c4) = 2;
}

long fn_004d33a8( long arg1, long arg2 );
// HYPOTHESIS: five values starting at 0x0084abb8.
extern long g_0084abb8[5];

// FUNCTION: STARFLEETOP 0x0047ffe0
void tState_Skirmish::fn_0047ffe0()
{
	for (int i = 1; i < 6; i++)
		fn_004d33a8( g_0084abb8[i - 1], ((fn_00482eef_Object *)field_0x1154)->fn_00482eef( i, 0 ) + 1 );
}

void fn_004d3306( long arg1 );
// HYPOTHESIS: a table of control names (every other word) and a table of three names.
extern const char *g_0084a654[];
extern const char *g_0084a890[];
// HYPOTHESIS: a stdcall helper taking an output slot, an iterator pair and a range's end.
int __stdcall fn_0045df74( int *arg1, int arg2, void *arg3, int arg4, void *arg5 );

// FUNCTION: STARFLEETOP 0x00480016
bool tState_Skirmish::vfunction4()
{
	int local[2];
	if (field_0x129c != 0)
	{
		fn_004d32c4( field_0x1118 );
		return true;
	}
	field_0x1120 = 0;
	const char **p;
	for (p = g_0084a654; (int)p < (int)&g_0084a654[0x90]; p += 2)
		fn_004d35fa( *p );
	for (p = g_0084a890; (int)p < (int)&g_0084a890[3]; p++)
	{
		tValueBase *v = fn_004d35bf( *p, 0 );
		if (v != 0)
			delete v;
	}
	field_0x1228.fn_0046b731();
	fn_004d3306( (long)"MapIcons" );
	fn_004d32c4( field_0x1118 );
	fn_004d3306( (long)"systemicons" );
	fn_0047f7ab_VectorAndWord *v1 = &field_0x1124;
	int w1 = v1->fWord;
	fn_0045df74( local, ((int *)v1)[3], v1, w1 + ((int *)v1)[3], v1 );
	fn_0047f7ab_VectorAndWord *v2 = &field_0x1138;
	int w2 = v2->fWord;
	fn_0045df74( local, ((int *)v2)[3], v2, w2 + ((int *)v2)[3], v2 );
	return true;
}

// HYPOTHESIS: partial. After fn_0060ca42 the original calls slot 114 of *0x0092deb4 with 0, then
// for i below slot 9's count calls slot 10 with two arguments: i plus the value of the method at
// 0x005e8f45 on the object at 0x008e1060 (given i and 0), and 0. Neither slot 114, the two-argument
// slot 10 nor that method is declared in a header this lane may edit.
// FUNCTION: STARFLEETOP 0x004800f2
void tState_Skirmish::vfunction22()
{
	vfunction20();
	gObject_0090b730.fn_0060ca42( 1 );
	gObject_0092deb4->vfunction9();
}

#include "unsorted/tNetControl.h"
#include "Source/network/NetMsgClass.h"

// HYPOTHESIS: layout from the constructor stores (a word at 4, a byte at 8); no header declares this message yet.
struct tNetMsg_kSetPlayerReady : public tNetMsg
{
	tNetMsg_kSetPlayerReady() { fField4 = 1; fField8 = 1; }
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
	long fField4;
	unsigned char fField8;
};

// HYPOTHESIS: the object fn_0060e11e returns; fn_0060C000.cpp declares the same class locally.
C_0060e11e *fn_0060e11e();

// FUNCTION: STARFLEETOP 0x00480152
void tState_Skirmish::vfunction20()
{
	C_0060e11e *p = fn_0060e11e();
	vfunction21();
	fn_00480677( p );
	tNetMsg_kSetPlayerReady msg;
	gNetControl.mQueueMsg(&msg, 0x29, -1, -1.0);
}

// HYPOTHESIS: the data at 0x0084a8a0 is a table the helper takes by address.
extern char g_0084a8a0[];
int fn_00481bf2( int id );
// HYPOTHESIS: __stdcall; the original leaves its two arguments to the callee (no cleanup after the call).
bool __stdcall fn_00481a1c( int a, int b );

// HYPOTHESIS: fn_004d3331 returns a found flag and slot 39 returns an int; the shared headers declare both void.
// MATCH-ONLY: the calls go through casts until those headers are corrected.
// FUNCTION: STARFLEETOP 0x004819a4
void tState_Skirmish::fn_004819a4()
{
	field_0x1118 = "shipHull";
	if (!((bool (*)( const char * ))fn_004d3331)( "shipHull" ))
		fn_004d31ed( field_0x1118, 0, g_0084a8a0, 0x280, 0x1e0 );
	for (int id = 0x1001c; id <= 0x1001f; id++)
	{
		int a = fn_00481bf2( id );
		int (tObject_0092deb4::*f)( int ) = (int (tObject_0092deb4::*)( int ))&tObject_0092deb4::vfunction39;
		int b = (gObject_0092deb4->*f)( a );
		fn_004d34c5( id, !fn_00481a1c( b, a ) );
	}
}

// FUNCTION: STARFLEETOP 0x004817a9
void tState_Skirmish::vfunction6( int param_1 )
{
	field_0x1228.fn_0046b995();
}

// FUNCTION: STARFLEETOP 0x004817b7
void tState_Skirmish::vfunction15( c_0046aa10_message *param_1 )
{
	switch ( param_1->m_04 )
	{
	case 0x102:
		param_1->id = 0x103;
		break;
	case 0x103:
		param_1->id = field_0x1118 == "CustomSkmsh" ? 0x10022 : 0x104;
		break;
	}
}
