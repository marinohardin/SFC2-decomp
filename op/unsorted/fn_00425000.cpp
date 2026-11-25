#include "Tiger/tstring.h"

// HYPOTHESIS: the value a node holds at +0xc, with a tString first. Its implicit destructor
// is the out-of-line copy at 0x004bd853 that jumps to tString::~tString; the second member's
// type is not known.
struct fn_00425066_value
{
	tString mKey;
	int mValue;
};

// HYPOTHESIS: the string-keyed tree of fn_00424e71_map (nil node at +8); this frees the nodes below
// a subtree, recursing on the right link and walking the left one.
struct fn_00425066_tree
{
	struct node
	{
		node* left;
		node* parent;
		node* right;
		fn_00425066_value value;
	};
	void fn_00425066(node* n);

	char fPad[4];
	node* fHead;
	node* fNil;
};

// FUNCTION: STARFLEETOP 0x00425066
void fn_00425066_tree::fn_00425066(node* n)
{
	for (node* y = n; n != fNil; n = y) {
		fn_00425066(y->right);
		y = y->left;
		n->value.~fn_00425066_value();
		operator delete(n);
	}
}
