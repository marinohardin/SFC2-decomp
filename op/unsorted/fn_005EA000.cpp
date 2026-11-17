#include <algorithm>
#include "Common/Thread/Optex.h"

// HYPOTHESIS: a red-black tree keyed on std::pair<int, int>. Its layout differs from the
// compiler's own std::map (head at +4, nil node per instance at +8, multi flag at +0xc),
// so it is written as its own class; the element type past the key is not recovered.
class tTree_005ea14b {
public:
	// HYPOTHESIS: the stored value is the key pair followed by 16 bytes not yet recovered;
	// erase destroys it in place through 0x005e93fa.
	struct tValue {
		~tValue();
		std::pair<int, int> first;
		int m_08[4];
	};
	struct tNode {
		tNode *m_left;
		tNode *m_parent;
		tNode *m_right;
		tValue m_value;
		char m_color;
		char m_isnil;
	};
	class iterator {
	public:
		iterator() {}
		iterator(tNode *node) : m_node(node) {}
		void fn_005ea78b();
		iterator operator++(int)
			{iterator tmp = *this; fn_005ea7c6(); return tmp; }
		void fn_005ea7c6();
		tNode *m_node;
	};
	struct tPairib {
		tPairib(const iterator &f, bool s) : first(f), second(s) {}
		iterator first;
		bool second;
	};

	tPairib fn_005ea14b(const std::pair<int, int> &v);
	iterator fn_005ea63f(tNode *x, tNode *y, const std::pair<int, int> &v);
	iterator fn_005ea1fe(iterator p);
	void fn_005ea469(tNode *x);
	void fn_00556fd0(tNode *x);
	void fn_00558526(tNode *x);
	// HYPOTHESIS: leftmost and rightmost helpers inlined into erase; their own form (a
	// returned value, not a loop on a local) decides which node stays in a register.
	static tNode *Min(tNode *p)
		{while (!p->m_left->m_isnil)
			p = p->m_left;
		return p; }
	static tNode *Max(tNode *p)
		{while (!p->m_right->m_isnil)
			p = p->m_right;
		return p; }

	int m_00;
	tNode *m_head;
	tNode *m_nil;
	bool m_multi;
	int m_size;
};

// FUNCTION: STARFLEETOP 0x005ea14b
tTree_005ea14b::tPairib tTree_005ea14b::fn_005ea14b(const std::pair<int, int> &v)
{
	tNode *x = m_head->m_parent;
	tNode *y = m_head;
	bool ans = true;
	while (x != m_nil)
	{
		y = x;
		ans = v < x->m_value.first;
		x = ans ? x->m_left : x->m_right;
	}
	if (m_multi)
		return tPairib(fn_005ea63f(x, y, v), true);
	iterator p(y);
	if (!ans)
		;
	else if (p.m_node == m_head->m_left)
		return tPairib(fn_005ea63f(x, y, v), true);
	else
		p.fn_005ea78b();
	if (p.m_node->m_value.first < v)
		return tPairib(fn_005ea63f(x, y, v), true);
	return tPairib(p, false);
}

