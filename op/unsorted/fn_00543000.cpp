#include "Common/Database/DatabaseID.h"

// HYPOTHESIS: a member of an unidentified class that returns a one-bit flag kept in
// bit 8 of the dword at +0x114, read as a bit-field (`shr eax, 8; and al, 1`).
// HYPOTHESIS: 0x005430d3 works on the same object (it sits next to the bit-field
// accessors); +0x138 is a count it lowers without going below zero.
// HYPOTHESIS: +0xd4 and +0xd8 are the begin and end of a container of tIDs whose
// object starts at +0xd0 (0x005430fa walks them).
class fn_005430ad_owner
{
public:
	bool fn_005430ad();
	void fn_005430b9(bool flag);
	void fn_005430d3(int amount);
	char *fn_005430f3();
	bool fn_005430fa(const nDatabase::tID &id);
	int fn_0054312b();
	bool fn_00543132();
	bool fn_0054313e();

	char m_pad[0xd0];
	char m0d0[4];
	nDatabase::tID *m0d4;
	nDatabase::tID *m0d8;
	char m0dc[0x38];
	unsigned int m114_0 : 7;
	unsigned int m114_7 : 1;
	unsigned int m114_8 : 1;
	unsigned int m114_9 : 1;
	char m_pad118[0x1c];
	int m134;
	int m138;
};

// FUNCTION: STARFLEETOP 0x005430ad
bool fn_005430ad_owner::fn_005430ad()
{
	return m114_8 ? true : false;
}

// FUNCTION: STARFLEETOP 0x005430b9
void fn_005430ad_owner::fn_005430b9(bool flag)
{
	if (flag)
		m114_8 = 1;
	else
		m114_8 = 0;
}

// FUNCTION: STARFLEETOP 0x005430d3
void fn_005430ad_owner::fn_005430d3(int amount)
{
	if (amount < m138)
		m138 -= amount;
	else
		m138 = 0;
}

// HYPOTHESIS: 0x005430f3 works on the same object; it returns the address of a
// member embedded at +0xd0 whose type is not known yet.
// FUNCTION: STARFLEETOP 0x005430f3
char *fn_005430ad_owner::fn_005430f3()
{
	return m0d0;
}

// FUNCTION: STARFLEETOP 0x005430fa
bool fn_005430ad_owner::fn_005430fa(const nDatabase::tID &id)
{
	for (nDatabase::tID *entry = m0d4; entry != m0d8; entry++)
		if (*entry == id)
			return true;
	return false;
}

// HYPOTHESIS: the dword at +0x134 is returned as an int; its type is not known yet.
// FUNCTION: STARFLEETOP 0x0054312b
int fn_005430ad_owner::fn_0054312b()
{
	return m134;
}

// HYPOTHESIS: a second one-bit flag, bit 9 of the dword at +0x114.
// FUNCTION: STARFLEETOP 0x00543132
bool fn_005430ad_owner::fn_00543132()
{
	return m114_9 ? true : false;
}

// HYPOTHESIS: a third one-bit flag, bit 7 of the dword at +0x114. The original
// tests it as `test byte ptr [ecx+0x114], 0x80; setne al`; every source form tried
// (bit-field, mask, byte cast) compiles to a shift instead.
// FUNCTION: STARFLEETOP 0x0054313e
bool fn_005430ad_owner::fn_0054313e()
{
	// HYPOTHESIS: byte-level read of the flag; shr remains instead of test/setne.
	return (((unsigned char *)this)[0x114] & 0x80) ? true : false;
}
