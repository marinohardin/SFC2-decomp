// Unit:      unsorted/fn_00616000.cpp
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

// HYPOTHESIS: the thrown type, its size and the context getter are unknown; the owner class is unknown.
class fn_00616000_Error
{
public:
	fn_00616000_Error( const char* pName, int context );
	char fData[0x2c];
};

int fn_00619a20();
class fn_00616000_DirectPlayObject
{
public:
	virtual void v0();
	virtual void v1();
	virtual void v2();
	virtual void v3();
	virtual void v4();
	virtual void v5();
	virtual void v6();
	virtual void v7( int a, int b );
};

class fn_00616000_DirectPlayHolder
{
public:
	void fn_006160a0( int a, int b );
	char fPad[0x44];
	fn_00616000_DirectPlayObject* fDirectPlayObject;
};

// FUNCTION: STARFLEETOP 0x006160a0
void fn_00616000_DirectPlayHolder::fn_006160a0( int a, int b )
{
	try
	{
		if ( fDirectPlayObject == 0 )
			throw fn_00616000_Error( "fDirectPlayObject", fn_00619a20() );
		fDirectPlayObject->v7( a, b );
	}
	catch ( ... )
	{
	}
}
