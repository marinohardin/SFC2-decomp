// Unit:      unsorted/tSession.cpp
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// HYPOTHESIS: the analysis puts these functions under tSession; 0x0068ff60 is a free function
// that runs the HTML parser (RTTI tHtmlParser, tHtmlParserBase) over a file.

#include <vector>
#include "Tiger/tstring.h"
#include "Tiger/tgfsys.h"
#include "unsorted/tVisObject.h"

class fn_0068d830_Item;

// HYPOTHESIS: the object the parse builds; the parser keeps it at +0x6c and the caller gets it.
class fn_0068ff60_Root
{
public:
	void	fn_006926e0( tDir dir );
	void	fn_0068b7f0( fn_0068d830_Item *item );

	char	pad_00[0x34];
	tRect	f_34;
	char	pad_3c[0x68 - 0x3c];
	// HYPOTHESIS: a string whose wide text fn_006901b0 hands to the parser.
	tString	f_68;
};

// HYPOTHESIS: the parse result returned by tHtmlParser::fn_00692620; deleted through its
// virtual destructor when the parse built no root.
class fn_0068d830_Item
{
public:
	virtual	~fn_0068d830_Item();

	void	fn_0068d830( tRect rect );
	void	fn_0068db60();
};

// HYPOTHESIS: the layout is read from the inlined destructor in fn_0068ff60 only. The
// constructor is out of line at 0x0068f9c0.
class tHtmlParserBase
{
public:
	~tHtmlParserBase() {}

	char	pad_00[0x04];
	// HYPOTHESIS: the text fn_006904d0 hands to the browser it finds.
	char	*f_04;
	// HYPOTHESIS: the read position in the wide text; fn_00690900 reads and steps it.
	unsigned short	*f_08;
	char	pad_0c[0x10 - 0x0c];
};

class tHtmlParser: public tHtmlParserBase
{
public:
	tHtmlParser();

	void	fn_00690180( tString &name, fn_0068ff60_Root *root, tDir dir );
	// HYPOTHESIS: the argument is the wide text to parse (fn_006901b0 passes mWideStr()).
	fn_0068d830_Item	*fn_00692620( iBasicStringElement *text );
	bool	fn_0068f660();
	void	fn_0068f6b0( tString &name );
	void	fn_00690400();
	void	fn_006904d0();
	void	fn_00690900();
	void	fn_006916f0( char size, int b );
	bool	fn_0068f5c0( tString &name );
	int	fn_00691fd0();
	void	fn_0068f480();
	void	fn_006910a0( int align );

	tString	f_10;
	char	pad_10[0x3c - 0x10 - sizeof(tString)];
	// HYPOTHESIS: flags; fn_006904d0 clears bit 0 when it is done.
	int	f_3c;
	fn_0068d830_Item	*f_40;
	tString	f_44;
	fn_0068ff60_Root	*f_6c;
	tDir	f_70;
	char	pad_80[0x8c - 0x80];
	// HYPOTHESIS: the current alignment; fn_00690900 passes it on unless an align attribute sets one.
	unsigned char	f_8c;
	char	pad_8d[0x92 - 0x8d];
	unsigned short	f_92;
	std::vector<unsigned short>	f_94;
	std::vector<unsigned short>	f_a4;
	std::vector<unsigned short>	f_b4;
	char	pad_c4[0xd8 - 0xc4];
	// HYPOTHESIS: set, fn_006904d0 does nothing.
	char	f_d8;
	char	pad_d9[0xdc - 0xd9];
};

// FUNCTION: STARFLEETOP 0x0068ff60
fn_0068ff60_Root *fn_0068ff60( iBasicStringElement *a, tString &b, fn_0068ff60_Root *c, tDir dir )
{
	tHtmlParser	parser;

	parser.fn_00690180( b, c, dir );
	fn_0068d830_Item	*item = parser.fn_00692620( a );
	fn_0068ff60_Root	*root = parser.f_6c;
	if ( root )
	{
		if ( dir.mValid() )
			root->fn_006926e0( dir );
		item->fn_0068d830( root->f_34 );
		root->fn_0068b7f0( item );
	}
	else
		delete item;

	return parser.f_6c;
}

