// FUNCTION: STARFLEETOP 0x0047301e
// HYPOTHESIS: stdcall free function (the original returns with ret 4)
int __stdcall FUN_0047301e(int param_1)
{
	int result;

	switch (param_1) {
	case 0x10052:
		result = 4;
		break;
	case 0x10053:
		result = 3;
		break;
	case 0x10054:
		result = 2;
		break;
	case 0x10055:
		result = 0;
		break;
	default:
		result = -1;
		break;
	}
	return result;
}
