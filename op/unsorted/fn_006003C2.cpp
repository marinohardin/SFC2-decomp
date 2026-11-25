// HYPOTHESIS: the global is a string object defined in fn_00600208.cpp (a hand-written
// basic_string copy private to that unit); only its address is used here, so it is typed as bytes.
extern char g_00c39e48[];

// FUNCTION: STARFLEETOP 0x006003c2
char* fn_006003c2()
{
	return g_00c39e48;
}

// HYPOTHESIS: same string object as above (defined in fn_00600208.cpp); only its address is used.
extern char g_00c39e30[];

// FUNCTION: STARFLEETOP 0x006003c8
char* fn_006003c8()
{
	return g_00c39e30;
}

// HYPOTHESIS: same string object family as above (defined in fn_00600208.cpp); only its address is used.
extern char g_00c39e20[];

// FUNCTION: STARFLEETOP 0x006003ce
char* fn_006003ce()
{
	return g_00c39e20;
}

// HYPOTHESIS: same string object family as above (defined in fn_00600208.cpp); only its address is used.
extern char g_00c39e10[];

// FUNCTION: STARFLEETOP 0x006003d4
char* fn_006003d4()
{
	return g_00c39e10;
}
