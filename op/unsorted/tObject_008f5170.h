//	tObject_008f5170.h: the object at 0x008f5170, which keeps entities by map cell.
// Evidence:  calls (original name and file unknown)

#ifndef tObject_008f5170_h
#define tObject_008f5170_h 1

#include <vector>
#include "unsorted/tEntity.h"

class tObject_00425726_list;

// HYPOTHESIS: the class is inferred from its callers only. fn_004259bd turns a position into a
// map cell's two indices, fn_00425726 returns the entity list for a cell, and fn_004255b5 returns
// the entities within a range of a point, as a vector of pointers.
class tObject_008f5170
{
public:
	void fn_004257f5(tEntity *pEntity, int32 value);
	void fn_004258de(tEntity *pEntity, int32 kind, int32 value);
	void fn_004259bd(float x, float y, int32 *pColumn, int32 *pRow);
	tObject_00425726_list *fn_00425726(int32 column, int32 row);
	std::vector<tPhysicalEntity *> *fn_004255b5(tObject_00412f08 aPoint, float aRange);
};

extern tObject_008f5170 gObject_008f5170;

#endif // tObject_008f5170_h
