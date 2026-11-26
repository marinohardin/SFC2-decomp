// tState_Multiplayer: a game state of the multiplayer screens.
// Evidence:  class (RTTI; original file unknown)

#include "unsorted/tState_Multiplayer.h"

using namespace tState_Namespace_Multiplayer;

// FUNCTION: STARFLEETOP 0x0046ed64
char *tState_Multiplayer::vfunction13()
{
	return "multiplayer";
}

#include "unsorted/tNetControl.h"

// HYPOTHESIS: declared here until fn_004D3143.h (outside this lane) declares it.
void fn_004d3455( long arg1 );

// FUNCTION: STARFLEETOP 0x0046f160
void fn_0046f160()
{
	int local = gNetControl.fn_00431c79();
	int i = 0;
	do
	{
		if ( i != local - 1 )
		{
			fn_004d3455( i + 0x10064 );
			fn_004d3455( i + 0x1006a );
		}
		i++;
	} while ( i < 6 );
}

// FUNCTION: STARFLEETOP 0x0046f24b
int tState_Multiplayer::fn_0046f24b( int param_1 )
{
	unsigned int flags = field_2200;
	if ( ( flags & 8 ) && ( flags & 0x10 ) && ( flags & 0x20 ) && ( flags & 0x40 ) && ( flags & 0x80 ) && ( flags & 0x100 ) )
	{
		return 0;
	}
	while ( flags & ( 8 << param_1 ) )
	{
		param_1++;
		if ( param_1 >= 6 )
			param_1 = 0;
	}
	return param_1;
}

#include "unsorted/tObject_0092deb4.h"
#include "unsorted/tApplication.h"

long fn_004d33a8( long arg1, long arg2 );

// FUNCTION: STARFLEETOP 0x00470b59
void tState_Multiplayer::fn_00470b59( unsigned char param_1 )
{
	if ( param_1 > 0 )
	{
		do
		{
			if ( field_d90 == 4 )
			{
				field_d90 = 0;
				fn_004d33a8( 0x10063, 0 );
			}
			else if ( field_d90 == 3 )
			{
				field_d90 = 4;
				fn_004d33a8( 0x10063, 3 );
			}
			else if ( field_d90 == 2 )
			{
				field_d90 = 3;
				fn_004d33a8( 0x10063, 2 );
			}
			else if ( field_d90 == 0 )
			{
				field_d90 = 2;
				fn_004d33a8( 0x10063, 1 );
			}
			tShipUI *p = &field_e64;
			// MATCH-ONLY: the callee takes four words (ret 0x10); tShipUI.h declares three.
			void ( tShipUI::*f )( int, int, int, int ) = (void ( tShipUI::* )( int, int, int, int ))&tShipUI::fn_004cd1db;
			( p->*f )( gObject_0092deb4->vfunction39(), field_d90, (int)"HULL_TEMPLATE", field_0c );
			do
			{
				if ( fn_00473049( &p->m_0c[ p->m_100c ] ) )
					break;
				p->fn_004cd2f6();
			} while ( field_e64.m_100c != 0 );
			if ( fn_00473049( &p->m_0c[ p->m_100c ] ) )
				return;
			param_1--;
		} while ( param_1 != 0 );
	}
}

// FUNCTION: STARFLEETOP 0x00470c38
void tState_Multiplayer::fn_00470c38( unsigned char param_1 )
{
	if ( param_1 > 0 )
	{
		do
		{
			if ( field_d90 == 4 )
			{
				field_d90 = 3;
				fn_004d33a8( 0x10063, 2 );
			}
			else if ( field_d90 == 3 )
			{
				field_d90 = 2;
				fn_004d33a8( 0x10063, 1 );
			}
			else if ( field_d90 == 2 )
			{
				field_d90 = 0;
				fn_004d33a8( 0x10063, 0 );
			}
			else if ( field_d90 == 0 )
			{
				field_d90 = 4;
				fn_004d33a8( 0x10063, 3 );
			}
			tShipUI *p = &field_e64;
			// MATCH-ONLY: the callee takes four words (ret 0x10); tShipUI.h declares three.
			void ( tShipUI::*f )( int, int, int, int ) = (void ( tShipUI::* )( int, int, int, int ))&tShipUI::fn_004cd1db;
			( p->*f )( gObject_0092deb4->vfunction39(), field_d90, (int)"HULL_TEMPLATE", field_0c );
			do
			{
				if ( fn_00473049( &p->m_0c[ p->m_100c ] ) )
					break;
				p->fn_004cd2f6();
			} while ( field_e64.m_100c != 0 );
			if ( fn_00473049( &p->m_0c[ p->m_100c ] ) )
				return;
			param_1--;
		} while ( param_1 != 0 );
	}
}

char *__stdcall fn_00473194( tState_Multiplayer *param_1 );

// FUNCTION: STARFLEETOP 0x00470d1b
unsigned int __stdcall fn_00470d1b( tState_Multiplayer *param_1, unsigned int param_2 )
{
	int *p = (int *)fn_00473194( param_1 );
	if ( p != 0 )
	{
		unsigned int i;
		for ( i = 0, p += 1; i < 6; i++, p += 0x17 )
		{
			if ( (int)i < (int)param_2 && *p == -1 )
				return i;
		}
	}
	return param_2;
}

long fn_004d338b( long arg1 );

// HYPOTHESIS: a table of pointers the new object is entered in.
extern char **g_0090b73c;

// FUNCTION: STARFLEETOP 0x00470d4c
void tState_Multiplayer::fn_00470d4c()
{
	char *p;
	if ( !fn_004d338b( 0x1003d ) )
		p = field_1fa4;
	else
	{
		char *entry;
		int index = fn_00472e88( &entry, *(int *)&field_1f78[ 0xc ] );
		if ( !entry )
			goto done;
		p = entry + 0x40;
		g_0090b73c[ index ] = entry;
	}
	if ( p )
	{
		unsigned int i;
		char *dst = p + 0x30;
		tValueI< tString > *src = field_af8;
		for ( i = 0; i < 6; i++, src++, dst += 0x5c )
		{
			*(tString *)dst = *src;
			if ( p == field_1fa4 )
			{
				void ( tObject_0092deb4::*f )( void *, int ) = (void ( tObject_0092deb4::* )( void *, int ))&tObject_0092deb4::vfunction20;
				( gObject_0092deb4->*f )( dst, i );
			}
		}
	}
done:
	fn_00472356();
}
