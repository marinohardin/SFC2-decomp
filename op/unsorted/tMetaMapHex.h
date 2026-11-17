// tMetaMapHex: declarations of this class (original file unknown).
// Only what the rebuilt functions need is declared yet.
#pragma once

#if !defined( tMetaMapHex_h )
#define tMetaMapHex_h

#include "port/types.h"
#include "Common/Database/DatabaseObject.h"
#include "Common/Thread/Optex.h"
#include "unsorted/tHexPoint.h"
#include <map>

// HYPOTHESIS: the 0x14-byte container tMetaMapHex holds three of, keyed by an int with one
// 32-bit value per key. It is not this compiler's own std::map (0x10 bytes): its tree
// (constructor 0x005bc3de) keeps an allocator byte at +0, a comparison byte at +1, the head node
// at +4, a nil node of its own at +8, the multi flag at +0xc and the size at +0x10. Each node
// holds its key at +0x0c and its value at +0x10. Only the members the rebuilt functions call are
// declared; the index operator is 0x0053a73a and the iterator's increment 0x0052e144.
struct fn_0053a73a_map
{
	struct node
	{
		node*	left;
		node*	parent;
		node*	right;
		int32	first;
		int32	second;
		char	color;
		char	isnil;
	};

	class iterator
	{
	public:
		iterator( node* n ) : m_node( n ) {}
		node& operator*() const { return *m_node; }
		iterator& fn_0052e144();
		iterator& operator++() { return fn_0052e144(); }
		bool operator!=( const iterator& other ) const { return m_node != other.m_node; }
		node* m_node;
	};

	class tree
	{
	public:
		tree( const std::less< int32 >& pred, bool multi, const std::allocator< int32 >& al );
		~tree();
		char	m_00[4];
		node*	m_head;
		node*	m_nil;
		bool	m_multi;
		int32u	m_size;
	};

	explicit fn_0053a73a_map( const std::less< int32 >& pred = std::less< int32 >(), const std::allocator< int32 >& al = std::allocator< int32 >() );

	int32& operator[]( const int32& key );
	iterator begin() { return iterator( m_tree.m_head->left ); }
	iterator end() { return iterator( m_tree.m_head ); }

	tree m_tree;
};

inline fn_0053a73a_map::fn_0053a73a_map( const std::less< int32 >& pred, const std::allocator< int32 >& al )
	: m_tree( pred, false, al )
{
}

// HYPOTHESIS: m_48's container is another instantiation of the same tree (its destructor is the
// separate copy 0x0053a6cc); the value type is probably int where the others hold long.
struct fn_0053a6cc_map
{
	struct node
	{
		node*	left;
		node*	parent;
		node*	right;
		int32	first;
		int32	second;
		char	color;
		char	isnil;
	};

	class iterator
	{
	public:
		iterator( node* n ) : m_node( n ) {}
		node& operator*() const { return *m_node; }
		iterator& fn_0052e144();
		iterator& operator++() { return fn_0052e144(); }
		bool operator!=( const iterator& other ) const { return m_node != other.m_node; }
		node* m_node;
	};

	class tree
	{
	public:
		tree( const std::less< int32 >& pred, bool multi, const std::allocator< int32 >& al );
		~tree();
		char	m_00[4];
		node*	m_head;
		node*	m_nil;
		bool	m_multi;
		int32u	m_size;
	};

	explicit fn_0053a6cc_map( const std::less< int32 >& pred = std::less< int32 >(), const std::allocator< int32 >& al = std::allocator< int32 >() );

	iterator begin() { return iterator( m_tree.m_head->left ); }
	iterator end() { return iterator( m_tree.m_head ); }

	tree m_tree;
};

inline fn_0053a6cc_map::fn_0053a6cc_map( const std::less< int32 >& pred, const std::allocator< int32 >& al )
	: m_tree( pred, false, al )
{
}

// HYPOTHESIS: a database object; tBattles::StreamOut passes it to the tStreamable output
// operator with the base at offset 8, as for tDatabaseObject.
class tMetaMapHex : public nDatabase::tDatabaseObject
{
public:
	// HYPOTHESIS: the constructor tests its one argument as a byte; when it is set, the hex
	// takes its values from the HexValues settings. tBattles passes 0.
	tMetaMapHex( bool param_1 );
	~tMetaMapHex();
	// HYPOTHESIS: an out-of-line assignment; tBattles::operator= calls it for both copies.
	tMetaMapHex& operator=( const tMetaMapHex& other );

	// HYPOTHESIS: returns a map that tBattles::SQLFillInsertStream looks up with the keys 0
	// and 1; the key and value types are guesses from how the values are streamed.
	std::map< int32, float >& fn_0053a2fc();
	// HYPOTHESIS: a number per index; tBattles::SQLFillInsertStream writes it for 0 and 1.
	int32 fn_0053a414( int32 param_1 );

	// HYPOTHESIS: 0xd0 bytes in all, the distance between the two copies tBattles holds. The
	// member types follow the constructor.
	fn_0053a6cc_map m_48;
	int32 m_5c;
	int32 m_60;
	int32 m_64;
	long m_68;
	long m_6c;
	fn_0053a73a_map m_70;
	fn_0053a73a_map m_84;
	double m_98;
	double m_a0;
	tHexPoint m_a8;
	tOptex m_bc;
};

#endif
