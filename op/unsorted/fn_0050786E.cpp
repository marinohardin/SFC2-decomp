// HYPOTHESIS: the static's class and tFx_Tractor are not declared in any header yet; see HEADER REQUEST.
class tFx_Tractor
{
public:
	tFx_Tractor();
	char	pad00[0x40];
};

class fn_00967980_Class
{
public:
	fn_00967980_Class();
	~fn_00967980_Class();
};

// FUNCTION: STARFLEETOP 0x0050786e
tFx_Tractor* fn_0050786e()
{
	static fn_00967980_Class s_00967980;
	return new tFx_Tractor;
}
