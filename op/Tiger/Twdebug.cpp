//	Twdebug.cpp: twDebugPart, the Win32 debug output part, and its output targets.

#include <windows.h>
#include <string.h>
#include <stdio.h>

#include "Tiger/tsay.h"	// carries tdebug.h's tDebugPart under its guard

// HYPOTHESIS: twDebugPart and its targets are declared in this unit (no twdebug.h is known).
// Member names below fUnk* are not known from the exe or the SDK; offsets are from the
// constructor at 0x00628f30.
#define	kLogFileTargetCountMax	16

class	twDebugPart: public tDebugPart
{
public:
	class	tDbgTarget
	{
	public:
		virtual			~tDbgTarget	( void )	{}
		virtual	void	mPuts		( const char *str ) = 0;
	};

								twDebugPart		( void );
	virtual						~twDebugPart	( void );

	virtual	iTruth	mIsValid	( void );
	virtual	iTruth	mIsReady	( void );
	virtual	void	mActivate	( void );
	virtual	void	mDeactivate	( void );

	virtual void	mSendLine(const char *str, const int32u kTargets=kDefault);
	virtual int32u	mCreateLogFileTarget(
						const char	*filename,
						iTruth		bAppend	= kFalse,
						iTruth		bCreateIfNecessary = kTrue);
	virtual void	mCloseLogFileTarget(const char *filename);
	virtual void	mCloseLogFileTarget(const int32u kTargetID);

protected:
	// HYPOTHESIS: the window target reaches fWindowTarget from its window procedure thunk.
	friend	class	tWinDbg;
	// HYPOTHESIS: the COM port target reads the port number and baud rate when it opens.
	friend	class	tWinComPort;

	void	mGetConfiguration	( void );

	static	tDbgTarget	*fWindowTarget;
	static	int			fComPortNumber;
	static	int			fComPortBaud;

	tDbgTarget	*fMonoTarget;										// +0x0c
	tDbgTarget	*fComPortTarget;									// +0x10
	tDbgTarget	*fLogFileTargets[ kLogFileTargetCountMax ];			// +0x14
	int32u		fUnk54;												// +0x54
	int32u		fUnk58;												// +0x58
	iTruth		fUnk5c;												// +0x5c
};

class	tWinDbg: public twDebugPart::tDbgTarget
{
public:
	tWinDbg()
	{
		fUnk04 = 0;
		fUnk08 = 0;
		fUnk0c = 0;
		fUnk10 = 0;
		fUnk14 = 0;
		fUnk18 = 0;
		fUnk1c = 0;
	}
	virtual			~tWinDbg	( void );
	virtual	void	mPuts		( const char *str );

private:
	void	mDrawItem	( HWND hwnd, DRAWITEMSTRUCT *dis );
	long	mWndProc	( HWND hwnd, unsigned int msg, unsigned int wParam, long lParam );

	static	long	__stdcall	mWndProcThunk	( HWND hwnd, unsigned int msg, unsigned int wParam, long lParam );

	int		mInit		( void );
	void	mAppendToLastLine	( const char *str, int len );

public:
	int32u	fUnk04;
	int32u	fUnk08;
	int32u	fUnk0c;
	int32u	fUnk10;
	int32u	fUnk14;
	int32u	fUnk18;
	int32u	fUnk1c;
};

class	tWin32Mono: public twDebugPart::tDbgTarget
{
public:
	tWin32Mono()
	{
		fUnk04 = 0;
		fUnk08 = 0;
		fUnk0c = 0;
		fUnk10 = 4;
		fUnk14 = 0;
		fUnk18 = 0;
	}
	virtual			~tWin32Mono	( void );
	virtual	void	mPuts		( const char *str );

	int16u	*fUnk04;	// HYPOTHESIS: the monochrome text buffer
	int32s	fUnk08;
	int32s	fUnk0c;
	int32s	fUnk10;
	int32s	fUnk14;
	int32s	fUnk18;
};

class	tWinComPort: public twDebugPart::tDbgTarget
{
public:
	tWinComPort()
	{
		fUnk04 = -1;
		fUnk08 = -1;
		memset( fUnk0c, 0, sizeof(fUnk0c) );
		fUnk28 = 0;
		fUnk2c = 0;
	}
	virtual			~tWinComPort( void );
	virtual	void	mPuts		( const char *str );
private:
	int		mInit		( void );
public:

