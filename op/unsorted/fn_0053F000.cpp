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

// The class of the member at 0x7c of the owner below is declared in a shared header.
#include "unsorted/fn_0042b0fd_owner.h"
































































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

// HYPOTHESIS: the member at 0x58 of the owner below is this compiler's std::vector of the
// 0x74-byte elements above: its assign (0x0053f4c6), range insert (0x0053f834) and range erase
// (0x0053fa94) are claimed below, and the stream operators at 0x0053fe75 and 0x0053ff43 call them.
// MATCH-ONLY: the three are written out as this instance's explicit specializations, declared
// here before their first use so that the callers call them out of line.
template<> void std::vector< fn_0053f491_elem >::assign( const_iterator _F, const_iterator _L );
template<> void std::vector< fn_0053f491_elem >::insert( iterator _P, const_iterator _F, const_iterator _L );
template<> std::vector< fn_0053f491_elem >::iterator std::vector< fn_0053f491_elem >::erase( iterator _F, iterator _L );

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
	virtual const nDatabase::tID &vfunction12(); // HYPOTHESIS: the tID handed to 0x0042add2

	char fn_0054340c();
	fn_0054340c_owner &operator=( const fn_0054340c_owner &other );

	virtual void StreamOut( nDataStore::tOutput& output ) const;
	virtual void StreamIn( nDataStore::tInput& input );

	char m_14[0x14c];
};

// HYPOTHESIS: the member at 0x6c of the owner below is this compiler's std::vector of the
// elements above: its assign (0x0053f522), single erase (0x0053f546), range insert (0x0053faeb)
// and range erase (0x0053fd7e) are claimed below, and the stream operators at 0x0053fece and
// 0x0053ffd7 call them.
// MATCH-ONLY: the four are written out as this instance's explicit specializations, declared
// here before their first use so that the callers call them out of line.
template<> void std::vector< fn_0054340c_owner >::assign( const_iterator _F, const_iterator _L );
template<> std::vector< fn_0054340c_owner >::iterator std::vector< fn_0054340c_owner >::erase( iterator _P );
template<> void std::vector< fn_0054340c_owner >::insert( iterator _P, const_iterator _F, const_iterator _L );
template<> std::vector< fn_0054340c_owner >::iterator std::vector< fn_0054340c_owner >::erase( iterator _F, iterator _L );

// HYPOTHESIS: a vector-like class of the elements above (first at 4, last at 8, end at 0xc)
// with a tidy at 0x0053f4ea; nothing in this unit calls it. The names are placeholders.
class fn_0053f522_owner
{
public:
	void fn_0053f4ea();

	long m_00;
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
	std::vector< fn_0053f491_elem > *fn_0053f25c();
	char fn_0053f260();
	std::vector< fn_0054340c_owner > *fn_0053f264();
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
	std::vector< fn_0053f491_elem > m_58;
	char m_68;
	char m_69[3];
	std::vector< fn_0054340c_owner > m_6c;
	fn_0042b0fd_owner m_7c;
	long m_8c;
	long m_90;
	long m_94;
};

// FUNCTION: STARFLEETOP 0x0053f25c
std::vector< fn_0053f491_elem > *fn_0053f25c_owner::fn_0053f25c()
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
std::vector< fn_0054340c_owner > *fn_0053f25c_owner::fn_0053f264()
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
	m_58.assign( (const fn_0053f491_elem *)arg.m_04, (const fn_0053f491_elem *)arg.m_08 );
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
	m_6c.assign( (const fn_0054340c_owner *)arg.m_04, (const fn_0054340c_owner *)arg.m_08 );
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
	fn_0054340c_owner *it = m_6c.begin();
	while ( it != m_6c.end() )
	{
		if ( it->fn_0054340c() )
		{
			m_7c.fn_0042add2( it->vfunction12() );
			it = m_6c.erase( it );
		}
		else
			++it;
	}
}

// 0x0053f41a is not a function of its own: it is the code 0x0053f34e (stream operator with
// sentry and catch) runs after its catch block, in that function's frame, and is listed in
// analysis/results/boundary_errors.tsv.
//
//
//

