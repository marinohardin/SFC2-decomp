// Unit:      unsorted/fn_00616000.cpp
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

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
};

class fn_00616000_DirectPlayHolder
{
public:
	void fn_006160a0( int a, int b );
	long fn_00616150( int a );
	char fPad[0x44];
	fn_00616000_DirectPlayObject* fDirectPlayObject;
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
