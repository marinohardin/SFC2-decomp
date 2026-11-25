#include "Common/Thread/Optex.h"

// HYPOTHESIS: the original is tMetaClientPlayerListPanel::tRemoveItem; the outer class is declared
// only in another unit and has no header yet, so the item class stands alone here. Only what this
// destructor touches is declared: a cleanup member at 0x0060432e and a tOptex at +0x4.
// HYPOTHESIS: a base holding the word at +0, then tOptex as a second base at +0x4.
class tRemoveItemBase
{
protected:
	int32u	field_00;
};

class tRemoveItem : public tRemoveItemBase, public tOptex
{
public:
	~tRemoveItem();
	void fn_0060432e();
};

// FUNCTION: STARFLEETOP 0x006041a1
tRemoveItem::~tRemoveItem()
{
	fn_0060432e();
}

// HYPOTHESIS: a sibling item class of tRemoveItem with the same layout; its cleanup member is at 0x0060435f.
class tRemoveItem_0060435f : public tRemoveItemBase, public tOptex
{
public:
	~tRemoveItem_0060435f();
	void fn_0060435f();
};

// FUNCTION: STARFLEETOP 0x006041d0
tRemoveItem_0060435f::~tRemoveItem_0060435f()
{
	fn_0060435f();
}

// HYPOTHESIS: a third sibling item class with the same layout; its cleanup member is at 0x00604390.
class tRemoveItem_00604390 : public tRemoveItemBase, public tOptex
{
public:
	~tRemoveItem_00604390();
	void fn_00604390();
};

// FUNCTION: STARFLEETOP 0x006041ff
tRemoveItem_00604390::~tRemoveItem_00604390()
{
	fn_00604390();
}

// HYPOTHESIS: the SDK's tAccessPointer, whose destructor releases the lock through Done
// (0x005f4352) and then destroys the tOptex it holds first.
class tAccessPointer_0060422e
{
public:
	tOptex	field_00;
	~tAccessPointer_0060422e();
	void Done();
};

// FUNCTION: STARFLEETOP 0x0060422e
tAccessPointer_0060422e::~tAccessPointer_0060422e()
{
	Done();
}
