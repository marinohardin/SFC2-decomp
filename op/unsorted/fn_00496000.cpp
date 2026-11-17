#include <stdlib.h>
#include "Tiger/tstring.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tReconnector.h"

// HYPOTHESIS: the array elements at +0x28 are tValueI<tString> (0x40 bytes; 0x0043d344 sits
// between that class's functions), declared only in Source/states/SupplyDockItem.cpp; this
// stands in for it until the class moves to a header. 0x0043d344 returns the value by copy.
// The set step (0x0049629b) assigns the tString at +0x18 and notifies with (3, 0), as
// tValueI<tString>::operator= does there.
class fn_0043d344_object : public tValueBase
{
public:
	tString fn_0043d344();
	fn_0043d344_object &operator=( const tString &value );

	tString m18;
};

inline fn_0043d344_object &fn_0043d344_object::operator=( const tString &value )
{
	m18 = value;
	fn_006712c0( 3, 0 );
	return *this;
}

// HYPOTHESIS: the record held at +0x1c0 of the owner of 0x00496253; flag byte at +0x19, six
// values at +0x1bc.
class fn_00496253_record
{
public:
	char m00[0x19];
	unsigned char m19;
	char m1a[0x1bc - 0x1a];
	long m1bc[6];
};

// HYPOTHESIS: the owner of 0x00496214 and 0x00496253 (0x00496253 calls 0x00496214 with this in
// ecx); its class is not known, possibly tEntityConnector, whose functions precede these
// (0x1c4 bytes, constructor 0x004960ba, virtual destructor 0x00496179).
class fn_00496253_object
{
public:
	virtual ~fn_00496253_object();
	tString fn_004961be( long val );
	int fn_00496214( tString s );
	void fn_00496253();
	void fn_0049629b( fn_00496253_record *record );
	fn_00496253_object();
	void fn_0049639c();
	// HYPOTHESIS: called with this in ecx, the value at +0x28 + 0x40 * i and one of the labels
	// "influence1".."influence6"; whether it uses this is not known.
	void fn_00482c00( fn_0043d344_object *value, const char *name );

	char m04[0x28 - 0x04];
	fn_0043d344_object m28[6];
	long m1a8[6];
	fn_00496253_record *m1c0;
};

// A member that ignores this: 0x0049629b calls it with this in ecx (`ret 8`).
// FUNCTION: STARFLEETOP 0x004961be
tString fn_00496253_object::fn_004961be(long val)
{
	tString s;
	return s.mAppendInt32s(val, 1);
}

// FUNCTION: STARFLEETOP 0x00496214
int fn_00496253_object::fn_00496214( tString s )
{
	return atoi(s.mCharStr());
}

// FUNCTION: STARFLEETOP 0x00496253
void fn_00496253_object::fn_00496253()
{
	if( m1c0->m19 & 4 )
	{
		long *value = m1c0->m1bc;
		fn_0043d344_object *item = m28;
		int count = 6;
		do
		{
			*value++ = fn_00496214( item++->fn_0043d344() );
		}
		while( --count );
	}
}

// FUNCTION: STARFLEETOP 0x0049629b
void fn_00496253_object::fn_0049629b( fn_00496253_record *record )
{
	m1c0 = record;
	if( record && ( record->m19 & 4 ) )
	{
		for( int i = 0; i < 6; i++ )
		{
			m1a8[i] = record->m1bc[i];
			m28[i] = fn_004961be( m1a8[i] );
		}
	}
}

extern char g_0084ebf8[];
void fn_004d3592( tReconnector *object, char *value );

// HYPOTHESIS: the class of 0x00496330, which allocates a 0x1c4-byte object (the size of
// fn_00496253_object) with the constructor 0x004960ba.
class fn_00496330_object
{
public:
	fn_00496330_object();
	~fn_00496330_object();
	void fn_00496462( fn_00496253_record *record );
	void fn_0049647e();

	fn_00496253_object *m00;
	// constructed by 0x0047f864 (tReconnector::tReconnector)
	tReconnector m04;
};

