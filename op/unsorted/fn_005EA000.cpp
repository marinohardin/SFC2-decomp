#include <algorithm>

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

// HYPOTHESIS: a second red-black tree with the same layout as tTree_005ea14b, keyed on
// an int; the stored value (0x18 bytes, copied through 0x005eaa3f) is not recovered.
// Its rotations are the shared copies at 0x00556fd0 and 0x00558526.
class tTree_005ea4a1 {
public:
	struct tValue {
		int first;
		int m_04[5];
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

// HYPOTHESIS: a third red-black tree with the layout of tTree_005ea4a1; the stored value (0x18 bytes, constructed through 0x005eaaa4) is not recovered, and its keys
// are ordered by the comparison at 0x0042c8f7.
class tTree_005ea63f {
public:
	struct tValue {
		int m_00[6];
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
