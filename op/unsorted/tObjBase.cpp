// tObjBase.cpp: the class declaration only; the functions are written one at a time.

#include "unsorted/tObjBase.h"

// FUNCTION: STARFLEETOP 0x0067cb70
tObjBase::tObjBase()
{
	field_5C = -1;
	field_0C = 0;
	field_08 = 0;
	field_04 = 0;
	field_10 = 0;
	field_14 = 0;
	field_20 = 0;
	field_2C = 0;
	field_28 = 0;
	field_24 = 0;
	field_30 = 0;
	field_3C.y = 0;
	field_3C.x = 0;
	field_34.mSet( 0, 0, 0, 0 );
	field_40 = field_40 & 0xffffffc1 | 1;
	field_44 = -1;
	field_48 = -1;
	field_4C = 0;
	field_54 = 0;
	field_58 = 0;
	field_50 = 0;
}

// FUNCTION: STARFLEETOP 0x0067cbf0
void tObjBase::vfunction32( const tRect *area )
{
	if ( field_0C )
		field_0C->vfunction32( area );
}

// FUNCTION: STARFLEETOP 0x0067cc10
void tObjBase::vfunction33( tRect *area )
{
	if ( field_40 & 1 )
	{
		area->fn_006985f0( &field_34 );
		for ( tVisObject *child = field_24; child; child = child->field_04 )
			child->vfunction33( area );
	}
}

// MATCH-ONLY: the original inlined this test; it has no address of its own.
inline bool fn_RectNotEmpty( const tRect *r )
{
	return r->left < r->right && r->top < r->bottom;
}

// FUNCTION: STARFLEETOP 0x0067cc70
void tObjBase::vfunction9( short left, short top, short right, short bottom )
{
	if ( field_0C && fn_RectNotEmpty( &field_34 ) )
		field_0C->vfunction32( &field_34 );
	field_34 = tRect( left, top, right, bottom );
	if ( field_0C )
	{
		field_34 = field_0C->vfunction34( field_34 );
		if ( field_0C && fn_RectNotEmpty( &field_34 ) )
			field_0C->vfunction32( &field_34 );
	}
}

// FUNCTION: STARFLEETOP 0x0067cd50
void tObjBase::vfunction7( int x, int y )
{
	fn_0067cd70( x - field_3C.x, y - field_3C.y );
}

// FUNCTION: STARFLEETOP 0x0067cd70
void tObjBase::fn_0067cd70( int dx, int dy )
{
	if ( dy || dx )
	{
		if ( field_0C && fn_RectNotEmpty( &field_34 ) )
			field_0C->vfunction32( &field_34 );
		field_3C.x += (short)dx;
		field_3C.y += (short)dy;
		field_34.left += (short)dx;
		field_34.right += (short)dx;
		field_34.top += (short)dy;
		field_34.bottom += (short)dy;
		for ( tVisObject *child = field_24; child; child = child->field_04 )
			child->fn_0067cd70( dx, dy );
		tRect *r = &field_34;
		tVisObject *parent = field_0C;
		if ( parent && fn_RectNotEmpty( r ) )
			parent->vfunction32( r );
	}
}
