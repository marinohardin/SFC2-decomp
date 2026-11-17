// Unit:      unsorted/fn_0053F000.cpp
// Functions: 0x0053f22f to 0x0053ffd7, 40 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

#include "Common/Database/DatabaseID.h"
#include <xmemory>
#include "Common/DynamicAllocator.h"
#include "Common/DataStore/DataStoreOutput.h"
#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"

#include "unsorted/tPreparedMissions.h"

// HYPOTHESIS: the owner of this by-value getter is not known; the body copies a
// tID held at 0x48 into the return slot. The class and the member names are placeholders.
class fn_0053f22f_owner
{
public:
	nDatabase::tID fn_0053f22f() const;
	void fn_0053f281( nDatabase::tID id );

	char m_00[0x48];
	nDatabase::tID m_48;
};

// FUNCTION: STARFLEETOP 0x0053f22f
nDatabase::tID fn_0053f22f_owner::fn_0053f22f() const
{
	return m_48;
}

// HYPOTHESIS: the class of the member at 0x7c of the owner below is not known; it has
// a member function at 0x0042b0fd taking two dwords. The names are placeholders.
class fn_0042b0fd_owner
{
public:
	void fn_0042b0fd( nDatabase::tID *a, nDatabase::tID *b );
	void fn_0042add2( long value );
	void fn_0053f46d( const nDatabase::tID *first, const nDatabase::tID *last );
	void fn_0053f5d9( nDatabase::tID *pos, const nDatabase::tID *first, const nDatabase::tID *last );
	// MATCH-ONLY: the original read the size through an inlined accessor.
	unsigned int size() const
	{
		return ( m_04 == 0 ? 0 : m_08 - m_04 );
	}
	// MATCH-ONLY: the library's element destroy and uninitialized copy, inlined by the original.
	void _Destroy( nDatabase::tID *first, nDatabase::tID *last )
	{
		for( ; first != last; ++first )
			m_00.destroy( first );
	}
	nDatabase::tID *_Ucopy( const nDatabase::tID *first, const nDatabase::tID *last, nDatabase::tID *dst )
	{
		for( ; first != last; ++dst, ++first )
			m_00.construct( dst, *first );
		return dst;
	}

	std::allocator< nDatabase::tID > m_00;
	nDatabase::tID *m_04;
	nDatabase::tID *m_08;
	nDatabase::tID *m_0c;
};

// HYPOTHESIS: the element type of the container below is not known; elements are
// 0x74 bytes with a virtual destructor in slot 0, streamed out one by one as tStreamable
// by 0x0053fe75. The names are placeholders.
// HYPOTHESIS: the element is tMissionProfile; 0x0053ff43 default-constructs one through
// 0x0053d35e and destroys it through 0x0053d423, both known by that name.
class fn_0053f491_elem : public nDataStore::tStreamable
{
public:
	fn_0053f491_elem();
	virtual ~fn_0053f491_elem();
	fn_0053f491_elem( const fn_0053f491_elem &other );
	fn_0053f491_elem &operator=( const fn_0053f491_elem &other );

	virtual void StreamOut( nDataStore::tOutput& output ) const;
	virtual void StreamIn( nDataStore::tInput& input );

	char m_0c[0x68];
};

// HYPOTHESIS: the class of the member at 0x58 of the owner below is not known; it has
// a member function at 0x0053f4c6 taking two dwords and looks like this compiler's
// std::vector of the 0x74-byte elements above. The names are placeholders.
class fn_0053f4c6_owner
{
public:
	void fn_0053f4c6( long a, long b );
	fn_0053f491_elem *fn_0053fa94( fn_0053f491_elem *first, fn_0053f491_elem *last );
	void fn_0053f834( fn_0053f491_elem *pos, const fn_0053f491_elem *first, const fn_0053f491_elem *last );
	// MATCH-ONLY: the original read the size through an inlined accessor.
	unsigned int size() const
	{
		return ( m_04 == 0 ? 0 : m_08 - m_04 );
	}
	// MATCH-ONLY: the library's element destroy and uninitialized copy, inlined by the original.
	void _Destroy( fn_0053f491_elem *first, fn_0053f491_elem *last )
	{
		for( ; first != last; ++first )
			m_00.destroy( first );
	}
	fn_0053f491_elem *_Ucopy( const fn_0053f491_elem *first, const fn_0053f491_elem *last, fn_0053f491_elem *dst )
	{
		for( ; first != last; ++dst, ++first )
			m_00.construct( dst, *first );
		return dst;
	}

