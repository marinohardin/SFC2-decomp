#include "Tiger/tstring.h"
#include <new>
#include "unsorted/tEntityMgr.h"
#include "unsorted/tObject_008f5170.h"
#include "unsorted/fn_00571a58_owner.h"
#include "Tiger/tvectormath.h"

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
struct fn_004255b5_vector
{
	char fAlloc;
	unsigned int* fFirst;
	unsigned int* fLast;
	unsigned int* fEnd;

	void fn_00425ed6(unsigned int* first, unsigned int* last);
};
extern fn_004255b5_vector g_008f5188;

// FUNCTION: STARFLEETOP 0x00425463
void fn_00425463()
{
	char c;
	g_008f5188.fAlloc = c;
	g_008f5188.fFirst = 0;
	g_008f5188.fLast = 0;
	g_008f5188.fEnd = 0;
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
	if (id != 0) {
		node* p = fHead;
		while (p != 0) {
			if (p->mID == id) {
				p->mStamp = g_00c7c140;
				p->mValue = value;
				return;
			}
			p = p->mNext;
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
		int key = id;
		n->mID = key;
		n->mStamp = stamp;
		n->mValue = value;
		n->mNext = fHead;
		fHead = n;
	}
}

// HYPOTHESIS: a constructor of a 0x10-byte object owning a 0x100 by 0x100 table of 0x80000 bytes
// (0x10000 elements of 8 bytes, each built by the constructor at 0x005298a6).
struct fn_0042556b_element
{
	fn_0042556b_element() throw();
	char fPad[8];
};

struct fn_0042556b_table
{
	fn_0042556b_table() throw();

	fn_0042556b_element* fElements;
	int fWidth;
	int fHeight;
	int fDepth;
};

// FUNCTION: STARFLEETOP 0x0042556b
fn_0042556b_table::fn_0042556b_table() throw()
{
	fWidth = 0x100;
	fHeight = 0x100;
	fDepth = 0x80;
	fn_0042556b_element* e = (fn_0042556b_element*)operator new(0x80000);
	if (e != 0) {
		fn_0042556b_element* p = e;
		for (int i = 0x10000; i != 0; i--) {
			p->fn_0042556b_element::fn_0042556b_element();
			p++;
		}
	} else {
		e = 0;
	}
	fElements = e;
}

// HYPOTHESIS: the point built by the constructor at 0x00412f08 from the two floats of the query.
struct fn_00412f08_point
{
	fn_00412f08_point(const float* p) throw();
	fn_00412f08_point(const fn_00412f08_point& o) throw();
	float fX;
	float fY;
	float fZ;
};


extern void fn_00425f71(unsigned int* first, unsigned int* last, int v);
extern void fn_00425f08(fn_004255b5_vector* v, unsigned int* first, unsigned int* last);

// HYPOTHESIS: the static at 0x008f5184 is built by 0x0040ce21 from a string; its class is unknown.
struct fn_0040ce21_object
{
	fn_0040ce21_object(const char* s);
	~fn_0040ce21_object();
	int fPad;
};

// HYPOTHESIS: the class owning the query and the cell visitor; unknown.
struct fn_004255b5_class
{
	fn_004255b5_vector* fn_004255b5(float x, float y, int unused, float r);
	void fn_004259bd(float x, float y, unsigned int* col, unsigned int* row);
	void fn_004256b1(unsigned int col, unsigned int row, fn_00412f08_point p, float r);
};

// FUNCTION: STARFLEETOP 0x004255b5
fn_004255b5_vector* fn_004255b5_class::fn_004255b5(float x, float y, int unused, float r)
{
	static fn_0040ce21_object s("Round up the usual suspects?");
	g_008f5188.fn_00425ed6(g_008f5188.fFirst, g_008f5188.fLast);
	unsigned int col0, row0, col1, row1;
	float x0 = x - r;
	float y0 = y - r;
	this->fn_004259bd(x0, y0, &col0, &row0);
	float x1 = x + r;
	float y1 = y + r;
	this->fn_004259bd(x1, y1, &col1, &row1);
	for (unsigned int row = row0; row <= row1; row++) {
		for (unsigned int col = col0; col <= col1; col++) {
			this->fn_004256b1(col, row, fn_00412f08_point(&x), r);
		}
	}
	fn_00425f71(g_008f5188.fFirst, g_008f5188.fLast, 0);
	fn_00425f08(&g_008f5188, g_008f5188.fFirst, g_008f5188.fLast);
	return &g_008f5188;
}

// HYPOTHESIS: the object at 0x008e1060 is the entity manager.
extern tEntityMgr g_008e1060;

// FUNCTION: STARFLEETOP 0x004256b1
void fn_004255b5_class::fn_004256b1(unsigned int col, unsigned int row, fn_00412f08_point p, float r)
{
	static fn_0040ce21_object s("Clean it up Yummm!");
	// HYPOTHESIS: the list is a chain of (entity, next) nodes; seen as void** pairs here.
	void*** list = (void***)((tObject_008f5170*)this)->fn_00425726(col, row);
	if (list != 0) {
		for (void** n = *list; n != 0; n = (void**)n[1]) {
			if (g_008e1060.mGetDistanceFromEntityToPoint((tEntity*)n[0], *(const tPoint3*)&p) <= r)
				((fn_00571a58_owner*)&g_008f5188)->fn_00571a69((int)n);
		}
	}
}
