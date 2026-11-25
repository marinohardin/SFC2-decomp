// Common/Thread/ResourceGuard.cpp

#include "Common/Thread/ResourceGuard.h"

// FUNCTION: STARFLEETOP 0x0070efa0
tResourceGuard::tGuard::tGuard( tResourceGuard& resGuard )
	: fResGuard( resGuard )
{
	fResGuard.Guard();
}

// FUNCTION: STARFLEETOP 0x0070efc0
tResourceGuard::tGuard::~tGuard()
{
	fResGuard.Unguard();
}

tResourceGuard::~tResourceGuard()
{
}

// FUNCTION: STARFLEETOP 0x0070f100
inline tOptex::~tOptex()
{
	if (fUse != tThreadState::kIgnoreThreadObject) {
		if (fUseCriticalSections && fpSharedInfo != NULL)
			::DeleteCriticalSection(&fpSharedInfo->fCriticalSection);
		if (fEvent != NULL)
			::CloseHandle(fEvent);
		if (fFileMapping == NULL) {
			delete fpSharedInfo;
		} else {
			::UnmapViewOfFile(fpSharedInfo);
			::CloseHandle(fFileMapping);
		}
	}
}
