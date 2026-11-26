#include "Switch/DataHandler.h"

// The destructor of f_04 is inlined into ~tDataHandler.
inline tOptex::~tOptex()
{
	if( fUse != tThreadState::kIgnoreThreadObject )
	{
		if( fUseCriticalSections && fpSharedInfo != NULL )
		{
			::DeleteCriticalSection( &fpSharedInfo->fCriticalSection );
		}

		if( fEvent != NULL )
		{
			::CloseHandle( fEvent );
		}

		if( fFileMapping == NULL )
		{
			delete fpSharedInfo;
		}
		else
		{
			::UnmapViewOfFile( fpSharedInfo );
			::CloseHandle( fFileMapping );
		}
	}
}

// The body lacks one call until nSwitch::tSwitch::fn_00713fb0 is declared in Switch.h.
// FUNCTION: STARFLEETOP 0x0071c8a0
// FUNCTION: STARFLEETOP 0x0071c8c0
nSwitch::tDataHandler::~tDataHandler()
{
	bool value = f_58;
	if( value )
	{
		// HYPOTHESIS: the original unregisters the handler here, nSwitch::tSwitch::fn_00713b20()
		// ->fn_00713fb0( this ); the call is missing (see the HEADER REQUEST).
	}
	delete f_78;
	if( f_b0 != NULL )
	{
		*f_b0 = true;
	}
}
