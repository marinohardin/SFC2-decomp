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
