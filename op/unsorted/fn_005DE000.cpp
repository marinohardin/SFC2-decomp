#include "unsorted/tNewsStory.h"

extern char g_b00c38458;    // "ASCIICharacterSet"

// HYPOTHESIS: an ordering predicate on news stories: by detail time, then by text. It takes
// no this and pops its two arguments, so it is written as __stdcall.
// FUNCTION: STARFLEETOP 0x005de0cf
bool __stdcall fn_005de0cf( tNewsStory* a, tNewsStory* b )
{
	bool less;
	if ( a->fn_0054745f().fn_00546986( b->fn_0054745f() ) )
	{
		int ascii = g_b00c38458 != 0;
		fn_0040651e_type* text = b->fn_00547450( ascii );
		less = text->fn_0040651e( a->fn_00547450( ascii ) ) < 0;
	}
	else
	{
		char earlier = a->fn_0054745f().fn_0054695e( b->fn_0054745f() );
		return earlier ? true : false;
	}
	return less;
}

// HYPOTHESIS: a second ordering predicate on news stories: by the byte fn_0054744c returns
// (higher first), then by text.
// FUNCTION: STARFLEETOP 0x005de1b8
bool __stdcall fn_005de1b8( tNewsStory* a, tNewsStory* b )
{
	bool less;
	int ascii = g_b00c38458 != 0;
	if ( a->fn_0054744c() == b->fn_0054744c() )
	{
		fn_0040651e_type* text = b->fn_00547450( ascii );
		less = text->fn_0040651e( a->fn_00547450( ascii ) ) < 0;
	}
	else
	{
		less = a->fn_0054744c() > b->fn_0054744c();
	}
	return less;
}

// HYPOTHESIS: a third ordering predicate on news stories: by the byte fn_00547448 returns
// (higher first), then by text.
// FUNCTION: STARFLEETOP 0x005de222
bool __stdcall fn_005de222( tNewsStory* a, tNewsStory* b )
{
	bool less;
	int ascii = g_b00c38458 != 0;
	if ( a->fn_00547448() == b->fn_00547448() )
	{
		fn_0040651e_type* text = b->fn_00547450( ascii );
		less = text->fn_0040651e( a->fn_00547450( ascii ) ) < 0;
	}
	else
	{
		less = a->fn_00547448() > b->fn_00547448();
	}
	return less;
}

#include <new>

// HYPOTHESIS: a library vector of pointers (allocator byte, then first, last, end); the element
// type is taken as the news stories the predicates above order. The game's std::vector insert
// is defined in the class and so cannot be claimed here; the class is declared by hand.
class fn_005de28c_Vector
{
public:
	void fn_005de28c(tNewsStory** p, tNewsStory* const* f, tNewsStory* const* l);
	unsigned int size() const;
	void _Destroy(tNewsStory** f, tNewsStory** l);
	tNewsStory** _Ucopy(tNewsStory* const* f, tNewsStory* const* l, tNewsStory** p);

	char m00_allocator[0x4];
	tNewsStory** m04_first;
	tNewsStory** m08_last;
	tNewsStory** m0c_end;
};

// MATCH-ONLY: the library's inline helpers, written out for this hand-declared vector.
inline unsigned int fn_005de28c_Vector::size() const
{
	return m04_first == 0 ? 0 : m08_last - m04_first;
}

inline void fn_005de28c_Vector::_Destroy(tNewsStory** f, tNewsStory** l)
{
	for (; f != l; ++f)
		;
}

inline tNewsStory** fn_005de28c_Vector::_Ucopy(tNewsStory* const* f, tNewsStory* const* l, tNewsStory** p)
{
	for (; f != l; ++p, ++f)
		new ((void*)p) tNewsStory*(*f);
	return p;
}

inline tNewsStory** fn_005de28c_Allocate(int n, tNewsStory**)
{
	if (n < 0)
		n = 0;
	return (tNewsStory**)operator new((unsigned int)n * sizeof (tNewsStory*));
}

inline tNewsStory** fn_005de28c_copy(tNewsStory* const* f, tNewsStory* const* l, tNewsStory** x)
{
	for (; f != l; ++x, ++f)
		*x = *f;
	return x;
}

inline tNewsStory** fn_005de28c_copy_backward(tNewsStory** f, tNewsStory** l, tNewsStory** x)
{
	while (f != l)
		*--x = *--l;
	return x;
}

// FUNCTION: STARFLEETOP 0x005de28c
void fn_005de28c_Vector::fn_005de28c(tNewsStory** p, tNewsStory* const* f, tNewsStory* const* l)
{
	unsigned int m = l - f;
	if ((unsigned int)(m0c_end - m08_last) < m)
	{
		unsigned int n = size() + (m < size() ? size() : m);
		tNewsStory** s = fn_005de28c_Allocate(n, (tNewsStory**)0);
		tNewsStory** q = _Ucopy(m04_first, p, s);
		q = _Ucopy(f, l, q);
		_Ucopy(p, m08_last, q);
		_Destroy(m04_first, m08_last);
		operator delete(m04_first);
		m0c_end = s + n;
		m08_last = s + size() + m;
		m04_first = s;
	}
	else if ((unsigned int)(m08_last - p) < m)
	{
		_Ucopy(p, m08_last, p + m);
		_Ucopy(f + (m08_last - p), l, m08_last);
		fn_005de28c_copy(f, f + (m08_last - p), p);
		m08_last += m;
	}
	else if (0 < m)
	{
		_Ucopy(m08_last - m, m08_last, m08_last);
		fn_005de28c_copy_backward(p, m08_last - m, m08_last);
		fn_005de28c_copy(f, l, p);
		m08_last += m;
	}
}