// HYPOTHESIS: a vector-like class of bytes (first at 4, last at 8, end at 0xc); its layout
// (an allocator, then first, last, end) is VC6's std::vector<char>, and the two members
// below look like its clear (0x0053f461) and range erase (0x0053f5ad). Nothing in this unit
// uses the class, so it is written as a class of its own. The names are placeholders.
class fn_0053f5ad_owner
{
public:
	void fn_0053f461();
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

// HYPOTHESIS: ecx passes through unchanged to the callee; hands its own first and last
// to the range erase at 0x0053f5ad, like a clear.
// FUNCTION: STARFLEETOP 0x0053f461
void fn_0053f5ad_owner::fn_0053f461()
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

// HYPOTHESIS: the vector at 0x58 above; its assign erases its own range, then inserts the
// two arguments at its first.
// FUNCTION: STARFLEETOP 0x0053f4c6
template<>
void std::vector< fn_0053f491_elem >::assign( const_iterator _F, const_iterator _L )
{
	erase( begin(), end() );
	insert( begin(), _F, _L );
}

// HYPOTHESIS: a member of the vector-like class declared above; destroys every element,
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

// HYPOTHESIS: the vector at 0x6c above; its assign erases its own range, then inserts the
// two arguments at its first.
// FUNCTION: STARFLEETOP 0x0053f522
template<>
void std::vector< fn_0054340c_owner >::assign( const_iterator _F, const_iterator _L )
{
	erase( begin(), end() );
	insert( begin(), _F, _L );
}

// HYPOTHESIS: same vector; this member is its single erase: the
// elements after it are assigned down one place (0x00541f81 as the element's
// assignment), the last is destroyed and the end moves back.
// FUNCTION: STARFLEETOP 0x0053f546
template<>
std::vector< fn_0054340c_owner >::iterator std::vector< fn_0054340c_owner >::erase( iterator _P )
{
	fn_0054340c_owner *last = _Last;
	fn_0054340c_owner *dest = _P;
	for ( fn_0054340c_owner *src = _P + 1; src != last; ++dest, ++src )
		*dest = *src;
	fn_0054340c_owner *e = _Last;
	for ( fn_0054340c_owner *p = e - 1; p != e; ++p )
		p->~fn_0054340c_owner();
	--_Last;
	return _P;
}

// HYPOTHESIS: the byte class above; erases a range like a vector's erase: the bytes after
// it are copied down, the (trivial) destroy loop over the old tail leaves only a dead
// read, and the last moves back.
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

// HYPOTHESIS: the vector at 0x58 above; this member is its range insert.
// FUNCTION: STARFLEETOP 0x0053f834
template<>
void std::vector< fn_0053f491_elem >::insert( iterator pos, const_iterator first, const_iterator last )
{
	unsigned int m = 0;
	std::_Distance( first, last, m );
	if( (unsigned int)( _End - _Last ) < m )
	{
		unsigned int n = size() + ( m < size() ? size() : m );
		int count = n;
		if( count < 0 )
			count = 0;
		fn_0053f491_elem *s = allocator.allocate( n, (void *)0 );
		fn_0053f491_elem *q = _Ucopy( _First, pos, s );
		q = _Ucopy( first, last, q );
		_Ucopy( pos, _Last, q );
		_Destroy( _First, _Last );
		allocator.deallocate( _First, _End - _First );
		_End = s + n;
		_Last = s + size() + m;
		_First = s;
	}
	else if( (unsigned int)( _Last - pos ) < m )
	{
		_Ucopy( pos, _Last, pos + m );
		_Ucopy( first + ( _Last - pos ), last, _Last );
		std::copy( first, first + ( _Last - pos ), pos );
		_Last += m;
	}
	else if( 0 < m )
	{
		_Ucopy( _Last - m, _Last, _Last );
		std::copy_backward( pos, _Last - m, _Last );
		std::copy( first, last, pos );
		_Last += m;
	}
}

// HYPOTHESIS: same vector; this member is its range erase: the elements after
// the range are assigned down (0x005400e7 as the element's assignment), the old tail is
// destroyed and the last moves back.
// FUNCTION: STARFLEETOP 0x0053fa94
template<>
std::vector< fn_0053f491_elem >::iterator std::vector< fn_0053f491_elem >::erase( iterator first, iterator last )
{
	fn_0053f491_elem *s = std::copy( last, _Last, first );
	_Destroy( s, _Last );
	_Last = s;
	return first;
}

// HYPOTHESIS: the vector at 0x6c above; this member is its range insert.
// FUNCTION: STARFLEETOP 0x0053faeb
template<>
void std::vector< fn_0054340c_owner >::insert( iterator pos, const_iterator first, const_iterator last )
{
	unsigned int m = 0;
	std::_Distance( first, last, m );
	if( (unsigned int)( _End - _Last ) < m )
	{
		unsigned int n = size() + ( m < size() ? size() : m );
		fn_0054340c_owner *s = allocator.allocate( n, (void *)0 );
		fn_0054340c_owner *q = _Ucopy( _First, pos, s );
		q = _Ucopy( first, last, q );
		_Ucopy( pos, _Last, q );
		_Destroy( _First, _Last );
		allocator.deallocate( _First, _End - _First );
		_End = s + n;
		_Last = s + size() + m;
		_First = s;
	}
	else if( (unsigned int)( _Last - pos ) < m )
	{
		_Ucopy( pos, _Last, pos + m );
		_Ucopy( first + ( _Last - pos ), last, _Last );
		std::copy( first, first + ( _Last - pos ), pos );
		_Last += m;
	}
	else if( 0 < m )
	{
		_Ucopy( _Last - m, _Last, _Last );
		std::copy_backward( pos, _Last - m, _Last );
		std::copy( first, last, pos );
		_Last += m;
	}
}

// HYPOTHESIS: same vector; this member is its range erase: the elements after
// the range are assigned down (0x00541f81 as the element's assignment), the old tail is
// destroyed and the last moves back.
// FUNCTION: STARFLEETOP 0x0053fd7e
template<>
std::vector< fn_0054340c_owner >::iterator std::vector< fn_0054340c_owner >::erase( iterator first, iterator last )
{
	fn_0054340c_owner *s = std::copy( last, _Last, first );
	_Destroy( s, _Last );
	_Last = s;
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
