//	fn_005bc3de_tree.h: the balanced tree whose constructor is 0x005bc3de.
// Evidence:  calls (original name and file unknown)

#ifndef fn_005bc3de_tree_h
#define fn_005bc3de_tree_h 1

// HYPOTHESIS: another balanced tree laid out like fn_005bc161_tree (unsorted/fn_005BC000.cpp),
// with an empty allocator at +0x00, the key compare at +0x01 and _Multi at +0x0c; 0x005bc3de is
// its _Tree(_Parg, _Marg, _Al) constructor. Identical-COMDAT folding leaves that one constructor
// for every tree of this layout, so the trees of tMetaMapHex.h construct themselves through it.
struct fn_005bc3de_tree
{
	struct node;
	struct _Pr
	{
	};
	struct _A
	{
	};

	fn_005bc3de_tree(const _Pr &_Parg, bool _Marg, const _A &_Al);
	// HYPOTHESIS: _Init().
	void FUN_0042511e();

	_A allocator;
	_Pr key_compare;
	node *_Head;
	node *_Nil;
	bool _Multi;
	unsigned int _Size;
};

#endif // fn_005bc3de_tree_h
