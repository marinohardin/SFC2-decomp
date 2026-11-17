// HYPOTHESIS: a red-black tree of the library's map/set: 0x2c-byte nodes with the value at
// +0x0c, a colour byte at +0x28 and an is-nil byte at +0x29; the head node sits at +0x08.
// HYPOTHESIS: the node's 0x1c-byte value; fn_0042b0f5 destroys it in place.
struct fn_0042c053_Value
{
	void fn_0042b0f5();

	char m00_data[0x1c];
};

struct fn_0042c053_Node
{
	fn_0042c053_Node* m00_left;
	fn_0042c053_Node* m04_parent;
	fn_0042c053_Node* m08_right;
	fn_0042c053_Value m0c_value;
	char m28_color;
	char m29_isnil;
};

// HYPOTHESIS: constructs a node value in place from another (copy construction).
void fn_0042c948(void* dest, const void* src);

class fn_0042c053_Tree
{
public:
	fn_0042c053_Node* fn_0042c053(fn_0042c053_Node* x, fn_0042c053_Node* parent);
	void fn_0042c0d4(fn_0042c053_Node* x);

	char m00_pad[0x8];
	fn_0042c053_Node* m08_head;
};

// FUNCTION: STARFLEETOP 0x0042c053
fn_0042c053_Node* fn_0042c053_Tree::fn_0042c053(fn_0042c053_Node* x, fn_0042c053_Node* parent)
{
	fn_0042c053_Node* root = m08_head;
	if (!x->m29_isnil)
	{
		char color = x->m28_color;
		fn_0042c053_Node* y = (fn_0042c053_Node*)operator new(sizeof(fn_0042c053_Node));
		y->m04_parent = parent;
		y->m29_isnil = 0;
		y->m28_color = color;
		fn_0042c948(&y->m0c_value, &x->m0c_value);
		y->m00_left = m08_head;
		y->m08_right = m08_head;
		if (root == m08_head)
			root = y;
		y->m00_left = fn_0042c053(x->m00_left, y);
		y->m08_right = fn_0042c053(x->m08_right, y);
	}
	return root;
}


// FUNCTION: STARFLEETOP 0x0042c0d4
void fn_0042c053_Tree::fn_0042c0d4(fn_0042c053_Node* x)
{
	for (fn_0042c053_Node* y = x; y != m08_head; x = y)
	{
		fn_0042c0d4(y->m08_right);
		y = y->m00_left;
		x->m0c_value.fn_0042b0f5();
		operator delete(x);
	}
}

// HYPOTHESIS: the 0x14-byte element of a library vector; it has a virtual destructor (the
// vector destroys through vtable slot 0) and its assignment is 0x005b69b0, the address
// unsorted/tHexPoint.h gives tHexPoint's operator=; that header has no virtual destructor, so the
// two are not yet reconciled.
class fn_0042c10c_Elem
{
public:
	virtual ~fn_0042c10c_Elem();
	fn_0042c10c_Elem& operator=(const fn_0042c10c_Elem& other);

	int m04;
	int m08;
	int m0c;
	int m10;
};

// HYPOTHESIS: constructs an element in place from another (the library's _Construct).
void fn_0042c9c5(fn_0042c10c_Elem* p, const fn_0042c10c_Elem& v);

// HYPOTHESIS: a library vector of 0x14-byte elements: allocator byte, then first, last, end.
class fn_0042c10c_Vector
{
public:
	void fn_0042c10c(fn_0042c10c_Elem* p, unsigned int m, const fn_0042c10c_Elem& x);
	unsigned int size() const;
	void _Destroy(fn_0042c10c_Elem* f, fn_0042c10c_Elem* l);
	fn_0042c10c_Elem* _Ucopy(const fn_0042c10c_Elem* f, const fn_0042c10c_Elem* l, fn_0042c10c_Elem* p);
	void _Ufill(fn_0042c10c_Elem* f, unsigned int n, const fn_0042c10c_Elem& x);

	char m00_allocator[0x4];
	fn_0042c10c_Elem* m04_first;
	fn_0042c10c_Elem* m08_last;
	fn_0042c10c_Elem* m0c_end;
};

