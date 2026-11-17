//	tListBox_Base.h: tListBox_Base, the base of the list boxes.
// Evidence:  class (RTTI, vftable 0x007a3980; original file unknown)

#ifndef tListBox_Base_h
#define tListBox_Base_h 1

#include "unsorted/tVisObject.h"
#include "unsorted/tNetControl.h"	// tString

#include <map>

class tAsset;
struct tagRECT;
typedef tagRECT RECT;

// HYPOTHESIS: the value of the map at +0xb8 (0x14 bytes); vfunction37 sets the first two
// members from a tStaticTextAsset's colours under the key -1. vfunction44 draws an item's text
// in field_00 and, when field_10 is set, fills the item first in field_08.
struct c_tListBox_Base_Colors
{
	unsigned int field_00;
	unsigned int field_04;
	unsigned int field_08;
	unsigned int field_0C;
	unsigned char field_10;
	unsigned char field_11[ 0x14 - 0x11 ];
};

// HYPOTHESIS: draws a run of text clipped to a rectangle (0x005381a3); its arguments are not known.
void fn_005381a3( int a, tObjBase *font, int width, int x, int y, int color, int style, int b, unsigned short *text, int length, RECT *clip, int c, int d );

// HYPOTHESIS: the object at 0x00c68c24; vfunction44 passes what 0x00673840 returns to 0x005381a3.
class c_00c68c24_Object
{
public:
	int fn_00673840();
};

extern c_00c68c24_Object *g_p00c68c24;

// HYPOTHESIS: returns the font of a size (it formats "Arial%d").
tObjBase *fn_004be773( int a, int size );

// HYPOTHESIS: what 0x00677060 returns; +0x8 is its name.
struct c_00677060_Result
{
	unsigned char field_00[ 0x04 - 0x00 ];
	int field_04;	// HYPOTHESIS: an id; tScrollBar::mLoad (0x004c878a) passes it to 0x006770a0
	tString field_08;
	unsigned char field_30[ 0x128 - 0x30 ];
	int *field_128;	// HYPOTHESIS: ids by index; tMetaMapControl::fn_005e3f82 passes one to 0x00677380
};

struct c_tScrollBar_FieldA8;

// HYPOTHESIS: the object at 0x00c69100 (0x00677060 follows tCastMgr's functions in the exe);
// 0x00677060 looks up what a tVisObject's +0x48 identifies.
class c_00c69100_Object
{
public:
	c_00677060_Result *fn_00677060( int id );
	// HYPOTHESIS: tScrollBar's destructor (0x004c8201) passes it its +0xa4 when +0xa8 is set.
	void fn_006773e0( int id );
	// HYPOTHESIS: returns the image of an id, or 0 (tScrollBar::mLoad, 0x004c878a).
	c_tScrollBar_FieldA8 *fn_006770a0( int id );
	// HYPOTHESIS: returns the image of an id (tMetaMapControl::fn_005e3f82, 0x005e3f82).
	c_tScrollBar_FieldA8 *fn_00677380( int id );
};

extern c_00c69100_Object g_00c69100;

// HYPOTHESIS: finds the object of a name among the objects linked to a tVisObject (0x004c692a),
// searching the chain at +0x8, then the chain at +0x4.
tObjBase *fn_004c692a( tVisObject *object, tString *name );

// HYPOTHESIS: an item of the list, 0x14 bytes; +0xc links the next item, +0x10 the previous.
struct c_tListBox_Base_Item
{
	void *field_00;	// HYPOTHESIS: the item's data, or a tString copy of its text
	short field_04;
	int field_08;
	c_tListBox_Base_Item *field_C;
	c_tListBox_Base_Item *field_10;
};

// HYPOTHESIS: the argument 0x004c6225 passes to the slot at +0xac for each visible item.
struct c_tListBox_Base_DrawItem
{
	c_tVisObject_Field14 *field_00;	// the list box's field_14
	tRect *field_04;	// the item's rectangle clipped to the area drawn
	unsigned int field_08;	// the item's index
	tRect field_0C;	// the item's rectangle clipped to the list box
	c_tListBox_Base_Item *field_14;
};

// HYPOTHESIS: the event 0x004c63dd handles; type 0xd comes from a tScrollBar.
struct c_tListBox_Base_Event
{
	int field_00;	// the event type
	int field_04;	// HYPOTHESIS: the x of a mouse event
	int field_08;	// HYPOTHESIS: the y of a mouse event
	unsigned char field_0C[ 0x10 - 0x0c ];
	tObjBase *field_10;	// the sender
	int field_14;	// the scroll action, 1 to 9
	int field_18;	// the position for action 3
	tObjBase *field_1C;	// the object the event is for
};

