// HYPOTHESIS: the global is a string object defined in fn_00600208.cpp (a hand-written
// basic_string copy private to that unit); only its address is used here, so it is typed as bytes.
extern char g_00c39e48[];

// FUNCTION: STARFLEETOP 0x006003c2
char* fn_006003c2()
{
	return g_00c39e48;
}