// FUNCTION: STARFLEETOP 0x005ea1fe
tTree_005ea14b::iterator tTree_005ea14b::fn_005ea1fe(iterator p)
{
	tNode *x;
	tNode *y = (p++).m_node;
	tNode *z = y;
	if (y->m_left == m_nil)
		x = y->m_right;
	else if (y->m_right == m_nil)
		x = y->m_left;
	else
		y = Min(y->m_right), x = y->m_right;
	if (y != z)
	{
		z->m_left->m_parent = y;
		y->m_left = z->m_left;
		if (y == z->m_right)
			x->m_parent = y;
		else
		{
			x->m_parent = y->m_parent;
			y->m_parent->m_left = x;
			y->m_right = z->m_right;
			z->m_right->m_parent = y;
		}
		if (m_head->m_parent == z)
			m_head->m_parent = y;
		else if (z->m_parent->m_left == z)
			z->m_parent->m_left = y;
		else
			z->m_parent->m_right = y;
		y->m_parent = z->m_parent;
		std::swap(y->m_color, z->m_color);
		y = z;
	}
	else
	{
		x->m_parent = y->m_parent;
		if (m_head->m_parent == z)
			m_head->m_parent = x;
		else if (z->m_parent->m_left == z)
			z->m_parent->m_left = x;
		else
			z->m_parent->m_right = x;
		if (m_head->m_left != z)
			;
		else if (z->m_right == m_nil)
			m_head->m_left = z->m_parent;
		else
			m_head->m_left = Min(x);
		if (m_head->m_right != z)
			;
		else if (z->m_left == m_nil)
			m_head->m_right = z->m_parent;
		else
			m_head->m_right = Max(x);
	}
	if (y->m_color == 1)
	{
		while (x != m_head->m_parent && x->m_color == 1)
			if (x == x->m_parent->m_left)
			{
				tNode *w = x->m_parent->m_right;
				if (w->m_color == 0)
				{
					w->m_color = 1;
					x->m_parent->m_color = 0;
					fn_00556fd0(x->m_parent);
					w = x->m_parent->m_right;
				}
				if (w->m_left->m_color == 1 && w->m_right->m_color == 1)
				{
					w->m_color = 0;
					x = x->m_parent;
				}
				else
				{
					if (w->m_right->m_color == 1)
					{
						w->m_left->m_color = 1;
						w->m_color = 0;
						fn_00558526(w);
						w = x->m_parent->m_right;
					}
					w->m_color = x->m_parent->m_color;
					x->m_parent->m_color = 1;
					w->m_right->m_color = 1;
					fn_00556fd0(x->m_parent);
					break;
				}
			}
			else
			{
				tNode *w = x->m_parent->m_left;
				if (w->m_color == 0)
				{
					w->m_color = 1;
					x->m_parent->m_color = 0;
					fn_00558526(x->m_parent);
					w = x->m_parent->m_left;
				}
				if (w->m_right->m_color == 1 && w->m_left->m_color == 1)
				{
					w->m_color = 0;
					x = x->m_parent;
				}
				else
				{
					if (w->m_left->m_color == 1)
					{
						w->m_right->m_color = 1;
						w->m_color = 0;
						fn_00556fd0(w);
						w = x->m_parent->m_left;
					}
					w->m_color = x->m_parent->m_color;
					x->m_parent->m_color = 1;
					w->m_left->m_color = 1;
					fn_00558526(x->m_parent);
					break;
				}
			}
		x->m_color = 1;
	}
	(&y->m_value)->~tValue();
	operator delete(y);
	--m_size;
	return p;
}

// FUNCTION: STARFLEETOP 0x005ea469
void tTree_005ea14b::fn_005ea469(tNode *x)
{
	for (tNode *y = x; y != m_nil; x = y)
	{
		fn_005ea469(y->m_right);
		y = y->m_left;
		(&x->m_value)->~tValue();
		operator delete(x);
	}
}

// HYPOTHESIS: the 0x14-byte value copied by 0x005b6973 is tHexPoint (declared only inside
// unsorted/tClientCharacter.cpp, so not redeclared here); its members are not recovered.
class fn_005eaa3f_point {
public:
	fn_005eaa3f_point( const fn_005eaa3f_point& other );
	~fn_005eaa3f_point();
	int m_00[5];
};

// HYPOTHESIS: a second red-black tree with the same layout as tTree_005ea14b, keyed on
// an int; the stored value (an int and a 0x14-byte point) is copied through 0x005eaa3f.
// Its rotations are the shared copies at 0x00556fd0 and 0x00558526.
class tTree_005ea4a1 {
public:
	struct tValue {
		int first;
		fn_005eaa3f_point m_04;
	};
	struct tNode {
		tNode *m_left;
		tNode *m_parent;
		tNode *m_right;
		tValue m_value;
		char m_color;
		char m_isnil;
	};
	class iterator {
	public:
		iterator(tNode *node) : m_node(node) {}
		tNode *m_node;
	};

	iterator fn_005ea4a1(tNode *x, tNode *y, const tValue &v);
	void fn_00556fd0(tNode *x);
	void fn_00558526(tNode *x);

