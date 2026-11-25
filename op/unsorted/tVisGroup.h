//	tVisGroup.h: tVisGroup, a visual object that holds others.
// Evidence:  class (RTTI: tVisGroup type descriptor 0x0084d040, vftable 0x007b7d18, 40 slots,
// base tVisObject; original file unknown). Object size 0xdc: the constructor 0x0067da80 and the
// allocation sites of its derived classes agree on the layout below.

#ifndef tVisGroup_h
#define tVisGroup_h 1

#include "unsorted/tVisObject.h"

// HYPOTHESIS: tFleetEntry.h still declares a reduced tVisGroup of its own; it must include this
// header instead (HEADER REQUEST in the lane notes). Slot n of the vftable is vfunction(n+1),
// as the analysis names them; slots 1 to 21 are tObjBase's.
class tVisGroup : public tVisObject
{
public:
	tVisGroup();
	virtual ~tVisGroup();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();	// 0x0067dd00
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();	// 0x0067e340
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();	// 0x0067dcf0

	int field_64;	// HYPOTHESIS: the constructor never writes it
	// HYPOTHESIS: an 0x74-byte member with its own vftable 0x007b7dbc, constructed in place by
	// the constructor and destroyed by 0x0067f490 (the same type sits at tButton +0x74, whose
	// destructor 0x0067f490 also runs); layout in the lane notes.
	unsigned char field_68[ 0xdc - 0x68 ];
};

typedef char tVisGroup_size_is_0xdc[ sizeof( tVisGroup ) == 0xdc ? 1 : -1 ];

#endif	// tVisGroup_h
