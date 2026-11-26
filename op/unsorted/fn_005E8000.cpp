#include "Common/Log/LogStream.h"
#include "unsorted/tMetaClientMapPanel.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/fn_0042a480_object.h"
#include "Common/Thread/AccessPointer.h"
#include "Switch/Switch.h"
#include "unsorted/tCurrentTime.h"

// HYPOTHESIS: the original is the tMetaClientMapPanel member fn_005e80ef(); the lane may not
// edit the class header, so this is a free function over the panel until it is declared.
// FUNCTION: STARFLEETOP 0x005e80ef
void fn_005e80ef( tMetaClientMapPanel* panel )
{
	tOptex::tGuard guard( &panel->field_dc );
	nLog::debug << "The map server is not responding, please try again in a few minutes." << nLog::report;
}

// HYPOTHESIS: a member of the object fn_00607d61 returns (class declared only inside
// fn_00601343.cpp); declared here as a free function until a shared header holds the class.
std::string* fn_00607d61();
void __stdcall fn_0060a4b8( std::string* object, const std::string& str, int param_2 );

// HYPOTHESIS: the original is a tMetaClientMapPanel member taking (long, const std::string&);
// the lane may not edit the class header, so this is a free function over the panel.
// FUNCTION: STARFLEETOP 0x005e8140
void __stdcall fn_005e8140( tMetaClientMapPanel* panel, long param_1, const std::string& message )
{
	tOptex::tGuard guard( &panel->field_dc );
	fn_0060a4b8( fn_00607d61(), message, 0x53c05 );
}

// HYPOTHESIS: the stream object the original builds on its stack (constructor 0x00576d28,
// insert 0x00535919, destructor 0x0053b311); its class is not recovered.
class fn_00576d28_object
{
public:
	fn_00576d28_object( int param );
	~fn_00576d28_object();
	void fn_00535919( long value );
	unsigned char field_0[ 0x1c ];
};

// HYPOTHESIS: a tMetaClientMapPanel member taking a long; the lane may not edit the class
// header, so this is a free function over the panel. The members at +0xac (destination
// address) and +0xcc (switch) are read through offsets.
// FUNCTION: STARFLEETOP 0x005e8376
void __stdcall fn_005e8376( tMetaClientMapPanel* panel, long value )
{
	tOptex::tGuard guard( &panel->field_dc );
	bool changed = value != ( *gObject_0092deb4->fn_004d16b2() ).FUN_0042aa4b();
	if( changed )
	{
		fn_00576d28_object message( 0 );
		message.fn_00535919( value );
		( *(nSwitch::tSwitch**) ( (char*) panel + 0xcc ) )->fn_00714000( *(nSwitch::tAddress**) ( (char*) panel + 0xac ), 8, (const nDataStore::tStreamable*) &message );
	}
}

// HYPOTHESIS: the object at +0x120 of the panel (the cell layout of the map view); its class is
// not recovered, only the getters this function calls are placed.
class fn_005e89fd_layout
{
public:
	int fn_005e8efa();
	int fn_005e8efe();
	int fn_005e8f02( const tHexPoint& position );
	int fn_005e8f25();
};
long __fastcall fn_005c7fb3( char* param_1 );
long __fastcall fn_005c7fb7( class tCurrentTime* param_1 );

class fn_005e89fd_panel : public tMetaClientMapPanel
{
public:
	tPoint fn_005e89fd( const tHexPoint& position );
};

// HYPOTHESIS: a tMetaClientMapPanel member returning the screen position of a hex; the lane may
// not edit the class header, so this is a free __fastcall function with a dummy edx argument.
// FUNCTION: STARFLEETOP 0x005e89fd
tPoint fn_005e89fd_panel::fn_005e89fd( const tHexPoint& position )
{
	tMetaClientMapPanel* panel = this;
	tOptex::tGuard guard( &panel->field_dc );
	tMetaClientMapQueue* queue = &panel->field_fc;
	fn_005e89fd_layout* layout = (fn_005e89fd_layout*) ( (char*) panel + 0x120 );
	// HYPOTHESIS: 0x0055f269 is tCurrentTime's accessor in the symbol table; the hex point and the
	// queue share the same one-line getter, so both are called through it.
	tCurrentTime* hex = (tCurrentTime*) &position;
	long left = queue->fn_005e8efe();
	long x = ( hex->fn_0055f269() - left ) * layout->fn_005e8efe() + fn_005c7fb7( (tCurrentTime*) layout ) + layout->fn_005e8f25();
	long y = ( position.fn_005e8f21() - ( (tCurrentTime*) queue )->fn_0055f269() ) * layout->fn_005e8efa() + layout->fn_005e8f02( position ) + fn_005c7fb3( (char*) layout ) + fn_005c7fb3( (char*) layout );
	return tPoint( (short) x, (short) y );
}
