// Unit:      unsorted/fn_00616000.cpp
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

#include <windows.h>
#include <malloc.h>
#include <string.h>
#include <list>
#include <map>

#include "unsorted/tNetControl.h"

// HYPOTHESIS: the thrown type and its context argument are unknown; the owner class is unknown.
// The context's constructor (0x00619a20) zeroes three longs and a byte.
class fn_00619a20_Context
{
public:
	fn_00619a20_Context();
	long fA;
	long fB;
	long fC;
	char fD;
};

// HYPOTHESIS: its constructor (0x00619b10) calls a base constructor and sets vtable 0x007b65f0.
class fn_00616000_Error
{
public:
	fn_00616000_Error( const char* pName, const fn_00619a20_Context& context );
	fn_00616000_Error( const fn_00616000_Error& other );
	virtual ~fn_00616000_Error();
	virtual const char* v1();
	char fData[0x18];
};

// HYPOTHESIS: reports the caught error's text (slot 1); the argument type is a guess.
void fn_006123b0( const char* pText );
// HYPOTHESIS: a DirectPlay COM interface (this is pushed, methods are __stdcall); slots 7 and 8
// fit IDirectPlay4::DeletePlayerFromGroup and DestroyGroup.
class fn_00616000_DirectPlayObject
{
public:
	virtual long __stdcall v0();
	virtual long __stdcall v1();
	// HYPOTHESIS: slot 2 fits IUnknown::Release.
	virtual unsigned long __stdcall Release();
	virtual long __stdcall v3();
	virtual long __stdcall v4();
	virtual long __stdcall v5();
	virtual long __stdcall v6();
	virtual long __stdcall v7( int a, int b );
	virtual long __stdcall v8( int a );
	virtual long __stdcall v9();
	virtual long __stdcall v10();
	virtual long __stdcall v11();
	virtual long __stdcall v12();
	virtual long __stdcall v13();
	virtual long __stdcall v14();
	virtual long __stdcall v15();
	virtual long __stdcall v16();
	virtual long __stdcall v17();
	// HYPOTHESIS: slot 18 fits IDirectPlay4::GetPlayerAddress (player, data, size).
	virtual long __stdcall GetPlayerAddress( int player, void* pData, unsigned long* pSize );
	virtual long __stdcall v19();
	// HYPOTHESIS: slot 20 fits IDirectPlay4::GetPlayerData (player, data, size, flags).
	virtual long __stdcall GetPlayerData( int player, void* pData, unsigned long* pSize, unsigned long flags );
	virtual long __stdcall v21();
	// HYPOTHESIS: slot 22 fits IDirectPlay4::GetSessionDesc (data, size); 0x8877001e is DPERR_BUFFERTOOSMALL.
	virtual long __stdcall GetSessionDesc( void* pData, unsigned long* pSize );
	virtual long __stdcall v23();
	virtual long __stdcall v24();
	virtual long __stdcall v25();
	// HYPOTHESIS: slot 26 fits IDirectPlay4::Send (from, to, flags, data, size); "Send failed".
	virtual long __stdcall Send( int from, int to, int flags, void* pData, unsigned long size );
	virtual long __stdcall v27();
	virtual long __stdcall v28();
	virtual long __stdcall v29();
	virtual long __stdcall v30();
	// HYPOTHESIS: slot 31 fits IDirectPlay4::SetSessionDesc (data, flags).
	virtual long __stdcall SetSessionDesc( void* pDesc, unsigned long flags );
	virtual long __stdcall v32();
	virtual long __stdcall v33();
	virtual long __stdcall v34();
	virtual long __stdcall v35();
	virtual long __stdcall v36();
	virtual long __stdcall v37();
	// HYPOTHESIS: slot 38 fits IDirectPlay4::InitializeConnection (connection, flags).
	virtual long __stdcall InitializeConnection( void* pConnection, unsigned long flags );
	virtual long __stdcall v39();
	virtual long __stdcall v40();
	virtual long __stdcall v41();
	virtual long __stdcall v42();
	virtual long __stdcall v43();
	virtual long __stdcall v44();
	virtual long __stdcall v45();
	virtual long __stdcall v46();
	virtual long __stdcall v47();
	virtual long __stdcall v48();
	// HYPOTHESIS: slot 49 fits IDirectPlay4::SendEx; "SendEx failed".
	virtual long __stdcall SendEx( int from, int to, int flags, void* pData, unsigned long size, int priority, int timeout, void* pContext, unsigned long* pMsgID );
};

