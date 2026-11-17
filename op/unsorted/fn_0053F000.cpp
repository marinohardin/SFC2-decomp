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

	char m_00[0x48];
	nDatabase::tID m_48;
};

// FUNCTION: STARFLEETOP 0x0053f22f
nDatabase::tID fn_0053f22f_owner::fn_0053f22f() const
{
	return m_48;
}
