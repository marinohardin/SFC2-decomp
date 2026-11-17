#include "Common/Thread/Optex.h"
#include <string>

// HYPOTHESIS: class and member names are not recovered; the object is a tOptex
// with a member at +0x238 read under the lock.
class fn_005A4173_Owner : public tOptex
{
public:
	int		fn_005A4173();
	int		fn_005A418e();

	int		fn_005A41a9();

	int*		fn_005A4208();
	int*		fn_005A4220();
	int*		fn_005A4257();
	int*		fn_005A426f();

	int			fValue14;
	char		pad0[0x24 - sizeof(tOptex) - 4];
	int			fValue24;
	char		pad4[0x34 - 0x28];
	int			fValue34;
	char		pad5[0x44 - 0x38];
	int			fValue44;
	char		pad3[0x60 - 0x48];
	float		fValue60;
	float		fValue64;
	std::string	fString68;
	std::string	fString78;
	std::string	fString88;
	std::string	fString98;
	std::string	fStringA8;
	char		pad[0x208 - 0xa8 - sizeof(std::string)];
	int		fValue208;
	int		fValue20c;
	float		fValue210;
	float		fValue214;
	char		pad2[0x238 - 0x218];
	int		fValue;

	float		fn_005A41c4();
	float		fn_005A41e6();
	float		fn_005A4238();
	float		fn_005A4287();
	std::string	fn_005A42a6();
	std::string	fn_005A4309();
	std::string	fn_005A436c();
	std::string	fn_005A43d5();
	std::string	fn_005A443e();
};

// Inline in the SDK; Optex.h declares it only.
inline tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->Leave();
	}
}

// FUNCTION: STARFLEETOP 0x005a4173
int fn_005A4173_Owner::fn_005A4173()
{
	Enter();
	int value = fValue;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a418e
int fn_005A4173_Owner::fn_005A418e()
{
	Enter();
	int value = fValue20c;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a41a9
int fn_005A4173_Owner::fn_005A41a9()
{
	Enter();
	int value = fValue208;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a41c4
float fn_005A4173_Owner::fn_005A41c4()
{
	Enter();
	float value = fValue210;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a41e6
float fn_005A4173_Owner::fn_005A41e6()
{
	Enter();
	float value = fValue214;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a4208
int* fn_005A4173_Owner::fn_005A4208()
{
	Enter();
	Leave();
	return &fValue14;
}

// FUNCTION: STARFLEETOP 0x005a4220
int* fn_005A4173_Owner::fn_005A4220()
{
	int* p = &fValue24;
	Enter();
	Leave();
	return p;
}

// FUNCTION: STARFLEETOP 0x005a4238
float fn_005A4173_Owner::fn_005A4238()
{
	Enter();
	float value = fValue60;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a4257
int* fn_005A4173_Owner::fn_005A4257()
{
	tGuard guard(this);
	return &fValue34;
}

// FUNCTION: STARFLEETOP 0x005a426f
int* fn_005A4173_Owner::fn_005A426f()
{
	tGuard guard(this);
	return &fValue44;
}

// FUNCTION: STARFLEETOP 0x005a4287
float fn_005A4173_Owner::fn_005A4287()
{
	Enter();
	float value = fValue64;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a42a6
std::string fn_005A4173_Owner::fn_005A42a6()
{
	tGuard guard(this);
	return fString68;
}

// FUNCTION: STARFLEETOP 0x005a4309
std::string fn_005A4173_Owner::fn_005A4309()
{
	tGuard guard(this);
	return fString78;
}

// FUNCTION: STARFLEETOP 0x005a436c
std::string fn_005A4173_Owner::fn_005A436c()
{
	tGuard guard(this);
	return fString88;
}

// FUNCTION: STARFLEETOP 0x005a43d5
std::string fn_005A4173_Owner::fn_005A43d5()
{
	tGuard guard(this);
	return fString98;
}

// FUNCTION: STARFLEETOP 0x005a443e
std::string fn_005A4173_Owner::fn_005A443e()
{
	tGuard guard(this);
	return fStringA8;
}
