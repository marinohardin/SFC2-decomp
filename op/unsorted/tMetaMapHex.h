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
		// MATCH-ONLY: the copy constructor of tMetaMapHex has this tree constructor inline where
		// the other constructor calls it; the second form stands for that.
		tree( const std::allocator< int32 >& al, const std::less< int32 >& pred, bool multi )
			: m_alloc( al ), m_comp( pred ), m_multi( multi ) { fn_0042511e(); }
		void fn_0042511e();
		~tree();
		std::allocator< int32 > m_alloc;
		std::less< int32 > m_comp;
		char	m_pad[2];
		node*	m_head;
		node*	m_nil;
		bool	m_multi;
		int32u	m_size;
	};

	explicit fn_0053a73a_map( const std::less< int32 >& pred = std::less< int32 >(), const std::allocator< int32 >& al = std::allocator< int32 >() );
	fn_0053a73a_map( const std::allocator< int32 >& al, const std::less< int32 >& pred, bool multi )
		: m_tree( al, pred, multi ) {}

	struct value_type
	{
		value_type( const int32& k, const int32& v ) : first( k ), second( v ) {}
		int32	first;
		int32	second;
	};

	// HYPOTHESIS: the out-of-line assignment 0x0053a79b.
	fn_0053a73a_map& operator=( const fn_0053a73a_map& other );
	std::pair< iterator, bool > insert( const value_type& v );
	int32& operator[]( const int32& key );
	// HYPOTHESIS: the out-of-line lookup 0x0053aa7a, taken to be the map's find.
	iterator find( const int32& key );
	iterator begin() const { return iterator( m_tree.m_head->left ); }
	iterator end() const { return iterator( m_tree.m_head ); }

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
		int32u	second;
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
		tree( const std::allocator< int32 >& al, const std::less< int32 >& pred, bool multi )
			: m_alloc( al ), m_comp( pred ), m_multi( multi ) { fn_0042511e(); }
		void fn_0042511e();
		~tree();
		std::allocator< int32 > m_alloc;
		std::less< int32 > m_comp;
		char	m_pad[2];
		node*	m_head;
		node*	m_nil;
		bool	m_multi;
		int32u	m_size;
	};

	explicit fn_0053a6cc_map( const std::less< int32 >& pred = std::less< int32 >(), const std::allocator< int32 >& al = std::allocator< int32 >() );
	fn_0053a6cc_map( const std::allocator< int32 >& al, const std::less< int32 >& pred, bool multi )
		: m_tree( al, pred, multi ) {}

	// HYPOTHESIS: the out-of-line assignment 0x0053a704.
	fn_0053a6cc_map& operator=( const fn_0053a6cc_map& other );
	// HYPOTHESIS: the index operator 0x0053a6a3. tMetaMapHex's StreamIn copies a long key into a
	// temporary before the call, so the key is an int here.
	int& operator[]( const int& key );
	iterator begin() { return iterator( m_tree.m_head->left ); }
	// HYPOTHESIS: the out-of-line 0x0053a8ff, which tMetaMapHex's StreamOut calls for the first
	// element; that it is the const begin is a guess.
	iterator begin() const;
	iterator end() const { return iterator( m_tree.m_head ); }

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
	// HYPOTHESIS: the copy constructor (0x005394f4): builds the members, then assigns.
	tMetaMapHex( const tMetaMapHex& other );
	~tMetaMapHex();
	// HYPOTHESIS: an out-of-line assignment; tBattles::operator= calls it for both copies.
	tMetaMapHex& operator=( const tMetaMapHex& other );

	// HYPOTHESIS: returns a map that tBattles::SQLFillInsertStream looks up with the keys 0
	// and 1; the key and value types are guesses from how the values are streamed.
	std::map< int32, float >& fn_0053a2fc();
	// HYPOTHESIS: a number per index; tBattles::SQLFillInsertStream writes it for 0 and 1.
	int32 fn_0053a414( int32 param_1 );
	// HYPOTHESIS: the hex point's first coordinate, read under the lock.
	long fn_00539f04();
	// HYPOTHESIS: the hex point's second coordinate, read under the lock.
	long fn_00539f50();
	// HYPOTHESIS: a copy of the hex point, taken under the lock.
	tHexPoint fn_00539f9c();
	// HYPOTHESIS: the six hexes around this one; when both arguments are set, only those
	// fn_0053a287 accepts.
	std::vector< tHexPoint > fn_00539ff1( int param_1, int param_2 );
	// HYPOTHESIS: a test of a neighbouring hex against the two arguments of fn_00539ff1, which
	// look like the map's width and height; the original compares them unsigned.
	bool fn_0053a287( tHexPoint& point, unsigned int param_1, unsigned int param_2 );
	// HYPOTHESIS: the terrain type (the TerrainType column), read under the lock.
	int32 fn_0053a31c();
	int32 fn_0053a33c();
	int32 fn_0053a35c();
	long fn_0053a37c();
	long fn_0053a39c();
	long fn_0053a3bc( int32 param_1 );
	// HYPOTHESIS: the current speed points (the CurrentSpeedPoints column), read under the lock.
	double fn_0053a46f();
	// HYPOTHESIS: sets the hex point without taking the lock; the callee 0x005b69b0 is taken to
	// be tHexPoint's assignment.
	void fn_0053a49c( const tHexPoint& point );
	// HYPOTHESIS: stores a value under a key in m_48, under the lock.
	void fn_0053a4ae( int param_1, int param_2 );
	// HYPOTHESIS: sets the terrain type (the TerrainType column) under the lock.
	void fn_0053a4fe( int32 param_1 );
	// HYPOTHESIS: sets the planet type (the PlanetType column) under the lock.
	void fn_0053a522( int32 param_1 );
	// HYPOTHESIS: sets the star base type (the StarBaseType column) under the lock.
	void fn_0053a546( int32 param_1 );
	// HYPOTHESIS: sets the base economic points (the BaseEconomicPoints column) under the lock.
	void fn_0053a56a( long param_1 );
	// HYPOTHESIS: sets the current economic points (the CurrentEconomicPoints column) under the lock.
	void fn_0053a58e( long param_1 );
	// HYPOTHESIS: stores the first argument under the key given by the second in m_70, under
	// the lock.
	void fn_0053a5b2( long param_1, int32 param_2 );

	virtual void StreamOut( nDataStore::tOutput& output ) const;
	virtual void StreamIn( nDataStore::tInput& input );
	virtual void SQLFillInsertStream( std::stringstream* SQL, bool lastOne = false );
	virtual void SQLFillUpdateStream( std::stringstream* SQL, bool lastOne = false );
	virtual const char* SQLTableName() const;
	// HYPOTHESIS: the SDK's SQLFieldNames, the virtual after SQLTableName in tDatabaseObject.
	virtual const char* SQLFieldNames() const;

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
	mutable tOptex m_bc;
};

#endif
