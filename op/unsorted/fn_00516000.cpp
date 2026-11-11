//	fn_00516000.cpp: free functions of the page at 0x00516000 whose unit is not known.
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

// HYPOTHESIS: whether an angle (0x2000 to a turn) lies between two others, wrapping around
// the circle. It returns int (all of eax is set) while its caller in tAIHelmOfficer.cpp tests
// only al, so the two were compiled in different units; C linkage lets both name it.
extern "C" int fn_005166ed(short param_1, short param_2, short param_3);

// FUNCTION: STARFLEETOP 0x005166ed
int fn_005166ed(short param_1, short param_2, short param_3)
{
	if (param_1 < 0)
		param_1 += 0x2000;
	if (param_2 < 0 && param_1 > 0x1800)
		param_1 -= 0x2000;
	if (param_3 >= 0x2000 && param_1 < 0x800)
		param_1 += 0x2000;
	if (param_1 >= param_2 && param_1 <= param_3)
		return 1;
	return 0;
}