// MATCH-ONLY: the library's inline helpers, written out for this hand-declared vector.
inline unsigned int fn_0042c10c_Vector::size() const
{
	return m04_first == 0 ? 0 : m08_last - m04_first;
}

inline void fn_0042c10c_Vector::_Destroy(fn_0042c10c_Elem* f, fn_0042c10c_Elem* l)
{
	for (; f != l; ++f)
		f->~fn_0042c10c_Elem();
}

inline fn_0042c10c_Elem* fn_0042c10c_Vector::_Ucopy(const fn_0042c10c_Elem* f, const fn_0042c10c_Elem* l, fn_0042c10c_Elem* p)
{
	for (; f != l; ++p, ++f)
		fn_0042c9c5(p, *f);
	return p;
}

inline void fn_0042c10c_Vector::_Ufill(fn_0042c10c_Elem* f, unsigned int n, const fn_0042c10c_Elem& x)
{
	for (; 0 < n; --n, ++f)
		fn_0042c9c5(f, x);
}

inline fn_0042c10c_Elem* fn_0042c10c_Allocate(int n, fn_0042c10c_Elem*)
{
	if (n < 0)
		n = 0;
	return (fn_0042c10c_Elem*)operator new((unsigned int)n * sizeof (fn_0042c10c_Elem));
}

inline void fn_0042c10c_fill(fn_0042c10c_Elem* f, fn_0042c10c_Elem* l, const fn_0042c10c_Elem& x)
{
	for (; f != l; ++f)
		*f = x;
}

inline fn_0042c10c_Elem* fn_0042c10c_copy_backward(fn_0042c10c_Elem* f, fn_0042c10c_Elem* l, fn_0042c10c_Elem* x)
{
	while (f != l)
		*--x = *--l;
	return x;
}

// FUNCTION: STARFLEETOP 0x0042c10c
void fn_0042c10c_Vector::fn_0042c10c(fn_0042c10c_Elem* p, unsigned int m, const fn_0042c10c_Elem& x)
{
	if ((unsigned int)(m0c_end - m08_last) < m)
	{
		unsigned int n = size() + (m < size() ? size() : m);
		fn_0042c10c_Elem* s = fn_0042c10c_Allocate(n, (fn_0042c10c_Elem*)0);
		fn_0042c10c_Elem* q = _Ucopy(m04_first, p, s);
		_Ufill(q, m, x);
		_Ucopy(p, m08_last, q + m);
		_Destroy(m04_first, m08_last);
		operator delete(m04_first);
		m0c_end = s + n;
		m08_last = s + size() + m;
		m04_first = s;
	}
	else if ((unsigned int)(m08_last - p) < m)
	{
		_Ucopy(p, m08_last, p + m);
		_Ufill(m08_last, m - (m08_last - p), x);
		fn_0042c10c_fill(p, m08_last, x);
		m08_last += m;
	}
	else if (0 < m)
	{
		_Ucopy(m08_last - m, m08_last, m08_last);
		fn_0042c10c_copy_backward(p, m08_last - m, m08_last);
		fn_0042c10c_fill(p, p + m, x);
		m08_last += m;
	}
}

// HYPOTHESIS: the 0x0c-byte value (key first) of another library map/set.
struct fn_0042c360_Value
{
	char m00_data[0xc];
};

// HYPOTHESIS: its 0x1c-byte red-black tree node: colour byte at +0x18 (red 0, black 1) and an
// is-nil byte at +0x19.
struct fn_0042c360_Node
{
	fn_0042c360_Node* m00_left;
	fn_0042c360_Node* m04_parent;
	fn_0042c360_Node* m08_right;
	fn_0042c360_Value m0c_value;
	char m18_color;
	char m19_isnil;
};

// HYPOTHESIS: constructs a node value in place from another (copy construction).
void fn_0042c9b1(void* dest, const fn_0042c360_Value& src);
// HYPOTHESIS: the tree's key_compare (key less-than).
bool fn_0042c8f7(const fn_0042c360_Value& left, const fn_0042c360_Value& right);

