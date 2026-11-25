// Common/Thread/ResourceGuard.cpp

#include "Common/Thread/ResourceGuard.h"
#include "Common/OSVersion.h"

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

// HYPOTHESIS: the SDK defines Enter and TryEnter inline; Guard expands both.
inline bool tOptex::TryEnter()
{
	if (fUse == tThreadState::kIgnoreThreadObject)
		return false;

	if (fUseCriticalSections)
		return false;

	int32u threadId = ::GetCurrentThreadId();

	bool thisThreadOwnsTheOptex = false;
	int32u spinCount = fpSharedInfo->fSpinCount;

	do {
		thisThreadOwnsTheOptex =
			(0 == (*(OSSPECIFIC::OS_InterlockedCompareExchange))(&fpSharedInfo->fLockCount, 1, 0));

		if (thisThreadOwnsTheOptex) {
			fpSharedInfo->fThreadId = threadId;
			fpSharedInfo->fRecurseCount = 1;
		} else {
			if (fpSharedInfo->fThreadId == threadId) {
				::InterlockedIncrement(&fpSharedInfo->fLockCount);
				++fpSharedInfo->fRecurseCount;
				thisThreadOwnsTheOptex = TRUE;
			}
		}
	} while (!thisThreadOwnsTheOptex && (spinCount-- > 0));

	return thisThreadOwnsTheOptex;
}

inline void tOptex::Enter()
{
	if (fUse != tThreadState::kIgnoreThreadObject) {
		if (fUseCriticalSections) {
			::EnterCriticalSection(&fpSharedInfo->fCriticalSection);
		} else if (!TryEnter()) {
			int32u threadId = ::GetCurrentThreadId();
			if (::InterlockedIncrement(&fpSharedInfo->fLockCount) == 1) {
				fpSharedInfo->fThreadId = threadId;
				fpSharedInfo->fRecurseCount = 1;
			} else if (fpSharedInfo->fThreadId == threadId) {
				++fpSharedInfo->fRecurseCount;
			} else {
				::WaitForSingleObject(fEvent, INFINITE);
				fpSharedInfo->fThreadId = threadId;
				fpSharedInfo->fRecurseCount = 1;
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x0070f160
void tResourceGuard::Guard()
{
	fCriticalSection.Enter();
	++fGuardCount;
}
