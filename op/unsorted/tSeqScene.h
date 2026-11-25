//	tSeqScene.h: tSeqScene, a visual group that plays a sequence.
// Evidence:  class (RTTI: tSeqScene type descriptor 0x00883130, vftable 0x007b8260, 44 slots,
// base tVisGroup; original file unknown). Object size 0x158 (the allocation at 0x0068543d).

#ifndef tSeqScene_h
#define tSeqScene_h 1

#include "Tiger/tstring.h"
#include "unsorted/tVisGroup.h"

class tSeqScene : public tVisGroup
{
public:
	tSeqScene();
	virtual ~tSeqScene();
	// Slots 1 to 40 are tVisGroup's; the ones tSeqScene overrides are redeclared here.
	virtual void vfunction30();	// 0x0067e2d0
	virtual void vfunction37();	// 0x0067e3f0
	virtual void vfunction40();	// 0x0067dd40
	virtual void vfunction41();	// 0x0067de00
	virtual void vfunction42();	// the slot holds the thunk 0x0067dca0, which jumps to fn_0067dbc0
	virtual void vfunction43();	// 0x0067dcb0
	virtual void vfunction44();	// 0x0067e1a0

	// HYPOTHESIS: called directly by the constructor (0x00685494) after it sets the vftable, and by
	// the thunk in slot 42: it clears the playback fields and builds both strings from 0x008930a4.
	void fn_0067dbc0();
	// HYPOTHESIS: the four below are unclassed in the symbol table; their callers pass a
	// tSeqScene in ecx and they read and write only members from +0xe0 to +0xfc.
	void fn_0067dd90();	// clears bit 0 of field_EC
	void fn_0067dda0();	// sets bit 0 of field_EC and stores a clock reading in field_E8
	void fn_0067dde0();	// sets bit 0 of field_EC and copies field_E0 to field_F0
	void fn_0067e1d0();	// called by vfunction30's neighbour 0x0067e2d0 with the tick count

	// HYPOTHESIS: field names unknown; offsets and constant stores from fn_0067dbc0.
	int field_DC;
	int field_E0;
	int field_E4;
	unsigned long field_E8;	// a millisecond clock reading (0x00672870)
	unsigned long field_EC;	// flags; bit 0 is set while playing
	int field_F0;
	int field_F4;	// set to -1
	int field_F8;	// set to -1
	int field_FC;	// set to 10, a divisor of 1000
	int field_100;
	tString field_104;
	int field_12C;
	tString field_130;
};

typedef char tSeqScene_size_is_0x158[ sizeof( tSeqScene ) == 0x158 ? 1 : -1 ];

#endif	// tSeqScene_h
