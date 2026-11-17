#include "unsorted/tNewsStory.h"

extern char g_b00c38458;    // "ASCIICharacterSet"

// HYPOTHESIS: an ordering predicate on news stories: by detail time, then by text. It pops
// its two arguments and ignores this; std::sort (0x005de905) calls it as a function object.
struct fn_005de0cf_Pred
{
	bool operator()( tNewsStory* a, tNewsStory* b ) const;
};
// FUNCTION: STARFLEETOP 0x005de0cf
bool fn_005de0cf_Pred::operator()( tNewsStory* a, tNewsStory* b ) const
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
// (higher first), then by text. Like 0x005de0cf it ignores this; std::sort (0x005de987)
// calls it as a function object.
struct fn_005de1b8_Pred
{
	bool operator()( tNewsStory* a, tNewsStory* b ) const;
};
// FUNCTION: STARFLEETOP 0x005de1b8
bool fn_005de1b8_Pred::operator()( tNewsStory* a, tNewsStory* b ) const
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
// (higher first), then by text. Like 0x005de0cf it ignores this; std::sort (0x005dea09)
// calls it as a function object.
struct fn_005de222_Pred
{
	bool operator()( tNewsStory* a, tNewsStory* b ) const;
};
// FUNCTION: STARFLEETOP 0x005de222
bool fn_005de222_Pred::operator()( tNewsStory* a, tNewsStory* b ) const
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
	void fn_005de471(tNewsStory** p, unsigned int m, tNewsStory* const& x);
	unsigned int size() const;
	void _Destroy(tNewsStory** f, tNewsStory** l);
	tNewsStory** _Ucopy(tNewsStory* const* f, tNewsStory* const* l, tNewsStory** p);
	void _Ufill(tNewsStory** f, unsigned int n, tNewsStory* const& x);

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

// MATCH-ONLY: the library's inline fill helpers, written out for this hand-declared vector.
inline void fn_005de28c_Vector::_Ufill(tNewsStory** f, unsigned int n, tNewsStory* const& x)
{
	for (; 0 < n; --n, ++f)
		new ((void*)f) tNewsStory*(x);
}

inline void fn_005de471_fill(tNewsStory** f, tNewsStory** l, tNewsStory* const& x)
{
	for (; f != l; ++f)
		*f = x;
}

// HYPOTHESIS: the same vector's insert of m copies of x before p.
// FUNCTION: STARFLEETOP 0x005de471
void fn_005de28c_Vector::fn_005de471(tNewsStory** p, unsigned int m, tNewsStory* const& x)
{
	if ((unsigned int)(m0c_end - m08_last) < m)
	{
		unsigned int n = size() + (m < size() ? size() : m);
		tNewsStory** s = fn_005de28c_Allocate(n, (tNewsStory**)0);
		tNewsStory** q = _Ucopy(m04_first, p, s);
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
		fn_005de471_fill(p, m08_last, x);
		m08_last += m;
	}
	else if (0 < m)
	{
		_Ucopy(m08_last - m, m08_last, m08_last);
		fn_005de28c_copy_backward(p, m08_last - m, m08_last);
		fn_005de471_fill(p, p + m, x);
		m08_last += m;
	}
}

#include "Common/MemberCallback.h"
#include "unsorted/tMetaClientNewsCtlrC.h"

// MATCH-ONLY: the SDK's template CreateCallback, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.
// FUNCTION: STARFLEETOP 0x005de641
template<>
tMemberCallback< tMetaClientNewsCtlrC, tNewsStory* > CreateCallback( tMetaClientNewsCtlrC* object, void (tMetaClientNewsCtlrC::*func)( tNewsStory* result ) )
{
	return 	tMemberCallback< tMetaClientNewsCtlrC, tNewsStory* >( object, func );
}

#include "Common/Callback.h"

// MATCH-ONLY: the SDK's template destructor, written out as this instance's inline explicit
// specialization so that its annotation names the one instance. The out-of-line copy is the
// one the deleting destructor and for_each's unwinding call; its body (storing the base
// vtable) is the same as ~tCallback's, which the analysis names at this address.
// FUNCTION: STARFLEETOP 0x005de78d
// FUNCTION: STARFLEETOP 0x005de770
// FUNCTION: STARFLEETOP 0x005de660
template<>
inline tMemberCallback< tMetaClientNewsCtlrC, tNewsStory* >::~tMemberCallback()
{}

#include <algorithm>

