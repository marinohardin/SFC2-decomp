#ifndef FN_005EA9CE_PRED_H
#define FN_005EA9CE_PRED_H

#include "Common/Database/DatabaseID.h"

// HYPOTHESIS: a 0x64-byte element searched by 0x005ea9ce; its type is not identified. The map
// panel (0x005e62c4) keeps a vector of them behind a single-access wrapper at +0x15c. 0x005e66b7
// compares two of them by the ids at +0x4 and +0x54 and the value at +0x4c, and assigns one
// through 0x005e4942; the other bytes are not recovered.
struct fn_005ea9ce_elem {
	fn_005ea9ce_elem& operator=( const fn_005ea9ce_elem& other );

	char m_00[0x4];
	nDatabase::tID m_04;
	char m_14[0x4c - 0x14];
	long m_4c;
	char m_50[0x54 - 0x50];
	nDatabase::tID m_54;
};

// HYPOTHESIS: the predicate handed by value to 0x005ea9ce, with its test at 0x005e4db9, its
// constructor from an element at 0x005e4cc3, its copy constructor at 0x005e4cd5 and its
// destructor at 0x005e0d50 (in the metamap viewport unit); 0x005e62c4 passes a 0x64-byte copy,
// so it holds an element. Neither type is identified yet.
class fn_005ea9ce_pred {
public:
	fn_005ea9ce_pred( const fn_005ea9ce_elem& elem );
	fn_005ea9ce_pred( const fn_005ea9ce_pred& other );
	~fn_005ea9ce_pred();
	bool operator()( const fn_005ea9ce_elem& elem );

	fn_005ea9ce_elem m_00;
};

#endif
