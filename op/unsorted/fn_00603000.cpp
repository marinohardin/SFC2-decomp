#include "Common/Thread/Optex.h"
#include "unsorted/fn_0042a480_object.h"

// HYPOTHESIS: an object holding an optex at +0xb8 and a vector of owned pointers at +0x150;
// the owning class is not recovered.
class fn_006032e7_object
{
public:
	void fn_006032e7();
	void fn_005ff7ac();

	int8u	field_00[0xb8];
	tOptex	field_b8;
	int8u	field_e4[0x150 - 0xb8 - sizeof( tOptex )];
	int32u	field_150;
	void**	field_154;
	void**	field_158;
};

// HYPOTHESIS: a vector of owned pointers (begin at +4, end at +8); the class is not recovered.
class fn_00603351_vector
{
public:
	void fn_00603e35();

	int32u	field_00;
	void**	field_04;
	void**	field_08;
};

// HYPOTHESIS: an object holding an optex at +0xb8; the owning class is not recovered.
class fn_00603351_object
{
public:
	void fn_00603351( fn_00603351_vector* list );

	int8u	field_00[0xb8];
	tOptex	field_b8;
};

// Inline in the SDK; Optex.h declares it only.
inline tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->Leave();
	}
}

// FUNCTION: STARFLEETOP 0x006032e7
void fn_006032e7_object::fn_006032e7()
{
	tOptex::tGuard guard( &field_b8 );
	for ( void** it = field_154; it != field_158; ++it )
	{
		if ( *it )
			delete *it;
	}
	( (fn_006032e7_object*)&field_150 )->fn_005ff7ac();
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x00603351
void fn_00603351_object::fn_00603351( fn_00603351_vector* list )
{
	tOptex::tGuard guard( &field_b8 );
	for ( void** it = list->field_04; it != list->field_08; ++it )
	{
		if ( *it )
			delete *it;
	}
	list->fn_00603e35();
	guard.Leave();
}

// HYPOTHESIS: the character object (see fn_0042a480_object) extended with two counts at +0x140
// and +0x148; the owning class is not recovered.
class fn_006033b4_object : public fn_0042a480_object
{
public:
	int fn_006033b4( int* record );

	int8u	field_110[0x140 - sizeof( fn_0042a480_object )];
	int32u	field_140;
	int8u	field_144[4];
	int32u	field_148;
};

// FUNCTION: STARFLEETOP 0x006033b4
int fn_006033b4_object::fn_006033b4( int* record )
{
	int result = 0;
	switch ( *record )
	{
	case 2:
		// HYPOTHESIS: the record holds a character object at +8.
		result = ( (fn_0042a480_object*)( record + 2 ) )->FUN_0042aa2b();
		result += ( *(byte*)( record + 1 ) & 2 ) ? field_148 : field_140;
		break;
	}
	return result;
}
