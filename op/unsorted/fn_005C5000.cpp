#include "Common/Thread/Optex.h"
#include <string>
// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the string
// copied under the lock at offset 0x3b4.
class fn_005c5062_Holder
{
public:
	std::string FUN_005c5062();

	int			m00;
	tOptex		m04;
	char		pad[0x3b4 - 4 - sizeof(tOptex)];
	std::string	m3b4;
};

inline tOptex::tGuard::~tGuard()
{
	if( fNeedToLeave )
	{
		fOptex->Leave();
	}
}

// FUNCTION: STARFLEETOP 0x005c5062
std::string fn_005c5062_Holder::FUN_005c5062()
{
	tOptex::tGuard guard(&m04);
	return m3b4;
}