// HYPOTHESIS: the tree's iterator, returned by value.
struct fn_0042c360_Iterator
{
	fn_0042c360_Node* m00_node;
};

// HYPOTHESIS: the tree: head node at +0x04, nil node at +0x08, size at +0x10.
class fn_0042c360_Tree
{
public:
	fn_0042c360_Iterator fn_0042c360(fn_0042c360_Node* x, fn_0042c360_Node* y, const fn_0042c360_Value& v);
	void fn_00556fd0(fn_0042c360_Node* x);
	void fn_00558526(fn_0042c360_Node* x);

	char m00_pad[0x4];
	fn_0042c360_Node* m04_head;
	fn_0042c360_Node* m08_nil;
	char m0c_pad[0x4];
	unsigned int m10_size;
};

// FUNCTION: STARFLEETOP 0x0042c360
fn_0042c360_Iterator fn_0042c360_Tree::fn_0042c360(fn_0042c360_Node* x, fn_0042c360_Node* y, const fn_0042c360_Value& v)
{
	fn_0042c360_Node* z = (fn_0042c360_Node*)operator new(sizeof(fn_0042c360_Node));
	z->m18_color = 0;
	z->m19_isnil = 0;
	z->m04_parent = y;
	z->m00_left = m08_nil;
	z->m08_right = m08_nil;
	fn_0042c9b1(&z->m0c_value, v);
	++m10_size;
	if (y == m04_head || x != m08_nil || fn_0042c8f7(v, y->m0c_value))
	{
		y->m00_left = z;
		if (y == m04_head)
		{
			m04_head->m04_parent = z;
			m04_head->m08_right = z;
		}
		else if (y == m04_head->m00_left)
			m04_head->m00_left = z;
	}
	else
	{
		y->m08_right = z;
		if (y == m04_head->m08_right)
			m04_head->m08_right = z;
	}
	for (x = z; x != m04_head->m04_parent && x->m04_parent->m18_color == 0; )
		if (x->m04_parent == x->m04_parent->m04_parent->m00_left)
		{
			y = x->m04_parent->m04_parent->m08_right;
			if (y->m18_color == 0)
			{
				x->m04_parent->m18_color = 1;
				y->m18_color = 1;
				x->m04_parent->m04_parent->m18_color = 0;
				x = x->m04_parent->m04_parent;
			}
			else
			{
				if (x == x->m04_parent->m08_right)
				{
					x = x->m04_parent;
					fn_00556fd0(x);
				}
				x->m04_parent->m18_color = 1;
				x->m04_parent->m04_parent->m18_color = 0;
				fn_00558526(x->m04_parent->m04_parent);
			}
		}
		else
		{
			y = x->m04_parent->m04_parent->m00_left;
			if (y->m18_color == 0)
			{
				x->m04_parent->m18_color = 1;
				y->m18_color = 1;
				x->m04_parent->m04_parent->m18_color = 0;
				x = x->m04_parent->m04_parent;
			}
			else
			{
				if (x == x->m04_parent->m00_left)
				{
					x = x->m04_parent;
					fn_00558526(x);
				}
				x->m04_parent->m18_color = 1;
				x->m04_parent->m04_parent->m18_color = 0;
				fn_00556fd0(x->m04_parent->m04_parent);
			}
		}
	m04_head->m04_parent->m18_color = 1;
	fn_0042c360_Iterator it;
	it.m00_node = z;
	return it;
}

// HYPOTHESIS: the key of another library map/set: a pair of ints compared as std::pair's
// operator< (first, then second), inlined into the tree's lower bound.
struct fn_0042c4ac_Key
{
	int first;
	int second;
};

// MATCH-ONLY: stands for the inlined key_compare; the original's name is not known.
inline bool operator<(const fn_0042c4ac_Key& x, const fn_0042c4ac_Key& y)
{
	return x.first < y.first || !(y.first < x.first) && x.second < y.second;
}

