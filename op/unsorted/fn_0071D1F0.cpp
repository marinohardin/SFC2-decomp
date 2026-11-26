#include "Switch/DataHandler.h"
#include "Common/Log/LogStream.h"

// HYPOTHESIS: 0x00716040 writes an address to a stream, like an operator<<; not recovered.
std::ostream& __cdecl fn_00716040( std::ostream& stream, int32u* address );

// FUNCTION: STARFLEETOP 0x0071d220
void nSwitch::tDataHandler::fn_0071d220( tRefCount* packet )
{
	// HYPOTHESIS: the packet's fields are not recovered: a word at +0x50, a 0x14 byte
	// object at +0x54 and the flag at +0x68 that says whether the object is set.
	if( !f_38 )
	{
		void* second = NULL;
		if( *( (char*)packet + 0x6c ) != 0 )
		{
			second = (char*)packet + 0x54;
		}
		fn_slot5( *(int32u*)( (char*)packet + 0x50 ), (int32u)second );
	}
	else
	{
		fn_00716040( nLog::info, f_7c ) << " skipped handling packet " << *(long*)( (char*)packet + 0x50 ) << " while waiting to be deleted" << nLog::report;
	}
	packet->Release();
	fn_slot2();
}