	std::allocator< fn_0053f491_elem > m_00;
	fn_0053f491_elem *m_04;
	fn_0053f491_elem *m_08;
	fn_0053f491_elem *m_0c;
};

// HYPOTHESIS: the element type of the container below is not known; elements are
// 0x160 bytes with a vtable, a byte test at 0x0054340c and a virtual in slot 12.
// Slot 0 is the virtual destructor (0x0053f4ea calls it with 0). The names are placeholders.
// HYPOTHESIS: 0x0053fece streams each element as the tStreamable at 8, so the element has a
// polymorphic 8-byte primary base (vtable and one dword) and tStreamable as its second base.
class fn_0054340c_base
{
public:
	virtual ~fn_0054340c_base();

	long m_04;
};

class fn_0054340c_owner : public fn_0054340c_base, public nDataStore::tStreamable
{
public:
	// HYPOTHESIS: 0x0053ffd7 default-constructs an element through an out-of-line constructor
	// at 0x00541a72 called with one argument of 0; its type and default are guesses.
	fn_0054340c_owner( long p = 0 );
	virtual ~fn_0054340c_owner();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual long vfunction12();

	char fn_0054340c();
	fn_0054340c_owner &operator=( const fn_0054340c_owner &other );

	virtual void StreamOut( nDataStore::tOutput& output ) const;
	virtual void StreamIn( nDataStore::tInput& input );

	char m_14[0x14c];
};

// HYPOTHESIS: the class of the member at 0x6c of the owner below is not known; it has
// a member function at 0x0053f522 taking two dwords and looks like a vector of the
// elements above (first at 4, last at 8) with an erase at 0x0053f546. The names are
// placeholders.
class fn_0053f522_owner
{
public:
	void fn_0053f522( long a, long b );
	fn_0054340c_owner *fn_0053f546( fn_0054340c_owner *it );
	void fn_0053f461();
	void fn_0053f4ea();
	fn_0054340c_owner *fn_0053f5ad( fn_0054340c_owner *first, fn_0054340c_owner *last );
	fn_0054340c_owner *fn_0053fd7e( fn_0054340c_owner *first, fn_0054340c_owner *last );
	void fn_0053faeb( fn_0054340c_owner *pos, const fn_0054340c_owner *first, const fn_0054340c_owner *last );
	// MATCH-ONLY: the original read the size through an inlined accessor.
	unsigned int size() const
	{
		return ( m_04 == 0 ? 0 : m_08 - m_04 );
	}
	// MATCH-ONLY: the library's element destroy and uninitialized copy, inlined by the original.
	void _Destroy( fn_0054340c_owner *first, fn_0054340c_owner *last )
	{
		for( ; first != last; ++first )
			m_00.destroy( first );
	}
	fn_0054340c_owner *_Ucopy( const fn_0054340c_owner *first, const fn_0054340c_owner *last, fn_0054340c_owner *dst )
	{
		for( ; first != last; ++dst, ++first )
			m_00.construct( dst, *first );
		return dst;
	}

	std::allocator< fn_0054340c_owner > m_00;
	fn_0054340c_owner *m_04;
	fn_0054340c_owner *m_08;
	fn_0054340c_owner *m_0c;
};

// HYPOTHESIS: the type of the argument at 0x0053f2a2 is not known; only its dwords at
// 4 and 8 are read. The names are placeholders.
struct fn_0053f2a2_arg
{
	long m_00;
	long m_04;
	long m_08;
};

