// tListBox_Select: original source file unknown.

#include <windows.h>

#include "unsorted/tListBox_Select.h"

// HYPOTHESIS: the message tListBox_Select::vfunction31 passes to 0x0046aa2b when an item is picked.
struct c_0046aa2b_Message
{
	int field_00;	// the message type, 0x202
	tObjBase *field_04;	// the sender
	int field_08;	// the item picked
	int field_0C;	// the type of the event that picked it
	int field_10;	// HYPOTHESIS: not set by vfunction31
	int field_14;
	int field_18;
	int field_1C;
};

// HYPOTHESIS: sends a message (0x0046aa2b).
void fn_0046aa2b( c_0046aa2b_Message *message );


// FUNCTION: STARFLEETOP 0x004c70b3
tListBox_Select::tListBox_Select()
	: field_CC( -1 ), field_D0( true ), field_D1( false ), field_D4( 0 )
{
}

// FUNCTION: STARFLEETOP 0x004c70e1
bool tListBox_Select::vfunction31( c_tListBox_Base_Event *event )
{
	if ( tListBox_Base::vfunction31( event ) )
		return true;

	if ( !( field_40 & 1 ) || !field_D0 )
		return false;

	int x = event->field_04;
	int y = event->field_08;
	bool hit = vfunction36( x, y );
	bool handled = false;
	int height = vfunction43();

	switch ( event->field_00 )
	{
	case 1:
		if ( hit )
			handled = true;
		break;
	case 4:
	case 6:
		if ( hit )
		{
			int index = vfunction46() + ( y - field_34.top ) / height;
			if ( index < 0 || (unsigned int)index >= field_6C )
				return false;

			field_CC = index;
			fn_0045f4c2();

			if ( field_D1 )
			{
				c_0046aa2b_Message message;
				message.field_14 = 0;
				message.field_18 = -1;
				message.field_1C = -1;
				message.field_00 = 0x202;
				message.field_04 = this;
				message.field_08 = field_CC;
				message.field_0C = event->field_00;
				fn_0046aa2b( &message );
			}

			int value = vfunction69();
			if ( field_D4 && value )
				field_D4->vfunction2( value );
			handled = true;
		}
		break;
	}
	return handled;
}

// HYPOTHESIS: the code at 0x004c71fc (its exception-frame setup and body) is rebuilt as
// tListBox_Select::fn_004c7206, which claims 0x004c71fc. This empty definition is not the original; it stays
// until that code moves here, because a caller or a vtable needs it.
bool tListBox_Select::vfunction44( c_tListBox_Base_DrawItem *item )
{
	return false;
}