class tListBox_Base : public tVisObject
{
public:
	virtual ~tListBox_Base();
	// Slots 12 to 41 are not called by the functions rebuilt so far.
	virtual void vfunction13();
	virtual bool vfunction14();
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
	// HYPOTHESIS: +0x78 handles the events of the list box's tScrollBar; true when handled.
	virtual bool vfunction31( c_tListBox_Base_Event *event );
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	// HYPOTHESIS: +0x8c is true when a point lies on the list box.
	virtual bool vfunction36( int x, int y );
	// HYPOTHESIS: +0x90 sets up the list box from the asset that describes it.
	virtual void vfunction37( tAsset *asset );
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();
	// HYPOTHESIS: +0xa0 adds an item; its entry 0x004c661f is the frame setup of 0x004c6629, so the
	// slot is that function and these are its arguments. 0 when it fails.
	virtual c_tListBox_Base_Item *vfunction41( void *data, short key, int value, unsigned int index );
	// HYPOTHESIS: +0xa4 removes an item from the list and frees it (0x004c670c).
	virtual void vfunction42( c_tListBox_Base_Item *item );
	// HYPOTHESIS: +0xa8 returns the height of one item; +0xac draws one item.
	virtual int vfunction43();
	virtual bool vfunction44( c_tListBox_Base_DrawItem *item );
	// Slots 45 to 64 are not called by the functions rebuilt so far.
	// HYPOTHESIS: +0xb0 returns the number of items.
	virtual unsigned int vfunction45();
	// HYPOTHESIS: returns the first visible item (vfunction52 reads it).
	virtual unsigned int vfunction46();
	// HYPOTHESIS: +0xbc scrolls the first visible item to an index; false when it is past the end.
	virtual bool vfunction47( unsigned int index );
	// HYPOTHESIS: +0xc0 returns the items scrolled by one page.
	virtual int vfunction48();
	virtual bool vfunction49();
	// HYPOTHESIS: +0xc8 sets the flag vfunction49 returns.
	virtual void vfunction50( bool flag );
	// HYPOTHESIS: +0xcc empties the list (0x004c6a80); the destructor calls it.
	virtual void vfunction51();
	virtual void vfunction52( unsigned int index );
	virtual int vfunction53( unsigned short *text );
	// HYPOTHESIS: adds a copy of an item at an index through vfunction55.
	virtual void vfunction54( c_tListBox_Base_Item *item, unsigned int index );
	virtual unsigned int vfunction55( void *data, short key, int value, unsigned int index );
	virtual bool vfunction56( unsigned int index );
	// HYPOTHESIS: returns the data of the item at an index.
	virtual void *vfunction57( unsigned int index );
	// HYPOTHESIS: sets the key of the item at an index and tells +0x100 about it.
	virtual void vfunction58( unsigned int index, int key );
	// HYPOTHESIS: returns the key of the item at an index, or -1 when there is none.
	virtual short vfunction59( unsigned int index );
	// HYPOTHESIS: returns the value of the item at an index, or 0 when there is none.
	virtual int vfunction60( unsigned int index );
	// HYPOTHESIS: sets the value of the item at an index; false when there is none.
	virtual bool vfunction61( unsigned int index, int value );
	// HYPOTHESIS: sets the colours drawn for items with a key.
	virtual void vfunction62( short key, c_tListBox_Base_Colors *colors );
	virtual c_tListBox_Base_Colors vfunction63( short key );
	// HYPOTHESIS: returns the map of colours by key at +0xb8 (0x004c6d2e).
	virtual std::map< int, c_tListBox_Base_Colors > *vfunction64();
	// HYPOTHESIS: +0x100 is told the key of an item being added (0x004c6629).
	virtual void vfunction65( short key );
	// HYPOTHESIS: adds an item before the one at an index, or at the end; returns it, or 0.
	c_tListBox_Base_Item *fn_004c6629( void *data, int key, int value, unsigned int index );
	// HYPOTHESIS: draws the visible items inside the area given; the slot of tObjBase::vfunction8.
	void vfunction8( const tRect *area );
	// HYPOTHESIS: returns the item at an index.
	c_tListBox_Base_Item *fn_004c676a( unsigned int index );
	// HYPOTHESIS: updates the list box after field_70 changes.
	void fn_004c6a16( int a );
	// HYPOTHESIS: returns the list box's tScrollBar, looked up once by the name at +0x7c.
	tScrollBar *fn_004c68dd();
	// HYPOTHESIS: works out the items one page holds and the last first-visible item from the
	// list box's height, and passes the range to its tScrollBar.
	void fn_004c69b8( int a );

	// HYPOTHESIS: field types and names unknown; +0x6c and +0x70 are the item count and first
	// visible item (0x004c6225).
	c_tListBox_Base_Item *field_64;	// HYPOTHESIS: the first item (0x004c6629)
	c_tListBox_Base_Item *field_68;	// HYPOTHESIS: the last item (0x004c6629)
	unsigned int field_6C;
	int field_70;
	unsigned char field_74;	// HYPOTHESIS: nonzero when the list box keeps its own copy of each item's text
	unsigned char field_75[ 0x78 - 0x75 ];
	tScrollBar *field_78;	// HYPOTHESIS: the list box's tScrollBar, found by the name at +0x7c (0x004c68dd)
	tString field_7C;
	int field_A4;	// HYPOTHESIS: the last first-visible item a scroll may reach
	int field_A8;	// HYPOTHESIS: the items scrolled by one page
	bool field_AC;	// HYPOTHESIS: a flag; vfunction49 returns it
	unsigned char field_AD[ 0xb0 - 0xad ];
	tObjBase *field_B0;	// HYPOTHESIS: a tObjBase; the destructor calls its slot at +0x2c
	int field_B4;	// HYPOTHESIS: the font style, 0x80 to 0x83
	std::map< int, c_tListBox_Base_Colors > field_B8;
	// HYPOTHESIS: tListBox_Select's members start at +0xcc; what fills +0xc8 is not known.
	unsigned char field_C8[ 0xcc - 0xc8 ];
};

#endif	// tListBox_Base_h
