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
	~fn_0042556b_table() throw();

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
extern unsigned int* fn_00425f08(fn_004255b5_vector* v, unsigned int* first, unsigned int* last);
// HYPOTHESIS: std::unique over the unsigned ints of the vector; 0x005bbd03 is the helper that
// takes the count of the kept elements and the address of a null pointer.
extern void fn_005bbd03(int n, unsigned int** p);

// HYPOTHESIS: the static at 0x008f5184 is built by 0x0040ce21 from a string; its class is unknown.
struct fn_0040ce21_object
{
	fn_0040ce21_object(const char* s);
	~fn_0040ce21_object();
	int fPad;
};

// HYPOTHESIS: a plain three-word value (no constructor) passed by value.
struct fn_00425d5f_words
{
	int a;
	int b;
	int c;
	fn_00425d5f_words(const fn_00425d5f_words& o) { a = o.a; b = o.b; c = o.c; }
};

// HYPOTHESIS: the class owning the query and the cell visitor; unknown.
struct fn_004255b5_class
{
	fn_004255b5_vector* fn_004255b5(float x, float y, int unused, float r);
	void fn_004259bd(float x, float y, unsigned int* col, unsigned int* row);
	void fn_004256b1(unsigned int col, unsigned int row, fn_00412f08_point p, float r);
	void fn_0042574b(int unused0, int unused1, float x, float y, float r, unsigned int* col0, unsigned int* col1, unsigned int* row0, unsigned int* row1);
	void fn_004257b4(tEntity* pEntity);
	void fn_00425824(tEntity* pEntity, int value);
	void fn_00425928(tPoint3 aPoint, float aRadius, tEntity* pEntity, int kind, int value);
	char fn_00425a18(tEntity* a, tEntity* b, char c);
	// HYPOTHESIS: the callee at 0x00425aad takes the callback at 0x004260fa, both positions and both entities.
	char fn_00425aad(void (*callback)(), tPoint3 a, tPoint3 b, tEntity* pA, tEntity* pB, int flag);
	char fn_00425d5f(char (__fastcall *callback)(tEntity*), int col, int row, tPoint3 a, tPoint3 b, float r, fn_00425d5f_words c, tEntity* pA, tEntity* pB);
	char fn_00425def(tEntity* e, tPoint3 a, tPoint3 b, float r, fn_00425d5f_words c);
};

extern void fn_004260fa();

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

// FUNCTION: STARFLEETOP 0x00425726
tObject_00425726_list *tObject_008f5170::fn_00425726(int32 column, int32 row)
{
	// HYPOTHESIS: the object's layout is not declared in its header yet; the fields are
	// read through this: +0 cells (8 bytes each), +4 width, +8 height (the row stride).
	int *self = (int *)this;
	if ((unsigned int)column < (unsigned int)self[1] && (unsigned int)row < (unsigned int)self[2])
		return (tObject_00425726_list *)(self[0] + (self[2] * row + column) * 8);
	return 0;
}

// FUNCTION: STARFLEETOP 0x0042574b
void fn_004255b5_class::fn_0042574b(int unused0, int unused1, float x, float y, float r, unsigned int* col0, unsigned int* col1, unsigned int* row0, unsigned int* row1)
{
	this->fn_004259bd(x - r, y - r, col0, row0);
	this->fn_004259bd(x + r, y + r, col1, row1);
	if (*col0 > *col1)
		*col0 = 0;
	if (*row0 > *row1)
		*row0 = 0;
}

// FUNCTION: STARFLEETOP 0x004257b4
void fn_004255b5_class::fn_004257b4(tEntity* pEntity)
{
	// HYPOTHESIS: fn_00425824 belongs to the cell object like fn_004258de.
	if (pEntity != 0) {
		this->fn_00425824(pEntity, 0);
		((tObject_008f5170*)this)->fn_004258de(pEntity, 0, 0);
		pEntity->fLastMapPos = pEntity->fCurrPos;
		pEntity->fMapRadius = pEntity->vfunction14();
	}
}

// FUNCTION: STARFLEETOP 0x004257f5
void tObject_008f5170::fn_004257f5(tEntity *pEntity, int32 value)
{
	((fn_004255b5_class*)this)->fn_00425824(pEntity, value);
	pEntity->fLastMapPos = pEntity->fCurrPos;
	pEntity->fMapRadius = pEntity->vfunction14();
}