	int m_00;
	tNode *m_head;
	tNode *m_nil;
	bool m_multi;
	int m_size;
};

// HYPOTHESIS: the library's construct helper, left out of line; copies v into p.
void fn_005eaa3f(tTree_005ea4a1::tValue *p, const tTree_005ea4a1::tValue &v);

// FUNCTION: STARFLEETOP 0x005ea4a1
tTree_005ea4a1::iterator tTree_005ea4a1::fn_005ea4a1(tNode *x, tNode *y, const tValue &v)
{
	tNode *z = (tNode *)operator new(sizeof (tNode));
	z->m_parent = y;
	z->m_color = 0;
	z->m_isnil = 0;
	z->m_left = m_nil;
	z->m_right = m_nil;
	fn_005eaa3f(&z->m_value, v);
	++m_size;
	if (y == m_head || x != m_nil || v.first < y->m_value.first)
	{
		y->m_left = z;
		if (y == m_head)
		{
			m_head->m_parent = z;
			m_head->m_right = z;
		}
		else if (y == m_head->m_left)
			m_head->m_left = z;
	}
	else
	{
		y->m_right = z;
		if (y == m_head->m_right)
			m_head->m_right = z;
	}
	for (x = z; x != m_head->m_parent && x->m_parent->m_color == 0; )
		if (x->m_parent == x->m_parent->m_parent->m_left)
		{
			y = x->m_parent->m_parent->m_right;
			if (y->m_color == 0)
			{
				x->m_parent->m_color = 1;
				y->m_color = 1;
				x->m_parent->m_parent->m_color = 0;
				x = x->m_parent->m_parent;
			}
			else
			{
				if (x == x->m_parent->m_right)
				{
					x = x->m_parent;
					fn_00556fd0(x);
				}
				x->m_parent->m_color = 1;
				x->m_parent->m_parent->m_color = 0;
				fn_00558526(x->m_parent->m_parent);
			}
		}
		else
		{
			y = x->m_parent->m_parent->m_left;
			if (y->m_color == 0)
			{
				x->m_parent->m_color = 1;
				y->m_color = 1;
				x->m_parent->m_parent->m_color = 0;
				x = x->m_parent->m_parent;
			}
			else
			{
				if (x == x->m_parent->m_left)
				{
					x = x->m_parent;
					fn_00558526(x);
				}
				x->m_parent->m_color = 1;
				x->m_parent->m_parent->m_color = 0;
				fn_00556fd0(x->m_parent->m_parent);
			}
		}
	m_head->m_parent->m_color = 1;
	return iterator(z);
}

// HYPOTHESIS: 0x005ea63e is one byte (pop esi), the tail of the routine at 0x005ea5e5; not a function of its own.
// FUNCTION: STARFLEETOP 0x005ea63e
void fn_005ea63e()
{
}

// HYPOTHESIS: a 0x10-byte member whose copy constructor (0x005e9013) is out of line; its type
// is not identified.
class fn_005e9013_member {
public:
	fn_005e9013_member( const fn_005e9013_member& other );
	int m_00[4];
};

// HYPOTHESIS: a third red-black tree with the layout of tTree_005ea4a1; the stored value (0x18 bytes, constructed through 0x005eaaa4) is two ints and a member, and its keys
// are ordered by the comparison at 0x0042c8f7.
class tTree_005ea63f {
public:
	struct tValue {
		int m_00[2];
		fn_005e9013_member m_08;
	};
	struct tNode {
		tNode *m_left;
		tNode *m_parent;
		tNode *m_right;
		tValue m_value;
		char m_color;
		char m_isnil;
	};
	class iterator {
	public:
		iterator(tNode *node) : m_node(node) {}
		tNode *m_node;
	};

	iterator fn_005ea63f(tNode *x, tNode *y, const tValue &v);
	void fn_00556fd0(tNode *x);
	void fn_00558526(tNode *x);

	int m_00;
	tNode *m_head;
	tNode *m_nil;
	bool m_multi;
	int m_size;
};

