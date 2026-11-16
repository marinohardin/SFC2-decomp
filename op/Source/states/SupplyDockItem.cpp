#include "Common/Exceptions/Validate.h"

// HYPOTHESIS: the 0x64-byte member at +0x10; only its constructor (0x0045e574) is known, and
// the constructor's unwind frame shows it has a destructor.
class tObject_0045e574
{
public:
	tObject_0045e574();
	~tObject_0045e574();
	void fn_004d269b( long arg1, void *a, void *b );

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
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8( void *a, void *b );
	virtual void vfunction9() = 0;
	virtual void vfunction10() = 0;
	virtual void vfunction11();
	virtual void vfunction12() = 0;

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
// HYPOTHESIS: vfunction9, 10 and 12 are pure; vtable 0x0079fc7c holds 0x0073ea74 in those slots.
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

// FUNCTION: STARFLEETOP 0x004861ef
void tSupplyDockItem::vfunction6()
{
	vfunction12();
}

// HYPOTHESIS: m10 looks like the 0x64-byte class tPanelMgr holds at +0x100 (declared only in
// tPanelMgr.cpp), whose 0x004d269b there takes two pointers and a flag.
// FUNCTION: STARFLEETOP 0x004861f4
void tSupplyDockItem::vfunction8( void *a, void *b )
{
	m10.fn_004d269b( m04, a, b );
}