// HYPOTHESIS: thrown with the failing result; its constructor is 0x00614460.
class fn_00614460_Error
{
public:
	fn_00614460_Error( long hr, const char* pName, const fn_00619a20_Context& context );
	fn_00614460_Error( const fn_00614460_Error& other );
	virtual ~fn_00614460_Error();
	virtual const char* v1();
	char fData[0x1c];
};

// HYPOTHESIS: the global at 0x00c3b62c; 0x00614330 returns a value kept once in +0x154.
class fn_00614330_Owner
{
public:
	long fn_00614330();
};
extern fn_00614330_Owner* g_00c3b62c;

// HYPOTHESIS: a received message: two longs, the data size, then the data.
struct fn_00616470_Message
{
	long fA;
	long fB;
	unsigned long fSize;
	char fData[4];
};

// HYPOTHESIS: the sent message: a type, an unwritten long, then the caller's data.
struct fn_006161f0_Message
{
	long fType;
	long f4;
	char fData[4];
};

// HYPOTHESIS: 0x00616540 hands the session description it read to this member function.
class fn_00613f80_Target
{
public:
	void fn_00613f80( void* pDesc );
};

// HYPOTHESIS: the layout of DirectPlay's DPNAME (size, flags left unwritten, short and long
// name).
struct fn_006166c0_Name
{
	unsigned long fSize;
	unsigned long fFlags;
	const char* fShortName;
	const char* fLongName;
};

// HYPOTHESIS: a DirectPlay lobby COM interface; slot 5 fits IDirectPlayLobby::EnumAddress
// (callback, address, size, context).
class fn_00616000_LobbyObject
{
public:
	virtual long __stdcall v0();
	virtual long __stdcall v1();
	// HYPOTHESIS: slot 2 fits IUnknown::Release.
	virtual unsigned long __stdcall Release();
	virtual long __stdcall v3();
	virtual long __stdcall v4();
	virtual long __stdcall EnumAddress( int ( __stdcall *pCallback )( const GUID& guid, unsigned long size, const void* pData, void* pContext ), const void* pAddress, unsigned long size, void* pContext );
	virtual long __stdcall v6();
	virtual long __stdcall v7();
	// HYPOTHESIS: slot 8 fits IDirectPlayLobby::GetConnectionSettings (application, data, size).
	virtual long __stdcall GetConnectionSettings( unsigned long appId, void* pData, unsigned long* pSize );
	virtual long __stdcall v9();
	virtual long __stdcall v10();
	virtual long __stdcall v11();
	// HYPOTHESIS: slot 12 fits IDirectPlayLobby::SetConnectionSettings (flags, application, data).
	virtual long __stdcall SetConnectionSettings( unsigned long flags, unsigned long appId, void* pSettings );
	virtual long __stdcall v13();
	virtual long __stdcall v14();
	// HYPOTHESIS: slot 15 fits IDirectPlayLobby3::ConnectEx (flags, interface id, interface, outer).
	virtual long __stdcall ConnectEx( unsigned long flags, const GUID& riid, void** ppInterface, void* pOuter );
	virtual long __stdcall v16();
	virtual long __stdcall v17();
	// HYPOTHESIS: slot 18 fits IDirectPlayLobby2::WaitForConnectionSettings (flags).
	virtual long __stdcall WaitForConnectionSettings( unsigned long flags );
};

// HYPOTHESIS: the address callback 0x00616700 hands to EnumAddress.
int __stdcall fn_0061a2c0( const GUID& guid, unsigned long size, const void* pData, void* pContext );

// HYPOTHESIS: the class id and interface id 0x00616700 creates the temporary interface with.
extern const GUID g_007bb118;
extern const GUID g_007bb128;
// HYPOTHESIS: the class id and interface id of the lobby object 0x00616ac0 creates.
extern const GUID g_007bb0b8;
extern const GUID g_007bb0c8;

