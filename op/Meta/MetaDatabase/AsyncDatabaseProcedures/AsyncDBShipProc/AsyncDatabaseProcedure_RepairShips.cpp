#include "Common/Exceptions/Validate.h"

// HYPOTHESIS: a record reader keyed on a kind (fKind) and an offset (fOffset); the class is not
// known, so the members are named by their offsets.
class fn_005686b8_owner
{
public:
	int fKind;
	int m_04;
	int m_08;
	int fOffset;

	int fn_005686b8( int base, int value );
	int fn_0056872c( int base, char kind, int value );
	int fn_0056882f( int base, int a, int b, int value );
};

// FUNCTION: STARFLEETOP 0x005686b8
int fn_005686b8_owner::fn_005686b8( int base, int value )
{
	switch( fKind )
	{
	case 0:
		return fn_0056872c( fOffset + base + 0x25, *(char*)( fOffset + base + 0xc ), value );
	case 1:
		return fn_0056882f( base, 10, 0x18, value );
	case 2:
		return fn_0056882f( base, 0, 9, value );
	}
#line 83
	validate( false );
	return value;
}
