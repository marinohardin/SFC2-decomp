#ifndef FN_005EA9CE_PRED_H
#define FN_005EA9CE_PRED_H

// HYPOTHESIS: a 0x64-byte element searched by 0x005ea9ce; its type is not identified. The map
// panel (0x005e62c4) keeps a vector of them behind a single-access wrapper at +0x15c.
struct fn_005ea9ce_elem {
	char m_00[0x64];
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