// HYPOTHESIS: 0x00618920 returns +0x1c (the connection data); 0x00618d60 copies the entries
// of a vector into its own vector at +0x70.
class fn_00616700_Provider
{
public:
	void* fn_00618920();
	void fn_00618d60( std::vector< c_004329af_entry >* pEntries );
};

// HYPOTHESIS: the value kept in the map under key 3 (the modem); +0x1c holds its provider.
class fn_00616700_Entry
{
public:
	char fPad[0x1c];
	fn_00616700_Provider* fProvider;
};

// HYPOTHESIS: the player data 0x00616d30 reads (0x24 bytes); the implicit assignment copies
// the members one by one and the trailing bytes in a loop.
struct fn_00616d30_Data
{
	long fA;
	char fB;
	c_004329af_entry fC;
	long fD;
	long fE;
	char fF[6];
};

class fn_00616000_DirectPlayHolder
{
public:
	void fn_006160a0( int a, int b );
	long fn_00616150( int a );
	long fn_006161f0( int to, int from, const void* pData, unsigned long size, int flags, long type );
	long fn_00616330( int from, int to, int flags, const void* pData, unsigned long size, int priority, int timeout, void* pContext, unsigned long* pMsgID );
	// HYPOTHESIS: the fifth argument is never read (ret 0x14); its type is unknown.
	long fn_00616470( long* pA, long* pB, void* pData, unsigned long* pSize, int unused );
	long fn_00616540( fn_00613f80_Target* pTarget );
	void fn_00616620( void* pDesc );
	// HYPOTHESIS: a member that leaves this unread; the arguments' class is a guess from the
	// shared name getter 0x0060f6d0.
	fn_006166c0_Name fn_006166c0( c_004329af_entry* pShort, c_004329af_entry* pLong );
	std::vector< c_004329af_entry >* fn_00616700();
	void fn_006169a0();
	void fn_006169e0();
	void fn_00616aa0();
	void fn_00616ac0();
	void fn_00616b70();
	// HYPOTHESIS: returns the lobby's connection settings in a buffer the caller frees.
	long fn_00616b90( unsigned long appId, void** ppSettings );
	long fn_00616c20( unsigned long appId, void* pSettings );
	// HYPOTHESIS: a true argument waits for settings; false cancels the wait (DPLWAIT_CANCEL).
	void fn_00616c50( bool wait );
	long fn_00616c80( unsigned long flags );
	long fn_00616d30( int player, fn_00616d30_Data* pData );
	// HYPOTHESIS: takes the provider map; the parameter type is a guess.
	void fn_00616fd0( std::map< long, long >* pProviders );
	char fPad[0x18];
	long f18;
	char fPad1C[0x28];
	fn_00616000_DirectPlayObject* fDirectPlayObject;
	fn_00616000_LobbyObject* fLobby;
	// HYPOTHESIS: the modem addresses EnumAddress fills.
	std::vector< c_004329af_entry > fAddresses;
	// HYPOTHESIS: the insert at 0x006135a0 is map<long, long>'s (an SDK match); the values are
	// read as pointers.
	std::map< long, long > fProviders;
	char fPad6C[0x44];
	// HYPOTHESIS: a std::list of received messages (head at +0xb4, size at +0xb8).
	std::list<fn_00616470_Message*> fMessages;
	char fPadBC[0x7c];
	bool f138;
	char fPad139[0x13];
	long f14c;
	long f150;
	long f154;
	char fPad158[0x10];
	CRITICAL_SECTION fMessagesLock;
};

// FUNCTION: STARFLEETOP 0x006160a0
void fn_00616000_DirectPlayHolder::fn_006160a0( int a, int b )
{
	try
	{
		if ( fDirectPlayObject == 0 )
			throw fn_00616000_Error( "fDirectPlayObject", fn_00619a20_Context() );
		fDirectPlayObject->v7( a, b );
	}
	catch ( ... )
	{
	}
}

// FUNCTION: STARFLEETOP 0x00616150
long fn_00616000_DirectPlayHolder::fn_00616150( int a )
{
	try
	{
		if ( fDirectPlayObject == 0 )
			throw fn_00616000_Error( "fDirectPlayObject", fn_00619a20_Context() );
		return fDirectPlayObject->v8( a );
	}
	catch ( fn_00616000_Error& e )
	{
		fn_006123b0( e.v1() );
	}
	// HYPOTHESIS: the catch path returns the value in the parameter's slot.
	return a;
}

