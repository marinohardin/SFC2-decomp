// Unit:      unsorted/tCastMgr.cpp
// Functions: 0x00675db0 to 0x00677010, 12 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "Tiger/tFile.h"
#include <vector>

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
	void fn_00675d90();

	char	field_04[0x104];
	tBufFile	field_108;
	char	field_160[0x10];
	std::vector<void *>	field_170;
	fn_00675db0_map	field_180;
	fn_00675db0_vector	field_194;
};

// FUNCTION: STARFLEETOP 0x00675db0
tCastMgr::tCastMgr()
	: field_108( 0x4000 )
{
	fn_00675d90();
}