	int32s	fUnk04;
	int32s	fUnk08;
	int32u	fUnk0c[ 7 ];
	int32u	fUnk28;
	int32u	fUnk2c;
};

class	tWinDbgFile: public twDebugPart::tDbgTarget
{
public:
	tWinDbgFile()
	{
		fUnk04 = -1;
		fUnk08[ 0 ] = 0;
	}
	// The scalar deleting destructor is compiler-generated from this declaration; its claim
	// sits on the out-of-class definition below.
	virtual			~tWinDbgFile( void );
	virtual	void	mPuts		( const char *str );

	int		mInit		( const char *filename, iTruth bAppend, iTruth bCreateIfNecessary );

	int32s	fUnk04;		// HYPOTHESIS: the log file's Win32 HANDLE, -1 when closed
	char	fUnk08[ 260 ];
};


tDebugPart				*tDebugPart::fTheDebugPart;
twDebugPart::tDbgTarget	*twDebugPart::fWindowTarget;


// FUNCTION: STARFLEETOP 0x00628f30
twDebugPart::twDebugPart( void )
{
	fUnk54 = 0;
	fUnk58 = 0;
	fUnk5c = kFalse;

	fWindowTarget = new tWinDbg;
	fMonoTarget = new tWin32Mono;
	fComPortTarget = new tWinComPort;

	for( iIndex i = 0; i < kLogFileTargetCountMax; i++ )
	{
		fLogFileTargets[ i ] = new tWinDbgFile;
	}

	fTheDebugPart = this;
}

// The scalar deleting destructor is compiler-generated from this definition.
// FUNCTION: STARFLEETOP 0x006290b0
// FUNCTION: STARFLEETOP 0x006290d0
twDebugPart::~twDebugPart( void )
{
	fTheDebugPart = NULL;

	if( fWindowTarget )
	{
		delete fWindowTarget;
		fWindowTarget = NULL;
	}

	if( fMonoTarget )
	{
		delete fMonoTarget;
		fMonoTarget = NULL;
	}

	if( fComPortTarget )
	{
		delete fComPortTarget;
		fComPortTarget = NULL;
	}

	for( iIndex i = 0; i < kLogFileTargetCountMax; i++ )
	{
		if( fLogFileTargets[ i ] )
		{
			delete fLogFileTargets[ i ];
			fLogFileTargets[ i ] = NULL;
		}
	}
}
// FUNCTION: STARFLEETOP 0x00629060
// FUNCTION: STARFLEETOP 0x00629080
tWinDbgFile::~tWinDbgFile( void )
{
	if( fUnk04 != -1 )
	{
		CloseHandle( (HANDLE)fUnk04 );
		fUnk04 = -1;
	}
}

// The scalar deleting destructor is compiler-generated from this definition.
// FUNCTION: STARFLEETOP 0x00629ad0
// FUNCTION: STARFLEETOP 0x00629af0
tWinDbg::~tWinDbg( void )
{
	// HYPOTHESIS: fUnk04 is the debug window's HWND and fUnk0c a GDI object (its font).
	if( fUnk04 )
	{
		DestroyWindow( (HWND)fUnk04 );
		fUnk04 = 0;
	}

	if( fUnk0c )
	{
		DeleteObject( (HGDIOBJ)fUnk0c );
		fUnk0c = 0;
	}

	fUnk1c = 0;
}

// FUNCTION: STARFLEETOP 0x00629b30
void tWinDbg::mDrawItem( HWND hwnd, DRAWITEMSTRUCT *dis )
{
	switch( dis->itemAction )
	{
	case ODA_DRAWENTIRE:
	case ODA_SELECT:
		{
			HDC		hdc = dis->hDC;
			RECT	rect = dis->rcItem;
			HGDIOBJ	oldFont;
			char	text[ 512 ];

			if( fUnk0c )
				oldFont = SelectObject( hdc, (HGDIOBJ)fUnk0c );

			SetBkMode( hdc, TRANSPARENT );
			SendMessage( dis->hwndItem, LB_GETTEXT, dis->itemID, (LPARAM)text );
			ExtTextOut( hdc, rect.left + 2, rect.top, ETO_CLIPPED, &rect, text, lstrlen( text ), NULL );

			if( fUnk0c )
				SelectObject( hdc, oldFont );
		}
		break;
	}
}

