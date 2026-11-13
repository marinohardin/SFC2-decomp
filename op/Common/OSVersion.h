// Common/OSVersion.h

#if !defined( UTILITY_OSVERSION_H )
#define UTILITY_OSVERSION_H

#include <windows.h>

// PORT: the game was built with the SDK's replacement STL headers, whose auto_ptr assigns
// without a self-assignment test and releases the source only when the pointers differ or the
// source owns; the compiler's own <memory> has the later form. Only what this unit uses is
// declared. A unit that has already included the compiler's own <memory> keeps that one: it
// only calls tOSVersion, and the auto_ptr is a static member's type, so nothing it compiles
// depends on which form it is.
#if !defined( _MEMORY_ )
namespace std
{
	template<class _Ty>
	class auto_ptr
	{
	public:
		explicit auto_ptr( _Ty* _P = 0 )
			: _Owns( _P != 0 ), _Ptr( _P )
		{}
		auto_ptr<_Ty>& operator=( const auto_ptr<_Ty>& _Y )
		{
			if( _Ptr != _Y.get() )
			{
				if( _Owns )
					delete _Ptr;
				_Owns = _Y._Owns;
				_Ptr = _Y.release();
			}
			else if( _Y._Owns )
				_Owns = true, _Y.release();
			return *this;
		}
		~auto_ptr()
		{
			if( _Owns )
				delete _Ptr;
		}
		_Ty* get() const
		{
			return _Ptr;
		}
		_Ty* release() const
		{
			((auto_ptr<_Ty>*)this)->_Owns = false;
			return _Ptr;
		}
	private:
		bool _Owns;
		_Ty* _Ptr;
	};
}
#endif

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
