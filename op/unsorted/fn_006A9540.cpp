#include "unsorted/tView3DExt.h"

// FUNCTION: STARFLEETOP 0x006a9540
void tView3DExt::fn_006a9540()
{
	mAssertVtxCache( 0x150 );
	field_0x554 = 0;
	field_0x558 = NULL;
	vfunction23( 7 );
	vfunction19( 1 );
	vfunction21( 5 );
	field_0x1ed0 = 1;
	vfunction45( 1.0f );
	vfunction47( false );
	fn_0040105e_Vector v( 1.0f, 0.0f, 10.0f );
	vfunction35( &v );
	field_0x1eb8 = true;
	field_0x1edc = NULL;
	field_0x1ee4 = NULL;
	field_0x1ee0 = NULL;
	field_0x1ee8 = NULL;
	field_0x1eec.fn_006ac780();
}