// HYPOTHESIS: the tree's 0x1c-byte node, key at +0x0c.
struct fn_0042c4ac_Node
{
	fn_0042c4ac_Node* m00_left;
	fn_0042c4ac_Node* m04_parent;
	fn_0042c4ac_Node* m08_right;
	fn_0042c4ac_Key m0c_key;
	int m14_mapped;
	char m18_color;
	char m19_isnil;
};

// HYPOTHESIS: the tree: head node at +0x04, nil node at +0x08, size at +0x10.
class fn_0042c4ac_Tree
{
public:
	fn_0042c4ac_Node* fn_0042c4ac(const fn_0042c4ac_Key& kv) const;

	char m00_pad[0x4];
	fn_0042c4ac_Node* m04_head;
	fn_0042c4ac_Node* m08_nil;
	char m0c_pad[0x4];
	unsigned int m10_size;
};

// HYPOTHESIS: the tree's lower bound (first node whose key is not less than kv).
// FUNCTION: STARFLEETOP 0x0042c4ac
fn_0042c4ac_Node* fn_0042c4ac_Tree::fn_0042c4ac(const fn_0042c4ac_Key& kv) const
{
	fn_0042c4ac_Node* x = m04_head->m04_parent;
	fn_0042c4ac_Node* y = m04_head;
	while (x != m08_nil)
		if (x->m0c_key < kv)
			x = x->m08_right;
		else
			y = x, x = x->m00_left;
	return y;
}

// HYPOTHESIS: the key_compare of the tree of fn_0042c053_Node; the linker folded it with the
// identical one of fn_0042c360_Tree at 0x0042c8f7.
bool fn_0042c8f7(const fn_0042c053_Value& left, const fn_0042c053_Value& right);

// HYPOTHESIS: the iterator of the tree of fn_0042c053_Node, returned by value.
struct fn_0042c4e6_Iterator
{
	fn_0042c053_Node* m00_node;
};

// HYPOTHESIS: the tree of fn_0042c053_Node seen from its insert: head node at +0x04, nil node
// at +0x08, size at +0x10; its rotations folded with fn_0042c360_Tree's.
class fn_0042c4e6_Tree
{
public:
	fn_0042c4e6_Iterator fn_0042c4e6(fn_0042c053_Node* x, fn_0042c053_Node* y, const fn_0042c053_Value& v);
	void fn_00556fd0(fn_0042c053_Node* x);
	void fn_00558526(fn_0042c053_Node* x);

	char m00_pad[0x4];
	fn_0042c053_Node* m04_head;
	fn_0042c053_Node* m08_nil;
	char m0c_pad[0x4];
	unsigned int m10_size;
};

// HYPOTHESIS: the tree's _Insert.
// FUNCTION: STARFLEETOP 0x0042c4e6
fn_0042c4e6_Iterator fn_0042c4e6_Tree::fn_0042c4e6(fn_0042c053_Node* x, fn_0042c053_Node* y, const fn_0042c053_Value& v)
{
	fn_0042c053_Node* z = (fn_0042c053_Node*)operator new(sizeof(fn_0042c053_Node));
	z->m28_color = 0;
	z->m29_isnil = 0;
	z->m04_parent = y;
	z->m00_left = m08_nil;
	z->m08_right = m08_nil;
	fn_0042c948(&z->m0c_value, &v);
	++m10_size;
	if (y == m04_head || x != m08_nil || fn_0042c8f7(v, y->m0c_value))
	{
		y->m00_left = z;
		if (y == m04_head)
		{
			m04_head->m04_parent = z;
			m04_head->m08_right = z;
		}
		else if (y == m04_head->m00_left)
			m04_head->m00_left = z;
	}
	else
	{
		y->m08_right = z;
		if (y == m04_head->m08_right)
			m04_head->m08_right = z;
	}
	for (x = z; x != m04_head->m04_parent && x->m04_parent->m28_color == 0; )
		if (x->m04_parent == x->m04_parent->m04_parent->m00_left)
		{
			y = x->m04_parent->m04_parent->m08_right;
			if (y->m28_color == 0)
			{
				x->m04_parent->m28_color = 1;
				y->m28_color = 1;
				x->m04_parent->m04_parent->m28_color = 0;
				x = x->m04_parent->m04_parent;
			}
			else
			{
				if (x == x->m04_parent->m08_right)
				{
					x = x->m04_parent;
					fn_00556fd0(x);
				}
				x->m04_parent->m28_color = 1;
				x->m04_parent->m04_parent->m28_color = 0;
				fn_00558526(x->m04_parent->m04_parent);
			}
		}
		else
		{
			y = x->m04_parent->m04_parent->m00_left;
			if (y->m28_color == 0)
			{
				x->m04_parent->m28_color = 1;
				y->m28_color = 1;
				x->m04_parent->m04_parent->m28_color = 0;
				x = x->m04_parent->m04_parent;
			}
			else
			{
				if (x == x->m04_parent->m00_left)
				{
					x = x->m04_parent;
					fn_00558526(x);
				}
				x->m04_parent->m28_color = 1;
				x->m04_parent->m04_parent->m28_color = 0;
				fn_00556fd0(x->m04_parent->m04_parent);
			}
		}
	m04_head->m04_parent->m28_color = 1;
	fn_0042c4e6_Iterator it;
	it.m00_node = z;
	return it;
}

