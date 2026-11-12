// tScrollBar: original source file unknown.

#include "unsorted/tScrollBar.h"
#include "unsorted/tListBox_Base.h"	// HYPOTHESIS: g_00c69100 is declared there for now
#include "unsorted/tAsset.h"
#include "Tiger/tsay.h"
extern tSay g_00c693a0;	// HYPOTHESIS: the tSay mLoad (0x004c878a) warns through
extern unsigned char g_00c69414;	// HYPOTHESIS: set, it silences that warning
void fn_006c9980( const char *message );	// HYPOTHESIS: empty in the release build

// FUNCTION: STARFLEETOP 0x004c8113
tScrollBar::tScrollBar()
{
	field_40 |= 9;
	field_64 = 0;
	field_68 = 0;
	field_6C = 0;
	field_70 = 0;
	field_72.left = 0;
	field_72.top = 0;
	field_72.right = 0;
	field_72.bottom = 0;
	field_7A = 0;
	field_7C = 0;
	field_80 = 0;
	field_84 = 0;
	field_94 = 0;
	field_96 = 0;
	field_8C = 0;
	field_8E = 0;
	field_90 = 0;
	field_92 = 0;
	field_9C = 0;
	field_A0 = 0;
	field_A4 = 0;
	field_A8 = 0;
	field_D4 = 0;
	field_D8 = 5;
}

// FUNCTION: STARFLEETOP 0x004c820b
tScrollBar::~tScrollBar()
{
	if ( field_80 && field_0C )
		field_0C->fn_0067d311( this );
	if ( field_A8 )
		g_00c69100.fn_006773e0( field_A4 );
}