// HYPOTHESIS: the owner of this getter is not known; the body returns the address of
// a member at 0x58. The class, the member names and the member type are placeholders.
class fn_0053f25c_owner
{
public:
	fn_0053f4c6_owner *fn_0053f25c();
	char fn_0053f260();
	fn_0053f522_owner *fn_0053f264();
	fn_0042b0fd_owner *fn_0053f268();
	long fn_0053f26c();
	long fn_0053f273();
	long fn_0053f27a();
	void fn_0053f2a2( const fn_0053f2a2_arg &arg );
	void fn_0053f2b7( char value );
	void fn_0053f2c1( const fn_0053f2a2_arg &arg );
	void fn_0053f2d6( long value );
	void fn_0053f2e3( long value );
	void fn_0053f2f0( long value );
	void fn_0053f2fd();
	void fn_0053f30f();

	char m_00[0x58];
	fn_0053f4c6_owner m_58;
	char m_68;
	char m_69[3];
	fn_0053f522_owner m_6c;
	fn_0042b0fd_owner m_7c;
	long m_8c;
	long m_90;
	long m_94;
};

// FUNCTION: STARFLEETOP 0x0053f25c
fn_0053f4c6_owner *fn_0053f25c_owner::fn_0053f25c()
{
	return &m_58;
}

// HYPOTHESIS: the neighbouring getter's owner also holds the byte at 0x68 this one returns.
// FUNCTION: STARFLEETOP 0x0053f260
char fn_0053f25c_owner::fn_0053f260()
{
	return m_68;
}

// HYPOTHESIS: same owner; returns the address of the member at 0x6c.
// FUNCTION: STARFLEETOP 0x0053f264
fn_0053f522_owner *fn_0053f25c_owner::fn_0053f264()
{
	return &m_6c;
}

// HYPOTHESIS: same owner; returns the address of the member at 0x7c.
// FUNCTION: STARFLEETOP 0x0053f268
fn_0042b0fd_owner *fn_0053f25c_owner::fn_0053f268()
{
	return &m_7c;
}

// HYPOTHESIS: same owner; returns the dword at 0x8c, type unknown.
// FUNCTION: STARFLEETOP 0x0053f26c
long fn_0053f25c_owner::fn_0053f26c()
{
	return m_8c;
}

// HYPOTHESIS: same owner; returns the dword at 0x90, type unknown.
// FUNCTION: STARFLEETOP 0x0053f273
long fn_0053f25c_owner::fn_0053f273()
{
	return m_90;
}

// HYPOTHESIS: same owner; returns the dword at 0x94, type unknown.
// FUNCTION: STARFLEETOP 0x0053f27a
long fn_0053f25c_owner::fn_0053f27a()
{
	return m_94;
}

// HYPOTHESIS: the setter paired with the getter at 0x0053f22f; it takes the tID by
// value and assigns it to the member at 0x48.
// FUNCTION: STARFLEETOP 0x0053f281
void fn_0053f22f_owner::fn_0053f281( nDatabase::tID id )
{
	m_48 = id;
}

// HYPOTHESIS: same owner; forwards two dwords of the argument to the member at 0x58.
// FUNCTION: STARFLEETOP 0x0053f2a2
void fn_0053f25c_owner::fn_0053f2a2( const fn_0053f2a2_arg &arg )
{
	m_58.fn_0053f4c6( arg.m_04, arg.m_08 );
}

// HYPOTHESIS: same owner; the setter paired with the getter of the byte at 0x68.
// FUNCTION: STARFLEETOP 0x0053f2b7
void fn_0053f25c_owner::fn_0053f2b7( char value )
{
	m_68 = value;
}

// HYPOTHESIS: same owner; forwards two dwords of the argument to the member at 0x6c.
// FUNCTION: STARFLEETOP 0x0053f2c1
void fn_0053f25c_owner::fn_0053f2c1( const fn_0053f2a2_arg &arg )
{
	m_6c.fn_0053f522( arg.m_04, arg.m_08 );
}

