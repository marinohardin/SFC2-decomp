#include "Common/Log/LogStream.h"
#include "unsorted/tMetaClientMapPanel.h"

// HYPOTHESIS: the original is the tMetaClientMapPanel member fn_005e80ef(); the lane may not
// edit the class header, so this is a free function over the panel until it is declared.
// FUNCTION: STARFLEETOP 0x005e80ef
void fn_005e80ef( tMetaClientMapPanel* panel )
{
	tOptex::tGuard guard( &panel->field_dc );
	nLog::info << "x" << nLog::report;
}
