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
