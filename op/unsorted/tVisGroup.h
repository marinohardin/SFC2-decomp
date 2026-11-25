//	tVisGroup.h: tVisGroup, a visual object that holds others.
// Evidence:  class (RTTI: tVisGroup type descriptor 0x0084d040, vftable 0x007b7d18, 40 slots,
// base tVisObject; original file unknown). Object size 0xd4: the highest store of the constructor 0x0067da80 is the
// word at +0xd0, and tRadioGroup puts its second vftable at +0xd4 (the exact tFleetEntry constructor).

#ifndef tVisGroup_h
#define tVisGroup_h 1

#include "unsorted/tVisObject.h"

class tVisGroup;

// HYPOTHESIS: the 0x6c-byte member at +0x68; 0x0067f010 takes the group and a pointer into an
// event record, and answers whether the member handled it.
class c_tVisGroup_Field68
{
public:
	bool fn_0067f010( tVisGroup *group, int *data );
};

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
	// HYPOTHESIS: the event handler of tObjBase (slot 31), as in tListBox_Base; 0x0067d1c0 is the base's.
	virtual bool vfunction31( int *event );	// 0x0067dd00
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();	// 0x0067e340
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();	// 0x0067dcf0
	// HYPOTHESIS: past slot 40 the real vtable is the derived class's; these are the slots the
	// code that goes through a tVisGroup pointer calls (kept from the reduced declaration that
	// tFleetEntry.h held before), and tSeqScene overrides 41 to 44.
	virtual void vfunction41(int);
	virtual void vfunction42();
	virtual void vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48( bool b );
	virtual void vfunction49( bool b );

	// HYPOTHESIS: tObjBase::vfunction31 (0x0067d1c0), which tObjBase.h does not declare yet.
	bool fn_0067d1c0( int *event );

	int field_64;	// HYPOTHESIS: the constructor never writes it
	// HYPOTHESIS: a 0x6c-byte member with its own vftable 0x007b7dbc, constructed in place by
	// the constructor and destroyed by 0x0067f490 (the same type sits at tButton +0x74, whose
	// destructor 0x0067f490 also runs); layout in the lane notes.
	unsigned char field_68[ 0xd4 - 0x68 ];
};

typedef char tVisGroup_size_is_0xd4[ sizeof( tVisGroup ) == 0xd4 ? 1 : -1 ];

#endif	// tVisGroup_h
