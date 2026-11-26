#include "Common/Log/LogStream.h"
#include "unsorted/tMetaClientMapPanel.h"

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
