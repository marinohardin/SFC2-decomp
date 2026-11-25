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
		bool color;
		bool isnil;
	};
	void fn_00425066(node* n);
	void fn_0042509e();
	void fn_004250ee(node* n);

	char fPad[4];
	node* fHead;
	node* fNil;
	bool fMulti;
	unsigned int fSize;
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

// FUNCTION: STARFLEETOP 0x0042509e
void fn_00425066_tree::fn_0042509e()
{
	node* n = (node*)operator new(sizeof(node));
	n->parent = 0;
	n->color = 1;
	n->isnil = 0;
	fNil = n;
	fNil->isnil = 1;
	fNil->left = 0;
	fNil->right = 0;
	node* sentinel = fNil;
	node* h = (node*)operator new(sizeof(node));
	h->parent = sentinel;
	h->color = 0;
	h->isnil = 0;
	fHead = h;
	h->left = h;
	fHead->right = fHead;
	fSize = 0;
}

// HYPOTHESIS: a tree like fn_00425066_tree whose node holds two plain words at +0xc.
struct fn_0042511e_tree
{
	struct node
	{
		node* left;
		node* parent;
		node* right;
		int mKey;
		int mValue;
		bool color;
		bool isnil;
	};
	void fn_0042511e();

	char fPad[4];
	node* fHead;
	node* fNil;
	bool fMulti;
	unsigned int fSize;
};

// FUNCTION: STARFLEETOP 0x004250ee
void fn_00425066_tree::fn_004250ee(node* n)
{
	for (node* y = n; n != fNil; n = y) {
		fn_004250ee(y->right);
		y = y->left;
		operator delete(n);
	}
}

// FUNCTION: STARFLEETOP 0x0042511e
void fn_0042511e_tree::fn_0042511e()
{
	node* n = (node*)operator new(sizeof(node));
	n->parent = 0;
	n->color = 1;
	n->isnil = 0;
	fNil = n;
	fNil->isnil = 1;
	fNil->left = 0;
	fNil->right = 0;
	node* sentinel = fNil;
	node* h = (node*)operator new(sizeof(node));
	h->parent = sentinel;
	h->color = 0;
	h->isnil = 0;
	fHead = h;
	h->left = h;
	fHead->right = fHead;
	fSize = 0;
}

// HYPOTHESIS: an iterator of a red-black tree (node: left +0, parent +4, right +8, isnil +0x39);
// this advances it to the in-order successor.
struct fn_004253c2_iterator
{
	struct node
	{
		node* left;
		node* parent;
		node* right;
		char fPad[0x2c];
		bool color;
		bool isnil;
	};
	void fn_004253c2();

	// MATCH-ONLY: helper the original inlined, it has no address
	static node* fn_Min(node* p)
	{
		while (!p->left->isnil)
			p = p->left;
		return p;
	}

	node* fNode;
};

// FUNCTION: STARFLEETOP 0x004253c2
void fn_004253c2_iterator::fn_004253c2()
{
	if (!fNode->right->isnil)
		fNode = fn_Min(fNode->right);
	else {
		node* p;
		while (fNode == (p = fNode->parent)->right)
			fNode = p;
		if (fNode->right != p)
			fNode = p;
	}
}
