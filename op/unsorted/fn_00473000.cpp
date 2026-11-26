// FUNCTION: STARFLEETOP 0x0047301e
// HYPOTHESIS: stdcall free function (the original returns with ret 4)
int __stdcall FUN_0047301e(int param_1)
{
	int result;

	switch (param_1) {
	case 0x10052:
		result = 4;
		break;
	case 0x10053:
		result = 3;
		break;
	case 0x10054:
		result = 2;
		break;
	case 0x10055:
		result = 0;
		break;
	default:
		result = -1;
		break;
	}
	return result;
}

#include "unsorted/tState_Multiplayer.h"

#include "unsorted/tObject_0090b730.h"
#include "unsorted/tObject_0092deb4.h"

using namespace tState_Namespace_Multiplayer;

extern const char *g_00847e78;	// HYPOTHESIS: the name of the "HostSettings" control group
void fn_004d3340( const char *pszName );

char *__stdcall fn_00473194( tState_Multiplayer *param_1 );

// FUNCTION: STARFLEETOP 0x00473141
int __stdcall FUN_00473141( tState_Multiplayer *param_1 )
{
	int count = 0;
	char *p = fn_00473194( param_1 );
	unsigned int i;
	char *q;
	for ( i = 0, q = p; i < 6; i++, q += 0x5c )
	{
		if ( p == 0 )
			break;
		if ( *q != 0 )
			count++;
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x0047316a
int tState_Multiplayer::FUN_0047316a( tState_Multiplayer *param_1 )
{
	int count = 0;
	char *p = fn_00473194( param_1 );
	unsigned int i;
	int *q;
	for ( i = 0, q = (int *)( p + 4 ); i < 6; i++, q += 0x17 )
	{
		if ( p == 0 )
			break;
		if ( *q != -1 )
			count++;
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x0047328b
void tState_Multiplayer::fn_0047328b()
{
	gObject_0090b730.fn_0060ca42( 2 );
	field_1f78[0] = 0;
	fn_004d3340( g_00847e78 );
	fn_00472356();
	int count = gObject_0092deb4->vfunction9();
	for ( int i = 0; i < count; i++ )
	{
		gObject_0092deb4->vfunction10( i, 0, 0 );
	}
}

long fn_004d338b( long arg1 );
void fn_004d34c5( int id, bool enable );

// FUNCTION: STARFLEETOP 0x0047339f
void tState_Multiplayer::fn_0047339f()
{
	tState_Multiplayer *p;
	if ( fn_004d338b( 0x1003d ) == 0 )
		p = *(tState_Multiplayer **)&field_1f78[ 8 ];
	else
		p = *(tState_Multiplayer **)&field_1f78[ 0xc ];
	bool enable;
	if ( FUN_0047316a( p ) == 0 )
		enable = true;
	else
		enable = ( ~(unsigned char)field_2200 & 1 ) != 0;
	fn_004d34c5( 0x10062, enable );
}

// FUNCTION: STARFLEETOP 0x0047378b
int __stdcall FUN_0047378b( int param_1 )
{
	return param_1;
}

// FUNCTION: STARFLEETOP 0x00473792
long __stdcall FUN_00473792( long param_1 )
{
	if ( param_1 < 0x1000e || param_1 > 0x10013 )
		return -1;
	return fn_004d338b( param_1 );
}

// FUNCTION: STARFLEETOP 0x004737bd
tState_Namespace_Multiplayer::tState_Multiplayer g_tState_Multiplayer;
