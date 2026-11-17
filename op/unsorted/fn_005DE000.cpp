#include "unsorted/tNewsStory.h"

extern char g_b00c38458;    // "ASCIICharacterSet"

// HYPOTHESIS: an ordering predicate on news stories: by detail time, then by text. It takes
// no this and pops its two arguments, so it is written as __stdcall.
// FUNCTION: STARFLEETOP 0x005de0cf
bool __stdcall fn_005de0cf( tNewsStory* a, tNewsStory* b )
{
	bool less;
	if ( a->fn_0054745f().fn_00546986( b->fn_0054745f() ) )
	{
		int ascii = g_b00c38458 != 0;
		fn_0040651e_type* text = b->fn_00547450( ascii );
		less = text->fn_0040651e( a->fn_00547450( ascii ) ) < 0;
	}
	else
	{
		char earlier = a->fn_0054745f().fn_0054695e( b->fn_0054745f() );
		return earlier ? true : false;
	}
	return less;
}

// HYPOTHESIS: a second ordering predicate on news stories: by the byte fn_0054744c returns
// (higher first), then by text.
// FUNCTION: STARFLEETOP 0x005de1b8
bool __stdcall fn_005de1b8( tNewsStory* a, tNewsStory* b )
{
	bool less;
	int ascii = g_b00c38458 != 0;
	if ( a->fn_0054744c() == b->fn_0054744c() )
	{
		fn_0040651e_type* text = b->fn_00547450( ascii );
		less = text->fn_0040651e( a->fn_00547450( ascii ) ) < 0;
	}
	else
	{
		less = a->fn_0054744c() > b->fn_0054744c();
	}
	return less;
}
