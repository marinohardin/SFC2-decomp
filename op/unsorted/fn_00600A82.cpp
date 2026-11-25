// Unit:      unsorted/fn_00600A82.cpp
// Functions: 0x00600a82 to 0x00600ab8, 3 in op/UNITS.tsv
// Profile:   game-release (harness/units.json)

// HYPOTHESIS: the global's class and name are not known; its destructor is the funclet at 0x00600a9d.
struct fn_00600a82_object
{
	fn_00600a82_object()
	{
		m_flag = 0;
		m_value = 0;
	}
	~fn_00600a82_object();
	char m_flag;
	int m_value;
};

// FUNCTION: STARFLEETOP 0x00600a91
// FUNCTION: STARFLEETOP 0x00600a82
fn_00600a82_object g_00c39e60;
