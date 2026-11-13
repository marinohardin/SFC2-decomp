// Common/OSVersion.cpp

#include "OSVersion.h"

tOSVersion::iAutoOSVersionPtr tOSVersion::fOSVersion;
OSVERSIONINFO tOSVersion::fInfo;
HINSTANCE tOSVersion::fOSDLL;

// FUNCTION: STARFLEETOP 0x006f53f0
tOSVersion* tOSVersion::Instance()
{
	if( fOSVersion.get() == NULL )
	{
		fOSVersion = iAutoOSVersionPtr( new tOSVersion );
	}

	return fOSVersion.get();
}

// FUNCTION: STARFLEETOP 0x006f5570
bool tOSVersion::Init()
{
	return fOSDLL != NULL;
}

// FUNCTION: STARFLEETOP 0x006f5580
bool tOSVersion::IsWindows95()
{
	return ( ( fInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ) && ( fInfo.dwMinorVersion == 0 ) );
}

// FUNCTION: STARFLEETOP 0x006f55a0
bool tOSVersion::IsWindows98()
{
	return ( ( fInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ) && ( fInfo.dwMinorVersion > 0 ) );
}

// FUNCTION: STARFLEETOP 0x006f54e0
tOSVersion::tOSVersion()
{
	// Find out our version
	fInfo.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &fInfo );

	// Load OS Specific DLL
	OSSPECIFIC::OS_InterlockedCompareExchange = (LONG (*)( LPLONG, LONG, LONG )) NULL;
	fOSDLL = LoadLibrary( IsWindows95() ? "OSSpecific95" : "OSSpecific98" );
	if ( fOSDLL )
	{
		// Set all of our "links"
		OSSPECIFIC::OS_InterlockedCompareExchange = (LONG (*)( LPLONG, LONG, LONG )) GetProcAddress( fOSDLL, "OS_InterlockedCompareExchange" );
	}

	if ( OSSPECIFIC::OS_InterlockedCompareExchange == NULL )
	{
		// If we fail, we'll know on first usage
		OSSPECIFIC::OS_InterlockedCompareExchange = (LONG (*)( LPLONG, LONG, LONG )) _OS_InterlockedCompareExchange;
		FreeLibrary( fOSDLL ), fOSDLL = 0;
	}
}
