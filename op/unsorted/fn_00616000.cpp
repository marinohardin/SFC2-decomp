// Unit:      unsorted/fn_00616000.cpp
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

#include <malloc.h>
#include <string.h>

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
	char fPad[0x44];
	fn_00616000_DirectPlayObject* fDirectPlayObject;
	char fPad48[0x104];
	long f14c;
	long f150;
	long f154;
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
