#include "Common/Thread/Optex.h"
#include <string>
// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the string
// copied under the lock at offset 0x3b4.
class fn_005c5062_Holder
{
public:
	std::string FUN_005c5062();
	std::string fn_005c50d0();
	std::string fn_005c513e();
	std::string fn_005c51ac();
	std::string fn_005c521a();
	std::string fn_005c5288();
	std::string fn_005c52f6();
	std::string fn_005c5364();
	std::string fn_005c53d2();
	std::string fn_005c54ae();
	std::string fn_005c551c();
	std::string fn_005c558a();

	int			m00;
	tOptex		m04;
	char		pad[0x3b4 - 4 - sizeof(tOptex)];
	std::string	m3b4;
	std::string	m3c4;
	std::string	m3d4;
	std::string	m3e4;
	std::string	m3f4;
	std::string	m404;
	std::string	m414;
	std::string	m424;
	std::string	m434;
	std::string	m444;
	std::string	m454;
	std::string	m464;
	std::string	m474;
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

// FUNCTION: STARFLEETOP 0x005c50d0
std::string fn_005c5062_Holder::fn_005c50d0()
{
	tOptex::tGuard guard(&m04);
	return m3c4;
}

// FUNCTION: STARFLEETOP 0x005c513e
std::string fn_005c5062_Holder::fn_005c513e()
{
	tOptex::tGuard guard(&m04);
	return m3d4;
}

// FUNCTION: STARFLEETOP 0x005c51ac
std::string fn_005c5062_Holder::fn_005c51ac()
{
	tOptex::tGuard guard(&m04);
	return m3e4;
}

// FUNCTION: STARFLEETOP 0x005c521a
std::string fn_005c5062_Holder::fn_005c521a()
{
	tOptex::tGuard guard(&m04);
	return m3f4;
}

// FUNCTION: STARFLEETOP 0x005c5288
std::string fn_005c5062_Holder::fn_005c5288()
{
	tOptex::tGuard guard(&m04);
	return m404;
}

// FUNCTION: STARFLEETOP 0x005c52f6
std::string fn_005c5062_Holder::fn_005c52f6()
{
	tOptex::tGuard guard(&m04);
	return m414;
}

// FUNCTION: STARFLEETOP 0x005c5364
std::string fn_005c5062_Holder::fn_005c5364()
{
	tOptex::tGuard guard(&m04);
	return m424;
}

// FUNCTION: STARFLEETOP 0x005c53d2
std::string fn_005c5062_Holder::fn_005c53d2()
{
	tOptex::tGuard guard(&m04);
	return m434;
}

// FUNCTION: STARFLEETOP 0x005c54ae
std::string fn_005c5062_Holder::fn_005c54ae()
{
	tOptex::tGuard guard(&m04);
	return m454;
}

// FUNCTION: STARFLEETOP 0x005c551c
std::string fn_005c5062_Holder::fn_005c551c()
{
	tOptex::tGuard guard(&m04);
	return m464;
}

// FUNCTION: STARFLEETOP 0x005c558a
std::string fn_005c5062_Holder::fn_005c558a()
{
	tOptex::tGuard guard(&m04);
	return m474;
}
