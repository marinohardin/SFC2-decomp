// tListBox_Base: original source file unknown.

#include <windows.h>

#include "unsorted/tListBox_Base.h"
#include "unsorted/tScrollBar.h"
#include "unsorted/tAsset.h"

// FUNCTION: STARFLEETOP 0x004c61a8
tListBox_Base::~tListBox_Base()
{
	vfunction51();
	if ( field_B0 )
		field_B0->vfunction12();
	field_B0 = 0;
}

// FUNCTION: STARFLEETOP 0x004c6376
void tRect::mMakeClippedRect( const tRect *pClip, tRect *pNewRect ) const
{
	if ( left < pClip->left )		pNewRect->left		= pClip->left;
	else							pNewRect->left		= left;
	if ( top < pClip->top )			pNewRect->top		= pClip->top;
	else							pNewRect->top		= top;
	if ( right > pClip->right )		pNewRect->right		= pClip->right;
	else							pNewRect->right		= right;
	if ( bottom > pClip->bottom )	pNewRect->bottom	= pClip->bottom;
	else							pNewRect->bottom	= bottom;
}

// FUNCTION: STARFLEETOP 0x004c6225
void tListBox_Base::vfunction8( const tRect *area )
{
	{
		tRect clipped;
		field_34.mMakeClippedRect( area, &clipped );
		if ( clipped.mIsEmpty() )
			return;
	}
	if ( field_B0 == 0 )
		return;

	c_tListBox_Base_Item *item = fn_004c676a( field_70 );
	if ( item == 0 )
		return;

	int height = vfunction43();
	unsigned int index = field_70;
	int left = field_34.left;
	int y = field_34.top;

	for ( ; index < field_6C; index++ )
	{
		tRect rowRect( left, y, field_34.right, y + height );
		rowRect.mClip( &field_34 );
		// HYPOTHESIS: a fresh rect built field by field; the implicit copy
		// constructor copies dwords, the original copies shorts.
		tRect clipped( rowRect.left, rowRect.top, rowRect.right, rowRect.bottom );
		clipped.mClip( area );
		if ( !clipped.mIsEmpty() )
		{
			c_tListBox_Base_DrawItem draw;
			draw.field_00 = field_14;
			draw.field_04 = &clipped;
			draw.field_08 = index;
			draw.field_0C = rowRect;
			draw.field_14 = item;
			vfunction44( &draw );
		}
		y += height;
		if ( y >= field_34.bottom )
			return;
		if ( y >= area->bottom )
			return;
		item = item->field_C;
	}
}