// HYPOTHESIS: the iterator of a library map/set whose 0x1c-byte node has the layout of
// fn_0042c360_Node (colour at +0x18, is-nil at +0x19); which tree it belongs to is not known.
class fn_0042c632_Iterator
{
public:
	void fn_0042c632();
	void fn_0042c66d();

	fn_0042c360_Node* m00_node;
};

// HYPOTHESIS: the iterator's decrement (_Dec).
// FUNCTION: STARFLEETOP 0x0042c632
void fn_0042c632_Iterator::fn_0042c632()
{
	if (m00_node->m18_color == 0 && m00_node->m04_parent->m04_parent == m00_node)
		m00_node = m00_node->m08_right;
	else if (!m00_node->m00_left->m19_isnil)
	{
		fn_0042c360_Node* p = m00_node->m00_left;
		while (!p->m08_right->m19_isnil)
			p = p->m08_right;
		m00_node = p;
	}
	else
	{
		fn_0042c360_Node* p;
		while (m00_node == (p = m00_node->m04_parent)->m00_left)
			m00_node = p;
		m00_node = p;
	}
}

// HYPOTHESIS: the iterator's increment (_Inc).
// FUNCTION: STARFLEETOP 0x0042c66d
void fn_0042c632_Iterator::fn_0042c66d()
{
	if (!m00_node->m08_right->m19_isnil)
	{
		fn_0042c360_Node* p = m00_node->m08_right;
		while (!p->m00_left->m19_isnil)
			p = p->m00_left;
		m00_node = p;
	}
	else
	{
		fn_0042c360_Node* p;
		while (m00_node == (p = m00_node->m04_parent)->m08_right)
			m00_node = p;
		if (m00_node->m08_right != p)
			m00_node = p;
	}
}

// HYPOTHESIS: the iterator of the tree of fn_0042c053_Node (colour at +0x28, is-nil at +0x29).
class fn_0042c69f_Iterator
{
public:
	void fn_0042c69f();

	fn_0042c053_Node* m00_node;
};

// HYPOTHESIS: the iterator's decrement (_Dec).
// FUNCTION: STARFLEETOP 0x0042c69f
void fn_0042c69f_Iterator::fn_0042c69f()
{
	if (m00_node->m28_color == 0 && m00_node->m04_parent->m04_parent == m00_node)
		m00_node = m00_node->m08_right;
	else if (!m00_node->m00_left->m29_isnil)
	{
		fn_0042c053_Node* p = m00_node->m00_left;
		while (!p->m08_right->m29_isnil)
			p = p->m08_right;
		m00_node = p;
	}
	else
	{
		fn_0042c053_Node* p;
		while (m00_node == (p = m00_node->m04_parent)->m00_left)
			m00_node = p;
		m00_node = p;
	}
}