// HYPOTHESIS: the library's construct helper, left out of line; copies v into p.
void fn_005eaaa4(tTree_005ea63f::tValue *p, const tTree_005ea63f::tValue &v);
// HYPOTHESIS: the key comparison, true when a orders before b.
bool fn_0042c8f7(const tTree_005ea63f::tValue &a, const tTree_005ea63f::tValue &b);

// FUNCTION: STARFLEETOP 0x005ea63f
tTree_005ea63f::iterator tTree_005ea63f::fn_005ea63f(tNode *x, tNode *y, const tValue &v)
{
	tNode *z = (tNode *)operator new(sizeof (tNode));
	z->m_parent = y;
	z->m_color = 0;
	z->m_isnil = 0;
	z->m_left = m_nil;
	z->m_right = m_nil;
	fn_005eaaa4(&z->m_value, v);
	++m_size;
	if (y == m_head || x != m_nil || fn_0042c8f7(v, y->m_value))
	{
		y->m_left = z;
		if (y == m_head)
		{
			m_head->m_parent = z;
			m_head->m_right = z;
		}
		else if (y == m_head->m_left)
			m_head->m_left = z;
	}
	else
	{
		y->m_right = z;
		if (y == m_head->m_right)
			m_head->m_right = z;
	}
	for (x = z; x != m_head->m_parent && x->m_parent->m_color == 0; )
		if (x->m_parent == x->m_parent->m_parent->m_left)
		{
			y = x->m_parent->m_parent->m_right;
			if (y->m_color == 0)
			{
				x->m_parent->m_color = 1;
				y->m_color = 1;
				x->m_parent->m_parent->m_color = 0;
				x = x->m_parent->m_parent;
			}
			else
			{
				if (x == x->m_parent->m_right)
				{
					x = x->m_parent;
					fn_00556fd0(x);
				}
				x->m_parent->m_color = 1;
				x->m_parent->m_parent->m_color = 0;
				fn_00558526(x->m_parent->m_parent);
			}
		}
		else
		{
			y = x->m_parent->m_parent->m_left;
			if (y->m_color == 0)
			{
				x->m_parent->m_color = 1;
				y->m_color = 1;
				x->m_parent->m_parent->m_color = 0;
				x = x->m_parent->m_parent;
			}
			else
			{
				if (x == x->m_parent->m_left)
				{
					x = x->m_parent;
					fn_00558526(x);
				}
				x->m_parent->m_color = 1;
				x->m_parent->m_parent->m_color = 0;
				fn_00556fd0(x->m_parent->m_parent);
			}
		}
	m_head->m_parent->m_color = 1;
	return iterator(z);
}

// FUNCTION: STARFLEETOP 0x005ea78b
void tTree_005ea14b::iterator::fn_005ea78b()
{
	if (m_node->m_color == 0 && m_node->m_parent->m_parent == m_node)
		m_node = m_node->m_right;
	else if (!m_node->m_left->m_isnil)
		m_node = Max(m_node->m_left);
	else
	{
		tNode *p;
		while (m_node == (p = m_node->m_parent)->m_left)
			m_node = p;
		m_node = p;
	}
}

// FUNCTION: STARFLEETOP 0x005ea7c6
void tTree_005ea14b::iterator::fn_005ea7c6()
{
	if (!m_node->m_right->m_isnil)
		m_node = Min(m_node->m_right);
	else
	{
		tNode *p;
		while (m_node == (p = m_node->m_parent)->m_right)
			m_node = p;
		if (m_node->m_right != p)
			m_node = p;
	}
}

// HYPOTHESIS: by its layout (a pointer at +0, a tOptex at +4) and its body (Delete, then the
// optex) this is an out-of-line copy of the SDK's tSingleAccessWrapper<T>::~tSingleAccessWrapper,
// with 0x005ea827 as that instance's Delete. The template is declared only inside
// unsorted/tClientCharacter.cpp, so it is not declared a second time here.
// HYPOTHESIS: the pointee; only its non-virtual destructor (0x005e45a0) is known.
class tObject_005e45a0 {
public:
	~tObject_005e45a0();
};

class tObject_005ea7f8 {
public:
	~tObject_005ea7f8();
	void fn_005ea827();

