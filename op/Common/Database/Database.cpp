// Common/Database/Database.cpp

#include "Common/Database/Database.h"

inline void tOptex::Leave()
{
	if( fUse != tThreadState::kIgnoreThreadObject )
	{
		if( fUseCriticalSections )
		{
			::LeaveCriticalSection( &fpSharedInfo->fCriticalSection );
		}
		else
		{
			--fpSharedInfo->fRecurseCount;
			if( fpSharedInfo->fRecurseCount > 0 )
			{
				::InterlockedDecrement( &fpSharedInfo->fLockCount );
			}
			else
			{
				fpSharedInfo->fThreadId = 0;
				if( ::InterlockedDecrement( &fpSharedInfo->fLockCount ) > 0 )
				{
					::SetEvent( fEvent );
				}
			}
		}
	}
}

inline tOptex::tGuard::~tGuard()
{
	if( fNeedToLeave )
	{
		fOptex->Leave();
	}
}

namespace nDatabase
{

// The release of the container at +0x48 and the two maps' erase calls that follow the guard are
// not modelled yet (the member types are padding in Database.h).
// FUNCTION: STARFLEETOP 0x00703af0
// FUNCTION: STARFLEETOP 0x00703cb0
tDatabase::~tDatabase()
{
	tOptex::tGuard guard( &fAccess );

	if( fAlarm )
		fAlarm->StopTimer();
	delete fAlarm;
	fAlarm = NULL;

	delete fAlarmCB;
	fAlarmCB = NULL;

	delete fDatabaseStorage;
}

}
