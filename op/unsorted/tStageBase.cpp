#include "unsorted/tVisObject.h"

// HYPOTHESIS: tFillObj has no header yet; 0x64 bytes, vtable 0x007b89f0, a value at +0x60.
class tFillObj : public tObjBase
{
public:
	void *m60;
};

// HYPOTHESIS: the owner's class and layout are not recovered; +0x14 points at a record whose
// +0x64 holds four shorts.
class fn_00693ef0_Owner
{
public:
	char pad_00[0x10];
	void *m10;
	char *m14;
	char pad_18[0x4];
	void fn_00693fc0( int a, int b, int c, int d, short *r, int m );
	bool fn_00693ef0( void *arg );
	void fn_00693e90( tObjBase *obj );
};

// FUNCTION: STARFLEETOP 0x00693ef0
bool fn_00693ef0_Owner::fn_00693ef0( void *arg )
{
	tFillObj *obj = new tFillObj;
	if ( obj != 0 )
	{
		obj->m60 = arg;
		short *r = (short *)( m14 + 0x64 );
		obj->vfunction9( (int)r[0], (int)r[1], (int)r[2], (int)r[3] );
		fn_00693e90( obj );
		return true;
	}
	return false;
}

// HYPOTHESIS: the callee's real class and parameter types are not recovered.
void fn_006969c0( int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m );

// FUNCTION: STARFLEETOP 0x00693fc0
void fn_00693ef0_Owner::fn_00693fc0( int a, int b, int c, int d, short *r, int m )
{
	if ( m10 != 0 )
	{
		char *p = m14;
		if ( p != 0 )
		{
			unsigned int w = *(short *)( p + 0x6a ) - *(short *)( p + 0x66 );
			w = ( ( (int)w < 0 ) - 1 ) & w;
			fn_006969c0( a, b, c, d, r[0], r[1], r[2], r[3], *(int *)( p + 0x60 ), *(int *)( p + 0x70 ), w, *(unsigned char *)( p + 0x74 ), m );
		}
	}
}
