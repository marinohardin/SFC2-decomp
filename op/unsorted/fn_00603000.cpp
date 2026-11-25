#include "Common/Thread/Optex.h"

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
