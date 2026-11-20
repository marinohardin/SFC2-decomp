// Unit:      unsorted/tCastMgr.cpp
// Functions: 0x00675db0 to 0x00677010, 12 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "Tiger/tFile.h"
#include "unsorted/tListBox_Base.h"	// HYPOTHESIS: g_00c69100 is declared there for now
#include "unsorted/c_00692f60_Error.h"
#include "unsorted/tAsset.h"
#include "unsorted/tVisGroupAsset.h"
#include "unsorted/tButtonAsset.h"
#include <string.h>
#include <vector>
#include <map>
inline tBoundAsset::tBoundAsset() { field_5E = 0; field_5C = 0; }	// MATCH-ONLY: the original expands this constructor in the factory
// HYPOTHESIS: the map tCastMgr holds at +0x180. It is not this compiler's own std::map (0x10
// bytes, one static nil node): its tree keeps an allocator byte at +0, a comparison byte at +1,
// the head node at +4, a nil node of its own at +8, the multi flag at +0xc and the size at
// +0x10, and the constructor builds both nodes inline. Each node holds 8 bytes of value at +0xc.
// Only the constructor is declared yet.
struct fn_00675db0_map
{
	struct node
	{
		node*	left;
		node*	parent;
		node*	right;
		long	first;
		long	second;
		char	color;
		char	isnil;
	};
	struct _Pr
	{
	};
	struct _A
	{
	};

	fn_00675db0_map( const _Pr& pred = _Pr(), const _A& al = _A() )
		: allocator( al ), key_compare( pred ), _Multi( false )
	{
		_Init();
	}
	void _Init()
	{
		_Nil = _Buynode( 0, 1 );
		_Nil->isnil = 1;
		_Nil->left = 0;
		_Nil->right = 0;
		_Head = _Buynode( _Nil, 0 );
		_Head->left = _Head;
		_Head->right = _Head;
		_Size = 0;
	}
	node* _Buynode( node* parent, char color )
	{
		node* s = (node*)operator new( sizeof( node ) );
		s->parent = parent;
		s->color = color;
		s->isnil = 0;
		return s;
	}

	_A	allocator;
	_Pr	key_compare;
	node*	_Head;
	node*	_Nil;
	bool	_Multi;
	unsigned int	_Size;
};

// HYPOTHESIS: the array tCastMgr holds at +0x194, laid out like std::vector (an allocator
// byte and three pointers). The constructor sets no unwind state after it or the map, so neither
// has a destructor this compiler sees; only the constructor is declared yet.
struct fn_00675db0_vector
{
	struct _A
	{
	};

	fn_00675db0_vector( const _A& al = _A() )
		: allocator( al ), _First( 0 ), _Last( 0 ), _End( 0 )
	{
	}

	_A	allocator;
	void**	_First;
	void**	_Last;
	void**	_End;
};

// HYPOTHESIS: the layout is read from the constructor alone; the element types of the
// containers are not known yet.
class tCastMgr
{
public:
	tCastMgr();
	virtual ~tCastMgr();
	virtual void vfunction1();
	void fn_00675d90();
	int fn_00675f90( unsigned int limit, int base );
	unsigned char fn_00675fe0( unsigned int pos );

	char	field_04[0x104];
	tBufFile	field_108;
	char	field_160[0x10];
	std::vector<void *>	field_170;
	fn_00675db0_map	field_180;
	fn_00675db0_vector	field_194;
	int	field_1a4;
};

// HYPOTHESIS: the 0x34-byte entries of field_194: a file name, then its length at +0x30.
struct fn_00675fe0_Entry
{
	tString	name;
	char	field_28[0x8];
	unsigned int	length;
};

// FUNCTION: STARFLEETOP 0x00675db0
tCastMgr::tCastMgr()
	: field_108( 0x4000 )
{
	fn_00675d90();
}

// HYPOTHESIS: the SDK's Tiger/tstring.h declares this comparator next to less_ss; op/Tiger is
// outside this lane, so it is declared here until it moves there.
class less_ps : public std::binary_function<const tString*, const tString*, bool>
{	public:
	bool operator () (const tString* x, const tString* y)
		const 	{	return ( (*x).mStrCmp( (*y) ) < 0); }
};

// HYPOTHESIS: a global map of names to values at 0x00c69280 (head node at +4); 0x0067bd90 is
// its lower_bound, inlined here into find.
extern std::map< const tString*, int, less_ps > g_00c69280;

// FUNCTION: STARFLEETOP 0x00675ec0
int fn_00675ec0( const tString& name )
{
	std::map< const tString*, int, less_ps >::iterator it = g_00c69280.find( &name );
	if ( it == g_00c69280.end() )
		return 0;
	return (*it).second;
}

// FUNCTION: STARFLEETOP 0x00675f50
// FUNCTION: STARFLEETOP 0x00675f40
tString g_00c690d8;

// HYPOTHESIS: returns the name of the cast member an id identifies, or an empty string.
// FUNCTION: STARFLEETOP 0x00675f70
tString* fn_00675f70( int id )
{
	c_00677060_Result* found = g_00c69100.fn_00677060( id );
	if ( found )
		return &found->field_08;
	return &g_00c690d8;
}

