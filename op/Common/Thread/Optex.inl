// Common/Thread/Optex.inl
//
// HYPOTHESIS: the inline members of tOptex as the SDK's units expand them. Only the
// single-process path is written: the named optex constructors that reach the cross-process
// path have not turned up.

inline void tOptex::SetSpinCount( int32u spinCount )
{
	if( fUse == tThreadState::kIgnoreThreadObject )
	{
		return;
	}

	// No spinning on single CPU machines
	if( fProccessorState == kMultiProccessor )
	{
		::InterlockedExchange( (long*) &fpSharedInfo->fSpinCount, spinCount );
	}
}

inline void tOptex::CommonConstructor( tThreadState::eUse use, int32u spinCount, bool unicode, const void* pName )
{
	fUse = use;

	if( fUse == tThreadState::kIgnoreThreadObject )
	{
		return;
	}

	if( fProccessorState == kNotSet )
	{
		// This is the 1st object constructed, get the number of CPUs
		SYSTEM_INFO sysInfo;
		::GetSystemInfo( &sysInfo );
		// MATCH-ONLY: validate( sysInfo.dwNumberOfProcessors > 0 ) written out: the original's
		// check names its file and line as below, and a #line naming that file breaks the
		// listing build, which must open the file it names.
		if( !(sysInfo.dwNumberOfProcessors > 0) )
		{
			nException::tValidationFailed failed( "sysInfo.dwNumberOfProcessors > 0", ".\\Thread\\Optex.cpp", 148 );
			failed.ReportException();
		}

		if( sysInfo.dwNumberOfProcessors == 1 )
		{
			fProccessorState = kSingleProccessor;
		}
		else
		{
			fProccessorState = kMultiProccessor;
		}

		fTreatWindows98As95 = false;

		fIsWindows95		= tOSVersion::Instance()->IsWindows95();
		fIsWindows98		= tOSVersion::Instance()->IsWindows98();
	}

	fUseCriticalSections = fIsWindows95;
	if ( fTreatWindows98As95 )
	{
		fUseCriticalSections = fIsWindows98;
	}

	fEvent				 = NULL;
	fFileMapping		 = NULL;
	fpSharedInfo		 = NULL;

	if( pName == NULL ) // Creating a single-process optex
	{
		if( ! fUseCriticalSections )
		{
			fEvent = ::CreateEventA( NULL, false, false, NULL );
		}

		fpSharedInfo = new tSharedInfo;
		::ZeroMemory( fpSharedInfo, sizeof(*fpSharedInfo) );
	}

	SetSpinCount( spinCount );

	// Windows 9x is slow at CreateEvent, so we use CriticalSections instead.
	if ( fUseCriticalSections )
	{
		::InitializeCriticalSection( &( fpSharedInfo->fCriticalSection ) );
	}
}