// FUNCTION: STARFLEETOP 0x006161f0
long fn_00616000_DirectPlayHolder::fn_006161f0( int to, int from, const void* pData, unsigned long size, int flags, long type )
{
	long hr;
	unsigned long length = size + sizeof( fn_006161f0_Message );
	fn_006161f0_Message* pMessage = (fn_006161f0_Message*)_alloca( length );
	pMessage->fType = type;
	memcpy( pMessage->fData, pData, size );
	try
	{
		if ( fDirectPlayObject == 0 )
			throw fn_00616000_Error( "fDirectPlayObject", fn_00619a20_Context() );
		hr = fDirectPlayObject->Send( from, to, flags, pMessage, length );
		if ( hr < 0 && hr != 0x8877010e )
			throw fn_00614460_Error( hr, "Send failed", fn_00619a20_Context() );
		if ( f154 == 0 )
			f154 = g_00c3b62c->fn_00614330();
		f14c += length;
	}
	catch ( fn_00616000_Error& e )
	{
		fn_006123b0( e.v1() );
	}
	catch ( fn_00614460_Error& e )
	{
		fn_006123b0( e.v1() );
	}
	return hr;
}

// FUNCTION: STARFLEETOP 0x00616330
long fn_00616000_DirectPlayHolder::fn_00616330( int from, int to, int flags, const void* pData, unsigned long size, int priority, int timeout, void* pContext, unsigned long* pMsgID )
{
	long hr;
	unsigned long length = size + sizeof( fn_006161f0_Message );
	fn_006161f0_Message* pMessage = (fn_006161f0_Message*)_alloca( length );
	pMessage->fType = 1;
	pMessage->f4 = size;
	memcpy( pMessage->fData, pData, size );
	try
	{
		if ( fDirectPlayObject == 0 )
			throw fn_00616000_Error( "fDirectPlayObject", fn_00619a20_Context() );
		hr = fDirectPlayObject->SendEx( from, to, flags, pMessage, length, priority, timeout, pContext, pMsgID );
		if ( hr < 0 && hr != 0x8877010e && hr != 0x8000000a )
		{
			fn_006123b0( "SendEx failed" );
			return hr;
		}
		if ( f154 == 0 )
			f154 = g_00c3b62c->fn_00614330();
		f14c += length;
		return 0;
	}
	catch ( fn_00616000_Error& e )
	{
		fn_006123b0( e.v1() );
	}
	return hr;
}

// FUNCTION: STARFLEETOP 0x00616470
long fn_00616000_DirectPlayHolder::fn_00616470( long* pA, long* pB, void* pData, unsigned long* pSize, int )
{
	if ( fMessages.size() == 0 )
	{
		*pSize = 0;
		return 0;
	}
	EnterCriticalSection( &fMessagesLock );
	fn_00616470_Message* pMessage = fMessages.front();
	LeaveCriticalSection( &fMessagesLock );
	*pA = pMessage->fA;
	*pB = pMessage->fB;
	unsigned long size = pMessage->fSize;
	*pSize = size;
	if ( pData != 0 )
		memcpy( pData, pMessage->fData, size );
	delete pMessage;
	EnterCriticalSection( &fMessagesLock );
	fMessages.pop_front();
	LeaveCriticalSection( &fMessagesLock );
	return 0;
}

// FUNCTION: STARFLEETOP 0x00616540
long fn_00616000_DirectPlayHolder::fn_00616540( fn_00613f80_Target* pTarget )
{
	long hr;
	unsigned long size = 0;
	try
	{
		if ( fDirectPlayObject == 0 )
			throw fn_00616000_Error( "fDirectPlayObject", fn_00619a20_Context() );
		hr = fDirectPlayObject->GetSessionDesc( 0, &size );
		if ( hr == 0x8877001e && size != 0 )
		{
			void* pDesc = _alloca( size );
			hr = fDirectPlayObject->GetSessionDesc( pDesc, &size );
			pTarget->fn_00613f80( pDesc );
		}
	}
	catch ( fn_00616000_Error& e )
	{
		fn_006123b0( e.v1() );
	}
	return hr;
}