// HYPOTHESIS: same owner; the setter paired with the getter of the dword at 0x8c.
// FUNCTION: STARFLEETOP 0x0053f2d6
void fn_0053f25c_owner::fn_0053f2d6( long value )
{
	m_8c = value;
}

// HYPOTHESIS: same owner; the setter paired with the getter of the dword at 0x90.
// FUNCTION: STARFLEETOP 0x0053f2e3
void fn_0053f25c_owner::fn_0053f2e3( long value )
{
	m_90 = value;
}

// HYPOTHESIS: same owner; the setter paired with the getter of the dword at 0x94.
// FUNCTION: STARFLEETOP 0x0053f2f0
void fn_0053f25c_owner::fn_0053f2f0( long value )
{
	m_94 = value;
}

// HYPOTHESIS: same owner; passes the member at 0x7c its own dwords at 4 and 8.
// FUNCTION: STARFLEETOP 0x0053f2fd
void fn_0053f25c_owner::fn_0053f2fd()
{
	m_7c.fn_0042b0fd( m_7c.m_04, m_7c.m_08 );
}

// HYPOTHESIS: same owner; erases every element of the container at 0x6c whose test at
// 0x0054340c is true, after handing its slot-12 value to the member at 0x7c.
// FUNCTION: STARFLEETOP 0x0053f30f
void fn_0053f25c_owner::fn_0053f30f()
{
	fn_0054340c_owner *it = m_6c.m_04;
	while ( it != m_6c.m_08 )
	{
		if ( it->fn_0054340c() )
		{
			m_7c.fn_0042add2( it->vfunction12() );
			it = m_6c.fn_0053f546( it );
		}
		else
			++it;
	}
}

// HYPOTHESIS: not a function of its own; the exception tail of 0x0053f34e (stream operator
// with sentry and catch). Placeholder until that function is written.
// FUNCTION: STARFLEETOP 0x0053f41a
void fn_0053f41a()
{
}

// HYPOTHESIS: a member of the vector-like class at 0x6c above (ecx passes through
// unchanged to the callee); hands its own first and last to the range member at
// 0x0053f5ad, like a clear.
// FUNCTION: STARFLEETOP 0x0053f461
void fn_0053f522_owner::fn_0053f461()
{
	fn_0053f5ad( m_04, m_08 );
}

// HYPOTHESIS: a member of the class at 0x7c above (ecx passes through to both callees);
// erases its own range at 4..8, then hands the two arguments to 0x0053f5d9 at its first,
// like an assign.
// FUNCTION: STARFLEETOP 0x0053f46d
void fn_0042b0fd_owner::fn_0053f46d( const nDatabase::tID *first, const nDatabase::tID *last )
{
	fn_0042b0fd( m_04, m_08 );
	fn_0053f5d9( m_04, first, last );
}

// HYPOTHESIS: a vector-like class (first at 4, last at 8, end at 0xc) of the 0x74-byte
// elements declared at the top; this member destroys every element, frees the block and clears the three
// pointers, like a vector's tidy. The names are placeholders.
class fn_0053f491_owner
{
public:
	void fn_0053f491();

	long m_00;
	fn_0053f491_elem *m_04;
	fn_0053f491_elem *m_08;
	fn_0053f491_elem *m_0c;
};

// FUNCTION: STARFLEETOP 0x0053f491
void fn_0053f491_owner::fn_0053f491()
{
	fn_0053f491_elem *last = m_08;
	fn_0053f491_elem *first = m_04;
	for ( ; first != last; ++first )
		first->~fn_0053f491_elem();
	operator delete( m_04 );
	m_04 = 0;
	m_08 = 0;
	m_0c = 0;
}

// HYPOTHESIS: a member of the class at 0x58 above (ecx passes through to both callees);
// erases its own range at 4..8, then hands the two arguments to 0x0053f834 at its first,
// like an assign.
// FUNCTION: STARFLEETOP 0x0053f4c6
void fn_0053f4c6_owner::fn_0053f4c6( long a, long b )
{
	fn_0053fa94( m_04, m_08 );
	// HYPOTHESIS: the two dwords are the first and last of another vector of the same elements.
	fn_0053f834( m_04, (const fn_0053f491_elem *)a, (const fn_0053f491_elem *)b );
}

