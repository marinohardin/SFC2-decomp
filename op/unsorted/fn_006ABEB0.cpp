#include "unsorted/tView3DExt.h"

// FUNCTION: STARFLEETOP 0x006abeb0
float tView3DExt::mGetPolyNormalZ()
{
	fn_006abe00_Vtx *a = mVtxCache + field_0x14[0];
	fn_006abe00_Vtx *b = mVtxCache + field_0x14[1];
	fn_006abe00_Vtx *c = mVtxCache + field_0x14[2];
	float ia, ib, ic;

	if ( a->field_0x00 & 0x10 )
		ia = a->field_0x20;
	else
	{
		ia = 1.0f / a->field_0x10;
		a->field_0x00 |= 0x10;
		a->field_0x20 = ia;
	}

	if ( b->field_0x00 & 0x10 )
		ib = b->field_0x20;
	else
	{
		ib = 1.0f / b->field_0x10;
		b->field_0x00 |= 0x10;
		b->field_0x20 = ib;
	}

	if ( c->field_0x00 & 0x10 )
		ic = c->field_0x20;
	else
	{
		ic = 1.0f / c->field_0x10;
		c->field_0x00 |= 0x10;
		c->field_0x20 = ic;
	}

	float x0 = a->field_0x04 * ia;
	float y0 = a->field_0x08 * ia;

	return ( c->field_0x04 * ic - x0 ) * ( b->field_0x08 * ib - y0 )
		- ( c->field_0x08 * ic - y0 ) * ( b->field_0x04 * ib - x0 );
}

// FUNCTION: STARFLEETOP 0x006abf80
void tView3DExt::mSubmitVertices( int count, const tPoint3 *param_2 )
{
	mAssertVtxCache( count );
	field_0x554 = param_2;
	fn_006abe00_Vtx *vtx = mVtxCache;
	for ( int i = 0; i < count; i++ )
	{
		vtx->field_0x00 = 0;
		vtx++;
	}
}

// FUNCTION: STARFLEETOP 0x006abfb0
void tView3DExt::vfunction80( void *param_1 )
{
	field_0x558 = param_1;
}
