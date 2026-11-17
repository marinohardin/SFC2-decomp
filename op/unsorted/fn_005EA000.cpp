#include <utility>

// HYPOTHESIS: a red-black tree keyed on std::pair<int, int>. Its layout differs from the
// compiler's own std::map (head at +4, nil node per instance at +8, multi flag at +0xc),
// so it is written as its own class; the element type past the key is not recovered.
class tTree_005ea14b {
public:
	struct tNode {
		tNode *m_left;
		tNode *m_parent;
		tNode *m_right;
		std::pair<int, int> m_value;
	};
	class iterator {
	public:
		iterator() {}
		iterator(tNode *node) : m_node(node) {}
		void fn_005ea78b();
		tNode *m_node;
	};
	struct tPairib {
		tPairib(const iterator &f, bool s) : first(f), second(s) {}
		iterator first;
		bool second;
	};

	tPairib fn_005ea14b(const std::pair<int, int> &v);
	iterator fn_005ea63f(tNode *x, tNode *y, const std::pair<int, int> &v);

	int m_00;
	tNode *m_head;
	tNode *m_nil;
	bool m_multi;
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
		ans = v < x->m_value;
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
	if (p.m_node->m_value < v)
		return tPairib(fn_005ea63f(x, y, v), true);
	return tPairib(p, false);
}
