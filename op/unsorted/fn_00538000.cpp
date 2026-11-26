#include "unsorted/tBitmapFontEx.h"

// FUNCTION: STARFLEETOP 0x0053805e
void tBitmapFontEx::vfunction10()
{
	field_20++;
}

void __fastcall fn_00537bfd(int* self);

// FUNCTION: STARFLEETOP 0x00538062
void tBitmapFontEx::vfunction11()
{
	if (field_20 != 0) {
		field_20--;
		if (field_20 == 0) {
			fn_00537bfd((int*)this);
			delete this;
		}
	} else {
		field_20 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00538093
void tBitmapFontEx::vfunction12()
{
	vfunction11();
}

tBitmapFontEx *fn_005380dd();
extern "C" int __cdecl stricmp( const char *a, const char *b );
extern char g_00c3224c[];

// HYPOTHESIS: walks the fonts from the first one (0x005380dd) through slot 9 (the next) for the
// one whose name, copied by slot 4, matches.
// FUNCTION: STARFLEETOP 0x00538098
tBitmapFontEx *fn_00538098( const char *name )
{
	tBitmapFontEx *font = fn_005380dd();
	while ( font != 0 ) {
		font->vfunction4( g_00c3224c, 0x100 );
		if ( stricmp( g_00c3224c, name ) == 0 )
			return font;
		font = (tBitmapFontEx *)font->vfunction9();
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x0053860e
char fn_0053860e( int c )
{
	return "; !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"[( c - 0x20U & 0xff ) + 1];
}
