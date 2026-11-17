// Unit:      unsorted/fn_004D3143.cpp
// Functions: 0x004d3143 to 0x004d3b8d, 61 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

// HYPOTHESIS: the global at 0x0092e4a0 is an object whose method 0x004d482a takes four
// longs; its class is not recovered yet.
class fn_004d482a_owner
{
public:
	long fn_004d482a( long arg1, long arg2, long arg3, long arg4 );
};

extern fn_004d482a_owner g_0092e4a0;

// FUNCTION: STARFLEETOP 0x004d315e
long fn_004d315e( long arg1, long arg2, long arg3, long arg4 )
{
	return g_0092e4a0.fn_004d482a( arg1, arg2, arg3, arg4 );
}

// HYPOTHESIS: the global at 0x009254f8 is the object whose method 0x004bf6b9 takes two longs;
// its class is declared only inside fn_004D2000.cpp (fn_004cb4a7_target).
class fn_004bf6b9_owner
{
public:
	void fn_004bf6b9( long arg1, long arg2 );
};

extern fn_004bf6b9_owner g_009254f8;

// FUNCTION: STARFLEETOP 0x004d3179
void fn_004d3179( long arg1, long arg2 )
{
	g_009254f8.fn_004bf6b9( arg1, arg2 );
}
