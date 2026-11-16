#include "Common/Exceptions/Validate.h"

// HYPOTHESIS: the 0x64-byte member at +0x10; only its constructor (0x0045e574) is known, and
// the constructor's unwind frame shows it has a destructor.
class tObject_0045e574
{
public:
	tObject_0045e574();
	~tObject_0045e574();

	char m00[0x64];
};

// HYPOTHESIS: layout from the constructor's stores; 0x7c bytes per the RTTI recovery. The
// fifth argument's name comes from the original's validate text ("mask").
class tSupplyDockItem
{
public:
	tSupplyDockItem( long arg1, long arg2, long arg3, long mask, long arg5 );

	virtual long vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();

	long m04;
	long m08;
	long m0c;
	tObject_0045e574 m10;
	long m74;
	long m78;
};

// HYPOTHESIS: a free cdecl function of seven arguments; its unit is not written yet.
long fn_004d3278( long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7 );
// HYPOTHESIS: a free cdecl function of one argument; other units pass it a name string.
void fn_004d3306( long arg1 );











// The analysis names this address ~tSupplyDockItem; the code is the constructor.
// FUNCTION: STARFLEETOP 0x0048612c
tSupplyDockItem::tSupplyDockItem( long arg1, long arg2, long arg3, long mask, long arg5 )
	: m04( arg1 ), m08( arg2 ), m0c( arg3 ), m74( mask ), m78( arg5 )
{
	// The original's validate sits on line 54 (push 0x36).
	validate( mask );
}

// FUNCTION: STARFLEETOP 0x004861aa
long tSupplyDockItem::vfunction1()
{
	long result = 0;
	if( m04 )
		result = fn_004d3278( m04, m04, 0, m08, m0c, 800, 600 );
	vfunction3();
	return result;
}

// FUNCTION: STARFLEETOP 0x004861e0
void tSupplyDockItem::vfunction2()
{
	if( m04 )
		fn_004d3306( m04 );
}
