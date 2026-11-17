#include <stdlib.h>
#include "Tiger/tstring.h"

// HYPOTHESIS: the array elements at +0x28 are tValueI<tString> (0x40 bytes; 0x0043d344 sits
// between that class's functions), declared only in Source/states/SupplyDockItem.cpp; this
// stands in for it until the class moves to a header. 0x0043d344 returns the value by copy.
class fn_0043d344_object
{
public:
	tString fn_0043d344();

	char m00[0x40];
};

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
	int fn_00496214( tString s );
	void fn_00496253();

	char m00[0x28];
	fn_0043d344_object m28[6];
	char m1a8[0x1c0 - 0x1a8];
	fn_00496253_record *m1c0;
};

// HYPOTHESIS: `ret 8` with ecx unused; may be a member of an unknown class that ignores this.
// FUNCTION: STARFLEETOP 0x004961be
tString __stdcall fn_004961be(long val)
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
