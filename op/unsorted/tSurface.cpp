#include "Tiger/tstring.h"
#include "unsorted/tVisObject.h"

// HYPOTHESIS: the exception the constructor throws when its pixels cannot be allocated; 0x00692f60
// builds it from a message and a value, and it is thrown with the throw information at 0x008273b8.
// Its size is read from the constructor's frame.
class c_00692f60_Error
{
public:
	c_00692f60_Error( const tString &text, int a );
	virtual ~c_00692f60_Error();
	unsigned char data[ 0x28 ];
};

// HYPOTHESIS: the object at 0x00c690d0; 0x006740f0 returns the text of an error id.
class c_00c690d0_Strings
{
public:
	const tString &fn_006740f0( int id );
};

extern c_00c690d0_Strings *g_00c690d0;
// HYPOTHESIS: the text the constructor's empty message starts from; the original reaches it
// at 0x0083b344, not the pooled empty literal.
extern const char g_0083b344[];

// HYPOTHESIS: 0x006741e0 looks up an error id by its name.
int fn_006741e0( const char *name );
// HYPOTHESIS: 0x00695150 allocates a count of bytes; the first value is not known.
void *fn_00695150( int a, int size );

// HYPOTHESIS: a surface of pixels the size of its rectangle; the layout is read from the
// constructor (0x00675080). The original destructor at 0x00675250 is not recovered yet.
class tSurface : public tObjBase
{
public:
	tSurface( short x, short y, int width, int height, int a, unsigned char b, unsigned char c );
	virtual ~tSurface();

	void *field_60;
	tRect field_64;
	tPoint field_6C;
	int field_70;
	unsigned char field_74;
	unsigned char field_75;
	unsigned char field_76;
};

// FUNCTION: STARFLEETOP 0x00675060
tSurface::~tSurface()
{
}

// FUNCTION: STARFLEETOP 0x00675080
tSurface::tSurface( short x, short y, int width, int height, int a, unsigned char b, unsigned char c )
	: field_70( a ), field_74( c ), field_75( b ), field_76( 1 )
{
	field_3C.x = x;
	field_3C.y = y;
	field_6C.x = 0;
	field_6C.y = 0;
	field_64.mSet( 0, 0, width, height );
	field_34.mSet( x, y, x + width, y + height );

	field_60 = fn_00695150( 1, width * height );
	if ( field_60 == 0 )
	{
		int id = fn_006741e0( "tQ3Error_Q3SURFACE_Alloc" );
		static tString sEmpty( g_0083b344 );
		throw c_00692f60_Error( g_00c690d0 ? g_00c690d0->fn_006740f0( id ) : sEmpty, 1 );
	}
}
