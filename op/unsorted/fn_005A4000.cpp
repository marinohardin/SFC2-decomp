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
	std::string	fStringB8;
	std::string	fStringC8;
	std::string	fStringD8;
	std::string	fStringE8;
	std::string	fStringF8;
	std::string	fString108;
	std::string	fString118;
	std::string	fString128;
	std::string	fString138;
	std::string	fString148;
	std::string	fString158;
	std::string	fString168;
	std::string	fString178;
	std::string	fString188;
	std::string	fString198;
	std::string	fString1A8;
	std::string	fString1B8;
	std::string	fString1C8;
	std::string	fString1D8;
	std::string	fString1E8;
	std::string	fString1F8;
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
	std::string	fn_005A44a7();
	std::string	fn_005A4510();
	std::string	fn_005A4579();
	std::string	fn_005A45e2();
	std::string	fn_005A464b();
	std::string	fn_005A46b4();
	std::string	fn_005A4a65();
	std::string	fn_005A471d();
	std::string	fn_005A4786();
	std::string	fn_005A47ef();
	std::string	fn_005A4858();
	std::string	fn_005A48c1();
	std::string	fn_005A492a();
	std::string	fn_005A4993();
	std::string	fn_005A49fc();
	std::string	fn_005A4ace();
	std::string	fn_005A4b37();
	std::string	fn_005A4ba0();
	std::string	fn_005A4c09();
	std::string	fn_005A4c72();
	std::string	fn_005A4cdb();
};

// Inline in the SDK; Optex.h declares it only. Its out-of-line copy (0x004297fa) is claimed in
// tClientCharacter.cpp. With the body in view the compiler knows Leave cannot throw (it calls
// only C functions), so the string getters below keep no unwind state for their return value.
inline void tOptex::Leave()
{
	if (fUse != tThreadState::kIgnoreThreadObject) {
		if (fUseCriticalSections) {
			::LeaveCriticalSection(&fpSharedInfo->fCriticalSection);
		} else if (--fpSharedInfo->fRecurseCount > 0) {
			::InterlockedDecrement(&fpSharedInfo->fLockCount);
		} else {
			fpSharedInfo->fThreadId = 0;
			if (::InterlockedDecrement(&fpSharedInfo->fLockCount) > 0)
				::SetEvent(fEvent);
		}
	}
}

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

// FUNCTION: STARFLEETOP 0x005a44a7
std::string fn_005A4173_Owner::fn_005A44a7()
{
	tGuard guard(this);
	return fStringB8;
}

// FUNCTION: STARFLEETOP 0x005a4510
std::string fn_005A4173_Owner::fn_005A4510()
{
	tGuard guard(this);
	return fStringC8;
}

// FUNCTION: STARFLEETOP 0x005a4579
std::string fn_005A4173_Owner::fn_005A4579()
{
	tGuard guard(this);
	return fStringD8;
}

// FUNCTION: STARFLEETOP 0x005a45e2
std::string fn_005A4173_Owner::fn_005A45e2()
{
	tGuard guard(this);
	return fStringE8;
}

// FUNCTION: STARFLEETOP 0x005a464b
std::string fn_005A4173_Owner::fn_005A464b()
{
	tGuard guard(this);
	return fStringF8;
}

// FUNCTION: STARFLEETOP 0x005a46b4
std::string fn_005A4173_Owner::fn_005A46b4()
{
	tGuard guard(this);
	return fString108;
}

// FUNCTION: STARFLEETOP 0x005a471d
std::string fn_005A4173_Owner::fn_005A471d()
{
	tGuard guard(this);
	return fString118;
}

// FUNCTION: STARFLEETOP 0x005a4786
std::string fn_005A4173_Owner::fn_005A4786()
{
	tGuard guard(this);
	return fString128;
}

// FUNCTION: STARFLEETOP 0x005a47ef
std::string fn_005A4173_Owner::fn_005A47ef()
{
	tGuard guard(this);
	return fString138;
}

// FUNCTION: STARFLEETOP 0x005a4858
std::string fn_005A4173_Owner::fn_005A4858()
{
	tGuard guard(this);
	return fString148;
}

// FUNCTION: STARFLEETOP 0x005a48c1
std::string fn_005A4173_Owner::fn_005A48c1()
{
	tGuard guard(this);
	return fString158;
}

// FUNCTION: STARFLEETOP 0x005a492a
std::string fn_005A4173_Owner::fn_005A492a()
{
	tGuard guard(this);
	return fString168;
}

// FUNCTION: STARFLEETOP 0x005a4993
std::string fn_005A4173_Owner::fn_005A4993()
{
	tGuard guard(this);
	return fString178;
}

// FUNCTION: STARFLEETOP 0x005a49fc
std::string fn_005A4173_Owner::fn_005A49fc()
{
	tGuard guard(this);
	return fString188;
}

// FUNCTION: STARFLEETOP 0x005a4a65
std::string fn_005A4173_Owner::fn_005A4a65()
{
	tGuard guard(this);
	return fString198;
}

// FUNCTION: STARFLEETOP 0x005a4ace
std::string fn_005A4173_Owner::fn_005A4ace()
{
	tGuard guard(this);
	return fString1A8;
}

// FUNCTION: STARFLEETOP 0x005a4b37
std::string fn_005A4173_Owner::fn_005A4b37()
{
	tGuard guard(this);
	return fString1B8;
}

// FUNCTION: STARFLEETOP 0x005a4ba0
std::string fn_005A4173_Owner::fn_005A4ba0()
{
	tGuard guard(this);
	return fString1C8;
}

// FUNCTION: STARFLEETOP 0x005a4c09
std::string fn_005A4173_Owner::fn_005A4c09()
{
	tGuard guard(this);
	return fString1D8;
}

// FUNCTION: STARFLEETOP 0x005a4c72
std::string fn_005A4173_Owner::fn_005A4c72()
{
	tGuard guard(this);
	return fString1E8;
}

// FUNCTION: STARFLEETOP 0x005a4cdb
std::string fn_005A4173_Owner::fn_005A4cdb()
{
	tGuard guard(this);
	return fString1F8;
}
