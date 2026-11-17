// Unit:      unsorted/fn_0053F000.cpp
// Functions: 0x0053f22f to 0x0053ffd7, 40 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

#include "Common/Database/DatabaseID.h"

// HYPOTHESIS: the owner of this by-value getter is not known; the body copies a
// tID held at 0x48 into the return slot. The class and the member names are placeholders.
class fn_0053f22f_owner
{
public:
	nDatabase::tID fn_0053f22f() const;
	void fn_0053f281( nDatabase::tID id );

	char m_00[0x48];
	nDatabase::tID m_48;
};

// FUNCTION: STARFLEETOP 0x0053f22f
nDatabase::tID fn_0053f22f_owner::fn_0053f22f() const
{
	return m_48;
}

// HYPOTHESIS: the owner of this getter is not known; the body returns the address of
// a member at 0x58. The class, the member names and the member type are placeholders.
class fn_0053f25c_owner
{
public:
	char *fn_0053f25c();
	char fn_0053f260();
	char *fn_0053f264();
	char *fn_0053f268();
	long fn_0053f26c();
	long fn_0053f273();
	long fn_0053f27a();

	char m_00[0x58];
	char m_58[4];
	char m_5c[0x0c];
	char m_68;
	char m_69[3];
	char m_6c[4];
	char m_70[0x0c];
	char m_7c[4];
	char m_80[0x0c];
	long m_8c;
	long m_90;
	long m_94;
};

// FUNCTION: STARFLEETOP 0x0053f25c
char *fn_0053f25c_owner::fn_0053f25c()
{
	return m_58;
}

// HYPOTHESIS: the neighbouring getter's owner also holds the byte at 0x68 this one returns.
// FUNCTION: STARFLEETOP 0x0053f260
char fn_0053f25c_owner::fn_0053f260()
{
	return m_68;
}

// HYPOTHESIS: same owner; returns the address of a member at 0x6c of unknown type.
// FUNCTION: STARFLEETOP 0x0053f264
char *fn_0053f25c_owner::fn_0053f264()
{
	return m_6c;
}

// HYPOTHESIS: same owner; returns the address of a member at 0x7c of unknown type.
// FUNCTION: STARFLEETOP 0x0053f268
char *fn_0053f25c_owner::fn_0053f268()
{
	return m_7c;
}

// HYPOTHESIS: same owner; returns the dword at 0x8c, type unknown.
// FUNCTION: STARFLEETOP 0x0053f26c
long fn_0053f25c_owner::fn_0053f26c()
{
	return m_8c;
}

// HYPOTHESIS: same owner; returns the dword at 0x90, type unknown.
// FUNCTION: STARFLEETOP 0x0053f273
long fn_0053f25c_owner::fn_0053f273()
{
	return m_90;
}

// HYPOTHESIS: same owner; returns the dword at 0x94, type unknown.
// FUNCTION: STARFLEETOP 0x0053f27a
long fn_0053f25c_owner::fn_0053f27a()
{
	return m_94;
}

// HYPOTHESIS: the setter paired with the getter at 0x0053f22f; it takes the tID by
// value and assigns it to the member at 0x48.
// FUNCTION: STARFLEETOP 0x0053f281
void fn_0053f22f_owner::fn_0053f281( nDatabase::tID id )
{
	m_48 = id;
}