// The game's out-of-line copy of the SDK's inline destructor for the wrapper whose Delete is
// 0x0042cc05 (the member at +0xb4 of tClientCharacter).
// HYPOTHESIS: the wrapped type; the element type of the vector is not recovered.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit; the SDK
// defines it once in the header. The original stores no -1 state before the member's
// ~tOptex; this build does.
#include <vector>
#include "Common/Thread/SingleAccessWrapper.h"

// FUNCTION: STARFLEETOP 0x0042c6da
template<> tSingleAccessWrapper<std::vector<int> >::~tSingleAccessWrapper()
{
	Delete();
}

// FUNCTION: STARFLEETOP 0x0042c709
template<> tSingleAccessWrapper<std::vector<int> >::iPtr tSingleAccessWrapper<std::vector<int> >::Lock()
{
	fPointerOptex.Enter();
	return iPtr( fpPointer, this );
}

// The game's out-of-line copy of the SDK's inline destructor for the wrapper whose Delete is
// 0x005ff84a (the member at +0xcc of tClientCharacter).
// HYPOTHESIS: tObject_005391b0 is declared only inside unsorted/tClientCharacter.cpp; it is
// forward-declared here rather than declared a second time.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c6da.
class tObject_005391b0;

// FUNCTION: STARFLEETOP 0x0042c72f
template<> tSingleAccessWrapper<tObject_005391b0>::~tSingleAccessWrapper()
{
	Delete();
}

// The game's out-of-line copy of the SDK's inline tAccessPointer destructor.
// HYPOTHESIS: the instantiation is the pointer 0x0042c709 returns; Done (0x005f4352) is shared
// through identical code folding, so the call does not tell the instantiations apart.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c6da.
// FUNCTION: STARFLEETOP 0x0042c75e
template<> tAccessPointer<std::vector<int>, tSingleAccessWrapper<std::vector<int> > >::~tAccessPointer()
{
	Done();
}

// The game's out-of-line copy of the SDK's inline tAccessPointer destructor.
// HYPOTHESIS: the instantiation is the pointer of the wrapper of 0x0042c72f, by its place after
// 0x0042c75e; Done (0x005f4352) is shared through identical code folding.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c6da.
// FUNCTION: STARFLEETOP 0x0042c789
template<> tAccessPointer<tObject_005391b0, tSingleAccessWrapper<tObject_005391b0> >::~tAccessPointer()
{
	Done();
}

// The game's out-of-line copy of the SDK's inline tAccessPointer destructor.
// HYPOTHESIS: the instantiation is the const pointer of the vector wrapper, which
// tObject_00543551::fn_005434f7 returns; Done (0x005f4352) is shared through identical code folding.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c6da.
// FUNCTION: STARFLEETOP 0x0042c7b4
template<> tAccessPointer<const std::vector<int>, tSingleAccessWrapper<std::vector<int> > >::~tAccessPointer()
{
	Done();
}

// The game's out-of-line copy of the SDK's inline tAccessPointer destructor.
// HYPOTHESIS: the instantiation is the const pointer of the wrapper of 0x0042c72f, by its place
// after 0x0042c7b4; Done (0x005f4352) is shared through identical code folding.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c6da.
// FUNCTION: STARFLEETOP 0x0042c7df
template<> tAccessPointer<const tObject_005391b0, tSingleAccessWrapper<tObject_005391b0> >::~tAccessPointer()
{
	Done();
}

