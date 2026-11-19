// Common/OSVersion.h

#if !defined( UTILITY_OSVERSION_H )
#define UTILITY_OSVERSION_H

#include <windows.h>

// A unit that has already included the compiler's own <memory> keeps its auto_ptr: it only
// calls tOSVersion, and the auto_ptr is a static member's type, so nothing it compiles depends
// on which form it is.
#include "Common/AutoPtr.h"

namespace OSSPECIFIC
{
	extern LONG (*OS_InterlockedCompareExchange)( LPLONG Destination, LONG Exchange, LONG Comperand );
}

class tOSVersion
{
public:
	static tOSVersion* Instance();
	bool Init();

	bool IsWin32s();
	bool IsWindows95();
	bool IsWindows98();
	bool IsWindowsNT();
	bool IsWindowsNT4();
	bool IsWindowsNT5();
	bool IsWindows2000();

	const DWORD GetVersion();
	const DWORD GetRevision();
	const DWORD GetBuild();
	const char* GetMiscInfo();

private:
	typedef std::auto_ptr<tOSVersion> iAutoOSVersionPtr;
	friend iAutoOSVersionPtr;

	tOSVersion();
	~tOSVersion();
	tOSVersion( const tOSVersion& copy );
	tOSVersion& operator=( const tOSVersion& copy );

	static iAutoOSVersionPtr fOSVersion;

	static OSVERSIONINFO fInfo;

	static HINSTANCE fOSDLL;
	static LONG _OS_InterlockedCompareExchange( LPLONG Destination, LONG Exchange, LONG Comperand );
};

#endif
