// Unit:      unsorted/fn_0053F000.cpp
// Functions: 0x0053f22f to 0x0053ffd7, 40 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

#include "Common/Database/DatabaseID.h"

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
	void fn_0042b0fd( long a, long b );
	void fn_0042add2( long value );
	void fn_0053f46d( long first, long last );
	void fn_0053f5d9( long pos, long first, long last );

	long m_00;
	long m_04;
	long m_08;
};

// HYPOTHESIS: the class of the member at 0x58 of the owner below is not known; it has
// a member function at 0x0053f4c6 taking two dwords. The names are placeholders.
class fn_0053f4c6_owner
{
public:
	void fn_0053f4c6( long a, long b );
	void fn_0053fa94( long first, long last );
	void fn_0053f834( long pos, long first, long last );

	long m_00;
	long m_04;
	long m_08;
	long m_0c;
};

// HYPOTHESIS: the element type of the container below is not known; elements are
// 0x160 bytes with a vtable, a byte test at 0x0054340c and a virtual in slot 12.
// Slot 0 is the virtual destructor (0x0053f4ea calls it with 0). The names are placeholders.
class fn_0054340c_owner
{
public:
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

	char m_04[0x15c];
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
	void fn_0053fd7e( fn_0054340c_owner *first, fn_0054340c_owner *last );
	void fn_0053faeb( fn_0054340c_owner *pos, long first, long last );

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
	char m_88[4];
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
void fn_0042b0fd_owner::fn_0053f46d( long first, long last )
{
	fn_0042b0fd( m_04, m_08 );
	fn_0053f5d9( m_04, first, last );
}

// HYPOTHESIS: the element type of the container below is not known; elements are
// 0x74 bytes with a virtual destructor in slot 0. The names are placeholders.
class fn_0053f491_elem
{
public:
	virtual ~fn_0053f491_elem();

	char m_04[0x70];
};

// HYPOTHESIS: a vector-like class (first at 4, last at 8, end at 0xc) of the elements
// above; this member destroys every element, frees the block and clears the three
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
	fn_0053f834( m_04, a, b );
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
	fn_0053faeb( m_04, a, b );
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