// FUNCTION: STARFLEETOP 0x00629c00
long __stdcall tWinDbg::mWndProcThunk( HWND hwnd, unsigned int msg, unsigned int wParam, long lParam )
{
	tWinDbg	*target = (tWinDbg *)twDebugPart::fWindowTarget;

	if( target )
		return target->mWndProc( hwnd, msg, wParam, lParam );

	return DefWindowProc( hwnd, msg, wParam, lParam );
}

// FUNCTION: STARFLEETOP 0x00629c40
long tWinDbg::mWndProc( HWND hwnd, unsigned int msg, unsigned int wParam, long lParam )
{
	// HYPOTHESIS: fUnk08 is the list box child, fUnk0c its font and fUnk14 the always-on-top flag.
	switch( msg )
	{
	case WM_CREATE:
		{
			HDC		hdc = GetDC( NULL );
			int		height = -MulDiv( 9, GetDeviceCaps( hdc, LOGPIXELSY ), 72 );

			ReleaseDC( NULL, hdc );

			fUnk0c = (int32u)CreateFont( height, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
							OUT_RASTER_PRECIS, 0, 0, FIXED_PITCH, NULL );

			fUnk08 = (int32u)CreateWindowEx( 0, "Listbox", "",
							WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_DISABLENOSCROLL
							| LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_OWNERDRAWFIXED,
							0, 0, 0, 0, hwnd, NULL, GetModuleHandle( NULL ), NULL );
			if( !fUnk08 )
				return -1;

			HMENU	menu = GetSystemMenu( hwnd, FALSE );
			if( menu )
			{
				DeleteMenu( menu, SC_CLOSE, MF_BYCOMMAND );
				AppendMenu( menu, MF_STRING, 100, "Always on top" );
				CheckMenuItem( menu, 100, MF_UNCHECKED );
			}
			return 0;
		}

	case WM_SIZE:
		SetWindowPos( (HWND)fUnk08, NULL, 0, 0, LOWORD( lParam ), HIWORD( lParam ), SWP_NOZORDER );
		return 0;

	case WM_SETFOCUS:
		SetFocus( (HWND)fUnk08 );
		return 0;

	case WM_DESTROY:
		fUnk04 = 0;
		break;

	case WM_PAINT:
		{
			RECT		rect;
			PAINTSTRUCT	ps;

			if( GetUpdateRect( hwnd, &rect, TRUE ) )
			{
				BeginPaint( hwnd, &ps );
				EndPaint( hwnd, &ps );
			}
			return 0;
		}

	case WM_MEASUREITEM:
		{
			HDC			hdc = GetDC( hwnd );
			HGDIOBJ		oldFont;
			TEXTMETRIC	tm;

			if( fUnk0c )
				oldFont = SelectObject( hdc, (HGDIOBJ)fUnk0c );

			GetTextMetrics( hdc, &tm );

			if( fUnk0c )
				SelectObject( hdc, oldFont );

			ReleaseDC( hwnd, hdc );
			((MEASUREITEMSTRUCT *)lParam)->itemHeight = tm.tmHeight;
			return 1;
		}

	case WM_DRAWITEM:
		mDrawItem( hwnd, (DRAWITEMSTRUCT *)lParam );
		return 1;

	case WM_ERASEBKGND:
		return 1;

	case WM_SYSCOMMAND:
		if( wParam == 100 )
		{
			HMENU	menu = GetSystemMenu( hwnd, FALSE );

			if( fUnk14 )
			{
				if( SetWindowPos( hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE ) )
				{
					fUnk14 = 0;
					CheckMenuItem( menu, 100, MF_UNCHECKED );
				}
			}
			else
			{
				if( SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE ) )
				{
					fUnk14 = 1;
					CheckMenuItem( menu, 100, MF_CHECKED );
				}
			}
			return 0;
		}
		return DefWindowProc( hwnd, msg, wParam, lParam );
	}

	return DefWindowProc( hwnd, msg, wParam, lParam );
}