// FUNCTION: STARFLEETOP 0x00616620
void fn_00616000_DirectPlayHolder::fn_00616620( void* pDesc )
{
	try
	{
		if ( fDirectPlayObject == 0 )
			throw fn_00616000_Error( "fDirectPlayObject", fn_00619a20_Context() );
		fDirectPlayObject->SetSessionDesc( pDesc, 0 );
	}
	catch ( fn_00616000_Error& e )
	{
		fn_006123b0( e.v1() );
	}
}

// FUNCTION: STARFLEETOP 0x006166c0
fn_006166c0_Name fn_00616000_DirectPlayHolder::fn_006166c0( c_004329af_entry* pShort, c_004329af_entry* pLong )
{
	fn_006166c0_Name name;
	name.fSize = sizeof( fn_006166c0_Name );
	name.fShortName = pShort->fn_0060f6d0();
	name.fLongName = pLong->fn_0060f6d0();
	return name;
}

// FUNCTION: STARFLEETOP 0x00616700
std::vector< c_004329af_entry >* fn_00616000_DirectPlayHolder::fn_00616700()
{
	unsigned long size = 0;
	fn_00616000_DirectPlayObject* pDirectPlay = 0;
	fAddresses.clear();
	try
	{
		long hr = CoCreateInstance( g_007bb118, 0, CLSCTX_ALL, g_007bb128, (void**)&pDirectPlay );
		if ( hr < 0 )
			throw fn_00614460_Error( hr, "Fail to create temp DP Interface", fn_00619a20_Context() );
		fn_00616700_Entry* pEntry = (fn_00616700_Entry*)fProviders[3];
		hr = pDirectPlay->InitializeConnection( pEntry->fProvider->fn_00618920(), 0 );
		if ( hr < 0 )
			throw fn_00614460_Error( hr, "Fail Init Modem SP", fn_00619a20_Context() );
		void* pAddress = 0;
		hr = pDirectPlay->GetPlayerAddress( 0, 0, &size );
		if ( hr == 0x8877001e )
			pAddress = _alloca( size );
		else if ( hr < 0 )
			throw fn_00614460_Error( hr, "GetPlayerAddress size", fn_00619a20_Context() );
		if ( pAddress != 0 )
		{
			long result = pDirectPlay->GetPlayerAddress( 0, pAddress, &size );
			if ( result < 0 )
				throw fn_00614460_Error( result, "GetPlayerAddress", fn_00619a20_Context() );
			fn_00616ac0();
			hr = fLobby->EnumAddress( fn_0061a2c0, pAddress, size, &fAddresses );
			if ( hr < 0 )
				throw fn_00614460_Error( hr, "EnumAddress for modem failed", fn_00619a20_Context() );
		}
		pEntry->fProvider->fn_00618d60( &fAddresses );
		fProviders[3] = (long)pEntry;
		pDirectPlay->Release();
	}
	catch ( fn_00614460_Error& e )
	{
		fn_006123b0( e.v1() );
		if ( pDirectPlay != 0 )
			pDirectPlay->Release();
	}
	return &fAddresses;
}

// FUNCTION: STARFLEETOP 0x006169a0
void fn_00616000_DirectPlayHolder::fn_006169a0()
{
	if ( f138 )
		fn_00616150( f18 );
	fn_00616aa0();
	fn_00616b70();
	fn_00616fd0( &fProviders );
}

