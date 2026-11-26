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
		tVisObject *parent = field_0C;
		if ( parent && fn_RectNotEmpty( &field_34 ) )
			parent->vfunction32( &field_34 );
		field_3C.x += (short)dx;
		field_3C.y += (short)dy;
		field_34.left += (short)dx;
		field_34.right += (short)dx;
		field_34.top += (short)dy;
		field_34.bottom += (short)dy;
		tRect *r = &field_34;
		for ( tVisObject *child = field_24; child; child = child->field_04 )
			child->fn_0067cd70( dx, dy );
		parent = field_0C;
		if ( parent && fn_RectNotEmpty( r ) )
			parent->vfunction32( r );
	}
}

// FUNCTION: STARFLEETOP 0x0067ce20
void tObjBase::vfunction18()
{
	if ( field_40 & 1 )
	{
		for ( tVisObject *child = field_24; child; child = child->field_04 )
			child->vfunction18();
		tVisObject *parent = field_0C;
		if ( parent && fn_RectNotEmpty( &field_34 ) )
			parent->vfunction32( &field_34 );
		field_40 &= 0xfffffffe;
		parent = field_0C;
		if ( parent && ( field_2C || parent->field_2C == this ) )
		{
			parent->fn_0067d310( this );
			field_2C = 0;
		}
	}
}

// FUNCTION: STARFLEETOP 0x0067ce90
void tVisObject::fn_0067ce90()
{
	tVisObject *parent = field_0C;
	if ( parent && fn_RectNotEmpty( &field_34 ) )
		parent->vfunction32( &field_34 );
	field_40 |= 1;
	for ( tVisObject *child = field_24; child; child = child->field_04 )
		child->fn_0067ce90();
}

// HYPOTHESIS: the object at 0x00c690d0; 0x00674980 takes the object.
class fn_00674980_Owner
{
public:
	void fn_00674980( tObjBase *object );
};

extern fn_00674980_Owner *g_00c690d0;

// FUNCTION: STARFLEETOP 0x0067cee0
void tObjBase::fn_0067cee0()
{
	field_40 |= 0x20;
	if ( field_0C )
		field_0C->vfunction6( this, 1 );
	if ( g_00c690d0 )
		g_00c690d0->fn_00674980( this );
}

// FUNCTION: STARFLEETOP 0x0067cf10
bool tObjBase::vfunction36( int x, int y )
{
	if ( field_34.left <= (short)x && field_34.top <= (short)y && field_34.right > (short)x && field_34.bottom > (short)y )
	{
		if ( field_4C == 0 )
			return true;
		if ( !( field_40 & 2 ) )
			return fn_00696b40( field_4C, x - field_3C.x, y - field_3C.y );
		return true;
	}

	return false;
}

// FUNCTION: STARFLEETOP 0x0067cf70
void tObjBase::fn_0067cf70( c_tObjBase_Field4C *shape, unsigned char visible )
{
	if ( field_0C && fn_RectNotEmpty( &field_34 ) )
		field_0C->vfunction32( &field_34 );
	if ( visible != 0 )
		field_40 |= 2;
	else
		field_40 &= 0xfffffffd;
	field_4C = shape;
	if ( shape != 0 )
	{
		field_34.left = field_3C.x - shape->field_04;
		field_34.top = field_3C.y - shape->field_06;
		field_34.right = shape->field_00 + field_34.left;
		field_34.bottom = shape->field_02 + field_34.top;
		if ( field_0C && fn_RectNotEmpty( &field_34 ) )
			field_0C->vfunction32( &field_34 );
	}
	else
	{
		field_34.left = 0;
		field_34.top = 0;
		field_34.right = 0;
		field_34.bottom = 0;
	}
}

// FUNCTION: STARFLEETOP 0x0067d160
void tObjBase::vfunction30()
{
	field_18 = field_24;
	field_1C &= 0xfffffffe;
	while ( field_18 )
	{
		if ( field_18->field_40 & 8 )
		{
			if ( field_18->field_58 )
				field_18->field_58( field_18 );
			field_18->vfunction30();
		}
		if ( field_1C & 1 )
			field_1C &= 0xfffffffe;
		else
			field_18 = field_18->field_04;
	}
	field_18 = 0;
}

// FUNCTION: STARFLEETOP 0x0067d2d0
bool tVisObject::fn_0067d2d0( tVisObject *object )
{
	if ( field_2C != 0 )
		return false;
	if ( field_0C != 0 )
	{
		if ( field_0C->fn_0067d2d0( this ) )
		{
			field_2C = object;
			return true;
		}
		return false;
	}
	field_2C = object;
	return true;
}

// FUNCTION: STARFLEETOP 0x0067d310
bool tObjBase::fn_0067d310( tObjBase *child )
{
	if ( field_2C != child || field_2C == 0 )
		return false;
	if ( field_0C == 0 || field_0C->fn_0067d310( this ) )
	{
		field_2C = 0;
		return true;
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0067d350
bool tObjBase::vfunction2( tObjBase *a )
{
	if ( field_30 != 0 )
	{
		if ( vfunction3() )
		{
			if ( a != this )
				field_30 = a;
			return true;
		}
	}
	else if ( field_0C != 0 )
	{
		if ( field_0C->vfunction2( this ) )
		{
			if ( a != this )
				field_30 = a;
			return true;
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0067d3a0
bool tObjBase::vfunction3()
{
	if ( field_30 != 0 )
	{
		if ( field_30->vfunction3() )
		{
			field_30 = 0;
			return true;
		}
		return false;
	}
	vfunction13();
	if ( field_0C != 0 )
	{
		tRect *r = &field_34;
		if ( r->left < r->right && r->top < r->bottom )
			field_0C->vfunction32( r );
	}
	return true;
}