// MATCH-ONLY: the library's template for_each, written out as this instance's inline explicit
// specialization so that its annotation names the one instance; the body is the template's.
namespace std {
// FUNCTION: STARFLEETOP 0x005de81b
template<>
inline tMemberCallback< tMetaClientNewsCtlrC, tNewsStory* > for_each( tNewsStory** _F, tNewsStory** _L, tMemberCallback< tMetaClientNewsCtlrC, tNewsStory* > _Op )
{
	for (; _F != _L; ++_F)
		_Op(*_F);
	return (_Op);
}
// FUNCTION: STARFLEETOP 0x005de869
template<>
inline tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem::eType > for_each( tMetaClientNewsCtlrC::tMenuItem::eType* _F, tMetaClientNewsCtlrC::tMenuItem::eType* _L, tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem::eType > _Op )
{
	for (; _F != _L; ++_F)
		_Op(*_F);
	return (_Op);
}
// FUNCTION: STARFLEETOP 0x005de8b7
template<>
inline tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem* > for_each( tMetaClientNewsCtlrC::tMenuItem** _F, tMetaClientNewsCtlrC::tMenuItem** _L, tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem* > _Op )
{
	for (; _F != _L; ++_F)
		_Op(*_F);
	return (_Op);
}
}

// HYPOTHESIS: applies a member callback to each story of a [first, last) pair of pointers.
// FUNCTION: STARFLEETOP 0x005de667
void fn_005de667( tNewsStory** range[2], const tMemberCallback< tMetaClientNewsCtlrC, tNewsStory* >& callback )
{
	std::for_each( range[0], range[1], callback );
}

// MATCH-ONLY: the SDK's template CreateCallback, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.
// FUNCTION: STARFLEETOP 0x005de6a6
template<>
tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem::eType > CreateCallback( tMetaClientNewsCtlrC* object, void (tMetaClientNewsCtlrC::*func)( tMetaClientNewsCtlrC::tMenuItem::eType result ) )
{
	return 	tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem::eType >( object, func );
}

// MATCH-ONLY: the SDK's template destructor, written out as this instance's inline explicit
// specialization so that its annotation names the one instance. The out-of-line copy is the
// one the deleting destructor calls; its body (storing the base vtable) is the same as
// ~tCallback's, which the analysis names at this address.
// HYPOTHESIS: 0x005de7a9 is tCallback's own deleting destructor (~tCallback inlined); it sits
// here, as 0x005de770 does above, until a template destructor in a header can claim it.
// FUNCTION: STARFLEETOP 0x005de7c6
// FUNCTION: STARFLEETOP 0x005de7a9
// FUNCTION: STARFLEETOP 0x005de6c5
template<>
inline tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem::eType >::~tMemberCallback()
{}

// HYPOTHESIS: applies a member callback to each menu item type of a [first, last) pair of
// pointers.
// FUNCTION: STARFLEETOP 0x005de6cc
void fn_005de6cc( tMetaClientNewsCtlrC::tMenuItem::eType* range[2], const tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem::eType >& callback )
{
	std::for_each( range[0], range[1], callback );
}

// MATCH-ONLY: the SDK's template CreateCallback, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.
// FUNCTION: STARFLEETOP 0x005de70b
template<>
tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem* > CreateCallback( tMetaClientNewsCtlrC* object, void (tMetaClientNewsCtlrC::*func)( tMetaClientNewsCtlrC::tMenuItem* result ) )
{
	return 	tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem* >( object, func );
}

// MATCH-ONLY: the SDK's template destructor, written out as this instance's inline explicit
// specialization so that its annotation names the one instance. The out-of-line copy is the
// one the deleting destructor calls; its body (storing the base vtable) is the same as
// ~tCallback's, which the analysis names at this address.
// FUNCTION: STARFLEETOP 0x005de7ff
// FUNCTION: STARFLEETOP 0x005de72a
template<>
inline tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem* >::~tMemberCallback()
{}

// HYPOTHESIS: applies a member callback to each menu item of a [first, last) pair of pointers.
// FUNCTION: STARFLEETOP 0x005de731
void fn_005de731( tMetaClientNewsCtlrC::tMenuItem** range[2], const tMemberCallback< tMetaClientNewsCtlrC, tMetaClientNewsCtlrC::tMenuItem* >& callback )
{
	std::for_each( range[0], range[1], callback );
}

