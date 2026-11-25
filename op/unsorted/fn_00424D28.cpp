#include "Tiger/tstring.h"
#include <vector>

// HYPOTHESIS: the owner is unknown; a vector of strings sits at +0x48 and this clears it (erase over begin and end).
class fn_00424d28_class
{
public:
	char fPad[0x48];
	std::vector<tString> fStrings;
	void fn_00424d28();
};

// FUNCTION: STARFLEETOP 0x00424d28
void fn_00424d28_class::fn_00424d28()
{
	fStrings.clear();
}
