// Unit:      unsorted/tMessageDialog.cpp
// Functions: 0x006005aa to 0x00600a4c, 11 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include <string>

#include "unsorted/fn_004D3143.h"

// HYPOTHESIS: the "General_Error" string defined in unsorted/fn_00600208.cpp.
extern std::string g_00c39ea0;

// FUNCTION: STARFLEETOP 0x006006b3
void fn_006006b3()
{
	fn_004d31ed( g_00c39ea0.c_str(), 1, 0, 0x280, 0x1e0 );
	fn_004d3340( g_00c39ea0.c_str() );
}

// HYPOTHESIS: the objects 0x00607d61 and 0x004bcfd3 return are unnamed; the methods are named
// by address.
class tMessageDialog;

class fn_00607d61_class
{
public:
	char fn_0060a66b();
	int fn_0060a617();
	void fn_00609e57( int param_1 );
};

class fn_004bcfd3_class
{
public:
	void fn_004bd0e5( tMessageDialog* param_1 );
};

fn_00607d61_class* fn_00607d61();
fn_004bcfd3_class* fn_004bcfd3();
bool fn_004d31b2( long param_1, long param_2 );

// HYPOTHESIS: tMessageDialog (HEADER REQUEST: unsorted/tMessageDialog.h) holds two members of
// 0x10 bytes at +0x4 and +0x14, a flag at +0x28 and a value at +0x24.
class tMessageDialog
{
public:
	void fn_0060072f( int param_1, char param_2 );
	void fn_006007f9( int param_1 );
	void fn_00600857( void* param_1 );
	void fn_00600922( void* param_1 );
	void fn_006009ed();

	char pad_00[ 4 ];
	char field_04[ 0x10 ];
	char field_14[ 0x10 ];
	int field_24;
	char field_28;
};

// FUNCTION: STARFLEETOP 0x0060072f
void tMessageDialog::fn_0060072f( int param_1, char param_2 )
{
	if ( fn_00607d61()->fn_0060a66b() == 0 )
	{
		field_28 = param_2;
		if ( param_2 != 0 )
			field_24 = fn_00607d61()->fn_0060a617();
		fn_00607d61()->fn_00609e57( param_1 );
		fn_006009ed();
		fn_004d31b2( (long) g_00c39ea0.c_str(), 0 );
		fn_004d3340( g_00c39ea0.c_str() );
		fn_004d3331( g_00c39ea0.c_str() );
		fn_006007f9( 1 );
		fn_00600857( field_04 );
		fn_00600922( field_14 );
		fn_004bcfd3()->fn_004bd0e5( this );
		fn_004d3331( g_00c39ea0.c_str() );
	}
}