// FUNCTION: STARFLEETOP 0x004c63dd
bool tListBox_Base::vfunction31( c_tListBox_Base_Event *event )
{
	if ( event->field_00 != 0xd )
		return false;
	if ( dynamic_cast< tScrollBar * >( event->field_10 ) == 0 )
		return false;
	if ( event->field_1C != this )
		return false;

	bool changed = false;
	int first = event->field_18;
	switch ( event->field_14 )
	{
	case 1:
	case 2:
	case 7:
	case 9:
		break;
	case 3:
		first = field_70;
		if ( first > 0 )
		{
			first--;
			changed = true;
		}
		break;
	case 4:
		first = field_70;
		if ( first < field_A4 )
		{
			first++;
			changed = true;
		}
		break;
	case 8:
		if ( (unsigned int)first < field_6C && first >= 0 )
			changed = true;
		break;
	case 5:
		if ( field_70 > 0 )
		{
			field_70 -= field_A8;
			first = field_70;
			if ( first < 0 )
				first = 0;
			changed = true;
		}
		break;
	case 6:
		first = field_70;
		if ( first < field_A4 )
		{
			field_70 = first + field_A8;
			first = field_70;
			if ( first > field_A4 )
				first = field_A4;
			changed = true;
		}
		break;
	default:
		return false;
	}
	if ( changed )
	{
		field_70 = first;
		fn_004c6a16( 1 );
		fn_0045f4c2();
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x004c64c6
void tListBox_Base::vfunction37( tAsset *asset )
{
	field_48 = asset->field_04;
	tStaticTextAsset *text = dynamic_cast< tStaticTextAsset * >( asset );
	if ( text == 0 )
		return;

	field_34.right = text->field_5C;
	field_34.bottom = text->field_5E;

	int size = text->field_88 & 0xff;
	switch ( text->field_88 >> 24 )
	{
	case 2:
		field_B4 = 0x81;
		break;
	case 3:
		field_B4 = 0x82;
		break;
	case 4:
		field_B4 = 0x83;
		break;
	default:
		field_B4 = 0x80;
		break;
	}
	if ( field_B0 )
		field_B0->vfunction11();
	field_B0 = fn_004be773( 0, size );

	// HYPOTHESIS: a colour with the top bit set is used whole, otherwise only its low byte counts.
	if ( (short)( ( text->field_8C & 0x80000000 ) ? text->field_8C : ( text->field_8C & 0xff ) ) )
		field_B8[ -1 ].field_00 = text->field_8C;
	if ( (short)( ( text->field_90 & 0x80000000 ) ? text->field_90 : ( text->field_90 & 0xff ) ) )
		field_B8[ -1 ].field_04 = text->field_90;

	field_7C = text->field_60;
}

// FUNCTION: STARFLEETOP 0x004c661f
// HYPOTHESIS: the original bytes (mov eax, <ehfuncinfo>; call __EH_prolog) are the frame setup of
// 0x004c6629, which the analysis splits off as a function of its own. This body only claims the address.
c_tListBox_Base_Item *tListBox_Base::vfunction41( void *data, short key, int value, unsigned int index )
{
	return 0;
}

// FUNCTION: STARFLEETOP 0x004c6629
// The rebuilt function starts with the 10 bytes the original keeps at 0x004c661f.
c_tListBox_Base_Item *tListBox_Base::fn_004c6629( void *data, int key, int value, unsigned int index )
{
	c_tListBox_Base_Item *item = new c_tListBox_Base_Item;
	if ( item == 0 )
		return 0;

	item->field_C = 0;
	item->field_10 = 0;
	item->field_00 = 0;
	item->field_04 = (short)key;
	item->field_08 = value;
	vfunction65( key );

	if ( field_74 && data )
	{
		item->field_00 = new tString( *(const tString *)data );
		if ( item->field_00 == 0 )
		{
			delete item;
			return 0;
		}
	}
	else
		item->field_00 = data;

	if ( index != (unsigned int)-1 && index < field_6C )
	{
		c_tListBox_Base_Item *at = fn_004c676a( index );
		item->field_C = at;
		item->field_10 = at->field_10;
		if ( at->field_10 )
			at->field_10->field_C = item;
		else
			field_64 = item;
		at->field_10 = item;
	}
	else
	{
		item->field_10 = field_68;
		if ( field_68 )
			field_68->field_C = item;
		else
		{
			field_64 = item;
			field_68 = item;
		}
		field_68 = item;
	}
	field_6C++;
	fn_0045f4c2();
	return item;
}

// FUNCTION: STARFLEETOP 0x004c670c
// HYPOTHESIS: unlinks an item from the list and frees it, with its tString copy when the list
// box keeps one (the reverse of 0x004c6629).
void tListBox_Base::vfunction42( c_tListBox_Base_Item *item )
{
	if ( item == 0 )
		return;

	if ( item->field_10 )
		item->field_10->field_C = item->field_C;
	else
		field_64 = item->field_C;

	if ( item->field_C )
		item->field_C->field_10 = item->field_10;
	else
		field_68 = item->field_10;

	if ( field_74 && item->field_00 )
		delete (tString *)item->field_00;
	delete item;
	field_6C--;
	fn_0045f4c2();
}

// FUNCTION: STARFLEETOP 0x004c676a
c_tListBox_Base_Item *tListBox_Base::fn_004c676a( unsigned int index )
{
	if ( index >= field_6C )
		return 0;

	c_tListBox_Base_Item *item = field_64;
	for ( ; index > 0; index-- )
		item = item->field_C;
	return item;
}

// FUNCTION: STARFLEETOP 0x004c6787
// HYPOTHESIS: the height of one item is the height of the font, or 10 without one.
int tListBox_Base::vfunction43()
{
	if ( field_B0 )
		return field_B0->vfunction5();
	return 10;
}

// HYPOTHESIS: 0x004c679a reads the four sides of a tRect into registers, bottom first, before it
// stores any of them: the shape of an inlined call whose arguments are the sides. The original's
// helper and its name are not known; this one is ours.
inline void fn_004c679a_SetRECT( RECT *r, int l, int t, int ri, int b )
{
	r->left = l;
	r->top = t;
	r->right = ri;
	r->bottom = b;
}

// FUNCTION: STARFLEETOP 0x004c679a
// HYPOTHESIS: draws one item's text, over a fill when its colours ask for one; false when the item
// has no text.
bool tListBox_Base::vfunction44( c_tListBox_Base_DrawItem *draw )
{
	if ( draw->field_14 == 0 || draw->field_14->field_00 == 0
		|| ( (tString *)draw->field_14->field_00 )->mStrLen() == 0 )
		return false;

	c_tListBox_Base_Colors colors = field_B8[ draw->field_14->field_04 ];
	RECT clip;
	fn_004c679a_SetRECT( &clip, draw->field_04->left, draw->field_04->top, draw->field_04->right, draw->field_04->bottom );
	if ( colors.field_10 )
		field_14->fn_00675470( (unsigned short)( ( colors.field_08 & 0x80000000 ) ? colors.field_08 : ( colors.field_08 & 0xff ) ),
			draw->field_04, 0, 1, 0 );

	int width = draw->field_0C.right - draw->field_0C.left;
	tString *text = (tString *)draw->field_14->field_00;
	int target = g_p00c68c24->fn_00673840();
	unsigned short *chars = text->fn_00618920();
	fn_005381a3( target, field_B0, width, draw->field_0C.left, draw->field_0C.top,
		(unsigned short)( ( colors.field_00 & 0x80000000 ) ? colors.field_00 : ( colors.field_00 & 0xff ) ),
		field_B4, 4, chars, text->mStrLen(), &clip, 0, 1 );
	return true;
}

// FUNCTION: STARFLEETOP 0x004c68dd
tScrollBar *tListBox_Base::fn_004c68dd()
{
	if ( field_78 == 0 && field_7C.mStrLen() )
	{
		field_78 = dynamic_cast< tScrollBar * >( fn_004c692a( this, &field_7C ) );
		field_7C = "";
	}
	return field_78;
}

// FUNCTION: STARFLEETOP 0x004c692a
tObjBase *fn_004c692a( tVisObject *object, tString *name )
{
	if ( object )
	{
		tVisObject *node;
		for ( node = object->field_08; node; node = node->field_08 )
		{
			c_00677060_Result *found = g_00c69100.fn_00677060( node->field_48 );
			if ( found && found->field_08.mStrLen() && found->field_08 == *name )
				return node;
		}
		for ( node = object->field_04; node; node = node->field_04 )
		{
			c_00677060_Result *found = g_00c69100.fn_00677060( node->field_48 );
			if ( found && found->field_08.mStrLen() && found->field_08 == *name )
				return node;
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x004c69b8
void tListBox_Base::fn_004c69b8( int a )
{
	tScrollBar *bar = fn_004c68dd();
	if ( bar )
	{
		unsigned int height = vfunction43();
		int extent = field_34.bottom - field_34.top;
		if ( extent < 0 )
			extent = 0;
		field_A8 = extent / height;
		field_A4 = field_6C - field_A8;
		field_A4 = field_A4 > 0 ? field_A4 : 0;
		bar->vfunction41( 0, field_A4, a );
	}
}

// FUNCTION: STARFLEETOP 0x004c6a16
void tListBox_Base::fn_004c6a16( int a )
{
	tScrollBar *bar = fn_004c68dd();
	if ( bar )
		bar->vfunction43( field_70, a );
}

// FUNCTION: STARFLEETOP 0x004c6a37
bool tListBox_Base::vfunction47( unsigned int index )
{
	if ( index >= field_6C )
		return false;
	if ( field_70 != index )
	{
		field_70 = index;
		fn_004c6a16( 1 );
		fn_0045f4c2();
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x004c6a65
int tListBox_Base::vfunction48()
{
	return field_A8;
}

// FUNCTION: STARFLEETOP 0x004c6a6c
bool tListBox_Base::vfunction49()
{
	return field_AC;
}

// FUNCTION: STARFLEETOP 0x004c6a73
void tListBox_Base::vfunction50( bool flag )
{
	field_AC = flag;
}

// FUNCTION: STARFLEETOP 0x004c6a80
void tListBox_Base::vfunction51()
{
	c_tListBox_Base_Item *item = field_64;
	while ( item )
	{
		c_tListBox_Base_Item *next = item->field_C;
		vfunction42( item );
		item = next;
	}
	field_64 = 0;
	field_68 = 0;
	fn_0045f4c2();
}

// HYPOTHESIS: scrolls the list so the item at an index is visible: to the top when it is
// above the first visible item, to the bottom of the page when it is below the last.
// FUNCTION: STARFLEETOP 0x004c6ab1
void tListBox_Base::vfunction52( unsigned int index )
{
	unsigned int first = vfunction46();
	unsigned int page = vfunction48();
	if ( index < first )
		vfunction47( index );
	else if ( index >= first + page )
		vfunction47( index - page + 1 );
}

// HYPOTHESIS: the width of a text in the list box's font, or 0 without a font.
// FUNCTION: STARFLEETOP 0x004c6af8
int tListBox_Base::vfunction53( unsigned short *text )
{
	if ( field_B0 )
		return field_B0->vfunction3( text, -1 );
	return 0;
}

// FUNCTION: STARFLEETOP 0x004c6bac
void tListBox_Base::vfunction54( c_tListBox_Base_Item *item, unsigned int index )
{
	vfunction55( item->field_00, item->field_04, item->field_08, index );
}

// HYPOTHESIS: adds an item at an index and keeps the list scrolled to its end when it was there
// and the flag of vfunction49 is set; returns the index the item landed at, or -1.
// FUNCTION: STARFLEETOP 0x004c6b14
unsigned int tListBox_Base::vfunction55( void *data, short key, int value, unsigned int index )
{
	unsigned int page = vfunction48();
	bool atEnd = vfunction46() == vfunction45() - page;
	if ( vfunction41( data, key, value, index ) == 0 )
		return (unsigned int)-1;
	if ( vfunction49() && atEnd )
		vfunction52( vfunction45() - 1 );
	fn_004c69b8( 1 );
	fn_0045f4c2();
	return index < field_6C ? index : field_6C - 1;
}

// HYPOTHESIS: removes the item at an index, scrolling up by one when it was the first visible
// item; false when there is no item there.
// FUNCTION: STARFLEETOP 0x004c6bcb
bool tListBox_Base::vfunction56( unsigned int index )
{
	c_tListBox_Base_Item *item = fn_004c676a( index );
	if ( item == 0 )
		return false;
	if ( index == field_70 && field_70 != 0 )
		field_70--;
	vfunction42( item );
	fn_004c69b8( 0 );
	fn_004c6a16( 1 );
	return true;
}

// FUNCTION: STARFLEETOP 0x004c6c13
void *tListBox_Base::vfunction57( unsigned int index )
{
	return fn_004c676a( index )->field_00;
}

// HYPOTHESIS: returns the colour key of the item at an index (the key vfunction58 sets), -1 when
// there is no item there.
// FUNCTION: STARFLEETOP 0x004c6c4f
short tListBox_Base::vfunction59( unsigned int index )
{
	c_tListBox_Base_Item *item = fn_004c676a( index );
	if ( item != 0 )
		return item->field_04;
	return -1;
}

// FUNCTION: STARFLEETOP 0x004c6c21
void tListBox_Base::vfunction58( unsigned int index, int key )
{
	c_tListBox_Base_Item *item = fn_004c676a( index );
	if ( item != 0 )
	{
		item->field_04 = key;
		vfunction65( key );
	}
	fn_0045f4ec();
}

// FUNCTION: STARFLEETOP 0x004c6c69
int tListBox_Base::vfunction60( unsigned int index )
{
	c_tListBox_Base_Item *item = fn_004c676a( index );
	if ( item != 0 )
		return item->field_08;
	return 0;
}

// FUNCTION: STARFLEETOP 0x004c6c9d
void tListBox_Base::vfunction62( short key, c_tListBox_Base_Colors *colors )
{
	field_B8[ key ] = *colors;
	fn_0045f4c2();
}

// FUNCTION: STARFLEETOP 0x004c6ce9
// HYPOTHESIS: returns a copy of the colours stored under key (param_1 is the hidden return slot).
c_tListBox_Base_Colors tListBox_Base::vfunction63( short key )
{
	return field_B8[ key ];
}

// FUNCTION: STARFLEETOP 0x004c6c80
bool tListBox_Base::vfunction61( unsigned int index, int value )
{
	c_tListBox_Base_Item *item = fn_004c676a( index );
	if ( item != 0 )
	{
		item->field_08 = value;
		return true;
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x004c6d2e
std::map< int, c_tListBox_Base_Colors > *tListBox_Base::vfunction64()
{
	return &field_B8;
}

// FUNCTION: STARFLEETOP 0x004c6d35
// HYPOTHESIS: gives a key with no colours of its own a copy of the default colours stored
// under -1.
void tListBox_Base::vfunction65( short key )
{
	if ( field_B8.find( key ) == field_B8.end() )
		field_B8[ key ] = field_B8[ -1 ];
}