// FUNCTION: STARFLEETOP 0x004c71fc
// The rebuilt function starts with the 10 bytes the original keeps at 0x004c71fc.
bool tListBox_Select::fn_004c7206( c_tListBox_Base_DrawItem *draw )
{
	if ( draw->field_14 == 0 || draw->field_14->field_00 == 0
		|| ( (tString *)draw->field_14->field_00 )->mStrLen() == 0 )
		return false;
	if ( !( field_40 & 1 ) )
		return false;

	RECT clip;
	clip.left = draw->field_04->left;
	clip.top = draw->field_04->top;
	clip.right = draw->field_04->right;
	clip.bottom = draw->field_04->bottom;

	// HYPOTHESIS: a reference; the original reads the colours through the pointer operator[] returns.
	// The original calls operator[] out of line (0x004c75cc); this build inlines it.
	c_tListBox_Base_Colors &colors = field_B8[ draw->field_14->field_04 ];
	int color;
	int fill;
	unsigned char filled;
	// HYPOTHESIS: the selected item draws in the colours at +0x4 and +0x11 instead of +0x0 and +0x10.
	if ( draw->field_08 == field_CC )
	{
		color = (unsigned short)( ( colors.field_04 & 0x80000000 ) ? colors.field_04 : ( colors.field_04 & 0xff ) );
		fill = (unsigned short)( ( colors.field_0C & 0x80000000 ) ? colors.field_0C : ( colors.field_0C & 0xff ) );
		filled = colors.field_11[ 0 ];
	}
	else
	{
		color = (unsigned short)( ( colors.field_00 & 0x80000000 ) ? colors.field_00 : ( colors.field_00 & 0xff ) );
		fill = (unsigned short)( ( colors.field_0C & 0x80000000 ) ? colors.field_0C : ( colors.field_0C & 0xff ) );
		filled = colors.field_10;
	}
	if ( filled )
		field_14->fn_00675470( fill, draw->field_04, 0, 1, 0 );

	int width = draw->field_0C.right - draw->field_0C.left;
	tString *text = (tString *)draw->field_14->field_00;
	int length = text->mStrLen();
	// HYPOTHESIS: a '$' splits an item's text into two columns, the second 200 pixels to the right.
	int split = text->find( '$' );
	int target = g_p00c68c24->fn_00673840();
	if ( split >= 0 && split < length )
	{
		tString left = text->substr( 0, split );
		tString right = text->substr( split + 1 );
		int x = draw->field_0C.left;
		unsigned short *chars = left.fn_00618920();
		fn_005381a3( target, field_B0, width, x, draw->field_0C.top, color, 0, 4,
			chars, left.mStrLen(), &clip, 0, 1 );
		x += 200;
		chars = right.fn_00618920();
		fn_005381a3( target, field_B0, width, x, draw->field_0C.top, color, 0, 4,
			chars, right.mStrLen(), &clip, 0, 1 );
	}
	else
	{
		unsigned short *chars = text->fn_00618920();
		fn_005381a3( target, field_B0, width, draw->field_0C.left, draw->field_0C.top, color,
			field_B4, 4, chars, text->mStrLen(), &clip, 0, 1 );
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x004c7468
// HYPOTHESIS: empties the list and clears the selection.
void tListBox_Select::vfunction51()
{
	field_CC = -1;
	tListBox_Base::vfunction51();
}

// FUNCTION: STARFLEETOP 0x004c7474
// HYPOTHESIS: adds an item; keeps the selection on the same item when one is inserted before it,
// and selects the first item when the list had no selection.
unsigned int tListBox_Select::vfunction55( void *data, short key, int value, unsigned int index )
{
	int added = tListBox_Base::vfunction55( data, key, value, index );
	if ( added != -1 )
	{
		if ( added <= field_CC )
			field_CC++;
		else if ( field_6C > 0 && field_CC == -1 )
			field_CC = 0;
	}
	return added;
}

// FUNCTION: STARFLEETOP 0x004c74ba
// HYPOTHESIS: removes an item; keeps the selection on the same item when one before it is
// removed, and moves it back one when the selected item was the last one.
bool tListBox_Select::vfunction56( unsigned int index )
{
	bool removed = tListBox_Base::vfunction56( index );
	if ( removed )
	{
		if ( index < (unsigned int)field_CC )
			field_CC--;
		else if ( index == (unsigned int)field_CC && (unsigned int)field_CC >= field_6C )
			field_CC--;
	}
	return removed;
}

// FUNCTION: STARFLEETOP 0x004c74e9
void tListBox_Select::vfunction66( bool flag )
{
	if ( flag )
	{
		if ( !( field_40 & 1 ) )
			fn_0067ce90();
	}
	else if ( field_40 & 1 )
		vfunction18();
}

// FUNCTION: STARFLEETOP 0x004c750b
// HYPOTHESIS: turns selecting items by clicking on or off.
void tListBox_Select::vfunction67( bool flag )
{
	field_D0 = flag;
}

// FUNCTION: STARFLEETOP 0x004c7518
// HYPOTHESIS: sets whether picks are sent on and the object told about them, then tells it about
// the current item.
void tListBox_Select::vfunction68( bool flag, c_tListBox_Select_Field_D4 *target )
{
	field_D1 = flag;
	field_D4 = target;
	int value = vfunction69();
	if ( value && field_D4 )
		field_D4->vfunction2( value );
}

// FUNCTION: STARFLEETOP 0x004c754b
// HYPOTHESIS: returns the data of the selected item, or 0 when there is none.
int tListBox_Select::vfunction69()
{
	c_tListBox_Base_Item *item = fn_004c676a( field_CC );
	if ( item == 0 )
		return 0;
	return (int)item->field_00;
}

// FUNCTION: STARFLEETOP 0x004c755e
// HYPOTHESIS: selects the item at index and returns it, or returns -1 when index is out of range;
// the original accepts an index equal to the item count.
int tListBox_Select::vfunction70( int index )
{
	if ( index < 0 || (unsigned int)index > field_6C )
		return -1;
	field_CC = index;
	return index;
}

// FUNCTION: STARFLEETOP 0x004c7579
// HYPOTHESIS: returns the index of the selected item.
int tListBox_Select::vfunction71()
{
	return field_CC;
}