// The analysis ends this function at 0x00616a68, inside the catch block; the exe's bytes run on
// to 0x00616a96 and match the rest of this body.
// FUNCTION: STARFLEETOP 0x006169e0
void fn_00616000_DirectPlayHolder::fn_006169e0()
{
	if ( fDirectPlayObject == 0 )
	{
		try
		{
			long hr = CoCreateInstance( g_007bb118, 0, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, g_007bb128, (void**)&fDirectPlayObject );
			if ( hr < 0 || fDirectPlayObject == 0 )
				throw fn_00616000_Error( "CoCreateInstance failed", fn_00619a20_Context() );
		}
		catch ( fn_00616000_Error& e )
		{
			fn_006123b0( e.v1() );
			if ( fDirectPlayObject != 0 )
			{
				fDirectPlayObject->Release();
				fDirectPlayObject = 0;
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x00616aa0
void fn_00616000_DirectPlayHolder::fn_00616aa0()
{
	if ( fDirectPlayObject != 0 )
	{
		fDirectPlayObject->Release();
		fDirectPlayObject = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00616ac0
void fn_00616000_DirectPlayHolder::fn_00616ac0()
{
	if ( fLobby == 0 )
	{
		try
		{
			long hr = CoCreateInstance( g_007bb0b8, 0, CLSCTX_INPROC_SERVER, g_007bb0c8, (void**)&fLobby );
			if ( hr < 0 || fLobby == 0 )
				throw fn_00616000_Error( "CoCreateInstance failed", fn_00619a20_Context() );
		}
		catch ( fn_00616000_Error& e )
		{
			fn_006123b0( e.v1() );
			// HYPOTHESIS: 0x00616b70 releases the lobby object, as 0x00616aa0 releases the
			// DirectPlay object; the original passes this past the analysis's end.
			fn_00616b70();
		}
	}
}

// FUNCTION: STARFLEETOP 0x00616b70
void fn_00616000_DirectPlayHolder::fn_00616b70()
{
	if ( fLobby != 0 )
	{
		fLobby->Release();
		fLobby = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00616b90
long fn_00616000_DirectPlayHolder::fn_00616b90( unsigned long appId, void** ppSettings )
{
	unsigned long size = 0;
	fn_00616ac0();
	long hr = fLobby->GetConnectionSettings( appId, 0, &size );
	if ( size != 0 && hr == 0x8877001e )
	{
		char* pSettings = new char[size];
		if ( pSettings == 0 )
			return hr;
		hr = fLobby->GetConnectionSettings( appId, pSettings, &size );
		if ( hr != 0 )
		{
			delete [] pSettings;
			return hr;
		}
		*ppSettings = pSettings;
	}
	return hr;
}

// FUNCTION: STARFLEETOP 0x00616c20
long fn_00616000_DirectPlayHolder::fn_00616c20( unsigned long appId, void* pSettings )
{
	fn_00616ac0();
	return fLobby->SetConnectionSettings( 0, appId, pSettings );
}

// FUNCTION: STARFLEETOP 0x00616c50
void fn_00616000_DirectPlayHolder::fn_00616c50( bool wait )
{
	fn_00616ac0();
	fLobby->WaitForConnectionSettings( !wait );
}

// FUNCTION: STARFLEETOP 0x00616c80
long fn_00616000_DirectPlayHolder::fn_00616c80( unsigned long flags )
{
	long hr = 0;
	try
	{
		if ( fLobby == 0 )
			throw fn_00616000_Error( "fDirectPlayLobby", fn_00619a20_Context() );
		hr = fLobby->ConnectEx( flags, g_007bb128, (void**)&fDirectPlayObject, 0 );
	}
	catch ( fn_00616000_Error& e )
	{
		fn_006123b0( e.v1() );
	}
	return hr;
}

// The analysis ends this function at 0x00616e19, inside the catch block; the bytes up to there
// match this body, and the epilogue follows.
// FUNCTION: STARFLEETOP 0x00616d30
long fn_00616000_DirectPlayHolder::fn_00616d30( int player, fn_00616d30_Data* pData )
{
	long hr;
	unsigned long size;
	try
	{
		if ( fDirectPlayObject == 0 )
			throw fn_00616000_Error( "fDirectPlayObject", fn_00619a20_Context() );
		hr = fDirectPlayObject->GetPlayerData( player, 0, &size, 0 );
		if ( size > sizeof( fn_00616d30_Data ) )
			return 0x8877001e;
		fn_00616d30_Data* pBuffer = (fn_00616d30_Data*)_alloca( size );
		hr = fDirectPlayObject->GetPlayerData( player, pBuffer, &size, 0 );
		*pData = *pBuffer;
	}
	catch ( fn_00616000_Error& e )
	{
		fn_006123b0( e.v1() );
	}
	return hr;
}
