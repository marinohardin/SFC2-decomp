#include "Common/Exceptions/Validate.h"
#include "Common/Log/LogStream.h"

// HYPOTHESIS: a record reader keyed on a kind (fKind) and an offset (fOffset); the class is not
// known, so the members are named by their offsets.
class fn_005686b8_owner
{
public:
	int fKind;
	int m_04;
	char m_08;
	char m_09[3];
	int fOffset;

	int fn_005686b8( int base, int value );
	int fn_0056872c( int8* curState, int8 kind, unsigned int value );
	int fn_0056882f( int base, char a, char b, int value );
};

// FUNCTION: STARFLEETOP 0x005686b8
int fn_005686b8_owner::fn_005686b8( int base, int value )
{
	switch( fKind )
	{
	case 0:
		return fn_0056872c( (int8*)( fOffset + base + 0x25 ), *(int8*)( fOffset + base + 0xc ), value );
	case 1:
		return fn_0056882f( base, 10, 0x18, value );
	case 2:
		return fn_0056882f( base, 0, 9, value );
	}
#line 83
	validate( false );
	return value;
}

#line 38
// FUNCTION: STARFLEETOP 0x0056872c
int fn_005686b8_owner::fn_0056872c( int8* curState, int8 kind, unsigned int value )
{
	if( *curState >= 0 && kind >= 0 && *curState <= kind )
	{
		int8 repair;
		switch( m_04 )
		{
		case 0:
			repair = (char)( ( m_08 * 0.01f ) * kind + 0.5f - *curState );
			break;
		case 1:
			if( m_08 < kind )
				repair = m_08 - *curState;
			else
				repair = kind - *curState;
			break;
		default:
#line 133
			validate( false );
			return value;
		}
		if( repair > 0 )
		{
			if( (unsigned int)repair > value )
				repair = (char)value;
			*curState += repair;
			value -= repair;
			if( *curState > kind )
				*curState = kind;
		}
	}
	else
	{
		nLog::error << "Problem repairing ship, CurState = " << *curState << " MaxState = " << kind << nLog::report;
	}
	return value;
}

#line 77
// FUNCTION: STARFLEETOP 0x0056882f
int fn_005686b8_owner::fn_0056882f( int base, char a, char b, int value )
{
	int i = a;
	while( i < b && (unsigned int)value > 0 )
	{
		value = fn_0056872c( (int8*)( i + 0x57 + base ), *(int8*)( i + 0x3e + base ), value );
		i++;
	}
	return value;
}