	tObject_005e45a0 *m_00;
	mutable tOptex m_04;
};

// FUNCTION: STARFLEETOP 0x005ea7f8
tObject_005ea7f8::~tObject_005ea7f8()
{
	fn_005ea827();
}

// FUNCTION: STARFLEETOP 0x005ea827
void tObject_005ea7f8::fn_005ea827()
{
	m_04.Enter();
	if (m_00 != NULL) {
		delete m_00;
		m_00 = NULL;
	}
	m_04.Leave();
}

// HYPOTHESIS: a second out-of-line tSingleAccessWrapper<T>::~tSingleAccessWrapper (same layout and
// body as 0x005ea7f8), with 0x005ea887 as that instance's Delete.
// HYPOTHESIS: the pointee; only its non-virtual destructor (0x005e45d3) is known.
class tObject_005e45d3 {
public:
	~tObject_005e45d3();
};

class tObject_005ea858 {
public:
	~tObject_005ea858();
	void fn_005ea887();

	tObject_005e45d3 *m_00;
	mutable tOptex m_04;
};

// FUNCTION: STARFLEETOP 0x005ea858
tObject_005ea858::~tObject_005ea858()
{
	fn_005ea887();
}

// FUNCTION: STARFLEETOP 0x005ea887
void tObject_005ea858::fn_005ea887()
{
	m_04.Enter();
	if (m_00 != NULL) {
		delete m_00;
		m_00 = NULL;
	}
	m_04.Leave();
}

#include "Common/MemberCallback.h"
#include "unsorted/fn_0057a96e_info.h"

// HYPOTHESIS: tMetaClientMapPanel is not declared yet; the original saw the whole class, and
// its member pointer is one dword, so the class has single inheritance.
// fn_0057a96e_info stands in for nSwitch::tAddress::tStatusChangeInfo.
// MATCH-ONLY: __single_inheritance gives a forward declaration that member pointer form.
class __single_inheritance tMetaClientMapPanel;

// FUNCTION: STARFLEETOP 0x005ea8b8
template<>
void tMemberCallback<tMetaClientMapPanel,fn_0057a96e_info>::operator()( fn_0057a96e_info result )
{
	(fpMemberClass->*fpMemberFunction)( result );
}

// FUNCTION: STARFLEETOP 0x005ea918
template<>
void tMemberCallback<tMetaClientMapPanel,fn_0057a96e_info>::Callback( fn_0057a96e_info result )
{
	(fpMemberClass->*fpMemberFunction)( result );
}

// HYPOTHESIS: by its layout (a tOptex at +0) and its body (Done, then the optex) this is an
// out-of-line copy of the SDK's tAccessPointer<T, G>::~tAccessPointer, with 0x005f4352 as
// its Done. The template is declared only inside unsorted/tClientCharacter.cpp, so it is not
// declared a second time here.
class tObject_005ea978 {
public:
	~tObject_005ea978();
	void fn_005f4352();

	mutable tOptex m_00;
	void *m_04;
	const void *m_08;
};

// FUNCTION: STARFLEETOP 0x005ea978
tObject_005ea978::~tObject_005ea978()
{
	fn_005f4352();
}

// HYPOTHESIS: the same body as 0x005ea978 (same Done at 0x005f4352, same tOptex destructor),
// so the other of the const and non-const tAccessPointer instantiations.
class tObject_005ea9a3 {
public:
	~tObject_005ea9a3();
	void fn_005f4352();

	mutable tOptex m_00;
	const void *m_04;
	const void *m_08;
};

// FUNCTION: STARFLEETOP 0x005ea9a3
tObject_005ea9a3::~tObject_005ea9a3()
{
	fn_005f4352();
}

// HYPOTHESIS: a 0x64-byte element searched by 0x005ea9ce; its type is not identified.
struct fn_005ea9ce_elem {
	char m_00[0x64];
};

// HYPOTHESIS: the predicate handed by value to 0x005ea9ce, with its test at 0x005e4db9 and
// its destructor at 0x005e0d50 (in the metamap viewport unit); neither is identified yet.
class fn_005ea9ce_pred {
public:
	~fn_005ea9ce_pred();
	bool operator()( const fn_005ea9ce_elem& elem );

