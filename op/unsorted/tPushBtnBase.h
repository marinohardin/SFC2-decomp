//	tPushBtnBase.h: tPushBtnBase, the base of the push buttons.
// Evidence:  class (RTTI: tPushBtnBase type descriptor 0x00854808; vftable 0x007a3e20, 47 slots,
// and a second one, 0x007a3e14, for the object at +0x110; its derived classes tPushBtn,
// tPushRect and tTextButton). Object size 0x11c.

#ifndef tPushBtnBase_h
#define tPushBtnBase_h 1

#include "unsorted/tValueBtn.h"	// tButton

// HYPOTHESIS: the original's chain is tPushBtnBase : tButton, tActionUser, with tControl and
// tVisStateObject between tVisObject and tButton; tValueBtn.h declares tButton on tVisObject,
// filled to 0x110, which gives the same layout. tActionUser has no declaration yet (HEADER
// REQUEST in the lane notes), so its 12 bytes at +0x110 are a placeholder: vftable 0x007a3e14
// (2 slots: 0x00686780, 0x004c8e6b), then two words the constructor 0x00687060 sets.
class tPushBtnBase : public tButton
{
public:
	tPushBtnBase();
	virtual ~tPushBtnBase();	// slot 3; the deleting form is 0x004c7e19, the body 0x004c7db7
	// Slots 1 to 21 are tObjBase's; the slot n of the vftable is vfunction(n+1).
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	virtual void vfunction39();	// 0x00686770
	virtual void vfunction40();
	virtual void vfunction41();
	virtual void vfunction42();
	virtual void vfunction43();	// 0x00686640
	virtual void vfunction44();	// 0x006866f0
	virtual void vfunction45();	// 0x00686720
	virtual void vfunction46();
	virtual void vfunction47();	// 0x00686750

	// HYPOTHESIS: slot 0 of the second vftable (0x007a3e14): a thunk that takes this + 0x110, one
	// argument, and calls vfunction15 of the whole object (+0x38) with 0 when 1 or 3 is passed
	// and the word at +4 of the sub-object has its short at +0x18 clear.
	void fn_00686780( int a );

	unsigned char field_110[ 0x11c - 0x110 ];	// HYPOTHESIS: the tActionUser part
};

typedef char tPushBtnBase_size_is_0x11c[ sizeof( tPushBtnBase ) == 0x11c ? 1 : -1 ];

#endif	// tPushBtnBase_h
