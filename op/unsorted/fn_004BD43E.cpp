//	fn_004BD43E.cpp: functions of the page 0x004bd000 with no unit or class evidence.
// Profile:   game-release (harness/units.json)

#include "unsorted/tListBox_Base.h"
#include "unsorted/tListBox_News.h"

#include <memory>
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
	c_004bd805_Entry();

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

// HYPOTHESIS: a one-byte class whose constructor is 0x005cf073; its only instance is owned by
// the auto_ptr at 0x0091f928 and created on first use (0x004bdc4d).
class c_005cf073_Object
{
public:
	c_005cf073_Object();
	~c_005cf073_Object();
};

// HYPOTHESIS: the global at 0x0091f928 is a std::auto_ptr (owner flag, then the pointer); its
// constructor clears both (0x004bda7f) and its destructor is registered with atexit (0x004bda8e).
// The template is written out here, with the destructor outside the class, so the instance
// at 0x004bdccf can carry its claim.
template<class _Ty>
class c_auto_ptr
{
public:
	explicit c_auto_ptr( _Ty *_P = 0 ) : _Owns( _P != 0 ), _Ptr( _P ) {}
	c_auto_ptr<_Ty> &operator=( const c_auto_ptr<_Ty> &_Y ) throw()
	{
		if ( _Ptr != _Y.get() )
		{
			if ( _Owns )
				delete _Ptr;
			_Owns = _Y._Owns;
			_Ptr = _Y.release();
		}
		else if ( _Y._Owns )
			_Owns = true, _Y.release();
		return *this;
	}
	~c_auto_ptr();
	_Ty *get() const { return _Ptr; }
	_Ty *release() const
	{
		( (c_auto_ptr<_Ty> *)this )->_Owns = false;
		return _Ptr;
	}
private:
	bool _Owns;
	_Ty *_Ptr;
};


// FUNCTION: STARFLEETOP 0x004bdccf
template<class _Ty>
c_auto_ptr<_Ty>::~c_auto_ptr()
{
	if ( _Owns )
		delete _Ptr;
}

// FUNCTION: STARFLEETOP 0x004bda8e
// FUNCTION: STARFLEETOP 0x004bda7f
c_auto_ptr<c_005cf073_Object> g_0091f928;

// HYPOTHESIS: a std::string global at 0x0091f910 holding the text "Unknown_Failure".
// FUNCTION: STARFLEETOP 0x004bdaf2
// FUNCTION: STARFLEETOP 0x004bdabf
std::string g_0091f910 = "Unknown_Failure";

// HYPOTHESIS: a std::string global at 0x0091f900 holding the text "Log_on_name_not_found".
// FUNCTION: STARFLEETOP 0x004bdb58
// FUNCTION: STARFLEETOP 0x004bdb25
std::string g_0091f900 = "Log_on_name_not_found";

// HYPOTHESIS: a std::string global at 0x0091f8f0 holding the text "Already_Logged_on".
// FUNCTION: STARFLEETOP 0x004bdbbe
// FUNCTION: STARFLEETOP 0x004bdb8b
std::string g_0091f8f0 = "Already_Logged_on";

// HYPOTHESIS: a std::string global at 0x0091f8e0 holding the text "Character_name_invalid".
// FUNCTION: STARFLEETOP 0x004bdc24
// FUNCTION: STARFLEETOP 0x004bdbf1
std::string g_0091f8e0 = "Character_name_invalid";

// FUNCTION: STARFLEETOP 0x004bdc4d
c_005cf073_Object *fn_004bdc4d()
{
	if ( g_0091f928.get() == 0 )
		g_0091f928 = c_auto_ptr<c_005cf073_Object>( new c_005cf073_Object );
	return g_0091f928.get();
}

// FUNCTION: STARFLEETOP 0x004bdcb7
const std::string &fn_004bdcb7()
{
	return g_0091f910;
}

