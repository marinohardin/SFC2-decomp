#include <stdlib.h>
#include "Tiger/tstring.h"
#include "unsorted/tValueBase.h"

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
// ecx); its class is not known, possibly tEntityConnector, whose functions precede these.
class fn_00496253_object
{
public:
	tString fn_004961be( long val );
	int fn_00496214( tString s );
	void fn_00496253();
	void fn_0049629b( fn_00496253_record *record );

	char m00[0x28];
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