// FUNCTION: STARFLEETOP 0x0062a050
int tWinDbg::mInit( void )
{
	// HYPOTHESIS: the class name's text is not recovered; only the static's name and type are
	// (?pClassName@?1??mInit@tWinDbg@@AAEHXZ@4QBDB).
	static const char * const pClassName = "tWinDbg";

	WNDCLASS	wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	fUnk18 = 1;
	wc.lpfnWndProc = (WNDPROC)mWndProcThunk;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle( NULL );
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.lpszMenuName = NULL;
	wc.hbrBackground = NULL;
	wc.lpszClassName = pClassName;

	if( !RegisterClass( &wc ) )
		return 0;

	HWND	active = GetActiveWindow();
	int		cx = GetSystemMetrics( SM_CXSCREEN );
	int		cy = GetSystemMetrics( SM_CYSCREEN );

	fUnk04 = (int32u)CreateWindowEx( 0, pClassName, "Debug Window",
					WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
					cx * 3 / 4, cy / 2, cx / 4, cy / 2,
					NULL, NULL, GetModuleHandle( NULL ), NULL );

	if( fUnk04 )
	{
		fUnk1c = 1;
		ShowWindow( (HWND)fUnk04, SW_SHOWNOACTIVATE );
		UpdateWindow( (HWND)fUnk04 );

		if( active && active != (HWND)fUnk04 )
			SetActiveWindow( active );
	}

	return fUnk1c;
}

// FUNCTION: STARFLEETOP 0x0062a160
void tWinDbg::mAppendToLastLine( const char *str, int len )
{
	// HYPOTHESIS: len is not read by this build.
	char	text[ 512 ];
	int		index = SendMessage( (HWND)fUnk08, LB_GETCOUNT, 0, 0 );

	if( index != LB_ERR && index )
		index--;
	else
		index = 0;

	text[ 0 ] = 0;
	SendMessage( (HWND)fUnk08, LB_GETTEXT, index, (LPARAM)text );

	char	*p = text + lstrlen( text );

	while( p < &text[ sizeof( text ) - 1 ] )
	{
		char	c = *str++;

		if( !c || c == '\n' )
			break;

		*p++ = c;
	}
	*p = 0;

	SendMessage( (HWND)fUnk08, LB_DELETESTRING, index, 0 );
	SendMessage( (HWND)fUnk08, LB_ADDSTRING, 0, (LPARAM)text );
}

// FUNCTION: STARFLEETOP 0x0062a210
void tWinDbg::mPuts( const char *str )
{
	// HYPOTHESIS: fUnk10 is set while the last list line is unterminated, fUnk18 once mInit has
	// run and fUnk1c while the window exists; the list keeps at most 300 lines.
	if( !fUnk18 )
		mInit();

	if( !fUnk1c )
		return;

	int		count;
	int		len = strlen( str );
	if( !len )
		return;

	int			lines = ( fUnk10 == 0 );
	const char	*p = strchr( str, '\n' );

	while( p )
	{
		lines++;
		p = strchr( p + 1, '\n' );
	}

	SendMessage( (HWND)fUnk08, WM_SETREDRAW, FALSE, 0 );

	count = SendMessage( (HWND)fUnk08, LB_GETCOUNT, 0, 0 );
	if( count == LB_ERR )
		count = 0;

	lines += count;

	while( lines > 300 && count > 0 )
	{
		SendMessage( (HWND)fUnk08, LB_DELETESTRING, 0, 0 );
		count--;
		lines--;
	}

	const char	*end = str + len;
	const char	*s = str;

	while( s < end )
	{
		const char	*nl = strchr( s, '\n' );

		if( nl )
		{
			int		lineLen = nl - s;

			if( fUnk10 )
			{
				mAppendToLastLine( s, lineLen );
			}
			else
			{
				char	text[ 512 ];
				int		n = lineLen < 511 ? lineLen : 511;
				int		i;

				for( i = 0; i < n; i++ )
					text[ i ] = s[ i ];
				text[ i ] = 0;

				SendMessage( (HWND)fUnk08, LB_ADDSTRING, 0, (LPARAM)text );
				count++;
			}

			s += lineLen + 1;
			fUnk10 = 0;
		}
		else
		{
			int		lineLen = strlen( s );

			if( fUnk10 )
			{
				mAppendToLastLine( s, lineLen );
			}
			else
			{
				char	text[ 512 ];
				int		n = lineLen < 511 ? lineLen : 511;
				int		i;

				for( i = 0; i < n; i++ )
					text[ i ] = s[ i ];
				text[ i ] = 0;

				SendMessage( (HWND)fUnk08, LB_ADDSTRING, 0, (LPARAM)text );
				count++;
			}

			s += lineLen;
			fUnk10 = 1;
		}
	}

	SendMessage( (HWND)fUnk08, LB_SETCURSEL, count >= 0 ? count - 1 : 0, 0 );
	SendMessage( (HWND)fUnk08, WM_SETREDRAW, TRUE, 0 );
}

