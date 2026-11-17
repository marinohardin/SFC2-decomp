// Unit:      unsorted/fn_00616000.cpp
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

#include <windows.h>
#include <malloc.h>
#include <string.h>
#include <list>

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
	virtual long __stdcall v2();
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
	virtual long __stdcall v18();
	virtual long __stdcall v19();
	virtual long __stdcall v20();
	virtual long __stdcall v21();
	virtual long __stdcall v22();
	virtual long __stdcall v23();
	virtual long __stdcall v24();
	virtual long __stdcall v25();
	// HYPOTHESIS: slot 26 fits IDirectPlay4::Send (from, to, flags, data, size); "Send failed".
	virtual long __stdcall Send( int from, int to, int flags, void* pData, unsigned long size );
	virtual long __stdcall v27();
	virtual long __stdcall v28();
	virtual long __stdcall v29();
	virtual long __stdcall v30();
	virtual long __stdcall v31();
	virtual long __stdcall v32();
	virtual long __stdcall v33();
	virtual long __stdcall v34();
	virtual long __stdcall v35();
	virtual long __stdcall v36();
	virtual long __stdcall v37();
	virtual long __stdcall v38();
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

class fn_00616000_DirectPlayHolder
{
public:
	void fn_006160a0( int a, int b );
	long fn_00616150( int a );
	long fn_006161f0( int to, int from, const void* pData, unsigned long size, int flags, long type );
	long fn_00616330( int from, int to, int flags, const void* pData, unsigned long size, int priority, int timeout, void* pContext, unsigned long* pMsgID );
	// HYPOTHESIS: the fifth argument is never read (ret 0x14); its type is unknown.
	long fn_00616470( long* pA, long* pB, void* pData, unsigned long* pSize, int unused );
	char fPad[0x44];
	fn_00616000_DirectPlayObject* fDirectPlayObject;
	char fPad48[0x68];
	// HYPOTHESIS: a std::list of received messages (head at +0xb4, size at +0xb8).
	std::list<fn_00616470_Message*> fMessages;
	char fPadBC[0x90];
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