// FUNCTION: STARFLEETOP 0x00496330
fn_00496330_object::fn_00496330_object()
{
	fn_004d3592( &m04, g_0084ebf8 );
	m00 = new fn_00496253_object;
	m00->fn_0049639c();
}

// FUNCTION: STARFLEETOP 0x0049639c
void fn_00496253_object::fn_0049639c()
{
	fn_00482c00( &m28[0], "influence1" );
	fn_00482c00( &m28[1], "influence2" );
	fn_00482c00( &m28[2], "influence3" );
	fn_00482c00( &m28[3], "influence4" );
	fn_00482c00( &m28[4], "influence5" );
	fn_00482c00( &m28[5], "influence6" );
}

// FUNCTION: STARFLEETOP 0x0049640b
fn_00496330_object::~fn_00496330_object()
{
	if( m00 )
	{
		delete m00;
		m00 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00496462
void fn_00496330_object::fn_00496462( fn_00496253_record *record )
{
	m00->fn_0049629b( record );
	m04.fn_00671880( m00 );
}

// FUNCTION: STARFLEETOP 0x0049647e
void fn_00496330_object::fn_0049647e()
{
	m00->fn_00496253();
}

// FUNCTION: STARFLEETOP 0x004964f9
// HYPOTHESIS: 0x004964f9 is a mid-function slice (analysis boundary), not a real function; body is a placeholder.
void fn_004964f9()
{
}

// FUNCTION: STARFLEETOP 0x0049650d
// HYPOTHESIS: 0x0049650d is the tail (fstp [0x009172a4]; pop ecx; ret) of the same initializer as 0x004964f9 (analysis boundary); body is a placeholder.
void fn_0049650d()
{
}

// FUNCTION: STARFLEETOP 0x00496529
// HYPOTHESIS: 0x00496529 is a mid-function slice (fstp [0x009172a8]; fld 142.0; call fn_005050e2) of a global float initializer starting near 0x00496515 (analysis boundary); body is a placeholder.
void fn_00496529()
{
}

// FUNCTION: STARFLEETOP 0x0049653d
// HYPOTHESIS: 0x0049653d is a mid-function slice (fstp [0x009172ac]; fld 63.0; call fn_005050e2) of the same global float initializer (analysis boundary); body is a placeholder.
void fn_0049653d()
{
}

// FUNCTION: STARFLEETOP 0x00496551
// HYPOTHESIS: 0x00496551 is a mid-function slice (fstp [0x009172b0]; fld 104.0; call fn_005050e2) of the same global float initializer (analysis boundary); body is a placeholder.
void fn_00496551()
{
}

// FUNCTION: STARFLEETOP 0x00496565
// HYPOTHESIS: 0x00496565 is the tail (fstp [0x009172b4]; ret) of the same global float initializer (analysis boundary); body is a placeholder.
void fn_00496565()
{
}

// FUNCTION: STARFLEETOP 0x00496572
// HYPOTHESIS: 0x00496572 is a separate small function (frame, fld 187.0, call fn_005050e2); the real body is not recovered, so this is a placeholder.
void fn_00496572()
{
}

// FUNCTION: STARFLEETOP 0x00496586
// HYPOTHESIS: 0x00496586 is a mid-function slice (fstp [ebp-8]; fld 89.0; call fn_005050e2) of the function at 0x00496572 (analysis boundary); body is a placeholder.
void fn_00496586()
{
}

// FUNCTION: STARFLEETOP 0x00496597
// HYPOTHESIS: 0x00496597 is a mid-function slice (fstp [ebp-4]; fldz; fstp [esp]; call fn_005050e2) of the function at 0x00496572 (analysis boundary); body is a placeholder.
void fn_00496597()
{
}

// FUNCTION: STARFLEETOP 0x004965a4
// HYPOTHESIS: 0x004965a4 is the tail (stores of [ebp-4], st0, [ebp-8] to the global vector at 0x00917240; leave; ret) of the function at 0x00496572 (analysis boundary); body is a placeholder.
void fn_004965a4()
{
}
