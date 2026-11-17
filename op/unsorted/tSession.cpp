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
};

// HYPOTHESIS: the layout is read from the inlined destructor in fn_0068ff60 only. The
// constructor is out of line at 0x0068f9c0.
class tHtmlParserBase
{
public:
	~tHtmlParserBase() {}

	char	pad_00[0x10];
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

	tString	f_10;
	char	pad_10[0x44 - 0x10 - sizeof(tString)];
	tString	f_44;
	fn_0068ff60_Root	*f_6c;
	tDir	f_70;
	char	pad_80[0x94 - 0x80];
	std::vector<unsigned short>	f_94;
	std::vector<unsigned short>	f_a4;
	std::vector<unsigned short>	f_b4;
	char	pad_c4[0xdc - 0xc4];
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
