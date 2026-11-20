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

	tThreadState( eUse defaultThreadObjectUse );

	virtual ~tThreadState();

	static tThreadState* GetCurThreadState();

	static void SetCurThreadState( tThreadState* );

	virtual void SetDefaultThreadObjectUse( eUse defaultThreadObjectUse );

	virtual eUse DefaultThreadObjectUse();

protected:
	static tThreadState* fCurThreadState;

	// HYPOTHESIS: the constructor argument is stored as the default use
	eUse fDefaultThreadObjectUse;
};

#endif
