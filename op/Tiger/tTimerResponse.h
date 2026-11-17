//	tTimerResponse.h: tTimerResponse, the callback interface twClock::mSetTimer takes.

#ifndef	tTimerResponse_h
#define	tTimerResponse_h

// The SDK library names mGetIdentity and mSetIdentity; twClock::mSetTimer calls slot 2 with the
// timer's id, so slot 2 is mSetIdentity.
// HYPOTHESIS: slot 0 is what a timer calls when it fires; it takes no arguments and returns
// nothing, and its name is not known, so it is named after tMouseMgr_Ex::tTimer's override.
// HYPOTHESIS: slot 1 is mGetIdentity, and the identity is kept at +4.
class	tTimerResponse
{
	public:
		virtual		void			fn_0040bd8d		( void ) = 0;
		virtual		unsigned long	mGetIdentity	( void );
		virtual		void			mSetIdentity	( unsigned long identity );

		long		m_4;
};

#endif	// #ifndef tTimerResponse_h
