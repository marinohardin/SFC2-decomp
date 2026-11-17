#ifndef UNSORTED_TOBJECT_0042ADE3_H
#define UNSORTED_TOBJECT_0042ADE3_H

#include <functional>
#include <memory>

// HYPOTHESIS: a 0x14-byte member type with an out-of-line destructor at 0x0042ade3.
// HYPOTHESIS: a tree-like container: the constructor copies an empty allocator object to +0
// and an empty comparison object to +1, clears the flag at +0xc and calls 0x0042baaf on
// itself. It is not the compiler's own std::map (that one keeps its flag at +8 and is 0x10
// bytes); the key and element types, and so the real allocator and comparison types, are not
// recovered. The destructor erases [*m_04, m_04) through 0x0042b15f, then frees the head
// node at +4 and the node at +8.
class tObject_0042ade3 {
public:
	tObject_0042ade3(const std::less<int> &pred = std::less<int>(), const std::allocator<int> &al = std::allocator<int>());

	~tObject_0042ade3();
	void fn_0042baaf();
	// HYPOTHESIS: an iterator holding one node pointer, as the compiler's own tree iterators do.
	class iterator {
	public:
		iterator(void *p) : m_00(p) {}
		void *m_00;
	};
	iterator fn_0042b15f(iterator first, iterator last);
	std::allocator<int> m_00;
	std::less<int> m_01;
	void **m_04;
	void *m_08;
	bool m_0c;
	int m_10;
};

inline tObject_0042ade3::tObject_0042ade3(const std::less<int> &pred, const std::allocator<int> &al)
	: m_00(al)
	, m_01(pred)
	, m_0c(false)
{
	fn_0042baaf();
}

#endif