// HYPOTHESIS: a member of the vector-like class at 0x6c above; destroys every element,
// frees the block and clears the three pointers, like a vector's tidy.
// FUNCTION: STARFLEETOP 0x0053f4ea
void fn_0053f522_owner::fn_0053f4ea()
{
	fn_0054340c_owner *last = m_08;
	fn_0054340c_owner *first = m_04;
	for ( ; first != last; ++first )
		first->~fn_0054340c_owner();
	operator delete( m_04 );
	m_04 = 0;
	m_08 = 0;
	m_0c = 0;
}

// HYPOTHESIS: same vector-like class; erases its own range at 4..8, then hands the two
// arguments to 0x0053faeb at its first, like an assign.
// FUNCTION: STARFLEETOP 0x0053f522
void fn_0053f522_owner::fn_0053f522( long a, long b )
{
	fn_0053fd7e( m_04, m_08 );
	fn_0053faeb( m_04, (const fn_0054340c_owner *)a, (const fn_0054340c_owner *)b );
}

// HYPOTHESIS: same vector-like class; erases one element like a vector's erase: the
// elements after it are assigned down one place (0x00541f81 as the element's
// assignment), the last is destroyed and the end moves back.
// FUNCTION: STARFLEETOP 0x0053f546
fn_0054340c_owner *fn_0053f522_owner::fn_0053f546( fn_0054340c_owner *it )
{
	fn_0054340c_owner *last = m_08;
	fn_0054340c_owner *dest = it;
	for ( fn_0054340c_owner *src = it + 1; src != last; ++dest, ++src )
		*dest = *src;
	fn_0054340c_owner *end = m_08;
	for ( fn_0054340c_owner *p = end - 1; p != end; ++p )
		p->~fn_0054340c_owner();
	--m_08;
	return it;
}

// HYPOTHESIS: a vector-like class of bytes (first at 4, last at 8, end at 0xc); this
// member erases a range like a vector's erase: the bytes after it are copied down, the
// (trivial) destroy loop over the old tail leaves only a dead read, and the last moves
// back. The layout (an allocator, then first, last, end) is VC6's std::vector<char>,
// so 0x0053f461 above, which hands its own first and last here, is likely this class's
// clear rather than a member of the 0x160-byte container. The names are placeholders.
class fn_0053f5ad_owner
{
public:
	char *fn_0053f5ad( char *first, char *last );
	// MATCH-ONLY: the original read the last through an inlined accessor.
	char *end()
	{
		return m_08;
	}
	// MATCH-ONLY: the original inlined a destroy loop over the old tail; for bytes it
	// leaves only the dead store of the end into the argument slot.
	void fn_0053f5ad_destroy( char *first, char *last )
	{
		for ( ; first != last; ++first )
			m_00.destroy( first );
	}

	std::allocator<char> m_00;
	char *m_04;
	char *m_08;
	char *m_0c;
};

// FUNCTION: STARFLEETOP 0x0053f5ad
char *fn_0053f5ad_owner::fn_0053f5ad( char *first, char *last )
{
	char *s = std::copy( last, end(), first );
	fn_0053f5ad_destroy( s, end() );
	m_08 = s;
	return first;
}

