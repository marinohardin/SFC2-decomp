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
