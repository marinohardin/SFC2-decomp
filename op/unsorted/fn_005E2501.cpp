// Unit:      unsorted/fn_005E2501.cpp
// Functions: 0x005e2501 to 0x005e2f60, 50 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

#include <string>

// HYPOTHESIS: the owning class is unknown; only vtable slots 1 and 2 are used here.
class fn_005e2501_class
{
public:
	virtual void vfunction0();
	virtual long vfunction1();
	virtual long vfunction2();

	void fn_005e2501();
};

// 0x0079564c is std::string's _Nullstr, so this global is a std::string whose
// pointer sits at 0x00c38ddc.
extern std::string g_00c38dd8;
// HYPOTHESIS: a free cdecl function of seven arguments; its unit is not written yet.
long fn_004d3278( long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7 );

// FUNCTION: STARFLEETOP 0x005e2501
void fn_005e2501_class::fn_005e2501()
{
	fn_004d3278( (long) g_00c38dd8.c_str(), 0, 0, 0, 0, vfunction1(), vfunction2() );
}

// HYPOTHESIS: a free cdecl function of a name; other units pass it image names.
void fn_004d3306( const char *psz );

// FUNCTION: STARFLEETOP 0x005e2539
void fn_005e2539()
{
	fn_004d3306( g_00c38dd8.c_str() );
}

// HYPOTHESIS: the class whose thiscall methods at 0x005e41ec..0x005e42a6 the
// handler below calls; its unit is not written yet.
class fn_005e41ec_class
{
public:
	void fn_005e41ec();
	void fn_005e4211();
	void fn_005e4236();
	void fn_005e425b();
	void fn_005e4280();
	void fn_005e4293();
	void fn_005e42a6();
};

// HYPOTHESIS: a handler object that holds the target at +0x4 and dispatches
// message ids 0x11800..0x11806 to it; what sits at +0x0 is unknown.
class fn_005e254f_class
{
public:
	void fn_005e254f( long *msg );

	long m00;
	fn_005e41ec_class *m04;
};

// FUNCTION: STARFLEETOP 0x005e254f
void fn_005e254f_class::fn_005e254f( long *msg )
{
	switch ( *msg )
	{
	case 0x11800:
		m04->fn_005e4236();
		break;
	case 0x11801:
		m04->fn_005e425b();
		break;
	case 0x11802:
		m04->fn_005e4211();
		break;
	case 0x11803:
		m04->fn_005e41ec();
		break;
	case 0x11804:
		m04->fn_005e4280();
		break;
	case 0x11805:
		m04->fn_005e4293();
		break;
	case 0x11806:
		m04->fn_005e42a6();
		break;
	}
}

// FUNCTION: STARFLEETOP 0x005e27cc
// FUNCTION: STARFLEETOP 0x005e2799
std::string g_00c39048 = "MetaBigShip";

// FUNCTION: STARFLEETOP 0x005e2822
// FUNCTION: STARFLEETOP 0x005e27ef
std::string g_00c39038 = "MetaMediumPlanet";

// FUNCTION: STARFLEETOP 0x005e2845
std::string g_00c39028 = "MetaMediumSB1Icon";
