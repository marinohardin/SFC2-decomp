//	tListBox_Select.h: tListBox_Select, the list box whose items can be selected.
// Evidence:  class (RTTI; original file unknown)

#ifndef tListBox_Select_h
#define tListBox_Select_h 1

#include "unsorted/tListBox_Base.h"

// HYPOTHESIS: the object at tListBox_Select +0xd4, told through its slot at +0x4 what an item
// picked returns from the list box's slot at +0x110.
class c_tListBox_Select_Field_D4
{
public:
	virtual void vfunction1();
	virtual void vfunction2( int value );
};

class tListBox_Select : public tListBox_Base
{
public:
	tListBox_Select();
	bool vfunction31( c_tListBox_Base_Event *event );
	bool vfunction44( c_tListBox_Base_DrawItem *item );
	void vfunction51();
	unsigned int vfunction55( void *data, short key, int value, unsigned int index );
	bool vfunction56( unsigned int index );
	// HYPOTHESIS: the body of vfunction44, which the analysis splits off after its frame setup.
	bool fn_004c7206( c_tListBox_Base_DrawItem *draw );
	// HYPOTHESIS: the flag turns bit 0 of field_40 on (through 0x0067ce90) or off (through the slot at +0x44).
	virtual void vfunction66( bool flag );
	// HYPOTHESIS: slot 68 is not called by the functions rebuilt so far.
	virtual void vfunction67( bool flag );
	virtual void vfunction68( bool flag, c_tListBox_Select_Field_D4 *target );
	// HYPOTHESIS: +0x110 returns what to tell field_D4 about the item picked.
	virtual int vfunction69();
	virtual int vfunction70( int index );
	virtual int vfunction71();

	// HYPOTHESIS: field types and names unknown.
	int field_CC;	// HYPOTHESIS: the selected item, -1 when none
	bool field_D0;	// HYPOTHESIS: true when clicks select items
	bool field_D1;	// HYPOTHESIS: true when a pick is sent on through 0x0046aa2b
	unsigned char field_D2[ 0xd4 - 0xd2 ];
	c_tListBox_Select_Field_D4 *field_D4;
};

#endif	// tListBox_Select_h
