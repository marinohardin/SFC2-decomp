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

using namespace tState_Namespace_Multiplayer;

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
