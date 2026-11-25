//	tSlider.h: tSlider, the slider control.
// Evidence:  class (RTTI: tSlider type descriptor 0x008547d0; vftable 0x007b84a0, 56 slots, and a
// second one, 0x007b8584, for the tVisNotifiee at +0xe4). Object size 0x124 (the allocation at
// 0x00685718); constructor 0x00687ba0.

#ifndef tSlider_h
#define tSlider_h 1

#include "unsorted/tVisObject.h"

// HYPOTHESIS: the original's bases are tControl (0xe4 bytes, over tVisStateObject and
// tVisObject) and tVisNotifiee (0x30 bytes at +0xe4: vftable, a word, and a tString at +8).
// Neither has a declaration that fits (tControl has none; tFleetEntry.h's tVisNotifiee is four
// bytes), so tSlider is declared on tVisObject and filled; HEADER REQUEST in the lane notes.
class tSlider : public tVisObject
{
public:
	tSlider();
	virtual ~tSlider();	// slot 3; the deleting form is 0x0068a270, the body 0x0068a290
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
	virtual void vfunction39();
	virtual void vfunction40();
	virtual void vfunction41();
	virtual void vfunction42();
	virtual void vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual void vfunction55();
	virtual void vfunction56();

	// HYPOTHESIS: overrides, at the addresses of the analysis: vfunction20 0x0068a260,
	// vfunction21 0x00687fc0, vfunction37 0x0068a250, vfunction48 0x00688160, vfunction49
	// 0x00687ea0, vfunction50 0x006880c0, vfunction51 0x00687f80, vfunction53 and vfunction54
	// 0x0068a240, vfunction55 0x00688070, vfunction56 0x00688020.

	// HYPOTHESIS: the slots of the tVisNotifiee at +0xe4 (vftable 0x007b8584; this points at +0xe4
	// when they run): slot 0 is 0x006881c0, slot 1 is 0x0068a330.
	void fn_006881c0();
	void fn_0068a330();
	// HYPOTHESIS: unclassed in the symbol table; it lies between tSlider's constructor and its
	// slot functions.
	void fn_00687c30();

	unsigned char field_64[ 0xe4 - 0x64 ];	// HYPOTHESIS: tControl's members
	// HYPOTHESIS: the tVisNotifiee: vftable 0x007b8584 (0x007a1eec, 0x007a0ecc before the
	// constructor ends), field_E8 = 0, a tString constructed at +0xec by 0x006230d0.
	unsigned char field_E4[ 0x114 - 0xe4 ];
	int field_114;	// 0 in the constructor
	int field_118;	// 0 in the constructor
	int field_11C;	// 0 in the constructor
	int field_120;	// not written by the constructor
};

typedef char tSlider_size_is_0x124[ sizeof( tSlider ) == 0x124 ? 1 : -1 ];

#endif	// tSlider_h