// MATCH-ONLY: the library's template _Sort_0 (std::sort's body), written out as this
// instance's explicit specialization so that its annotation names the one instance; the body
// is the template's.
namespace std {
template<>
void _Sort( tNewsStory** _F, tNewsStory** _L, fn_005de0cf_Pred _P, tNewsStory** );
template<>
void _Sort( tNewsStory** _F, tNewsStory** _L, fn_005de1b8_Pred _P, tNewsStory** );
template<>
void _Sort( tNewsStory** _F, tNewsStory** _L, fn_005de222_Pred _P, tNewsStory** );
template<>
void _Insertion_sort_1( tNewsStory** _F, tNewsStory** _L, fn_005de0cf_Pred _P, tNewsStory** );
template<>
void _Insertion_sort_1( tNewsStory** _F, tNewsStory** _L, fn_005de1b8_Pred _P, tNewsStory** );
template<>
tNewsStory* _Median( tNewsStory* _X, tNewsStory* _Y, tNewsStory* _Z, fn_005de0cf_Pred _P );
template<>
tNewsStory* _Median( tNewsStory* _X, tNewsStory* _Y, tNewsStory* _Z, fn_005de1b8_Pred _P );
template<>
tNewsStory** _Unguarded_partition( tNewsStory** _F, tNewsStory** _L, tNewsStory* _Piv, fn_005de0cf_Pred _P );
template<>
tNewsStory** _Unguarded_partition( tNewsStory** _F, tNewsStory** _L, tNewsStory* _Piv, fn_005de1b8_Pred _P );
// FUNCTION: STARFLEETOP 0x005de905
template<>
void _Sort_0( tNewsStory** _F, tNewsStory** _L, fn_005de0cf_Pred _P, tNewsStory** )
{
	if (_L - _F <= _SORT_MAX)
		_Insertion_sort(_F, _L, _P);
	else
		{_Sort(_F, _L, _P, (tNewsStory**)0);
		_Insertion_sort(_F, _F + _SORT_MAX, _P);
		for (_F += _SORT_MAX; _F != _L; ++_F)
			_Unguarded_insert(_F, *_F, _P); }
}
// FUNCTION: STARFLEETOP 0x005de987
template<>
void _Sort_0( tNewsStory** _F, tNewsStory** _L, fn_005de1b8_Pred _P, tNewsStory** )
{
	if (_L - _F <= _SORT_MAX)
		_Insertion_sort(_F, _L, _P);
	else
		{_Sort(_F, _L, _P, (tNewsStory**)0);
		_Insertion_sort(_F, _F + _SORT_MAX, _P);
		for (_F += _SORT_MAX; _F != _L; ++_F)
			_Unguarded_insert(_F, *_F, _P); }
}
// FUNCTION: STARFLEETOP 0x005dea09
template<>
void _Sort_0( tNewsStory** _F, tNewsStory** _L, fn_005de222_Pred _P, tNewsStory** )
{
	if (_L - _F <= _SORT_MAX)
		_Insertion_sort(_F, _L, _P);
	else
		{_Sort(_F, _L, _P, (tNewsStory**)0);
		_Insertion_sort(_F, _F + _SORT_MAX, _P);
		for (_F += _SORT_MAX; _F != _L; ++_F)
			_Unguarded_insert(_F, *_F, _P); }
}
// HYPOTHESIS: the _Sort instance _Sort_0 at 0x005de905 calls; its median and partition
// helpers follow it at 0x005decc3 and 0x005ded2b.
// FUNCTION: STARFLEETOP 0x005deaac
template<>
void _Sort( tNewsStory** _F, tNewsStory** _L, fn_005de0cf_Pred _P, tNewsStory** )
	{for (; _SORT_MAX < _L - _F; )
		{tNewsStory** _M = _Unguarded_partition(_F, _L, _Median((tNewsStory*)(*_F),
			(tNewsStory*)(*(_F + (_L - _F) / 2)), (tNewsStory*)(*(_L - 1)), _P), _P);
		if (_L - _M <= _M - _F)
			_Sort(_M, _L, _P, (tNewsStory**)0), _L = _M;
		else
			_Sort(_F, _M, _P, (tNewsStory**)0), _F = _M; }}
// HYPOTHESIS: the _Sort instance _Sort_0 at 0x005de987 calls.
// FUNCTION: STARFLEETOP 0x005deb34
template<>
void _Sort( tNewsStory** _F, tNewsStory** _L, fn_005de1b8_Pred _P, tNewsStory** )
	{for (; _SORT_MAX < _L - _F; )
		{tNewsStory** _M = _Unguarded_partition(_F, _L, _Median((tNewsStory*)(*_F),
			(tNewsStory*)(*(_F + (_L - _F) / 2)), (tNewsStory*)(*(_L - 1)), _P), _P);
		if (_L - _M <= _M - _F)
			_Sort(_M, _L, _P, (tNewsStory**)0), _L = _M;
		else
			_Sort(_F, _M, _P, (tNewsStory**)0), _F = _M; }}
// HYPOTHESIS: the _Sort instance _Sort_0 at 0x005dea09 calls.
// FUNCTION: STARFLEETOP 0x005debbc
template<>
void _Sort( tNewsStory** _F, tNewsStory** _L, fn_005de222_Pred _P, tNewsStory** )
	{for (; _SORT_MAX < _L - _F; )
		{tNewsStory** _M = _Unguarded_partition(_F, _L, _Median((tNewsStory*)(*_F),
			(tNewsStory*)(*(_F + (_L - _F) / 2)), (tNewsStory*)(*(_L - 1)), _P), _P);
		if (_L - _M <= _M - _F)
			_Sort(_M, _L, _P, (tNewsStory**)0), _L = _M;
		else
			_Sort(_F, _M, _P, (tNewsStory**)0), _F = _M; }}
// HYPOTHESIS: the _Insertion_sort_1 instance _Sort_0 at 0x005de905 calls.
// FUNCTION: STARFLEETOP 0x005dec44
template<>
void _Insertion_sort_1( tNewsStory** _F, tNewsStory** _L, fn_005de0cf_Pred _P, tNewsStory** )
	{if (_F != _L)
		for (tNewsStory** _M = _F; ++_M != _L; )
			{tNewsStory* _V = *_M;
			if (!_P(_V, *_F))
				_Unguarded_insert(_M, _V, _P);
			else
				{copy_backward(_F, _M, _M + 1);
				*_F = _V; }}}
// HYPOTHESIS: the _Median instance _Sort at 0x005deaac calls.
// FUNCTION: STARFLEETOP 0x005decc3
template<>
tNewsStory* _Median( tNewsStory* _X, tNewsStory* _Y, tNewsStory* _Z, fn_005de0cf_Pred _P )
	{if (_P(_X, _Y))
		return (_P(_Y, _Z) ? _Y : _P(_X, _Z) ? _Z : _X);
	else
		return (_P(_X, _Z) ? _X : _P(_Y, _Z) ? _Z : _Y); }
// HYPOTHESIS: the _Unguarded_partition instance _Sort at 0x005deaac calls.
// FUNCTION: STARFLEETOP 0x005ded2b
template<>
tNewsStory** _Unguarded_partition( tNewsStory** _F, tNewsStory** _L, tNewsStory* _Piv, fn_005de0cf_Pred _P )
	{for (; ; ++_F)
		{for (; _P(*_F, _Piv); ++_F)
			;
		for (; _P(_Piv, *--_L); )
			;
		if (_L <= _F)
			return (_F);
		iter_swap(_F, _L); }}
// HYPOTHESIS: the _Insertion_sort_1 instance _Sort_0 at 0x005de987 calls.
// FUNCTION: STARFLEETOP 0x005ded73
template<>
void _Insertion_sort_1( tNewsStory** _F, tNewsStory** _L, fn_005de1b8_Pred _P, tNewsStory** )
	{if (_F != _L)
		for (tNewsStory** _M = _F; ++_M != _L; )
			{tNewsStory* _V = *_M;
			if (!_P(_V, *_F))
				_Unguarded_insert(_M, _V, _P);
			else
				{copy_backward(_F, _M, _M + 1);
				*_F = _V; }}}
// HYPOTHESIS: the _Median instance _Sort at 0x005deb34 calls.
// FUNCTION: STARFLEETOP 0x005dedf2
template<>
tNewsStory* _Median( tNewsStory* _X, tNewsStory* _Y, tNewsStory* _Z, fn_005de1b8_Pred _P )
	{if (_P(_X, _Y))
		return (_P(_Y, _Z) ? _Y : _P(_X, _Z) ? _Z : _X);
	else
		return (_P(_X, _Z) ? _X : _P(_Y, _Z) ? _Z : _Y); }
// HYPOTHESIS: the _Unguarded_partition instance _Sort at 0x005deb34 calls.
// FUNCTION: STARFLEETOP 0x005dee5a
template<>
tNewsStory** _Unguarded_partition( tNewsStory** _F, tNewsStory** _L, tNewsStory* _Piv, fn_005de1b8_Pred _P )
	{for (; ; ++_F)
		{for (; _P(*_F, _Piv); ++_F)
			;
		for (; _P(_Piv, *--_L); )
			;
		if (_L <= _F)
			return (_F);
		iter_swap(_F, _L); }}
}
