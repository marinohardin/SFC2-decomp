#ifndef TMETAMAPNORMALHEX_H
#define TMETAMAPNORMALHEX_H

#include "port/types.h"
#include "unsorted/tMetaMapHex.h"

// HYPOTHESIS: the base class whose copy constructor is 0x005e4d1c; it is 0x4c bytes with a
// vtable at +0, its name and members are not recovered.
class fn_005e4d1c_base {
public:
	fn_005e4d1c_base( const fn_005e4d1c_base& other );
	virtual ~fn_005e4d1c_base();
	char m_04[0x48];
};

// HYPOTHESIS: the class named by the RTTI string .?AVtMetaMapNormalHex@@; only its copy
// constructor (0x005e6d7c) is declared so far, the rest of its layout is not recovered.
class tMetaMapNormalHex : public fn_005e4d1c_base {
public:
	tMetaMapNormalHex( const tMetaMapNormalHex& other );
	virtual ~tMetaMapNormalHex();
	short m_4c;
	short m_4e;
	// HYPOTHESIS: a tree of fn_0053a6cc_map's layout that is not destroyed with the object.
	struct tTreeCopy {
		// MATCH-ONLY: the original expanded this copy inline in the constructor's initializers.
		__forceinline tTreeCopy( const fn_0053a6cc_map::tree* src )
		{
			fn_0053a6cc_map::tree* tree = (fn_0053a6cc_map::tree*)m_tree;
			tree->m_alloc = src->m_alloc;
			tree->m_comp = src->m_comp;
			tree->m_multi = src->m_multi;
			tree->fn_0042511e();
			tree->fn_0053a981( *src );
		}
		char m_tree[0x14];
	};
	tTreeCopy m_50;
};

#endif
