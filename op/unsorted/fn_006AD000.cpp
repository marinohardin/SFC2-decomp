// Unit:      unsorted/fn_006AD000.cpp
// Functions: 0x006ad130 to 0x006ad3e0, 9 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "unsorted/tView3DExt.h"

// FUNCTION: STARFLEETOP 0x006ad130
void tView3DExt::mVtxCoord( const tPoint3 &point )
{
	if ( field_0x1eec.field_0x00 != -1 )
	{
		fn_006abe00_Vtx *vtx = &mVtxCache[ field_0x1eec.field_0x04 ];
		// HYPOTHESIS: +0x04..+0x10 of the vertex is a tPoint4, the out-argument of 0x0069ca30
		field_0x6b0.mTransform( *(tPoint4 *)&vtx->field_0x04, point );
		vtx->field_0x00 = 1;
		if ( field_0x1ecc == 3 )
		{
			vtx->field_0x34 = field_0x1eec.field_0x1c.x;
			vtx->field_0x38 = field_0x1eec.field_0x1c.y;
			vtx->field_0x3c = field_0x1eec.field_0x1c.z;
		}
		else
		{
			if ( field_0x1eec.field_0x34 )
			{
				field_0x1eec.field_0x34 = 0;
				field_0x1eec.field_0x28 = field_0x1eec.field_0x1c.x * field_0x1eec.field_0x10.x;
				field_0x1eec.field_0x2c = field_0x1eec.field_0x1c.y * field_0x1eec.field_0x10.y;
				field_0x1eec.field_0x30 = field_0x1eec.field_0x1c.z * field_0x1eec.field_0x10.z;
			}
			vtx->field_0x34 = field_0x1eec.field_0x28;
			vtx->field_0x38 = field_0x1eec.field_0x2c;
			vtx->field_0x3c = field_0x1eec.field_0x30;
		}
		vtx->field_0x2c = field_0x1eec.field_0x08;
		vtx->field_0x30 = field_0x1eec.field_0x0c;
		if ( ++field_0x1eec.field_0x04 >= 0x150 )
			mFlushPrimitiveVtxs( 0 );
	}
}

// FUNCTION: STARFLEETOP 0x006ad230
void tView3DExt::mVtxCoord( float x, float y, float z )
{
	tPoint3 point( x, y, z );
	mVtxCoord( point );
}

// FUNCTION: STARFLEETOP 0x006ad260
void tView3DExt::mVtxNormal( const tPoint3 &point )
{
	if ( field_0x1ecc != 3 )
	{
		tPoint3 zero( 0.0f, 0.0f, 0.0f );
		field_0x1eec.field_0x10 = mCalcLightColor( zero, point );
		field_0x1eec.field_0x34 = 1;
	}
}

// FUNCTION: STARFLEETOP 0x006ad2d0
void tView3DExt::mVtxNormal( float x, float y, float z )
{
	tPoint3 point( x, y, z );
	mVtxNormal( point );
}

// FUNCTION: STARFLEETOP 0x006ad300
void tView3DExt::mVtxTexCoord( const tPoint2 &point )
{
	field_0x1eec.field_0x08 = point.x;
	field_0x1eec.field_0x0c = point.y;
}

// FUNCTION: STARFLEETOP 0x006ad320
void tView3DExt::mVtxTexCoord( float x, float y )
{
	field_0x1eec.field_0x08 = x;
	field_0x1eec.field_0x0c = y;
}

// FUNCTION: STARFLEETOP 0x006ad340
void tView3DExt::mVtxColor( float r, float g, float b )
{
	field_0x1eec.field_0x34 = 1;
	field_0x1eec.field_0x1c.mSet( r, g, b );
}

// FUNCTION: STARFLEETOP 0x006ad370
void tView3DExt::mVtxColor( tGLRgbQuad color )
{
	field_0x1eec.field_0x34 = 1;
	field_0x1eec.field_0x1c.mSet( color.field_0x02 * ( 1.0f / 255.0f ), color.field_0x01 * ( 1.0f / 255.0f ), color.field_0x00 * ( 1.0f / 255.0f ) );
}

// FUNCTION: STARFLEETOP 0x006ad3e0
void tView3DExt::mPrimitiveTexID( int value )
{
	if ( (unsigned int)value > 0x1000 )
		field_0x1eec.field_0x38 = 0;
	else
		field_0x1eec.field_0x38 = value;
}
