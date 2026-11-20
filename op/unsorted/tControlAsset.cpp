// Unit:      unsorted/tControlAsset.cpp
// Functions: 0x00678ce0 to 0x006793b0, 12 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "unsorted/tAsset.h"

// HYPOTHESIS: the body is empty and the members clean up themselves. The original destroys a
// vector of 0x30-byte entries, a tree and a tString inside field_60; c_00678500 is still
// declared as bytes, because declaring those members lowers the factory fn_00676a30.
// FUNCTION: STARFLEETOP 0x00678db0
tControlAsset::~tControlAsset()
{
}

// HYPOTHESIS: the library's red-black tree of a map keyed by tString, the tree inside
// field_60 that ~tControlAsset clears through 0x00678ea0. The SDK libraries name this shape
// std::map< tString, int, less_ss >; its pair destructor (0x006358d0), _Inc (0x00684840) and
// single erase (0x00684120) are folded with the other maps of the same shape. The mapped
// type is not known. Written out because the toolchain's xtree keeps a static nil node,
// where the original's library kept one per instance at +0x8 and the size at +0x10.
// The library's _Construct for the node value is held out of line at 0x0067c4d0.
namespace std
{
template<> void _Construct( std::pair< const tString, int >* _P, const std::pair< const tString, int >& _V );
};

struct fn_00678ea0_tree
{
	typedef std::pair< const tString, int > value_type;

	struct node
	{
		node*	left;
		node*	parent;
		node*	right;
		value_type	value;
		char	color;
		char	isnil;
	};

	class iterator
	{
	public:
		iterator( node* n ) : m_node( n ) {}
		void fn_00684840();
		iterator operator++( int ) { iterator tmp = *this; fn_00684840(); return tmp; }
		bool operator!=( const iterator& other ) const { return m_node != other.m_node; }
		node* m_node;
	};

	iterator erase( iterator first, iterator last );
	iterator erase( iterator where );
	void fn_00679210( const fn_00678ea0_tree& x );
	// MATCH-ONLY: the library's inline _Buynode, _Copy, _Min and _Max; the out-of-line copy of
	// _Copy is 0x00679310.
	node* _Buynode( node* parent, char color )
	{
		node* n = (node*)operator new( sizeof( node ) );
		n->parent = parent;
		n->color = color;
		n->isnil = 0;
		return n;
	}
	node* fn_00679310( node* root, node* where )
	{
		node* newroot = m_nil;
		if( !root->isnil )
		{
			node* n = _Buynode( where, root->color );
			std::_Construct( &n->value, root->value );
			n->left = m_nil;
			n->right = m_nil;
			if( newroot == m_nil )
				newroot = n;
			n->left = fn_00679310( root->left, n );
			n->right = fn_00679310( root->right, n );
		}
		return newroot;
	}
	static node* _Min( node* p )
	{
		while( !p->left->isnil )
			p = p->left;
		return p;
	}
	static node* _Max( node* p )
	{
		while( !p->right->isnil )
			p = p->right;
		return p;
	}
	// MATCH-ONLY: the library's inline _Erase; the out-of-line copy is 0x00678aa0.
	void fn_00678aa0( node* root )
	{
		for( node* y = root; y != m_nil; root = y )
		{
			fn_00678aa0( y->right );
			y = y->left;
			root->value.~pair();
			operator delete( root );
		}
	}

	char	m_pad[4];
	node*	m_head;
	node*	m_nil;
	bool	m_multi;
	unsigned int	m_size;
};

// FUNCTION: STARFLEETOP 0x00678ea0
fn_00678ea0_tree::iterator fn_00678ea0_tree::erase( iterator first, iterator last )
{
	if( m_size == 0 || first.m_node != m_head->left || last.m_node != m_head )
	{
		while( first != last )
			erase( first++ );
		return first;
	}
	else
	{
		fn_00678aa0( m_head->parent );
		m_head->parent = m_nil;
		m_size = 0;
		m_head->left = m_head;
		m_head->right = m_head;
		return iterator( m_head->left );
	}
}

// FUNCTION: STARFLEETOP 0x00679210
void fn_00678ea0_tree::fn_00679210( const fn_00678ea0_tree& x )
{
	m_head->parent = fn_00679310( x.m_head->parent, m_head );
	m_size = x.m_size;
	if( m_head->parent != m_nil )
	{
		m_head->left = _Min( m_head->parent );
		m_head->right = _Max( m_head->parent );
	}
	else
	{
		m_head->left = m_head;
		m_head->right = m_head;
	}
}

// HYPOTHESIS: the library vector's capacity() for 0x30-byte entries (first at +4, end of
// storage at +0xc), in the library's ternary form; the owning class is not known.
struct fn_00679140_vector
{
	struct entry
	{
		char	m_pad[0x30];
	};

	int fn_00679140();
	int fn_00679170();


	char	m_pad[4];
	entry*	m_first;
	entry*	m_last;
	entry*	m_end;
};

// FUNCTION: STARFLEETOP 0x00679140
int fn_00679140_vector::fn_00679140()
{
	return (m_first == 0 ? 0 : m_end - m_first);
}

// HYPOTHESIS: the same library vector's size() (last - first) in the same ternary form.
// FUNCTION: STARFLEETOP 0x00679170
int fn_00679140_vector::fn_00679170()
{
	return (m_first == 0 ? 0 : m_last - m_first);
}

// HYPOTHESIS: the library vector's _Destroy(first, last) for the 0x30-byte entries (a tString
// first); a member function, since the original returns with ret 8 and the owner is not known.
struct fn_006791a0_vector
{
	struct entry
	{
		std::pair< const tString, int >	m_pair;
		char	m_pad[4];
	};

	void fn_006791a0( entry* first, entry* last );
};

// FUNCTION: STARFLEETOP 0x006791a0
void fn_006791a0_vector::fn_006791a0( entry* first, entry* last )
{
	for( ; first != last; ++first )
		first->m_pair.~pair();
}

// HYPOTHESIS: the library vector's _Ucopy(first, last, dest) for the 0x30-byte entries; the
// per-entry copy-construct is the function at 0x0067c460.
struct fn_006791d0_vector
{
	struct entry
	{
		char	m_pad[0x30];
	};

	entry* fn_006791d0( entry* first, entry* last, entry* dest );
};

void fn_0067c460( fn_006791d0_vector::entry* dest, fn_006791d0_vector::entry* src );

// FUNCTION: STARFLEETOP 0x006791d0
fn_006791d0_vector::entry* fn_006791d0_vector::fn_006791d0( entry* first, entry* last, entry* dest )
{
	for( ; first != last; ++first, ++dest )
		fn_0067c460( dest, first );
	return dest;
}
