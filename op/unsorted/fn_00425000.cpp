#include "Tiger/tstring.h"
#include <new>

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

// MATCH-ONLY: the library's _Construct for one element, written out so that it has source lines
// here; not inline, since nothing in this unit instantiates it.
namespace std
{
// FUNCTION: STARFLEETOP 0x004253f4
inline void _Construct(tString* _P, const tString& _V)
{
	new ((void*)_P) tString(_V);
}
}

// MATCH-ONLY: takes the address of _Construct so that the unit emits its out-of-line copy.
void (*g_fn_004253f4_construct)(tString*, const tString&) = std::_Construct;

// HYPOTHESIS: globals are a char and three longs/pointers; the char store reads an uninitialized local, as the original does.
extern char g_008f5188;
extern long g_008f518c;
extern long g_008f5190;
extern long g_008f5194;

// FUNCTION: STARFLEETOP 0x00425463
void fn_00425463()
{
	char c;
	g_008f5188 = c;
	g_008f518c = 0;
	g_008f5190 = 0;
	g_008f5194 = 0;
}

// HYPOTHESIS: the atexit call for the global set up by fn_00425463; the destructor at 0x0042548c is not yet identified.
extern void fn_0042548c();

// FUNCTION: STARFLEETOP 0x00425480
void fn_00425480()
{
	atexit(fn_0042548c);
}

// HYPOTHESIS: a singly linked list of nodes (id +0, next +4, two words at +8 and +0xc); this
// removes the node with the given id unless it is unchanged and no force was asked for.
struct fn_004254a7_list
{
	struct node
	{
		int mID;
		node* mNext;
		int mValue;
		int mStamp;
	};
	void fn_004254a7(int id, int value, char force);
	void fn_00425501(int id, int value);

	node* fHead;
};

extern int g_00c7c140;

// FUNCTION: STARFLEETOP 0x004254a7
void fn_004254a7_list::fn_004254a7(int id, int value, char force)
{
	node* p = fHead;
	node* prev = 0;
	for (;;) {
		if (p == 0)
			return;
		if (p->mID == id)
			break;
		node* next = p->mNext;
		prev = p;
		p = next;
	}
	if (force == 0 && value != 2 && p->mStamp == g_00c7c140 && p->mValue == value)
		return;
	if (prev != 0)
		prev->mNext = p->mNext;
	else
		fHead = p->mNext;
	p->mNext = 0;
	operator delete(p);
}

// FUNCTION: STARFLEETOP 0x00425501
void fn_004254a7_list::fn_00425501(int id, int value)
{
	if (id == 0)
		return;
	node* p = fHead;
	for (; p != 0; p = p->mNext) {
		if (p->mID == id) {
			p->mStamp = g_00c7c140;
			p->mValue = value;
			return;
		}
	}
	node* n = (node*)operator new(0x10);
	if (n != 0) {
		n->mID = 0;
		n->mNext = 0;
		n->mValue = -1;
		n->mStamp = 0;
	} else {
		n = 0;
	}
	int stamp = g_00c7c140;
	n->mID = id;
	n->mStamp = stamp;
	n->mValue = value;
	n->mNext = fHead;
	fHead = n;
}