#include "Common/DataStore/DataStoreOutput.h"
#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"
#include "unsorted/tServCharacter.h"
template<> void std::vector< tServCharacter::tShipCache >::reserve( size_type _N );
namespace nDataStore
{
// The SDK's stream operator for a vector, declared as a friend of tOutput in its header and
// defined in DataStoreOutput.cpp.
template< class tType >
tOutput& operator<<( tOutput& output, const std::vector< tType >& value );

// The game's instantiation for the ship caches of tServCharacter.
// HYPOTHESIS: the element is tServCharacter::tShipCache, the only 0x4c-byte streamable known;
// the call to the tStreamable operator does not tell the element type apart.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c6da.
// FUNCTION: STARFLEETOP 0x0042c80a
template<> tOutput& operator<<( tOutput& output, const std::vector< tServCharacter::tShipCache >& value )
{
	int32u size = value.size();
	output.Write( (const byte*)&size, sizeof( size ) );

	for( std::vector< tServCharacter::tShipCache >::const_iterator i = value.begin();
		 i != value.end();
		 i++ )
	{
		output << *i;
	}

	return output;
}

// The SDK's stream operator for a vector, defined in DataStoreInput.cpp.
template< class tType >
tInput& operator>>( tInput& input, std::vector< tType >& value );

// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c80a.
// FUNCTION: STARFLEETOP 0x0042c863
template<> tInput& operator>>( tInput& input, std::vector< tServCharacter::tShipCache >& value )
{
	value.clear();

	int32u size = 0;
	input.Read( (byte*)&size, sizeof( size ) );

	value.reserve( size );

	for( int i = 0; i < size; i++ )
	{
		tServCharacter::tShipCache type;
		input >> type;
		value.push_back( type );
	}

	return input;
}

};

// HYPOTHESIS: the key of fn_0042c360_Value is a pair of ints compared as std::pair's operator<
// (first, then second).
// FUNCTION: STARFLEETOP 0x0042c8f7
bool fn_0042c8f7(const fn_0042c360_Value& left, const fn_0042c360_Value& right)
{
	const int* x = (const int*)left.m00_data;
	const int* y = (const int*)right.m00_data;
	return x[0] < y[0] || !(y[0] < x[0]) && x[1] < y[1];
}

// The game's out-of-line copy of the library's inline _Construct (<xmemory>) for the ship caches
// of tServCharacter, which vector<tShipCache> uses to copy an element into place.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c80a.
namespace std
{
// FUNCTION: STARFLEETOP 0x0042c91b
template<> void _Construct( tServCharacter::tShipCache* _P, const tServCharacter::tShipCache& _V )
{
	new ((void*)_P) tServCharacter::tShipCache(_V);
}
};

#include <utility>
#include "unsorted/tHexPoint.h"

// HYPOTHESIS: the library's _Construct for the 0x1c-byte value of fn_0042c053_Tree, taken to be
// the std::map< std::pair< int, int >, tHexPoint > that fn_0042a480_object::FUN_0042ac56 returns:
// two ints copied, then tHexPoint's copy constructor at +8.
// FUNCTION: STARFLEETOP 0x0042c948
void fn_0042c948(void* dest, const void* src)
{
	typedef std::pair< const std::pair< int, int >, tHexPoint > tValue;
	new (dest) tValue(*(const tValue*)src);
}

#include "Common/Database/DatabaseID.h"

// The game's out-of-line copy of the library's inline _Construct (<xmemory>) for a database ID:
// tID's implicit copy constructor, inlined.
// HYPOTHESIS: used by a std::vector< nDatabase::tID >; the caller is not identified yet.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c91b.
namespace std
{
// FUNCTION: STARFLEETOP 0x0042c986
template<> void _Construct( nDatabase::tID* _P, const nDatabase::tID& _V )
{
	new ((void*)_P) nDatabase::tID(_V);
}
};

// HYPOTHESIS: the library's _Construct for a 12-byte element copied as three dwords (the implicit
// copy constructor of a three-member struct); the element type and its caller are not identified.
// FUNCTION: STARFLEETOP 0x0042c9b1
void fn_0042c9b1(void* dest, const void* src)
{
	struct tValue { int m00; int m04; int m08; };
	new (dest) tValue(*(const tValue*)src);
}

// HYPOTHESIS: the library's _Construct for a tHexPoint element (its copy constructor at
// 0x005b6973); the container and its caller are not identified.
// FUNCTION: STARFLEETOP 0x0042c9c5
void fn_0042c9c5(tHexPoint* dest, const tHexPoint& src)
{
	new ((void*)dest) tHexPoint(src);
}