// HYPOTHESIS: the class at 0x7c above is this compiler's std::vector< nDatabase::tID >
// (0x10-byte elements); this member is its range insert.
// FUNCTION: STARFLEETOP 0x0053f5d9
void fn_0042b0fd_owner::fn_0053f5d9( nDatabase::tID *pos, const nDatabase::tID *first, const nDatabase::tID *last )
{
	unsigned int m = 0;
	std::_Distance( first, last, m );
	if( (unsigned int)( m_0c - m_08 ) < m )
	{
		unsigned int n = size() + ( m < size() ? size() : m );
		int count = n;
		if( count < 0 )
			count = 0;
		nDatabase::tID *s = m_00.allocate( n, (void *)0 );
		nDatabase::tID *q = _Ucopy( m_04, pos, s );
		q = _Ucopy( first, last, q );
		_Ucopy( pos, m_08, q );
		_Destroy( m_04, m_08 );
		m_00.deallocate( m_04, m_0c - m_04 );
		m_0c = s + n;
		m_08 = s + size() + m;
		m_04 = s;
	}
	else if( (unsigned int)( m_08 - pos ) < m )
	{
		_Ucopy( pos, m_08, pos + m );
		_Ucopy( first + ( m_08 - pos ), last, m_08 );
		std::copy( first, first + ( m_08 - pos ), pos );
		m_08 += m;
	}
	else if( 0 < m )
	{
		_Ucopy( m_08 - m, m_08, m_08 );
		std::copy_backward( pos, m_08 - m, m_08 );
		std::copy( first, last, pos );
		m_08 += m;
	}
}

// HYPOTHESIS: the class at 0x58 above is this compiler's std::vector of the 0x74-byte
// elements above (fn_0053f491_elem); this member is its range insert.
// FUNCTION: STARFLEETOP 0x0053f834
void fn_0053f4c6_owner::fn_0053f834( fn_0053f491_elem *pos, const fn_0053f491_elem *first, const fn_0053f491_elem *last )
{
	unsigned int m = 0;
	std::_Distance( first, last, m );
	if( (unsigned int)( m_0c - m_08 ) < m )
	{
		unsigned int n = size() + ( m < size() ? size() : m );
		int count = n;
		if( count < 0 )
			count = 0;
		fn_0053f491_elem *s = m_00.allocate( n, (void *)0 );
		fn_0053f491_elem *q = _Ucopy( m_04, pos, s );
		q = _Ucopy( first, last, q );
		_Ucopy( pos, m_08, q );
		_Destroy( m_04, m_08 );
		m_00.deallocate( m_04, m_0c - m_04 );
		m_0c = s + n;
		m_08 = s + size() + m;
		m_04 = s;
	}
	else if( (unsigned int)( m_08 - pos ) < m )
	{
		_Ucopy( pos, m_08, pos + m );
		_Ucopy( first + ( m_08 - pos ), last, m_08 );
		std::copy( first, first + ( m_08 - pos ), pos );
		m_08 += m;
	}
	else if( 0 < m )
	{
		_Ucopy( m_08 - m, m_08, m_08 );
		std::copy_backward( pos, m_08 - m, m_08 );
		std::copy( first, last, pos );
		m_08 += m;
	}
}

// HYPOTHESIS: same vector-like class; this member is its range erase: the elements after
// the range are assigned down (0x005400e7 as the element's assignment), the old tail is
// destroyed and the last moves back.
// FUNCTION: STARFLEETOP 0x0053fa94
fn_0053f491_elem *fn_0053f4c6_owner::fn_0053fa94( fn_0053f491_elem *first, fn_0053f491_elem *last )
{
	fn_0053f491_elem *s = std::copy( last, m_08, first );
	_Destroy( s, m_08 );
	m_08 = s;
	return first;
}

// HYPOTHESIS: the vector-like class at 0x6c above is this compiler's std::vector of the
// 0x160-byte elements (fn_0054340c_owner); this member is its range insert.
// FUNCTION: STARFLEETOP 0x0053faeb
void fn_0053f522_owner::fn_0053faeb( fn_0054340c_owner *pos, const fn_0054340c_owner *first, const fn_0054340c_owner *last )
{
	unsigned int m = 0;
	std::_Distance( first, last, m );
	if( (unsigned int)( m_0c - m_08 ) < m )
	{
		unsigned int n = size() + ( m < size() ? size() : m );
		fn_0054340c_owner *s = m_00.allocate( n, (void *)0 );
		fn_0054340c_owner *q = _Ucopy( m_04, pos, s );
		q = _Ucopy( first, last, q );
		_Ucopy( pos, m_08, q );
		_Destroy( m_04, m_08 );
		m_00.deallocate( m_04, m_0c - m_04 );
		m_0c = s + n;
		m_08 = s + size() + m;
		m_04 = s;
	}
	else if( (unsigned int)( m_08 - pos ) < m )
	{
		_Ucopy( pos, m_08, pos + m );
		_Ucopy( first + ( m_08 - pos ), last, m_08 );
		std::copy( first, first + ( m_08 - pos ), pos );
		m_08 += m;
	}
	else if( 0 < m )
	{
		_Ucopy( m_08 - m, m_08, m_08 );
		std::copy_backward( pos, m_08 - m, m_08 );
		std::copy( first, last, pos );
		m_08 += m;
	}
}

