#include "unsorted/tEntity_Seeker.h"
#include "unsorted/tEntityMgr.h"
#include "unsorted/tObject_0040ce21.h"

extern tEntityMgr g_008e1060;

// HYPOTHESIS: the object tEntity_Weapon keeps at +0x1f0 (declared there as an int); only its
// virtual slot 10, called without arguments, is known.
class fn_004204b2_Object
{
public:
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
};

// FUNCTION: STARFLEETOP 0x004204b2
void tEntity_HellboreSeeker::vfunction5()
{
	fn_004204b2_Object *object = (fn_004204b2_Object *)field_0x1f0;
	if (object)
		object->vfunction11();
	tEntity_HellboreBlast *blast = new tEntity_HellboreBlast(fTargetID, field_0x26c);
	if (blast)
	{
		static tObject_0040ce21 marker("Let em Have it!");
		blast->fID = g_008e1060.mGetNextPlayerID();
		blast->fn_0041089e(&fCurrPos);
		blast->vfunction13(fTargetID);
		g_008e1060.mAddEntity(blast);
	}
	tEntity_Seeker::vfunction5();
}
