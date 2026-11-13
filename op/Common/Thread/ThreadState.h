// Common/Thread/ThreadState.h

#if !defined( ThreadState_h )
#define ThreadState_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// HYPOTHESIS: only the members the decompiled units use are declared.
class tThreadState
{
public:
	enum eUse
	{
		kIgnoreThreadObject,
		kUseThreadObject,
	};

	static void SetCurThreadState( tThreadState* );
};

#endif
