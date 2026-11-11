//	tScrollBar.h: tScrollBar, the scroll bar of the list boxes.
// Evidence:  class (RTTI, vftable 0x007a3eec, type descriptor 0x008546d0; original file unknown)

#ifndef tScrollBar_h
#define tScrollBar_h 1

#include "unsorted/tVisObject.h"
#include "unsorted/tNetControl.h"	// tString

// HYPOTHESIS: the thumb's image at tScrollBar +0xa8; tScrollBar::vfunction8 (0x004c8439) adds
// +0x4 and +0x6 to the point it draws it at, and mLoad (0x004c878a) sizes the thumb from +0x0
// and +0x2.
struct c_tScrollBar_FieldA8
{
	short field_00;	// HYPOTHESIS: the width
	short field_02;	// HYPOTHESIS: the height
	short field_04;
	short field_06;
};

struct c_tListBox_Base_Event;
class tAsset;

// HYPOTHESIS: a millisecond clock; 0x004c828b compares its differences unsigned.
unsigned long fn_00672870();

// HYPOTHESIS: only the class and the slot tListBox_Base calls are needed so far.
class tScrollBar : public tVisObject
{
public:
	tScrollBar();
	virtual ~tScrollBar();
	// HYPOTHESIS: draws the bar inside the area given; the slot of tObjBase::vfunction8.
	void vfunction8( const tRect *area );
	// Slots 13 to 40 are not called by the functions rebuilt so far.
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual int vfunction20();
	virtual void vfunction21( int n );
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	// HYPOTHESIS: +0x7c handles a mouse event (1 press, 4 release, 7 move); true when handled.
	virtual unsigned char vfunction31( c_tListBox_Base_Event *event );
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();
	// HYPOTHESIS: +0xa0 sets the scroll range, from the first value to the second (0x004c69b8);
	// false when the range is empty or wider than a long; redraws the bar when the flag is set.
	virtual unsigned char vfunction41( int minimum, int maximum, unsigned char redraw );
	// HYPOTHESIS: +0xa4 reads the scroll range back into the pointers given (counterpart of +0xa0).
	virtual unsigned char vfunction42( int *minimum, int *maximum );
	// HYPOTHESIS: +0xa8 sets the scroll position (0x004c6a16 passes a list box's first visible item).
	virtual unsigned char vfunction43( int position, unsigned char redraw );

	// HYPOTHESIS: the part of the bar at a point (0x004c828b compares it with field_80).
	int fn_004c84d7( int x, int y );
	// HYPOTHESIS: steps the bar for the part given (0x004c828b repeats it while a part is held).
	void fn_004c8a57( int part, int a );
	// HYPOTHESIS: works out the thumb's rectangle from the position and range; false when the bar
	// has no range or no room for it.
	unsigned char fn_004c82f8( tRect *rect );
	// HYPOTHESIS: fills a rectangle for the thumb when a drag ends (0x004c8587 tests the release
	// point against it).
	void fn_004c8afe( tRect *rect );
	// HYPOTHESIS: drags the thumb of a vertical bar to a point (0x004c8587).
	void fn_004c8b60( int x, int y );
	// HYPOTHESIS: drags the thumb of a horizontal bar to a point (0x004c8587).
	void fn_004c8c6a( int x, int y );
	// The exe's message names it tScrollBar::mLoad(). HYPOTHESIS: the body of slot 37 (its entry
	// 0x004c8780 is the frame setup); loads the bar from the tSliderAsset that describes it.
	void mLoad( tAsset *asset );

	// HYPOTHESIS: field types and names unknown; the constructor (0x004c8113) clears them all
	// and sets field_D8 to 5.
	int field_64;
	int field_68;
	int field_6C;
	unsigned char field_70;
	unsigned char field_71;
	tRect field_72;
	short field_7A;
	short field_7C;
	short field_7E;
	int field_80;
	int field_84;
	short field_88;
	short field_8A;
	short field_8C;
	short field_8E;
	short field_90;
	short field_92;
	short field_94;
	short field_96;
	int field_98;
	int field_9C;
	int field_A0;
	int field_A4;
	c_tScrollBar_FieldA8 *field_A8;
	tString field_AC;	// HYPOTHESIS: a name, from the tSliderAsset (mLoad)
	int field_D4;
	int field_D8;
};

#endif	// tScrollBar_h