// HYPOTHESIS: tShipCache's implicit assignment operator, out of line for the copies in
// vector< tServCharacter::tShipCache >; written memberwise as the compiler generates it.
// FUNCTION: STARFLEETOP 0x0042c9f2
tServCharacter::tShipCache& tServCharacter::tShipCache::operator=( const tShipCache& other )
{
	(nDataStore::tStreamable&)*this = other;
	field_0c = other.field_0c;
	field_1c = other.field_1c;
	field_20 = other.field_20;
	field_24 = other.field_24;
	field_34 = other.field_34;
	field_44 = other.field_44;
	field_48 = other.field_48;
	return *this;
}

// HYPOTHESIS: tShipCache's implicit copy constructor, out of line for the copies in
// vector< tServCharacter::tShipCache >; written memberwise as the compiler generates it.
// FUNCTION: STARFLEETOP 0x0042ca5f
tServCharacter::tShipCache::tShipCache( const tShipCache& other )
	: nDataStore::tStreamable( other ),
	field_0c( other.field_0c ),
	field_1c( other.field_1c ),
	field_20( other.field_20 ),
	field_24( other.field_24 ),
	field_34( other.field_34 ),
	field_44( other.field_44 ),
	field_48( other.field_48 )
{
}

// The game's out-of-line copy of the library's vector::reserve (<vector>) for the ship caches of
// tServCharacter, called by the stream operator 0x0042c863.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c91b.
// FUNCTION: STARFLEETOP 0x0042cb45
template<> void std::vector< tServCharacter::tShipCache >::reserve( size_type _N )
{
	if (capacity() < _N)
		{iterator _S = allocator.allocate(_N, (void *)0);
		_Ucopy(_First, _Last, _S);
		_Destroy(_First, _Last);
		allocator.deallocate(_First, _End - _First);
		_End = _S + _N;
		_Last = _S + size();
		_First = _S; }
}

// The game's out-of-line copy of the SDK's Delete for the wrapper of 0x0042c6da.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c6da.
// FUNCTION: STARFLEETOP 0x0042cc05
template<> void tSingleAccessWrapper<std::vector<int> >::Delete()
{
	fPointerOptex.Enter();
	if( fpPointer )
	{
		delete fpPointer;
		fpPointer = NULL;
	}
	fPointerOptex.Leave();
}

#include <string>

// HYPOTHESIS: a global std::string at 0x008f5330 initialised from the literal at 0x0083e2a8; its
// users and name are not identified.
// FUNCTION: STARFLEETOP 0x0042cc94
// FUNCTION: STARFLEETOP 0x0042cc61
std::string g_008f5330 = "@";

// HYPOTHESIS: a global std::string at 0x008f5318 initialised from the literal at 0x0083e2ac; its
// users and name are not identified.
// FUNCTION: STARFLEETOP 0x0042ccfa
// FUNCTION: STARFLEETOP 0x0042ccc7
std::string g_008f5318 = "@@";

#include <fstream>

// HYPOTHESIS: the SDK's settings class (GetValue at 0x006f75d0 by SDK library match); declared
// here with the one member this unit uses until a shared header exists.
class tSettings
{
public:
	void GetValue( const std::string& name, std::string* value );
};

// HYPOTHESIS: returns the game's settings object; name and owner not identified.
tSettings* fn_005d481f();

// HYPOTHESIS: layout only as far as this unit reads it (a 0x168-byte object, RTTI names it, a
// string at +0x48); constructed from a stream at 0x005486ff.
class tCampaignInfo
{
public:
	tCampaignInfo( std::istream& in );
	virtual ~tCampaignInfo();

	char m04[0x44];
	std::string m48;
	char m58[0x110];
};

// HYPOTHESIS: an unidentified class whose first member is a string, filled from the campaign file.
class fn_0042cd23_Class
{
public:
	fn_0042cd23_Class();

	std::string m00;
};

// FUNCTION: STARFLEETOP 0x0042cd23
fn_0042cd23_Class::fn_0042cd23_Class()
{
	std::string path;
	fn_005d481f()->GetValue( "Assets/Paths/Campaigns", &path );
	std::string file;
	fn_005d481f()->GetValue( "ServerSetup/Settings/CampaignFile", &file );
	path += file;
	std::ifstream in( path.c_str(), std::ios::in | std::ios::binary );
	tCampaignInfo info( in );
	m00 = info.m48;
}