// HYPOTHESIS: same vector-like class; this member is its range erase: the elements after
// the range are assigned down (0x00541f81 as the element's assignment), the old tail is
// destroyed and the last moves back.
// FUNCTION: STARFLEETOP 0x0053fd7e
fn_0054340c_owner *fn_0053f522_owner::fn_0053fd7e( fn_0054340c_owner *first, fn_0054340c_owner *last )
{
	fn_0054340c_owner *s = std::copy( last, m_08, first );
	_Destroy( s, m_08 );
	m_08 = s;
	return first;
}

// FUNCTION: STARFLEETOP 0x0053fde1
template<>
tClassAllocator<tPreparedMissions>::tClassAllocator()
{
	tDynamicAllocator::Instance()->mAddToAllocList( typeid( tPreparedMissions ), this );
}

// MATCH-ONLY: the SDK's template mAllocate, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.

// FUNCTION: STARFLEETOP 0x0053fe1d
template<>
tAllocatedClass* tClassAllocator<tPreparedMissions>::mAllocate()
{
	tPreparedMissions* pClass = new tPreparedMissions();

	pClass->mSetAllocatorID( fAllocID );

	return pClass;
}

// MATCH-ONLY: the SDK's template mAssignAllocID, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.

// FUNCTION: STARFLEETOP 0x0053fe69
template<>
void tClassAllocator<tPreparedMissions>::mAssignAllocID( iAllocatorID allocID )
{
	fAllocID = allocID;
}

// MATCH-ONLY: the SDK's template stream operator for vectors, written out as this instance's
// explicit specialization so that its annotation names the one instance; the body is the template's.
namespace nDataStore
{

// FUNCTION: STARFLEETOP 0x0053fe75
template<>
tOutput& operator<<( tOutput& output, const std::vector< fn_0053f491_elem >& value )
{
	int32u size = value.size();
	output.Write( (const byte*)&size, sizeof( size ) );

	for( std::vector< fn_0053f491_elem >::const_iterator i = value.begin();
		 i != value.end();
		 i++ )
	{
		output << *i;
	}

	return output;
}

// FUNCTION: STARFLEETOP 0x0053fece
template<>
tOutput& operator<<( tOutput& output, const std::vector< fn_0054340c_owner >& value )
{
	int32u size = value.size();
	output.Write( (const byte*)&size, sizeof( size ) );

	for( std::vector< fn_0054340c_owner >::const_iterator i = value.begin();
		 i != value.end();
		 i++ )
	{
		output << *i;
	}

	return output;
}

// FUNCTION: STARFLEETOP 0x0053ff43
template<>
tInput& operator>>( tInput& input, std::vector< fn_0053f491_elem >& value )
{
	value.clear();

	int32u size = 0;
	input.Read( (byte*)&size, sizeof( size ) );

	value.reserve( size );

	for( int i = 0; i < size; i++ )
	{
		fn_0053f491_elem type;
		input >> type;
		value.push_back( type );
	}

	return input;
}

// FUNCTION: STARFLEETOP 0x0053ffd7
template<>
tInput& operator>>( tInput& input, std::vector< fn_0054340c_owner >& value )
{
	value.clear();

	int32u size = 0;
	input.Read( (byte*)&size, sizeof( size ) );

	value.reserve( size );

	for( int i = 0; i < size; i++ )
	{
		fn_0054340c_owner type;
		input >> type;
		value.push_back( type );
	}

	return input;
}

};
