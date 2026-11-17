//	fn_004BD43E.cpp: functions of the page 0x004bd000 with no unit or class evidence.
// Profile:   game-release (harness/units.json)

#include "unsorted/tListBox_Base.h"
#include "unsorted/tListBox_News.h"

#include <string>

// FUNCTION: STARFLEETOP 0x004bd443
int tListBox_Base::fn_004bd443( tString *text, int width )
{
	if ( fn_004be24a( text ) < width || width <= 0 )
		return -1;

	tString space( " " );
	int spaceWidth = field_B0->vfunction3( space.mWideStr(), 1 );
	unsigned short *chars = text->fn_00618920();
	int length = text->mStrLen();
	int lineWidth = fn_004be23f();
	int i = 0;
	while ( field_B0->vfunction3( chars, i ) + spaceWidth < lineWidth && i <= length )
		i++;

	if ( chars[ i ] != ' ' )
	{
		int n = 1;
		while ( chars[ i - n ] != ' ' )
			n++;
		if ( chars[ i - n ] == ' ' )
			i -= n;
	}
	return i;
}

// HYPOTHESIS: a global std::string; 0x004bd534 reads its pointer at 0x0091f8d4.
extern std::string g_0091f8d0;

// HYPOTHESIS: looks up what a name identifies (0x00675ec0 follows tCastMgr's constructor).
int fn_00675ec0( const tString &name );

// HYPOTHESIS: the object at 0x00c690d0; 0x00674750 returns the object of an id.
class c_00c690d0_Object
{
public:
	tObjBase *fn_00674750( int id );
};

extern c_00c690d0_Object *g_p00c690d0;

// FUNCTION: STARFLEETOP 0x004bd534
void fn_004bd534( void )
{
	int id = fn_00675ec0( tString( g_0091f8d0.c_str() ) );
	if ( id )
	{
		tListBox_News_Session *list = dynamic_cast< tListBox_News_Session * >( g_p00c690d0 ? g_p00c690d0->fn_00674750( id ) : 0 );
		if ( list )
			list->vfunction66();
	}
}

// FUNCTION: STARFLEETOP 0x004bd5bc
int fn_004bd5bc( void )
{
	int id = fn_00675ec0( tString( g_0091f8d0.c_str() ) );
	if ( id )
	{
		tListBox_Base *list = dynamic_cast< tListBox_Base * >( g_p00c690d0 ? g_p00c690d0->fn_00674750( id ) : 0 );
		if ( list )
			return list->fn_004be23f();
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x004bd645
int fn_004bd645( tString *text )
{
	int id = fn_00675ec0( tString( g_0091f8d0.c_str() ) );
	if ( id )
	{
		tListBox_Base *list = dynamic_cast< tListBox_Base * >( g_p00c690d0 ? g_p00c690d0->fn_00674750( id ) : 0 );
		if ( list )
			return list->fn_004be24a( text );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x004bd6d1
int fn_004bd6d1( tString *text, int width )
{
	int id = fn_00675ec0( tString( g_0091f8d0.c_str() ) );
	if ( id )
	{
		tListBox_Base *list = dynamic_cast< tListBox_Base * >( g_p00c690d0 ? g_p00c690d0->fn_00674750( id ) : 0 );
		if ( list )
			return list->fn_004bd443( text, width );
	}
	return -1;
}

// HYPOTHESIS: makes a news list box; the analysis starts it after its EH prologue.
// FUNCTION: STARFLEETOP 0x004bd782
tListBox_News *fn_004bd782( void )
{
	return new tListBox_News;
}

// HYPOTHESIS: makes a news session list box; the analysis starts it after its EH prologue.
// FUNCTION: STARFLEETOP 0x004bd7ce
tListBox_News_Session *fn_004bd7ce( void )
{
	return new tListBox_News_Session;
}

// HYPOTHESIS: an entry of 0x2c bytes; 0x004bdd5b builds it with tString's default
// constructor and 0x004bd853 destroys it by jumping to tString's destructor.
struct c_004bd805_Entry
{
	tString text;
	int field_28;
};

// HYPOTHESIS: a class of no known name holding a string and 500 entries; its constructor
// at 0x004bd805 is the only evidence so far.
class c_004bd805_Object
{
public:
	c_004bd805_Object();
	~c_004bd805_Object();
	void fn_004bdd67();
	void fn_004bd8d4( tString *text, int value );
	bool fn_004bda12();

	tString field_0;
	c_004bd805_Entry field_28[ 500 ];
	int field_5618;
	int field_561c;
	int field_5620;
	bool field_5624;
};

// FUNCTION: STARFLEETOP 0x004bd805
c_004bd805_Object::c_004bd805_Object()
{
	fn_004bdd67();
}

// HYPOTHESIS: the destructor; the analysis starts it after its EH prologue.
// FUNCTION: STARFLEETOP 0x004bd862
c_004bd805_Object::~c_004bd805_Object()
{
	field_5618 = 0;
	field_561c = 0;
	for ( int i = 0; i < 500; i++ )
	{
		field_28[ i ].text = "";
		field_28[ i ].field_28 = 0;
	}
}

// HYPOTHESIS: adds a text to the entries, splitting it where it is wider than the list box.
// FUNCTION: STARFLEETOP 0x004bd8d4
void c_004bd805_Object::fn_004bd8d4( tString *text, int value )
{
	field_5624 = true;
	int width = fn_004bd5bc();
	int textWidth = fn_004bd645( text );
	if ( field_561c >= 500 )
		field_561c = 0;
	if ( textWidth < width )
	{
		field_28[ field_561c ].text = *text;
		field_28[ field_561c ].field_28 = value;
		field_561c++;
	}
	else
	{
		int split = fn_004bd6d1( text, width );
		field_28[ field_561c ].text = text->substr( 0, split );
		field_28[ field_561c ].field_28 = value;
		field_561c++;
		if ( split > 0 )
			fn_004bd8d4( &text->substr( split, text->mStrLen() - split ), value );
	}
	field_5620++;
	if ( field_5620 > 500 )
	{
		field_5620 = 500;
		field_5618++;
		if ( field_5618 >= 500 )
			field_5618 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x004bda12
bool c_004bd805_Object::fn_004bda12()
{
	if ( field_5624 == true )
	{
		field_5624 = false;
		return true;
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x004bda3e
// FUNCTION: STARFLEETOP 0x004bda34
c_004bd805_Object g_004bd805_Object;

// HYPOTHESIS: a global object at 0x0091f928 holding a flag byte and a counter; its inline
// constructor clears both (0x004bda7f) and its destructor is registered with atexit (0x004bda8e).
class c_0091f928_Object
{
public:
	c_0091f928_Object() { field_0 = false; field_4 = 0; }
	~c_0091f928_Object();

	bool field_0;
	int field_4;
};

// FUNCTION: STARFLEETOP 0x004bda8e
// FUNCTION: STARFLEETOP 0x004bda7f
c_0091f928_Object g_0091f928;

// HYPOTHESIS: a std::string global at 0x0091f910 holding the text "Unknown_Failure".
// FUNCTION: STARFLEETOP 0x004bdabf
std::string g_0091f910 = "Unknown_Failure";
