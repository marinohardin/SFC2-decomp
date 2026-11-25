#include "Common/Thread/Optex.h"

// HYPOTHESIS: the original is tMetaClientPlayerListPanel::tRemoveItem; the outer class is declared
// only in another unit and has no header yet, so the item class stands alone here. Only what this
// destructor touches is declared: a cleanup member at 0x0060432e and a tOptex at +0x4.
class tRemoveItem
{
public:
	~tRemoveItem();
	void fn_0060432e();

protected:
	int32u	field_00;
	tOptex	field_04;
};

// FUNCTION: STARFLEETOP 0x006041a1
tRemoveItem::~tRemoveItem()
{
	fn_0060432e();
}