// FUNCTION: STARFLEETOP 0x004bdcbd
const std::string &fn_004bdcbd()
{
	return g_0091f900;
}

// FUNCTION: STARFLEETOP 0x004bdcc3
const std::string &fn_004bdcc3()
{
	return g_0091f8f0;
}

// FUNCTION: STARFLEETOP 0x004bdcc9
const std::string &fn_004bdcc9()
{
	return g_0091f8e0;
}

// HYPOTHESIS: a second class with the layout of c_004bd805_Object. Its constructor has the same
// bytes as 0x004bd805, and the exe has both call 0x004bdd67, so the two may share that method
// through a relation not yet recovered.
class c_004bdd0d_Object
{
public:
	c_004bdd0d_Object();
	~c_004bdd0d_Object();
	void fn_004bdd67();
	void fn_004bde26( tString *text, int value );
	c_004bd805_Entry *fn_004bdf64( int index );

	tString field_0;
	c_004bd805_Entry field_28[ 500 ];
	int field_5618;
	int field_561c;
	int field_5620;
	bool field_5624;
};

// FUNCTION: STARFLEETOP 0x004bdd0d
c_004bdd0d_Object::c_004bdd0d_Object()
{
	fn_004bdd67();
}

// HYPOTHESIS: the entry's constructor, which only builds its string; the vector constructor
// of field_28 calls it for each entry.
// FUNCTION: STARFLEETOP 0x004bdd5b
c_004bd805_Entry::c_004bd805_Entry()
{
}

// HYPOTHESIS: clears the entries; both constructors call it.
// FUNCTION: STARFLEETOP 0x004bdd67
void c_004bd805_Object::fn_004bdd67()
{
	field_5618 = 0;
	field_561c = 0;
	field_5620 = 0;
	for ( int i = 0; i < 500; i++ )
	{
		field_28[ i ].text = "";
		field_28[ i ].field_28 = 0;
	}
	field_5624 = true;
}

// HYPOTHESIS: the destructor; it clears the entries as 0x004bd862 does.
// FUNCTION: STARFLEETOP 0x004bddaa
c_004bdd0d_Object::~c_004bdd0d_Object()
{
	field_5618 = 0;
	field_561c = 0;
	for ( int i = 0; i < 500; i++ )
	{
		field_28[ i ].text = "";
		field_28[ i ].field_28 = 0;
	}
}

// HYPOTHESIS: the list box width, a text's width and the split position, as 0x004bd5bc,
// 0x004bd645 and 0x004bd6d1 give them for the first class.
int fn_004be4df( void );
int fn_004be55e( tString *text );
int fn_004be5e0( tString *text, int width );

// HYPOTHESIS: adds a text to the entries, as 0x004bd8d4 does for the first class.
// FUNCTION: STARFLEETOP 0x004bde26
void c_004bdd0d_Object::fn_004bde26( tString *text, int value )
{
	field_5624 = true;
	int width = fn_004be4df();
	int textWidth = fn_004be55e( text );
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
		int split = fn_004be5e0( text, width );
		field_28[ field_561c ].text = text->substr( 0, split );
		field_28[ field_561c ].field_28 = value;
		field_561c++;
		if ( split > 0 )
			fn_004bde26( &text->substr( split, text->mStrLen() - split ), value );
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

// HYPOTHESIS: the entry at a position counted from the oldest one, wrapping at 500.
// FUNCTION: STARFLEETOP 0x004bdf64
c_004bd805_Entry *c_004bdd0d_Object::fn_004bdf64( int index )
{
	int pos = field_5618 + index;
	if ( pos >= 500 )
		pos -= 500;
	return &field_28[ pos ];
}

// HYPOTHESIS: the one instance of the second class; 0x004bdf8d is its dynamic initializer.
// FUNCTION: STARFLEETOP 0x004bdf8d
c_004bdd0d_Object g_004bdd0d_Object;
