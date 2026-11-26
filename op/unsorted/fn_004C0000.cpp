#include "unsorted/tVisObject.h"

// FUNCTION: STARFLEETOP 0x004c000f
void tRect::fn_004c000f( const tRect *pR )
{
	if ( pR->left < left )		left	= pR->left;
	if ( pR->top < top )		top		= pR->top;
	if ( pR->right > right )	right	= pR->right;
	if ( pR->bottom > bottom )	bottom	= pR->bottom;
}
