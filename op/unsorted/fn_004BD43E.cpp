//	fn_004BD43E.cpp: functions of the page 0x004bd000 with no unit or class evidence.
// Profile:   game-release (harness/units.json)

#include "unsorted/tListBox_Base.h"

// FUNCTION: STARFLEETOP 0x004bd443
int tListBox_Base::fn_004bd443( tString *text, int width )
{
	if ( fn_004be24a( text ) < width || width <= 0 )
		return -1;

	tString space( " " );
	int spaceWidth = field_B0->vfunction3( space.mWideStr(), 1 );
	unsigned short *chars = text->fn_00618920();
	int length = text->mStrLen();
	int lineWidth = fn_004be23f();
	int i = 0;
	while ( field_B0->vfunction3( chars, i ) + spaceWidth < lineWidth && i <= length )
		i++;

	if ( chars[ i ] != ' ' )
	{
		int n = 1;
		while ( chars[ i - n ] != ' ' )
			n++;
		if ( chars[ i - n ] == ' ' )
			i -= n;
	}
	return i;
}
