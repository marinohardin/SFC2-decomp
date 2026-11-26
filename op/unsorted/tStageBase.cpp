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
	char pad_00[0x14];
	char *m14;
	char pad_18[0x4];
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
