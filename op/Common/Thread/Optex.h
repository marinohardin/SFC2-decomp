// Common/Thread/Optex.h

#if !defined( Optex_h )
#define Optex_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#include "Common/BaseTypes.h"
#include "Common/Thread/ThreadState.h"

// The destructor, Enter, TryEnter and Leave are declared here without their bodies: the SDK's
// units expand them inline and define them `inline` themselves, the game's units hold
// out-of-line copies of them.
class tOptex
{
public:
	// The SDK's scoped lock on a tOptex.
	class tGuard
	{
	public:
		tGuard( tOptex* optex );
		~tGuard();
		void Enter();
		void Leave();

	protected:
		bool	fNeedToLeave;
		tOptex*	fOptex;
	};

	tOptex( tThreadState::eUse use = tThreadState::kUseThreadObject, int32u spinCount = 4000 );
	~tOptex();

	void SetSpinCount( int32u spinCount );
	void Enter();
	bool TryEnter();
	void Leave();

protected:
	void CommonConstructor( tThreadState::eUse use, int32u spinCount, bool unicode, const void* pName );
	void Wait();

	// HYPOTHESIS: the block shared by every user of the optex; offsets follow Leave and the
	// destructor, member names are not recovered.
	struct tSharedInfo
	{
		int32u				fSpinCount;
		long				fLockCount;
		int32u				fThreadId;
		long				fRecurseCount;
		CRITICAL_SECTION	fCriticalSection;
	};

	enum eProccessorState
	{
		kNotSet,
		kMultiProccessor,
		kSingleProccessor
	};

	tThreadState::eUse	fUse;
	bool				fInitComplete;
	bool				fUseCriticalSections;
	void*				fEvent;
	void*				fFileMapping;
	tSharedInfo*		fpSharedInfo;

	static eProccessorState fProccessorState;

	static bool		fIsWindows95;
	static bool		fIsWindows98;
	static bool		fTreatWindows98As95;
};

inline tOptex::tOptex( tThreadState::eUse use, int32u spinCount )
{
	CommonConstructor( use, spinCount, false, NULL );
}

inline tOptex::tGuard::tGuard( tOptex* optex )
	: fOptex( optex )
{
	Enter();
}

inline void tOptex::tGuard::Enter()
{
	fNeedToLeave = true;
	fOptex->Enter();
}

inline void tOptex::tGuard::Leave()
{
	fOptex->Leave();
	fNeedToLeave = false;
}

#endif