// FUNCTION: STARFLEETOP 0x00690180
void tHtmlParser::fn_00690180( tString &name, fn_0068ff60_Root *root, tDir dir )
{
	f_44 = name;
	f_6c = root;
	f_70 = dir;
}

// FUNCTION: STARFLEETOP 0x006901b0
fn_0068d830_Item *fn_006901b0( fn_0068ff60_Root *root )
{
	tHtmlParser	parser;
	tDir	dir;

	parser.fn_00690180( tString( "" ), root, dir );
	return parser.fn_00692620( root->f_68.mWideStr() );
}

// FUNCTION: STARFLEETOP 0x00690300
// HYPOTHESIS: a wrapper with fn_0068ff60's parameters that passes them on unchanged; the
// argument types are taken from fn_0068ff60.
fn_0068ff60_Root *fn_00690300( iBasicStringElement *a, tString &b, fn_0068ff60_Root *c, tDir dir )
{
	return fn_0068ff60( a, b, c, dir );
}

// FUNCTION: STARFLEETOP 0x00690340
// HYPOTHESIS: the same wrapper taking the text as a narrow string.
fn_0068ff60_Root *fn_00690340( char *a, tString &b, fn_0068ff60_Root *c, tDir dir )
{
	tString	text;

	text = a;
	return fn_00690300( text.mWideStr(), b, c, dir );
}

// FUNCTION: STARFLEETOP 0x006903f0
// HYPOTHESIS: a wrapper that passes the root on to fn_006901b0 unchanged.
fn_0068d830_Item *fn_006903f0( fn_0068ff60_Root *root )
{
	return fn_006901b0( root );
}

// FUNCTION: STARFLEETOP 0x00690400
// HYPOTHESIS: both callees take this in ecx, so they are taken as members of the same class.
void tHtmlParser::fn_00690400()
{
	if ( fn_0068f660() )
	{
		tString	name;

		fn_0068f6b0( name );
		if ( f_44.mStrLen() == 0 )
			f_44 = name;
	}
}

// HYPOTHESIS: tBrowser (RTTI) is a visual object; only its being polymorphic and derived from
// tObjBase is known here, from the cast below.
class tBrowser : public tVisObject
{
};

// FUNCTION: STARFLEETOP 0x00690480
// HYPOTHESIS: finds the first browser in an object's tree, the object first, then its children
// (+0x24, siblings through +0x4) depth first.
tBrowser *fn_00690480( tObjBase *object )
{
	if ( object )
	{
		tBrowser	*browser = dynamic_cast< tBrowser * >( object );

		if ( browser )
			return browser;
		for ( tObjBase *child = object->field_24; child; child = child->field_04 )
		{
			browser = fn_00690480( child );
			if ( browser )
				return browser;
		}
	}
	return 0;
}

#include "Tiger/tsay.h"

extern tSay g_00c693a0;	// HYPOTHESIS: the say the warnings below go through (as in tScrollBar.cpp)
extern unsigned char g_00c69414;	// HYPOTHESIS: set, it silences those warnings
void fn_006c9980( const char *format, ... );	// HYPOTHESIS: the debug print beside each warning

// HYPOTHESIS: what 0x00675ec0 (in the tCastMgr range) finds for a group name.
class fn_00675ec0_Result;
fn_00675ec0_Result *fn_00675ec0( const tString &name );

// HYPOTHESIS: the group object 0x00674780 finds or loads; fn_00690480 searches its children.
class fn_00674780_Group : public tObjBase
{
public:
	tObjBase	*fn_00685ac0( fn_00675ec0_Result *name );
};

// HYPOTHESIS: the object at 0x00c690d0 that holds the groups.
class fn_00674750_Owner
{
public:
	tObjBase	*fn_00674750( fn_00675ec0_Result *name );
	fn_00674780_Group	*fn_00674780( fn_00675ec0_Result *name );
};

extern fn_00674750_Owner *g_00c690d0;