// FUNCTION: STARFLEETOP 0x004c828b
void tScrollBar::vfunction30()
{
	if ( field_80 >= 5 && field_80 <= 6 )
	{
		field_84 = fn_004c84d7( field_88, field_8A );
		if ( field_84 == field_80 )
		{
			unsigned long now = fn_00672870();
			if ( now - field_9C >= 500 && now - field_A0 >= 100 )
			{
				field_A0 = now;
				fn_004c8a57( field_80, 0 );
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x004c82f8
unsigned char tScrollBar::fn_004c82f8( tRect *rect )
{
	rect->left = 0;
	rect->top = 0;
	rect->right = 0;
	rect->bottom = 0;

	if ( field_A8 && field_14 )
	{
		int range = field_68 - field_64;
		if ( range > 0 )
		{
			int track;
			if ( field_70 )
				track = field_34.mWidth() - field_72.mWidth();
			else
				track = field_34.mHeight() - field_72.mHeight();

			if ( track > 0 )
			{
				int position = ( field_80 == 8 ) ? field_98 : field_6C;
				if ( position < field_64 )
					position = field_64;
				else if ( position > field_68 )
					position = field_68;

				double fraction = (double)position / range;
				if ( field_70 )
				{
					rect->left = (short)( track * fraction ) + field_34.left;
					rect->top = field_34.top;
					rect->right = rect->left + field_72.mWidth();
					rect->bottom = field_34.bottom;
				}
				else
				{
					rect->left = field_34.left;
					rect->top = (short)( track * fraction ) + field_34.top;
					rect->right = field_34.right;
					rect->bottom = rect->top + field_72.mHeight();
				}
				return 1;
			}
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x004c8439
void tScrollBar::vfunction8( const tRect *area )
{
	if ( field_14 == 0 || field_A8 == 0 )
		return;

	if ( field_80 == 8 )
	{
		// HYPOTHESIS: field_90 and field_92 are the thumb's corner while it is dragged.
		field_14->fn_00675280( field_A8, field_A8->field_04 + field_90,
			field_A8->field_06 + field_92, area, 0, 1 );
	}
	else
	{
		tRect thumb;
		if ( fn_004c82f8( &thumb ) )
			field_14->fn_00675280( field_A8, field_A8->field_04 + thumb.left,
				field_A8->field_06 + thumb.top, area, 0, 1 );
	}
}

// FUNCTION: STARFLEETOP 0x004c84d7
int tScrollBar::fn_004c84d7( int x, int y )
{
	tRect thumb;
	if ( fn_004c82f8( &thumb ) && field_34.mPointInside( x, y ) )
	{
		if ( thumb.mPointInside( x, y ) )
			return 8;
		if ( field_70 )
		{
			if ( x < thumb.left )
				return 5;
			if ( x >= thumb.right )
				return 6;
		}
		else
		{
			if ( y < thumb.top )
				return 5;
			if ( y >= thumb.bottom )
				return 6;
		}
	}
	return 0;
}

// tbasic.h's inline tRect::mPointInside, emitted out of line after its caller in this unit.
// FUNCTION: STARFLEETOP 0x004c855e
unsigned char tRect::mPointInside( int x, int y ) const
{
	return ( ( left <= (short)x ) && ( top <= (short)y )
		&& ( right > (short)x ) && ( bottom > (short)y ) );
}

// FUNCTION: STARFLEETOP 0x004c8587
unsigned char tScrollBar::vfunction31( c_tListBox_Base_Event *event )
{
	if ( ( field_40 & 1 ) && event->field_00 >= 1 && event->field_00 <= 7 )
	{
		int x = event->field_04;
		int y = event->field_08;
		field_88 = x;
		field_8A = y;

		switch ( event->field_00 )
		{
		case 7:
			if ( field_80 == 8 )
			{
				if ( field_70 )
					fn_004c8c6a( x, y );
				else
					fn_004c8b60( x, y );
				return true;
			}
			if ( field_34.mPointInside( x, y ) )
				return true;
			return false;

		case 1:
			field_84 = fn_004c84d7( x, y );
			if ( field_84 >= 5 && ( field_84 <= 6 || field_84 == 8 ) )
			{
				if ( field_80 == 0 )
				{
					tRect thumb;
					if ( fn_004c82f8( &thumb ) && field_0C->fn_0067d2d0( this ) )
					{
						field_80 = field_84;
						field_98 = field_6C;
						field_94 = x;
						field_96 = y;
						field_8C = thumb.left;
						field_8E = thumb.top;
						field_90 = thumb.left;
						field_92 = thumb.top;
						field_9C = fn_00672870();
					}
				}
				if ( field_80 )
					fn_004c8a57( field_80, ( field_80 == 8 ) ? field_98 : 0 );
				return true;
			}
			break;

		case 4:
			if ( field_80 )
			{
				field_0C->fn_0067d311( this );
				if ( field_80 == 8 )
				{
					tRect thumb;
					fn_004c8afe( &thumb );
					if ( thumb.mPointInside( x, y ) )
						fn_004c8a57( 7, field_98 );
					else
						fn_004c8a57( 7, field_6C );
					fn_0045f4c2();
				}
				field_80 = 0;
				field_84 = 0;
				field_9C = 0;
				field_A0 = 0;
				return true;
			}
			break;
		}
	}
	return false;
}

// HYPOTHESIS: the original 10 bytes (mov eax, <ehfuncinfo>; call __EH_prolog) are the frame
// setup of mLoad (0x004c878a), which the analysis splits off as a function of its own; slot 37
// is mLoad. This empty body only claims the address.
// STUB: STARFLEETOP 0x004c8780
void tScrollBar::vfunction37()
{
}

// FUNCTION: STARFLEETOP 0x004c878a
// The rebuilt function starts with the 10 bytes the original keeps at 0x004c8780.
void tScrollBar::mLoad( tAsset *asset )
{
	field_48 = asset->field_04;
	tSliderAsset *slider = dynamic_cast< tSliderAsset * >( asset );
	if ( slider == 0 )
		return;

	field_34 = tRect( 0, 0, slider->field_B0, slider->field_B2 );
	short width = slider->field_B0;
	short height = slider->field_B2;
	if ( height < width )
		field_70 = true;
	else if ( height > width )
		field_70 = false;
	else
	{
		const char *message = "tScrollBar::mLoad(): Can't determine if we are vertical or horizontal.";
		if ( !g_00c69414 )
		{
			fn_006c9980( message );
			g_00c693a0.mSay( message );
		}
		field_70 = true;
	}

	c_00677060_Result *found = g_00c69100.fn_00677060( fn_0067cc50( slider->field_B4 ) );
	if ( found )
	{
		c_tScrollBar_FieldA8 *image = g_00c69100.fn_006770a0( found->field_04 );
		if ( image )
		{
			field_A4 = found->field_04;
			field_A8 = image;
			field_72 = tRect( 0, 0, image->field_00, image->field_02 );

			if ( field_34.mWidth() < field_72.mWidth() )
				field_34.right = field_34.left + field_72.mWidth();
			if ( field_34.mHeight() < field_72.mHeight() )
				field_34.bottom = field_34.top + field_72.mHeight();
			if ( field_70 )
				field_34.bottom = field_34.top + field_72.mHeight();
			else
				field_34.right = field_34.left + field_72.mWidth();
		}
	}

	if ( slider->field_B8.mStrLen() )
	{
		long dot = slider->field_B8.find( ".", 0 );
		if ( dot )
			field_AC = slider->field_B8.substr( 0, dot );
		else
			field_AC = slider->field_B8;
	}
}

// FUNCTION: STARFLEETOP 0x004c899b
unsigned char tScrollBar::vfunction41( int minimum, int maximum, unsigned char redraw )
{
	if ( maximum < minimum )
		return false;
	__int64 range = (__int64)maximum - (__int64)minimum;
	if ( range > 0x7fffffff )
		return false;

	if ( field_6C < minimum )
		vfunction43( minimum, 0 );
	else if ( field_6C > maximum )
		vfunction43( maximum, 0 );

	field_64 = minimum;
	field_68 = maximum;
	if ( redraw )
		fn_0045f4c2();
	return true;
}

// FUNCTION: STARFLEETOP 0x004c8a0e
unsigned char tScrollBar::vfunction42( int *minimum, int *maximum )
{
	if ( minimum )
		*minimum = field_64;
	if ( maximum )
		*maximum = field_68;
	return true;
}

// FUNCTION: STARFLEETOP 0x004c8a2d
unsigned char tScrollBar::vfunction43( int position, unsigned char redraw )
{
	if ( position < field_64 )
		position = field_64;
	if ( position > field_68 )
		position = field_68;
	field_6C = position;
	if ( redraw )
		fn_0045f4c2();
	return true;
}