// FUNCTION: STARFLEETOP 0x00425824
void fn_004255b5_class::fn_00425824(tEntity* pEntity, int value)
{
	if (pEntity != 0) {
		float radius = pEntity->vfunction14();
		float x = pEntity->fCurrPos.x;
		float y = pEntity->fCurrPos.y;
		unsigned int col, row;
		this->fn_004259bd(x, y, &col, &row);
		unsigned int col0, col1, row0, row1;
		this->fn_0042574b(col, row, x, y, radius, &col0, &col1, &row0, &row1);
		for (unsigned int c = col0; c <= col1; c++) {
			for (unsigned int r = row0; r <= row1; r++) {
				fn_004254a7_list* list = (fn_004254a7_list*)((tObject_008f5170*)this)->fn_00425726(c, r);
				if (list == 0)
					return;
				list->fn_00425501((int)pEntity, value);
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x004258de
void tObject_008f5170::fn_004258de(tEntity *pEntity, int32 kind, int32 value)
{
	if (pEntity != 0) {
		// HYPOTHESIS: the original copied the three words through a plain local before the call.
		struct { int a; int b; int c; } pos;
		pos.a = *(int *)&pEntity->fLastMapPos.x;
		pos.b = *(int *)&pEntity->fLastMapPos.y;
		pos.c = *(int *)&pEntity->fLastMapPos.z;
		((fn_004255b5_class*)this)->fn_00425928(*(tPoint3 *)&pos, pEntity->fMapRadius, pEntity, kind, value);
	}
}

// FUNCTION: STARFLEETOP 0x00425928
void fn_004255b5_class::fn_00425928(tPoint3 aPoint, float aRadius, tEntity* pEntity, int kind, int value)
{
	unsigned int col, row;
	this->fn_004259bd(aPoint.x, aPoint.y, &col, &row);
	unsigned int col0, col1, row0, row1;
	this->fn_0042574b(col, row, aPoint.x, aPoint.y, aRadius, &col0, &col1, &row0, &row1);
	for (unsigned int c = col0; c <= col1; c++) {
		for (unsigned int r = row0; r <= row1; r++) {
			fn_004254a7_list* list = (fn_004254a7_list*)((tObject_008f5170*)this)->fn_00425726(c, r);
			if (list != 0)
				list->fn_004254a7((int)pEntity, kind, value);
		}
	}
}

// FUNCTION: STARFLEETOP 0x004259bd
void fn_004255b5_class::fn_004259bd(float x, float y, unsigned int* col, unsigned int* row)
{
	// HYPOTHESIS: +0xc holds the cell offset; adding the magic constant leaves the rounded value in the low word.
	x = x * 0.01f + 12582912.0f;
	y = y * 0.01f;
	*col = *(short*)&x + *(int*)((char*)this + 0xc);
	y += 12582912.0f;
	*row = *(short*)&y + *(int*)((char*)this + 0xc);
	if (*col >= 0x100)
		*col = 0xff;
	if (*row >= 0x100)
		*row = 0xff;
}

// FUNCTION: STARFLEETOP 0x00425a18
char fn_004255b5_class::fn_00425a18(tEntity* a, tEntity* b, char c)
{
	if (a == 0 || b == 0)
		return 0;
	if (a == b)
		return 1;
	if (c == 0) {
		if (a->fTeam == b->fTeam)
			return 1;
		if (b->vfunction9())
			return 1;
	}
	tPoint3 posA = a->fCurrPos;
	tPoint3 posB = b->fCurrPos;
	return fn_00425aad(fn_004260fa, posA, posB, a, b, 0);
}

// FUNCTION: STARFLEETOP 0x00425d5f
char fn_004255b5_class::fn_00425d5f(char (__fastcall *callback)(tEntity*), int col, int row, tPoint3 a, tPoint3 b, float r, fn_00425d5f_words c, tEntity* pA, tEntity* pB)
{
	char ok = 1;
	// HYPOTHESIS: the list is a chain of (entity, next) nodes; seen as void** pairs, as in fn_004256b1.
	void*** list = (void***)((tObject_008f5170*)this)->fn_00425726(col, row);
	if (list != 0) {
		for (void** n = *list; n != 0 && n[0] != 0; n = (void**)n[1]) {
			if (n[0] != pA && n[0] != pB && callback((tEntity*)n[0])) {
				ok = fn_00425def((tEntity*)n[0], a, b, r, c);
				if (ok == 0)
					return 0;
			}
		}
	}
	return ok;
}

// FUNCTION: STARFLEETOP 0x00425def
char fn_004255b5_class::fn_00425def(tEntity* e, tPoint3 a, tPoint3 b, float r, fn_00425d5f_words c)
{
	float radius = e->vfunction14();
	// HYPOTHESIS: the three-word locals are the original's copies of the returned points.
	fn_00425d5f_words d = *(fn_00425d5f_words*)&(const tPoint3&)(e->fCurrPos - *(tPoint3*)&c);
	float t = *(tPoint3*)&d * b;
	if (t < 0.0f || t > r)
		return 1;
	tPoint3 p;
	{
		tPoint3 scaled = b * t;
		p = scaled;
	}
	fn_00425d5f_words q = *(fn_00425d5f_words*)&(const tPoint3&)(p - *(tPoint3*)&d);
	if (*(tPoint3*)&q * *(tPoint3*)&q < radius * radius)
		return 0;
	return 1;
}

// HYPOTHESIS: the 10-byte function is the dynamic initializer of a global of this table type.
// FUNCTION: STARFLEETOP 0x00425ec0
// FUNCTION: STARFLEETOP 0x00425eb6
fn_0042556b_table g_fn_0042556b_table;

// HYPOTHESIS: the destructor at 0x00425ed6 releases the element array.
fn_0042556b_table::~fn_0042556b_table() throw()
{
	operator delete(fElements);
}

// MATCH-ONLY: the search for the first adjacent equal pair, written as the library's helper.
inline unsigned int* fn_00425f08_adjacent_find(unsigned int* first, unsigned int* last)
{
	if (first == last)
		return last;
	unsigned int* next = first;
	while (++next != last) {
		if (*first == *next)
			return first;
		first = next;
	}
	return last;
}

// FUNCTION: STARFLEETOP 0x00425f08
unsigned int* fn_00425f08(fn_004255b5_vector* v, unsigned int* first, unsigned int* last)
{
	unsigned int* a = first;
	if (first != last) {
		unsigned int* next = a;
		while (++next != last) {
			if (*a == *next)
				goto found;
			a = next;
		}
	}
	a = last;
found:
	if (a != last) {
		unsigned int* b = a + 1;
		unsigned int* out = b;
		for (; b != last; b++) {
			if (*a != *b) {
				*out = *b;
				a = out++;
			}
		}
		unsigned int* p = 0;
		fn_005bbd03(out - first, &p);
	}
	return v->fFirst;
}
