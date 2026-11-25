#include "Tiger/tstring.h"

// HYPOTHESIS: the static is a tString built from a literal; the class is unconfirmed
class tFx_Shockwave2
{
public:
	tFx_Shockwave2();
	char	pad00[0x30];
};

// FUNCTION: STARFLEETOP 0x00506f07
tFx_Shockwave2* fn_00506f07()
{
	static tString s_name("");
	return new tFx_Shockwave2;
}
