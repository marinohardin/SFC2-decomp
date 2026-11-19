//	s_00844ed0_entry.h: entries of the control name tables the network states register.
// Evidence:  layout from the loops of 0x00469271 and 0x00477440 (original file unknown)

#ifndef s_00844ed0_entry_h
#define s_00844ed0_entry_h 1

// HYPOTHESIS: tables of control names registered with fn_004d35e3; the meaning of the
// fields is not known.
struct s_00844ed0_entry
{
	const char *name;
	const char *group;
};
struct s_00844fd0_entry
{
	const char *name;
	const char *group;
	int n;
};

#endif	// s_00844ed0_entry_h
