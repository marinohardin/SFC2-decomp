//	tgthread.h: tCoopThreadPart, its cooperative threads, and the client and native threads they
//	run, shared by tgthread.cpp and twthread.cpp.

#ifndef tgthread_h
#define tgthread_h

// tsay.h carries tpart.h and the part binding macros.
#include "Tiger/tsay.h"

#include <vector>

// HYPOTHESIS: the slot names are not recovered. mNewThread calls the second slot on the thread it
// adds, mDoDelThread the third on the thread it removes, and twCoopThreadPart::mCoopRun the fifth.
class	tClientThread
{
	public:
		virtual		void	mUnknown_00	( void );
		virtual		void	mUnknown_04	( void );
		virtual		void	mUnknown_08	( void );
		virtual		void	mUnknown_0c	( void );
		virtual		void	mUnknown_10	( void );
};

// The thread event log of the original (the SDK exports it as LogThreadEvent).
void	LogThreadEvent( char* event, long value );

// HYPOTHESIS: the layout is read from twNativeThread's constructors, which inline these ones;
// field names are offsets until a symbol names them.
class	tNativeThread
{
	public:
		tNativeThread( void ) : f04( 0 ), f08( 0 ), f0c( 1 ), f0d( 0 ) {}
		tNativeThread( tClientThread* thread, long freq ) : f04( (long)thread ), f08( freq ), f0c( 0 ), f0d( 0 ) {}
		virtual		~tNativeThread	( void ) {}
		// Slot 1 hands control to another native thread; tCoopThreadPart::mSwitchTo calls it and
		// twNativeThread overrides it.
		virtual		void	mGotoThread	( tNativeThread* thread );

		long			f04;
		long			f08;
		unsigned char	f0c;
		unsigned char	f0d;
		// HYPOTHESIS: the thread handle and the thread id. twNativeThread fills them;
		// tCoopThreadPart::mDoSleep logs the id through a tNativeThread*, so they are declared here.
		void*			f10;
		unsigned long	f14;
};

// HYPOTHESIS: 0x48 bytes with a vtable, from the array of 100 that tCoopThreadPart embeds at +0x10.
class	tCoopThread
{
	public:
		// HYPOTHESIS: the first virtual (STARFLEETOP 0x00435265); its name is not recovered.
		virtual		void	fn_00435265	( void );

		// The vtable pointer is padded to 8 bytes because the class holds doubles.
		// HYPOTHESIS: the client thread this entry runs; mDeactivate passes it to mDelThread.
		tClientThread*	fUnknown_08;
		tNativeThread*	fUnknown_0c;
		// HYPOTHESIS: a one-byte flag; mDeactivate tests only the low byte.
		char			fUnknown_10;
		long			fUnknown_14;
		long			fUnknown_18;
		long			fUnknown_1c;
		double			fUnknown_20;
		double			fUnknown_28;
		double			fUnknown_30;
		double			fUnknown_38;
		double			fUnknown_40;
};

class	tCoopThreadPart : public tPart
{
	public:
		GenAbstractBinding( tCoopThreadPart );

		virtual		iTruth	mIsValid	( void );
		virtual		iTruth	mIsReady	( void );
		virtual		void	mActivate	( void );
		virtual		void	mDeactivate	( void );

		virtual		void	mNewThread			( tClientThread* thread, long freq );
		virtual		void	mDelThread			( tClientThread* thread );
	protected:
		virtual		void	mDoDelThread		( tClientThread* thread );
	public:
		virtual		void	mCoopRun			( tNativeThread* thread ) = 0;
		virtual		double	mSetTime			( void );
		virtual		double	mGetTime			( void );
		virtual		void	mResetPriorities	( void );
		virtual		void	mSetThreadFreq		( tClientThread* thread, long freq );
		virtual		void	mSetMainThreadFreq	( long freq );
		virtual		void	mSetThreadPriority	( tClientThread* thread, long priority );
		virtual		void	mSetMainThreadPriority	( long priority );
		virtual		void	mRest				( void );
		virtual		void	mSleepFor			( double time );
		virtual		void	mSleepUntil			( double time );
		virtual		void	mSwitchTo			( tClientThread* thread );
	private:
		virtual		tNativeThread*	mNewMainThread		( void ) = 0;
		virtual		tNativeThread*	mNewNativeThread	( tClientThread* thread, long freq ) = 0;
		virtual		void			mDelNativeThread	( tNativeThread* thread ) = 0;
	public:
		virtual		tCoopThread*	mAllocCoopThread	( void );
		virtual		void			mFreeCoopThread		( tCoopThread* thread );
		// MSVC places overloads in the vtable in reverse order: slot 26 takes a tNativeThread*.
		virtual		tCoopThread*	mFindCoopThread		( tClientThread* thread );
		virtual		tCoopThread*	mFindCoopThread		( tNativeThread* thread );
		virtual		tCoopThread*	mGetBestNextThread	( tCoopThread* thread, double time );
		virtual		void			mDoSleep			( double time, tNativeThread* thread );

	protected:
		// HYPOTHESIS: member names and the types of the scalars are not recovered yet.
		long						fUnknown_000c;
		tCoopThread					fUnknown_0010[ 100 ];
		long						fUnknown_1c30;
		std::vector<tCoopThread*>	fUnknown_1c34;
		std::vector<tCoopThread*>	fUnknown_1c44;
		std::vector<tCoopThread*>	fUnknown_1c54;
		tCoopThread*				fUnknown_1c64;
		tCoopThread*				fUnknown_1c68;
		// The thread mSwitchTo is switching to.
		tCoopThread*				fUnknown_1c6c;
		// HYPOTHESIS: the wake time of the thread being switched to, a double that mSwitchTo and
		// mActivate clear as two 32-bit halves (a double store gives another order).
		long						fUnknown_1c70;
		long						fUnknown_1c74;
		// HYPOTHESIS: two 32-bit fields, not one double: mActivate stores 0x1c30 between the
		// call and 0x1c78, an order a double store did not give.
		long						fUnknown_1c78;
		long						fUnknown_1c7c;
		double						fUnknown_1c80;
};

#define	aCoopThreadPart	(BindPart(tCoopThreadPart))

#endif	// #ifndef tgthread_h