// FUNCTION: STARFLEETOP 0x006904d0
// HYPOTHESIS: finds the browser named by f_44 ("group" or "group.object"), hands it the text and
// lays the result out in its rectangle. The browser found is taken as the root at +0x6c.
void tHtmlParser::fn_006904d0()
{
	if ( f_d8 )
		return;

	tVisObject	*browser = 0;

	if ( f_44.mStrLen() )
	{
		tString	groupName;
		tString	objectName;

		long	pos = f_44.find( ".", 0 );
		if ( pos >= 0 && pos < f_44.mStrLen() )
		{
			groupName = f_44.substr( 0, pos );
			objectName = f_44.substr( pos + 1 );
		}
		else
			groupName = f_44;

		fn_00675ec0_Result	*group = fn_00675ec0( groupName );
		if ( !group )
		{
			if ( !g_00c69414 )
			{
				fn_006c9980( "don't recognize group '%s'\n", groupName.mCharStr() );
				g_00c693a0.mSay( "don't recognize group '%s'\n", groupName.mCharStr() );
			}
		}
		else
		{
			if ( g_00c690d0 )
			{
				tObjBase	*object = g_00c690d0->fn_00674750( group );
				if ( object )
					browser = dynamic_cast< tVisObject * >( object );
			}
			if ( !browser )
			{
				fn_00674780_Group	*loaded = 0;

				if ( g_00c690d0 )
					loaded = g_00c690d0->fn_00674780( group );
				if ( !loaded )
				{
					if ( !g_00c69414 )
					{
						fn_006c9980( "failed to find or load group '%s'\n", groupName.mCharStr() );
						g_00c693a0.mSay( "failed to find or load group '%s'\n", groupName.mCharStr() );
					}
				}
				else
				{
					if ( objectName.mStrLen() )
					{
						fn_00675ec0_Result	*name = fn_00675ec0( objectName.mCharStr() );
						if ( !name )
						{
							if ( !g_00c69414 )
							{
								fn_006c9980( "found group '%s' but don't recognize '%s'\n", groupName.mCharStr(), objectName.mCharStr() );
								g_00c693a0.mSay( "found group '%s' but don't recognize '%s'\n", groupName.mCharStr(), objectName.mCharStr() );
							}
						}
						else
						{
							tObjBase	*object = loaded->fn_00685ac0( name );
							if ( !object )
							{
								if ( !g_00c69414 )
								{
									fn_006c9980( "found group '%s' but not object '%s'\n", groupName.mCharStr(), objectName.mCharStr() );
									g_00c693a0.mSay( "found group '%s' but not object '%s'\n", groupName.mCharStr(), objectName.mCharStr() );
								}
							}
							else
								browser = dynamic_cast< tVisObject * >( object );
						}
					}
					if ( !browser )
					{
						browser = fn_00690480( loaded );
						if ( !browser )
						{
							if ( !g_00c69414 )
							{
								fn_006c9980( "failed to find child browser in group '%s'\n", groupName.mCharStr() );
								g_00c693a0.mSay( "failed to find child browser in group '%s'\n", groupName.mCharStr() );
							}
						}
					}
				}
			}
		}
	}

	// HYPOTHESIS: the root at +0x6c is the browser (both carry the rectangle at +0x34).
	if ( browser && browser != (tVisObject *)f_6c )
		f_6c = (fn_0068ff60_Root *)browser;

	if ( f_6c )
	{
		f_6c->f_68 = f_04;
		f_40->fn_0068d830( f_6c->f_34 );
	}
	else
	{
		if ( !g_00c69414 )
		{
			fn_006c9980( "Cannot display text - no browser found!\n" );
			g_00c693a0.mSay( "Cannot display text - no browser found!\n" );
		}
		f_40->fn_0068d830( tRect( 0, 0, 200, 300 ) );
	}
	f_3c &= ~1;
}

// FUNCTION: STARFLEETOP 0x00690900
// HYPOTHESIS: a heading tag (h1 to h6): the digit after the tag name sets the size, an align
// attribute the alignment; other attributes are skipped.
void tHtmlParser::fn_00690900()
{
	int	level = *f_08 - '0';

	if ( level <= 6 && level >= 1 )
	{
		f_08++;
		f_40->fn_0068db60();
		fn_006916f0( 7 - level, 0 );

		tString	name;
		int	align = f_8c;

		while ( fn_0068f5c0( name ) )
		{
			if ( name == "align" )
				align = fn_00691fd0();
			else
				fn_0068f480();
		}
		fn_006910a0( align );
		f_92 = ' ';
	}
}
