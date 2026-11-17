//	tValueBtn.h: tButton, tVisNotifiee and tValueBtn, the bases of tMetaMapControl.
// Evidence:  class (RTTI: tValueBtn is a tButton, tControl, tVisStateObject, tVisObject, tObjBase,
// and a tVisNotifiee, tNotifiee; original file unknown)

#ifndef tValueBtn_h
#define tValueBtn_h 1

#include "Tiger/tstring.h"
#include "unsorted/tVisObject.h"
// HYPOTHESIS: tVisNotifiee is declared there for now; it would move to a header of its own.
#include "unsorted/tFleetEntry.h"

struct c_tListBox_Base_Event;

// HYPOTHESIS: tControl and tVisStateObject sit between tVisObject and tButton; their members are
// not recovered, so tButton is declared on tVisObject and filled to +0x110, where tValueBtn's
// tVisNotifiee part starts (tMetaMapControl's constructor sets a second vftable there).
class tButton : public tVisObject
{
public:
	// HYPOTHESIS: tControl's handler (0x00685c80, slot +0x7c) that tButton inherits;
	// tMetaMapControl::vfunction31 (0x005e4470) ends by calling it directly.
	bool vfunction31( c_tListBox_Base_Event *event );

	unsigned char field_64[ 0x110 - 0x64 ];
};

// HYPOTHESIS: the class whose type descriptor is at 0x00853a08. tFleetEntry.cpp still declares a
// reduced copy of its own (on tVisObject, its name at +0x118 as here); that copy is to be
// replaced by an include of this header.
class tValueBtn : public tButton, public tVisNotifiee
{
public:
	// HYPOTHESIS: 0x004c178f, out of line.
	tValueBtn();

	unsigned char field_114[ 0x118 - 0x114 ];
	tString m_s118;
	unsigned char field_140[ 0x17c - 0x140 ];
};

#endif	// tValueBtn_h