// The SDK library names this address ??1tWin32Mono@@UAE@XZ.
// FUNCTION: STARFLEETOP 0x0062a490
tWin32Mono::~tWin32Mono( void )
{
	fUnk18 = 0;
}

// FUNCTION: STARFLEETOP 0x0062a4a0
void tWin32Mono::mPuts( const char *str )
{
	// HYPOTHESIS: fUnk04 is the monochrome adapter's text buffer (80x25 cells of char and
	// attribute at 0xb0000, mapped only on Windows 9x), fUnk08 the column, fUnk0c the row,
	// fUnk10 the tab width, fUnk14 set once the adapter was probed and fUnk18 once it is usable.
	if( !fUnk14 )
	{
		OSVERSIONINFO	vi;

		fUnk14 = 1;
		vi.dwOSVersionInfoSize = sizeof( vi );

		if( GetVersionEx( &vi ) )
		{
			if( vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
			{
				fUnk04 = (int16u *)0xb0000;
				int32u	*p = (int32u *)fUnk04;
				for( int i = 80 * 25 / 2; i; i-- )
					*p++ = 0x07200720;
			}
			fUnk0c = 0;
			fUnk08 = 0;
			fUnk10 = 4;
			fUnk18 = 1;
		}
	}

	if( !fUnk18 || !fUnk04 )
		return;

	char	c;

	while( ( c = *str++ ) != 0 )
	{
		if( c == '\t' )
		{
			int		r = fUnk08 % fUnk10;

			if( r )
			{
				int		n = fUnk10 - r;

				while( n-- > 0 )
				{
					*(char *)&fUnk04[ fUnk0c * 80 + fUnk08 ] = ' ';
					if( ++fUnk08 >= 80 )
					{
						fUnk08 = 0;
						if( ++fUnk0c >= 25 )
						{
							fUnk0c = 24;
							memmove( fUnk04, fUnk04 + 80, 80 * 24 * 2 );
							int32u	*p = (int32u *)( fUnk04 + 80 * 24 );
							for( int i = 80 / 2; i; i-- )
								*p++ = 0x07200720;
						}
					}
				}
			}
		}
		else if( c == '\n' )
		{
			fUnk08 = 0;
			if( ++fUnk0c >= 25 )
			{
				fUnk0c = 24;
				memmove( fUnk04, fUnk04 + 80, 80 * 24 * 2 );
				int32u	*p = (int32u *)( fUnk04 + 80 * 24 );
				for( int i = 80 / 2; i; i-- )
					*p++ = 0x07200720;
			}
		}
		else
		{
			*(char *)&fUnk04[ fUnk0c * 80 + fUnk08 ] = c;
			if( ++fUnk08 >= 80 )
			{
				fUnk08 = 0;
				if( ++fUnk0c >= 25 )
				{
					fUnk0c = 24;
					memmove( fUnk04, fUnk04 + 80, 80 * 24 * 2 );
					int32u	*p = (int32u *)( fUnk04 + 80 * 24 );
					for( int i = 80 / 2; i; i-- )
						*p++ = 0x07200720;
				}
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x0062a6a0
tWinComPort::~tWinComPort( void )
{
	// HYPOTHESIS: fUnk08 is the port's Win32 HANDLE, -1 when closed, and fUnk0c the DCB saved
	// when the port was opened (7 dwords, sizeof(DCB)), restored before closing.
	if( fUnk08 != -1 )
	{
		SetCommState( (HANDLE)fUnk08, (DCB *)fUnk0c );
		CloseHandle( (HANDLE)fUnk08 );
		fUnk08 = -1;
	}

	fUnk2c = 0;
}

// FUNCTION: STARFLEETOP 0x0062a6e0
int tWinComPort::mInit( void )
{
	char	name[ 40 ];
	BOOL	ok = FALSE;

	fUnk28 = 1;

	if( twDebugPart::fComPortNumber <= 0 || twDebugPart::fComPortNumber >= 4 )
		return 0;

	wsprintf( name, "COM%d", twDebugPart::fComPortNumber );
	fUnk08 = (int32s)CreateFile( name, GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
								 FILE_ATTRIBUTE_NORMAL, NULL );
	if( fUnk08 == -1 )
		return 0;

	if( GetCommState( (HANDLE)fUnk08, (DCB *)fUnk0c ) )
	{
		DCB		dcb = *(DCB *)fUnk0c;
		dcb.BaudRate = twDebugPart::fComPortBaud;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fOutX = 1;
		dcb.fInX = 1;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		ok = SetCommState( (HANDLE)fUnk08, &dcb );
	}

	if( !ok )
	{
		CloseHandle( (HANDLE)fUnk08 );
		fUnk08 = -1;
	}
	else
		fUnk2c = 1;

	return fUnk2c;
}

// FUNCTION: STARFLEETOP 0x0062a7d0
void tWinComPort::mPuts( const char *str )
{
	// fUnk28 is set once the port was probed, fUnk2c once it is open; each line ending is
	// written to the port as CR LF.
	if( !fUnk28 )
		mInit();

	if( !fUnk2c )
		return;

	const char	*end = str + strlen( str );

	while( str < end )
	{
		const char	*nl = strchr( str, '\n' );

		if( nl )
		{
			DWORD	written;
			int		n = nl - str;

			if( n )
				WriteFile( (HANDLE)fUnk08, str, n, &written, NULL );
			WriteFile( (HANDLE)fUnk08, "\r\n", 2, &written, NULL );
			str += n + 1;
		}
		else
		{
			DWORD	written;
			int		n = strlen( str );

			if( n )
				WriteFile( (HANDLE)fUnk08, str, n, &written, NULL );
			str += n;
		}
	}
}

// FUNCTION: STARFLEETOP 0x0062a880
int tWinDbgFile::mInit( const char *filename, iTruth bAppend, iTruth bCreateIfNecessary )
{
	DWORD	disposition;

	if( bAppend )
		disposition = bCreateIfNecessary ? OPEN_ALWAYS : OPEN_EXISTING;
	else
		disposition = bCreateIfNecessary ? CREATE_ALWAYS : OPEN_EXISTING;

	fUnk04 = (int32s)CreateFile( filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, disposition,
								 FILE_ATTRIBUTE_NORMAL, NULL );
	if( fUnk04 == -1 )
		return 0;

	// An appended log continues at the end of the file.
	if( bAppend && SetFilePointer( (HANDLE)fUnk04, 0, NULL, FILE_END ) == 0xffffffff )
	{
		CloseHandle( (HANDLE)fUnk04 );
		fUnk04 = -1;
		return 0;
	}

	strncpy( fUnk08, filename, sizeof( fUnk08 ) - 1 );
	fUnk08[ sizeof( fUnk08 ) - 1 ] = 0;
	return 1;
}

// FUNCTION: STARFLEETOP 0x0062a930
void tWinDbgFile::mPuts( const char *str )
{
	// Each line ending is written to the log file as CR LF.
	if( fUnk04 == -1 )
		return;

	const char	*end = str + strlen( str );

	while( str < end )
	{
		const char	*nl = strchr( str, '\n' );

		if( nl )
		{
			DWORD	written;
			int		n = nl - str;

			if( n )
				WriteFile( (HANDLE)fUnk04, str, n, &written, NULL );
			WriteFile( (HANDLE)fUnk04, "\r\n", 2, &written, NULL );
			str += n + 1;
		}
		else
		{
			DWORD	written;
			int		n = strlen( str );

			if( n )
				WriteFile( (HANDLE)fUnk04, str, n, &written, NULL );
			str += n;
		}
	}
}

unsigned long	ClampBaudRate( unsigned long baud );

// HYPOTHESIS: the registry reads were inline helpers. The original inlines them everywhere, so
// they have no address of their own and their names are unknown. Locals of the first inlined
// copy and the later copies are laid out in opposite order, which a helper explains.
inline iTruth fn_RegGetDword( HKEY key, const char *name, DWORD *value )
{
	*value = 0;
	DWORD	size = sizeof( *value );
	DWORD	type = REG_DWORD;
	return RegQueryValueEx( key, name, 0, &type, (LPBYTE)value, &size ) == ERROR_SUCCESS;
}

inline iTruth fn_RegGetString( HKEY key, const char *name, char *buf, DWORD bufSize )
{
	DWORD	size = bufSize;
	DWORD	type = REG_SZ;
	return RegQueryValueEx( key, name, 0, &type, (LPBYTE)buf, &size ) == ERROR_SUCCESS;
}

// FUNCTION: STARFLEETOP 0x00629180
void twDebugPart::mGetConfiguration( void )
{
	HKEY	key;
	DWORD	value;
	char	buf[ MAX_PATH ];
	char	path[ MAX_PATH ];

	fUnk54 = ~0x3c;
	fUnk58 = 0;

	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, "SOFTWARE\\Quicksilver\\Debug", 0, KEY_READ, &key ) == ERROR_SUCCESS )
	{
		if( fn_RegGetDword( key, "ComPortEnabled", &value ) && value )
		{
			if( fn_RegGetDword( key, "ComPortNumber", &value ) && value > 0 && value <= 4 )
			{
				fComPortNumber = value;
				if( fn_RegGetDword( key, "ComPortBaud", &value ) )
				{
					fComPortBaud = ClampBaudRate( value );
				}
				fUnk54 |= 0x04;
			}
		}

		if( fn_RegGetDword( key, "WindowEnabled", &value ) && value )
		{
			fUnk54 |= 0x08;
		}

		if( fn_RegGetDword( key, "MonoEnabled", &value ) && value )
		{
			fUnk54 |= 0x20;
		}

		if( fn_RegGetDword( key, "2ndMonitorEnabled", &value ) && value )
		{
			fUnk54 |= 0x10;
		}

		if( fn_RegGetString( key, "Default", buf, sizeof( buf ) ) && buf[ 0 ] )
		{
			fUnk58 = !stricmp( buf, "Debugger" )	? 0x02 :
					 !stricmp( buf, "ComPort" )	? 0x04 :
					 !stricmp( buf, "Window" )		? 0x08 :
					 !stricmp( buf, "2ndMonitor" )	? 0x10 :
					 !stricmp( buf, "mono" )		? 0x20 : 0;
		}

		RegCloseKey( key );
	}

	if( GetCurrentDirectory( sizeof( path ), path ) )
	{
		strcat( path, "\\QSDEBUG.INI" );

		char	section[] = "DEBUG";
		UINT	n;

		n = GetPrivateProfileInt( section, "ComPortEnabled", -1, path );
		if( n != -1 && n > 0 )
		{
			n = GetPrivateProfileInt( section, "ComPortNumber", -1, path );
			if( n != -1 && n > 0 && n <= 4 )
			{
				fComPortNumber = n;
				n = GetPrivateProfileInt( section, "ComPortBaud", -1, path );
				if( n != -1 && n > 0 )
				{
					fComPortBaud = ClampBaudRate( n );
				}
				fUnk54 |= 0x04;
			}
		}

		n = GetPrivateProfileInt( section, "WindowEnabled", -1, path );
		if( n != -1 && n > 0 )
		{
			fUnk54 |= 0x08;
		}

		n = GetPrivateProfileInt( section, "MonoEnabled", -1, path );
		if( n != -1 && n > 0 )
		{
			fUnk54 |= 0x20;
		}

		n = GetPrivateProfileInt( section, "2ndMonitorEnabled", -1, path );
		if( n != -1 && n > 0 )
		{
			fUnk54 |= 0x10;
		}

		if( GetPrivateProfileString( section, "Default", "", buf, sizeof( buf ), path ) )
		{
			fUnk58 = !stricmp( buf, "Debugger" )	? 0x02 :
					 !stricmp( buf, "ComPort" )	? 0x04 :
					 !stricmp( buf, "Window" )		? 0x08 :
					 !stricmp( buf, "2ndMonitor" )	? 0x10 :
					 !stricmp( buf, "mono" )		? 0x20 : 0;
		}
	}
}

// FUNCTION: STARFLEETOP 0x00629660
unsigned long ClampBaudRate( unsigned long baud )
{
	if( baud <= CBR_110 )		return CBR_110;
	if( baud <= CBR_300 )		return CBR_300;
	if( baud <= CBR_600 )		return CBR_600;
	if( baud <= CBR_1200 )		return CBR_1200;
	if( baud <= CBR_2400 )		return CBR_2400;
	if( baud <= CBR_4800 )		return CBR_4800;
	if( baud <= CBR_9600 )		return CBR_9600;
	if( baud <= CBR_14400 )		return CBR_14400;
	if( baud <= CBR_19200 )		return CBR_19200;
	if( baud <= CBR_38400 )		return CBR_38400;
	if( baud <= CBR_56000 )		return CBR_56000;
	if( baud <= CBR_57600 )		return CBR_57600;
	if( baud <= CBR_115200 )	return CBR_115200;
	if( baud <= CBR_128000 )	return CBR_128000;
	return CBR_256000;
}

// FUNCTION: STARFLEETOP 0x00629720
void twDebugPart::mSendLine( const char *str, const int32u kTargets )
{
	if( !fTheDebugPart )
		return;

	if( !fUnk5c )
	{
		fUnk5c = kTrue;
		mGetConfiguration();
	}

	int32u	targets = kTargets & fUnk54;

	if( targets & 0x3f )
	{
		for( int16u bit = 1; bit < 0x21; bit <<= 1 )
		{
			if( bit > targets )
				break;

			if( bit & targets )
			switch( bit & targets )
			{
				case 0x01:
					switch( fUnk58 & fUnk54 )
					{
						case 0x02:
							OutputDebugString( str );
							break;
						case 0x04:
							if( fComPortTarget )
								fComPortTarget->mPuts( str );
							break;
						case 0x08:
							if( fWindowTarget )
								fWindowTarget->mPuts( str );
							break;
						case 0x20:
							if( fMonoTarget )
								fMonoTarget->mPuts( str );
							break;
					}
					break;
				case 0x02:
					OutputDebugString( str );
					break;
				case 0x04:
					if( fComPortTarget )
						fComPortTarget->mPuts( str );
					break;
				case 0x08:
					if( fWindowTarget )
						fWindowTarget->mPuts( str );
					break;
				case 0x20:
					if( fMonoTarget )
						fMonoTarget->mPuts( str );
					break;
			}
		}
	}

	if( targets >= 0x40 )
	{
		int16u	bit = 0x40;
		for( int16u i = 0; i < kLogFileTargetCountMax; bit <<= 1, i++ )
		{
			if( bit > targets )
				break;

			if( ( targets & bit ) && fLogFileTargets[ i ] )
				fLogFileTargets[ i ]->mPuts( str );
		}
	}
}

// FUNCTION: STARFLEETOP 0x006298e0
int32u twDebugPart::mCreateLogFileTarget( const char *filename, iTruth bAppend, iTruth bCreateIfNecessary )
{
	if( fTheDebugPart )
	{
		iIndex	i;

		// An open target already writing this file is reused.
		for( i = 0; i < kLogFileTargetCountMax; i++ )
		{
			tWinDbgFile	*target = (tWinDbgFile *)fLogFileTargets[ i ];

			if( target->fUnk04 != -1 && !stricmp( filename, target->fUnk08 ) )
				return 0x40 << i;
		}

		for( i = 0; i < kLogFileTargetCountMax; i++ )
		{
			tWinDbgFile	*target = (tWinDbgFile *)fLogFileTargets[ i ];

			if( target->fUnk04 == -1 && target->mInit( filename, bAppend, bCreateIfNecessary ) )
			{
				SYSTEMTIME	time;
				char		buf[ 120 ];		// HYPOTHESIS: size from the frame

				GetLocalTime( &time );
				sprintf( buf, "== Log file opened at %02d:%02d:%02d on %d/%d/%d ==\n",
						 time.wHour, time.wMinute, time.wSecond,
						 time.wMonth, time.wDay, time.wYear );
				target->mPuts( buf );
				return 0x40 << i;
			}
		}
	}

	return 0;
}

// FUNCTION: STARFLEETOP 0x00629a10
void twDebugPart::mCloseLogFileTarget( const int32u kTargetID )
{
	if( fTheDebugPart )
	{
		int32u	bit = 0x40;

		for( iIndex i = 0; i < kLogFileTargetCountMax; i++, bit <<= 1 )
		{
			tWinDbgFile	*target = (tWinDbgFile *)fLogFileTargets[ i ];

			if( target->fUnk04 != -1 && bit == kTargetID )
			{
				CloseHandle( (HANDLE)target->fUnk04 );
				target->fUnk04 = -1;
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x00629a60
void twDebugPart::mCloseLogFileTarget( const char *filename )
{
	if( fTheDebugPart )
	{
		for( iIndex i = 0; i < kLogFileTargetCountMax; i++ )
		{
			tWinDbgFile	*target = (tWinDbgFile *)fLogFileTargets[ i ];

			if( target->fUnk04 != -1 && !stricmp( filename, target->fUnk08 ) )
			{
				// HYPOTHESIS: the handle is checked again because the original closed it through
				// an inlined close helper of tWinDbgFile; the exe repeats the test after stricmp.
				if( target->fUnk04 != -1 )
				{
					CloseHandle( (HANDLE)target->fUnk04 );
					target->fUnk04 = -1;
				}
			}
		}
	}
}
