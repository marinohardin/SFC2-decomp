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
