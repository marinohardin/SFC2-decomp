#include "unsorted/tState_MetaLogon.h"
#include "unsorted/fn_004D3143.h"
#include "unsorted/c_0046aa10_msg.h"
#include <sstream>

// HYPOTHESIS: the original also calls 0x0045e574 (the constructor of the 0x64-byte object at
// +0x124) just before the store below; that class has no constructor declared in its header
// yet, so this attempt is partial.
// FUNCTION: STARFLEETOP 0x0046d0b0
tState_MetaLogon::tState_MetaLogon()
{
	field_188 = 0;
}

extern char *g_008471e4;
extern char *g_0084724c;
extern char g_0084731c[];

// FUNCTION: STARFLEETOP 0x0046d2a4
char *tState_MetaLogon::vfunction13()
{
	return g_008471e4;
}

// FUNCTION: STARFLEETOP 0x0046d2aa
void tState_MetaLogon::fn_0046d2aa( const char *text )
{
	std::stringstream stream;
	stream << text << g_0084731c << field_28.mCharStr();
	field_28 = stream.str().c_str();
	fn_004d3179( g_0084724c, field_28 );
}

// FUNCTION: STARFLEETOP 0x0046d370
void tState_MetaLogon::vfunction1( const std::string& text )
{
	fn_0046d2aa( text.c_str() );
}

// HYPOTHESIS: the screen's name is a pointer global ("Need Patching"); the second global's type is not known.
extern char *g_00847250;
extern char g_00847228[];

// FUNCTION: STARFLEETOP 0x0046d38c
void tState_MetaLogon::fn_0046d38c()
{
	field_24 = 1;
	fn_004d34c5( 0x10002, 1 );
	fn_004d34c5( 0x10001, 1 );
	fn_004d31ed( g_00847250, 1, g_00847228, 0x280, 0x1e0 );
}

// FUNCTION: STARFLEETOP 0x0046d3c8
void tState_MetaLogon::fn_0046d3c8()
{
	field_24 = 0;
	fn_004d34c5( 0x10002, 0 );
	fn_004d34c5( 0x10001, 0 );
	fn_004d32c4( g_00847250 );
}

// HYPOTHESIS: the screen's name is a pointer global ("Big Yes No").
extern char *g_00847254;

// FUNCTION: STARFLEETOP 0x0046d468
void tState_MetaLogon::fn_0046d468()
{
	fn_004d32c4( g_00847254 );
	field_188 = 0;
}

// HYPOTHESIS: 0x0046a713 is defined in tGameState.cpp; its result is unused here.
char fn_0046a713();

// FUNCTION: STARFLEETOP 0x0046d480
void tState_MetaLogon::fn_0046d480()
{
	c_0046aa10_message msg;

	while (fn_0046aa10(&msg))
	{
		switch (msg.id)
		{
		case 0x10007:
			fn_0046d468();
			fn_0046a713();
			break;
		case 0x10008:
			fn_0046d468();
			break;
		}
	}
}

// HYPOTHESIS: 0x0046db16 takes the program name to start.
void fn_0046db16( char *name );

// FUNCTION: STARFLEETOP 0x0046daf2
void fn_0046daf2()
{
	char name[25] = "\"SFC2 Refit Utility.exe\"";

	fn_0046db16( name );
}

#include <windows.h>

// HYPOTHESIS: the owner of 0x0046de63 is a reference-counted object with a virtual
// destructor at slot 0 and its count at +4; its real class is not known yet.
class fn_0046de63_Object
{
public:
	virtual ~fn_0046de63_Object();
	void fn_0046de63();

private:
	LONG fRefCount;
};

// FUNCTION: STARFLEETOP 0x0046de63
void fn_0046de63_Object::fn_0046de63()
{
	if( ::InterlockedDecrement( &fRefCount ) <= 0 )
	{
		delete this;
	}
}