	void *m_00;
};

namespace std {

// HYPOTHESIS: 0x005ea9ce is the library's find_if held out of line for this predicate; it is
// written out here so the claim has a definition in this unit.
// FUNCTION: STARFLEETOP 0x005ea9ce
template<>
fn_005ea9ce_elem* find_if( fn_005ea9ce_elem* _F, fn_005ea9ce_elem* _L, fn_005ea9ce_pred _P )
{
	for (; _F != _L; ++_F)
		if (_P(*_F))
			break;
	return (_F);
}
}

// HYPOTHESIS: an element whose copy constructor (0x005e4cd5) is out of line; its type is not
// identified.
class fn_005eaa12_elem {
public:
	fn_005eaa12_elem( const fn_005eaa12_elem& other );
};

namespace std {

// HYPOTHESIS: 0x005eaa12 is the library's _Construct held out of line for this element; the
// placement new and its null test are the library's.
// FUNCTION: STARFLEETOP 0x005eaa12
template<>
void _Construct( fn_005eaa12_elem* _P, const fn_005eaa12_elem& _V )
{
	new ((void *)_P) fn_005eaa12_elem(_V);
}
}

// HYPOTHESIS: the library's construct helper held out of line for this value; the placement
// new and its null test are the library's.
// FUNCTION: STARFLEETOP 0x005eaa3f
void fn_005eaa3f(tTree_005ea4a1::tValue *p, const tTree_005ea4a1::tValue &v)
{
	new ((void *)p) tTree_005ea4a1::tValue(v);
}

#include "unsorted/tMetaMapNormalHex.h"

namespace std {

// HYPOTHESIS: the library's _Construct held out of line for tMetaMapNormalHex; the placement
// new and its null test are the library's.
// FUNCTION: STARFLEETOP 0x005eaa77
template<>
void _Construct( tMetaMapNormalHex* _P, const tMetaMapNormalHex& _V )
{
	new ((void *)_P) tMetaMapNormalHex(_V);
}
}

// FUNCTION: STARFLEETOP 0x005eaaa4
void fn_005eaaa4(tTree_005ea63f::tValue *p, const tTree_005ea63f::tValue &v)
{
	new ((void *)p) tTree_005ea63f::tValue(v);
}

template< typename tnResult >
inline tCallback<tnResult>::~tCallback()
{}

// HYPOTHESIS: the analysis names 0x005eaafe ~tCallback by the vtable it stores, but it is the
// deleting destructor's callee: this destructor with ~tCallback inlined, the first store dropped.
// FUNCTION: STARFLEETOP 0x005eaae2
// FUNCTION: STARFLEETOP 0x005eaafe
template<>
inline tMemberCallback<tMetaClientMapPanel,fn_0057a96e_info>::~tMemberCallback()
{}

// HYPOTHESIS: the original unit built this callback, which emitted its vtable and deleting
// destructor here; the constructor's use is not identified, so an explicit instantiation does it.
template class tMemberCallback<tMetaClientMapPanel,fn_0057a96e_info>;

// HYPOTHESIS: a 0x30-byte value, an int, two tHexPoints (the stand-in above) and an int, whose
// member-wise copy constructor is held out of line at 0x005eab05; its type is not identified.
class fn_005eab05_value {
public:
	fn_005eab05_value( const fn_005eab05_value& other );

	int m_00;
	fn_005eaa3f_point m_04;
	fn_005eaa3f_point m_18;
	int m_2c;
};

// FUNCTION: STARFLEETOP 0x005eab05
fn_005eab05_value::fn_005eab05_value( const fn_005eab05_value& other )
	: m_00(other.m_00), m_04(other.m_04), m_18(other.m_18), m_2c(other.m_2c)
{
}

#include <string>

// HYPOTHESIS: a std::string global at 0x00c391e8 holding the text "MetaMovementGauge".
// FUNCTION: STARFLEETOP 0x005eabbe
std::string g_00c391e8 = "MetaMovementGauge";