// HYPOTHESIS: field_194 holds 0x34-byte entries whose dword at +0x30 is a length; this walks
// them until the running total reaches limit, adding the total before that entry to base.
// FUNCTION: STARFLEETOP 0x00675f90
int tCastMgr::fn_00675f90( unsigned int limit, int base )
{
	if ( base == 0 )
		return base;
	char* p = (char*)field_194._First;
	unsigned int total = 0;
	char* last = (char*)field_194._Last;
	for ( ; p != last; p += 0x34 )
	{
		unsigned int next = *(unsigned int*)( p + 0x30 ) + total;
		if ( limit <= next )
			return base + total;
		total = next;
	}
	return base;
}

// HYPOTHESIS: makes the cast file holding position pos current: finds the entry of field_194
// whose running length reaches pos and, when its number differs from field_1a4, closes field_108
// and opens the entry's file there by its name without the directory. Throws when it fails.
// FUNCTION: STARFLEETOP 0x00675fe0
unsigned char tCastMgr::fn_00675fe0( unsigned int pos )
{
	fn_00675fe0_Entry* e = (fn_00675fe0_Entry*)field_194._First;
	unsigned int total = 0;
	int index = 1;
	for ( ; e != (fn_00675fe0_Entry*)field_194._Last; e++, index++ )
	{
		unsigned int next = total + e->length;
		total = next;
		if ( pos <= next )
		{
			if ( index != field_1a4 )
			{
				tBufFile* file = &field_108;
				file->mClose();
				tString name( e->name + ".q3");
				tString path( name.mCharStr() );
				tString::iterator it = path.mStrRChr( '\\' );
				if ( it != path.end() )
				{
					tString tail( it + 1 );
					path = tail;
				}
				char* dest = field_04;
				strcpy( dest, path.mCharStr() );
				if ( !file->mOpenForRead( dest ) )
				{
					int id = fn_006741e0( "tQ3Error_Q3CAST_OpeningFile" );
					static tString sEmpty( g_0083b344 );
					tString text( g_00c690d0 ? g_00c690d0->fn_006740f0( id ) : sEmpty );
					text += " ";
					text += dest;
					throw c_00692f60_Error( text, 1 );
				}
				field_1a4 = index;
			}
			return 1;
		}
	}
	return 0;
}

// HYPOTHESIS: byte-swaps a 32-bit value held as two 16-bit halves (network to host order).
// FUNCTION: STARFLEETOP 0x00676a00
void fn_00676a00( unsigned short* p )
{
	unsigned short b = p[1];
	unsigned short a = p[0];
	p[0] = (unsigned short)( ( b << 8 ) | ( b >> 8 ) );
	p[1] = (unsigned short)( ( a << 8 ) | ( a >> 8 ) );
}

// HYPOTHESIS: makes an empty asset of the kind type names; id is stored in it (field_04).
// Kinds 2, 6, 7, 14, 19 and 20 make nothing.
// The original calls the base constructors out of line in some cases and expands them in others:
// tVisGroupAsset (0x0067acf0) in kind 17, tControlAsset (0x00678ce0) in 15 and 16, tBoundAsset
// (0x00678160) in 21 and 25, tBmpAsset (0x00678ae0) in 26; it expands tStaticTextAsset.
// FUNCTION: STARFLEETOP 0x00676a30
tAsset* fn_00676a30( unsigned short type, int id )
{
	tAsset* asset = 0;
	if ( id )
	{
		switch ( type )
		{
		case 1:
			asset = new tBmpAsset;
			break;
		case 4:
		case 5:
		case 22:
			asset = new tCastItem;
			break;
		case 17:
			asset = new tRadioGroupAsset;
			break;
		case 3:
			asset = new tVisGroupAsset;
			break;
		case 8:
			asset = new tPushBtnAsset;
			break;
		case 18:
			asset = new tPushRectAsset;
			break;
		case 23:
			asset = new tTextButtonAsset;
			break;
		case 12:
			asset = new tBrowserAsset;
			break;
		case 10:
			asset = new tValueBtnAsset;
			break;
		case 11:
			asset = new tSliderAsset;
			break;
		case 15:
			asset = new tScrollBoxAsset;
			break;
		case 16:
			asset = new tNavigatorAsset;
			break;
		case 13:
			asset = new tBoundAsset;
			break;
		case 21:
			asset = new tStaticTextAsset;
			break;
		case 24:
			asset = new tTableAsset;
			break;
		case 9:
			asset = new tTextEditAsset;
			break;
		case 25:
			asset = new tGenericRectAsset;
			break;
		case 26:
			asset = new tGenericBmpAsset;
			break;
		}
		if ( asset )
		{
			asset->field_30 = type;
			asset->field_04 = id;
		}
	}
	return asset;
}

// HYPOTHESIS: deletes every cast item held in field_170, closes the file and resets.
// FUNCTION: STARFLEETOP 0x00677010
void tCastMgr::vfunction1()
{
	std::vector<void *>::iterator it = field_170.begin();
	for ( ; it != field_170.end(); it++ )
	{
		tAsset* item = (tAsset*)*it;
		if ( item )
			delete item;
	}
	field_108.mClose();
	fn_00675d90();
}
