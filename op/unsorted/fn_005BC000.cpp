#include "Common/Database/DatabaseID.h"
#include "unsorted/tHexPoint.h"
#include <utility>

// FUNCTION: STARFLEETOP 0x005bc012
int __fastcall FUN_005bc012(int *param_1)
{
	return *param_1 - 0x2c;
}

// HYPOTHESIS: a 20-byte element with a virtual destructor in vtable slot 0;
// 0x005b69b0 is its assignment operator.
struct fn_005bc249_elem
{
	virtual ~fn_005bc249_elem();
	fn_005bc249_elem &FUN_005b69b0(const fn_005bc249_elem &_X);

	char m04[0x10];
};

// HYPOTHESIS: a 24-byte element whose destructor is 0x005bb6fc: a dword
// followed by a fn_005bc249_elem (0x005bce21 assigns it field by field).
struct fn_005bb6fc_elem
{
	int m00;
	fn_005bc249_elem m04;

	void FUN_005bb6fc();
	fn_005bb6fc_elem &operator=(const fn_005bb6fc_elem &_X)
		{m00 = _X.m00;
		m04.FUN_005b69b0(_X.m04);
		return (*this); }
};

// HYPOTHESIS: the library's _Construct(_P, _V) for this element.
void FUN_005bd32e(fn_005bb6fc_elem *_P, const fn_005bb6fc_elem &_V);

// HYPOTHESIS: an STL vector of 24-byte elements (allocator byte at +0,
// first/last/end at +4/+8/+0xc); this is its _Tidy.
struct fn_005bc018_vector
{
	char m00;
	fn_005bb6fc_elem *_First;
	fn_005bb6fc_elem *_Last;
	fn_005bb6fc_elem *_End;

	void FUN_005bc018();
	// HYPOTHESIS: insert(_P, _M, _X).
	void FUN_0053bcbe(fn_005bb6fc_elem *_P, unsigned int _M, const fn_005bb6fc_elem &_X);
	fn_005bb6fc_elem *FUN_005bc0de(fn_005bb6fc_elem *_P, const fn_005bb6fc_elem &_X);
};

// FUNCTION: STARFLEETOP 0x005bc018
void fn_005bc018_vector::FUN_005bc018()
{
	fn_005bb6fc_elem *l = _Last;
	for (fn_005bb6fc_elem *p = _First; p != l; p++)
		p->FUN_005bb6fc();
	operator delete(_First);
	_First = 0, _Last = 0, _End = 0;
}

// HYPOTHESIS: a second STL vector instance over the same 24-byte element,
// laid out like fn_005bc018_vector; 0x005bce21 is its insert(_P, _M, _X).
struct fn_005bce21_vector
{
	char m00;
	fn_005bb6fc_elem *_First;
	fn_005bb6fc_elem *_Last;
	fn_005bb6fc_elem *_End;

	unsigned int size() const
		{return (_First == 0 ? 0 : _Last - _First); }
	void _Destroy(fn_005bb6fc_elem *_F, fn_005bb6fc_elem *_L)
		{for (; _F != _L; ++_F)
			_F->FUN_005bb6fc(); }
	fn_005bb6fc_elem *_Ucopy(const fn_005bb6fc_elem *_F, const fn_005bb6fc_elem *_L,
		fn_005bb6fc_elem *_P)
		{for (; _F != _L; ++_P, ++_F)
			FUN_005bd32e(_P, *_F);
		return (_P); }
	void _Ufill(fn_005bb6fc_elem *_F, unsigned int _N, const fn_005bb6fc_elem &_X)
		{for (; 0 < _N; --_N, ++_F)
			FUN_005bd32e(_F, _X); }
	// HYPOTHESIS: the library's allocate, fill and copy_backward, inlined.
	fn_005bb6fc_elem *_Allocate(int _N, fn_005bb6fc_elem *)
		{if (_N < 0)
			_N = 0;
		return ((fn_005bb6fc_elem *)operator new((unsigned int)_N * sizeof (fn_005bb6fc_elem))); }
	void _Fill(fn_005bb6fc_elem *_F, fn_005bb6fc_elem *_L, const fn_005bb6fc_elem &_X)
		{for (; _F != _L; ++_F)
			*_F = _X; }
	fn_005bb6fc_elem *_Copy_backward(fn_005bb6fc_elem *_F, fn_005bb6fc_elem *_L,
		fn_005bb6fc_elem *_X)
		{while (_F != _L)
			*--_X = *--_L;
		return (_X); }
	void FUN_005bc04b(const fn_005bb6fc_elem &_X);
	void FUN_005bce21(fn_005bb6fc_elem *_P, unsigned int _M, const fn_005bb6fc_elem &_X);
};

// FUNCTION: STARFLEETOP 0x005bc04b
void fn_005bce21_vector::FUN_005bc04b(const fn_005bb6fc_elem &_X)
{
	FUN_005bce21(_Last, 1, _X);
}

// HYPOTHESIS: a record holding a database id and a hex position; the
// destructor stores no vtable, so the id is a member, not a base.
struct fn_005bc05c_record
{
	nDatabase::tID m00;
	tHexPoint m10;

	~fn_005bc05c_record();
};

// FUNCTION: STARFLEETOP 0x005bc05c
fn_005bc05c_record::~fn_005bc05c_record()
{
}

// HYPOTHESIS: an id and hex position pair laid out like fn_005bc05c_record
// but with a compiler-generated destructor: the original inlines its
// destruction into 0x005bc091 while 0x005bc05c stays out of line.
struct fn_005bc091_value
{
	nDatabase::tID m00;
	tHexPoint m10;
};

// HYPOTHESIS: a database id followed by an id and hex position pair.
struct fn_005bc091_entry
{
	nDatabase::tID m00;
	fn_005bc091_value m10;

	~fn_005bc091_entry();
};

// FUNCTION: STARFLEETOP 0x005bc091
fn_005bc091_entry::~fn_005bc091_entry()
{
}

// HYPOTHESIS: the STL vector's insert(_P, _X).
// FUNCTION: STARFLEETOP 0x005bc0de
fn_005bb6fc_elem *fn_005bc018_vector::FUN_005bc0de(fn_005bb6fc_elem *_P, const fn_005bb6fc_elem &_X)
{
	fn_005bb6fc_elem *_F = _First;
	FUN_0053bcbe(_P, 1, _X);
	return (_First + (_P - _F));
}

// HYPOTHESIS: a 32-byte element of another STL vector.
struct fn_005bc10e_elem
{
	char m00[0x20];
};

// HYPOTHESIS: an STL vector of 32-byte elements laid out like
// fn_005bc018_vector.
struct fn_005bc10e_vector
{
	char m00;
	fn_005bc10e_elem *_First;
	fn_005bc10e_elem *_Last;
	fn_005bc10e_elem *_End;

	// HYPOTHESIS: insert(_P, _M, _X).
	void FUN_0053bf12(fn_005bc10e_elem *_P, unsigned int _M, const fn_005bc10e_elem &_X);
	fn_005bc10e_elem *FUN_005bc10e(fn_005bc10e_elem *_P, const fn_005bc10e_elem &_X);
};

// HYPOTHESIS: the STL vector's insert(_P, _X).
// FUNCTION: STARFLEETOP 0x005bc10e
fn_005bc10e_elem *fn_005bc10e_vector::FUN_005bc10e(fn_005bc10e_elem *_P, const fn_005bc10e_elem &_X)
{
	fn_005bc10e_elem *_F = _First;
	FUN_0053bf12(_P, 1, _X);
	return (_First + (_P - _F));
}

// HYPOTHESIS: a 4-byte element of another STL vector.
struct fn_005bc138_elem
{
	char m00[0x4];
};

// HYPOTHESIS: an STL vector of 4-byte elements laid out like
// fn_005bc018_vector.
struct fn_005bc138_vector
{
	char m00;
	fn_005bc138_elem *_First;
	fn_005bc138_elem *_Last;
	fn_005bc138_elem *_End;

	// HYPOTHESIS: insert(_P, _M, _X).
	void FUN_00427883(fn_005bc138_elem *_P, unsigned int _M, const fn_005bc138_elem &_X);
	fn_005bc138_elem *FUN_005bc138(fn_005bc138_elem *_P, const fn_005bc138_elem &_X);
};

// HYPOTHESIS: the STL vector's insert(_P, _X).
// FUNCTION: STARFLEETOP 0x005bc138
fn_005bc138_elem *fn_005bc138_vector::FUN_005bc138(fn_005bc138_elem *_P, const fn_005bc138_elem &_X)
{
	fn_005bc138_elem *_F = _First;
	FUN_00427883(_P, 1, _X);
	return (_First + (_P - _F));
}

// HYPOTHESIS: a balanced tree laid out like the container at tPanelMgr+0x1f0
// (declared in that unit only): a head node at +0x04, the nil node at
// +0x08 and the size at +0x10; this is its erase(first, last).
struct fn_005bc161_tree
{
	struct node
	{
		node *_Left;
		node *_Parent;
		node *_Right;
	};
	class iterator
	{
	public:
		iterator() {}
		iterator(node *_P) : _Ptr(_P) {}
		// HYPOTHESIS: the iterator's increment, shared by every tree.
		iterator &FUN_0052e144();
		iterator operator++(int) { iterator _Tmp = *this; FUN_0052e144(); return (_Tmp); }
		bool operator==(const iterator &_X) const { return (_Ptr == _X._Ptr); }
		bool operator!=(const iterator &_X) const { return (!(*this == _X)); }
		node *_Ptr;
	};

	char m00[0x4];
	node *_Head;
	node *_Nil;
	char m0c[0x4];
	unsigned int _Size;

	iterator begin() { return (iterator(_Head->_Left)); }
	iterator end() { return (iterator(_Head)); }
	unsigned int size() const { return (_Size); }
	// HYPOTHESIS: erase(iterator).
	iterator FUN_0060ac82(iterator _P);
	// HYPOTHESIS: _Erase(node).
	void FUN_005bc570(node *_X);
	iterator FUN_005bc161(iterator _F, iterator _L);
	// HYPOTHESIS: the _Erase(node) of a second tree that shares this one's
	// iterator and erase(iterator); 0x005bc347 is its erase(first, last).
	void FUN_005bcd91(node *_X);
	iterator FUN_005bc347(iterator _F, iterator _L);
	// HYPOTHESIS: the _Erase(node) of a third such tree; 0x005bc404 is its
	// erase(first, last).
	void FUN_005bcdc1(node *_X);
	iterator FUN_005bc404(iterator _F, iterator _L);
	// HYPOTHESIS: the _Erase(node) of a fourth such tree; 0x005bc49b is its
	// erase(first, last).
	void FUN_005bcdf1(node *_X);
	iterator FUN_005bc49b(iterator _F, iterator _L);
};

// FUNCTION: STARFLEETOP 0x005bc161
fn_005bc161_tree::iterator fn_005bc161_tree::FUN_005bc161(iterator _F, iterator _L)
{
	if (size() == 0 || _F != begin() || _L != end())
		{while (_F != _L)
			FUN_0060ac82(_F++);
		return (_F); }
	else
		{FUN_005bc570(_Head->_Parent);
		_Head->_Parent = _Nil, _Size = 0;
		_Head->_Left = _Head, _Head->_Right = _Head;
		return (begin()); }
}

// HYPOTHESIS: another balanced tree laid out like fn_005bc161_tree, with
// its own iterator increment; this is its erase(first, last).
struct fn_005bc1d5_tree
{
	struct node
	{
		node *_Left;
		node *_Parent;
		node *_Right;
		// HYPOTHESIS: 0x005bc645 destroys the value at +0x0c with 0x005bc091
		// and reads the colour at +0x48 and the nil flag at +0x49.
		fn_005bc091_entry _Value;
		char m40[0x8];
		char _Color;
		char _Isnil;
	};
	class iterator
	{
	public:
		iterator() {}
		iterator(node *_P) : _Ptr(_P) {}
		// HYPOTHESIS: the iterator's increment.
		iterator &FUN_005bd22b();
		iterator operator++(int) { iterator _Tmp = *this; FUN_005bd22b(); return (_Tmp); }
		bool operator==(const iterator &_X) const { return (_Ptr == _X._Ptr); }
		bool operator!=(const iterator &_X) const { return (!(*this == _X)); }
		node *_Ptr;
	};

	char m00[0x4];
	node *_Head;
	node *_Nil;
	char m0c[0x4];
	unsigned int _Size;

	iterator begin() { return (iterator(_Head->_Left)); }
	iterator end() { return (iterator(_Head)); }
	unsigned int size() const { return (_Size); }
	node *&_Root() { return (_Head->_Parent); }
	node *&_Lmost() { return (_Head->_Left); }
	node *&_Rmost() { return (_Head->_Right); }
	static node *_Max(node *_P) { while (!_P->_Right->_Isnil) _P = _P->_Right; return (_P); }
	static node *_Min(node *_P) { while (!_P->_Left->_Isnil) _P = _P->_Left; return (_P); }
	// HYPOTHESIS: _Lrotate(node); the code is shared with other trees.
	void FUN_00556fd0(node *_X);
	// HYPOTHESIS: _Rrotate(node); the code is shared with other trees.
	void FUN_00558526(node *_X);
	// HYPOTHESIS: erase(iterator).
	iterator FUN_005bc645(iterator _P);
	// HYPOTHESIS: _Erase(node).
	void FUN_005bc8b0(node *_X);
	iterator FUN_005bc1d5(iterator _F, iterator _L);
	// HYPOTHESIS: _Buynode(parent, colour); this tree's copy also clears the
	// nil flag.
	node *_Buynode(node *_Parg, char _Carg)
		{node *_S = (node *)operator new(sizeof (node));
		_S->_Parent = _Parg;
		_S->_Color = _Carg;
		_S->_Isnil = 0;
		return (_S); }
	// HYPOTHESIS: _Init(), with a nil node per tree.
	void FUN_005bc8e8();
};

// FUNCTION: STARFLEETOP 0x005bc1d5
fn_005bc1d5_tree::iterator fn_005bc1d5_tree::FUN_005bc1d5(iterator _F, iterator _L)
{
	if (size() == 0 || _F != begin() || _L != end())
		{while (_F != _L)
			FUN_005bc645(_F++);
		return (_F); }
	else
		{FUN_005bc8b0(_Head->_Parent);
		_Head->_Parent = _Nil, _Size = 0;
		_Head->_Left = _Head, _Head->_Right = _Head;
		return (begin()); }
}

// HYPOTHESIS: an STL vector of 20-byte elements laid out like
// fn_005bc018_vector.
struct fn_005bc249_vector
{
	char m00;
	fn_005bc249_elem *_First;
	fn_005bc249_elem *_Last;
	fn_005bc249_elem *_End;

	fn_005bc249_elem *FUN_005bc249(fn_005bc249_elem *_F, fn_005bc249_elem *_L);
};

// MATCH-ONLY: the STL's copy(_F, _L, _X) and _Destroy(_F, _L), written out
// for this element because its assignment operator is not yet named.
inline fn_005bc249_elem *fn_005bc249_copy(fn_005bc249_elem *_F, fn_005bc249_elem *_L, fn_005bc249_elem *_X)
{
	for (; _F != _L; ++_X, ++_F)
		_X->FUN_005b69b0(*_F);
	return (_X);
}

inline void fn_005bc249_Destroy(fn_005bc249_elem *_F, fn_005bc249_elem *_L)
{
	for (; _F != _L; ++_F)
		_F->~fn_005bc249_elem();
}

// HYPOTHESIS: the STL vector's erase(_F, _L).
// FUNCTION: STARFLEETOP 0x005bc249
fn_005bc249_elem *fn_005bc249_vector::FUN_005bc249(fn_005bc249_elem *_F, fn_005bc249_elem *_L)
{
	fn_005bc249_elem *_S = fn_005bc249_copy(_L, _Last, _F);
	fn_005bc249_Destroy(_S, _Last);
	_Last = _S;
	return (_F);
}

// HYPOTHESIS: a 44-byte element with a virtual destructor in vtable slot 0;
// 0x005b676a is its assignment operator.
struct fn_005bc2cd_elem
{
	virtual ~fn_005bc2cd_elem();
	fn_005bc2cd_elem &FUN_005b676a(const fn_005bc2cd_elem &_X);

	char m04[0x28];
};

// HYPOTHESIS: the library's _Construct(_P, _V) for this element.
void FUN_005bd2ea(fn_005bc2cd_elem *_P, const fn_005bc2cd_elem &_V);

// HYPOTHESIS: an STL vector of 44-byte elements laid out like
// fn_005bc018_vector; the insert at 0x005bcb5b assigns with 0x005b676a, so
// its element is fn_005bc2cd_elem.
struct fn_005bc2a0_vector
{
	char m00;
	fn_005bc2cd_elem *_First;
	fn_005bc2cd_elem *_Last;
	fn_005bc2cd_elem *_End;

	unsigned int size() const
		{return (_First == 0 ? 0 : _Last - _First); }
	void _Destroy(fn_005bc2cd_elem *_F, fn_005bc2cd_elem *_L)
		{for (; _F != _L; ++_F)
			_F->~fn_005bc2cd_elem(); }
	fn_005bc2cd_elem *_Ucopy(const fn_005bc2cd_elem *_F, const fn_005bc2cd_elem *_L,
		fn_005bc2cd_elem *_P)
		{for (; _F != _L; ++_P, ++_F)
			FUN_005bd2ea(_P, *_F);
		return (_P); }
	void _Ufill(fn_005bc2cd_elem *_F, unsigned int _N, const fn_005bc2cd_elem &_X)
		{for (; 0 < _N; --_N, ++_F)
			FUN_005bd2ea(_F, _X); }
	// HYPOTHESIS: the library's allocate, fill and copy_backward, inlined.
	fn_005bc2cd_elem *_Allocate(int _N, fn_005bc2cd_elem *)
		{if (_N < 0)
			_N = 0;
		return ((fn_005bc2cd_elem *)operator new((unsigned int)_N * sizeof (fn_005bc2cd_elem))); }
	void _Fill(fn_005bc2cd_elem *_F, fn_005bc2cd_elem *_L, const fn_005bc2cd_elem &_X)
		{for (; _F != _L; ++_F)
			_F->FUN_005b676a(_X); }
	fn_005bc2cd_elem *_Copy_backward(fn_005bc2cd_elem *_F, fn_005bc2cd_elem *_L,
		fn_005bc2cd_elem *_X)
		{while (_F != _L)
			(--_X)->FUN_005b676a(*--_L);
		return (_X); }
	// HYPOTHESIS: insert(_P, _M, _X).
	void FUN_005bcb5b(fn_005bc2cd_elem *_P, unsigned int _M, const fn_005bc2cd_elem &_X);
	fn_005bc2cd_elem *FUN_005bc2a0(fn_005bc2cd_elem *_P, const fn_005bc2cd_elem &_X);
};

// HYPOTHESIS: the STL vector's insert(_P, _X).
// FUNCTION: STARFLEETOP 0x005bc2a0
fn_005bc2cd_elem *fn_005bc2a0_vector::FUN_005bc2a0(fn_005bc2cd_elem *_P, const fn_005bc2cd_elem &_X)
{
	fn_005bc2cd_elem *_F = _First;
	FUN_005bcb5b(_P, 1, _X);
	return (_First + (_P - _F));
}

// HYPOTHESIS: an STL vector of 44-byte elements laid out like
// fn_005bc018_vector.
struct fn_005bc2cd_vector
{
	char m00;
	fn_005bc2cd_elem *_First;
	fn_005bc2cd_elem *_Last;
	fn_005bc2cd_elem *_End;

	fn_005bc2cd_elem *FUN_005bc2cd(fn_005bc2cd_elem *_F, fn_005bc2cd_elem *_L);
};

// MATCH-ONLY: the STL's copy(_F, _L, _X) and _Destroy(_F, _L), written out
// for this element because its assignment operator is not yet named.
inline fn_005bc2cd_elem *fn_005bc2cd_copy(fn_005bc2cd_elem *_F, fn_005bc2cd_elem *_L, fn_005bc2cd_elem *_X)
{
	for (; _F != _L; ++_X, ++_F)
		_X->FUN_005b676a(*_F);
	return (_X);
}

inline void fn_005bc2cd_Destroy(fn_005bc2cd_elem *_F, fn_005bc2cd_elem *_L)
{
	for (; _F != _L; ++_F)
		_F->~fn_005bc2cd_elem();
}

// HYPOTHESIS: the STL vector's erase(_F, _L).
// FUNCTION: STARFLEETOP 0x005bc2cd
fn_005bc2cd_elem *fn_005bc2cd_vector::FUN_005bc2cd(fn_005bc2cd_elem *_F, fn_005bc2cd_elem *_L)
{
	fn_005bc2cd_elem *_S = fn_005bc2cd_copy(_L, _Last, _F);
	fn_005bc2cd_Destroy(_S, _Last);
	_Last = _S;
	return (_F);
}

// HYPOTHESIS: a balanced tree whose insert(_V) is 0x0057f946, returning the
// STL's pair of iterator and bool.
struct fn_005bc324_tree
{
	struct node;
	struct iterator
	{
		node *_Ptr;
	};
	struct _Pairib
	{
		_Pairib(const iterator &_V1, const bool &_V2) : first(_V1), second(_V2) {}
		iterator first;
		bool second;
	};

	_Pairib FUN_0057f946(const int &_V);
};

// HYPOTHESIS: a set holding that tree at +0; this is its insert(_X).
struct fn_005bc324_set
{
	fn_005bc324_tree _Tr;

	fn_005bc324_tree::_Pairib FUN_005bc324(const int &_X);
};

// FUNCTION: STARFLEETOP 0x005bc324
fn_005bc324_tree::_Pairib fn_005bc324_set::FUN_005bc324(const int &_X)
{
	fn_005bc324_tree::_Pairib _Ans = _Tr.FUN_0057f946(_X);
	return (fn_005bc324_tree::_Pairib(_Ans.first, _Ans.second));
}

// FUNCTION: STARFLEETOP 0x005bc347
fn_005bc161_tree::iterator fn_005bc161_tree::FUN_005bc347(iterator _F, iterator _L)
{
	if (size() == 0 || _F != begin() || _L != end())
		{while (_F != _L)
			FUN_0060ac82(_F++);
		return (_F); }
	else
		{FUN_005bcd91(_Head->_Parent);
		_Head->_Parent = _Nil, _Size = 0;
		_Head->_Left = _Head, _Head->_Right = _Head;
		return (begin()); }
}

// HYPOTHESIS: another set over the same tree type (its insert(_V) is again
// 0x0057f946); this is its insert(_X).
struct fn_005bc3bb_set
{
	fn_005bc324_tree _Tr;

	fn_005bc324_tree::_Pairib FUN_005bc3bb(const int &_X);
};

// FUNCTION: STARFLEETOP 0x005bc3bb
fn_005bc324_tree::_Pairib fn_005bc3bb_set::FUN_005bc3bb(const int &_X)
{
	fn_005bc324_tree::_Pairib _Ans = _Tr.FUN_0057f946(_X);
	return (fn_005bc324_tree::_Pairib(_Ans.first, _Ans.second));
}

// HYPOTHESIS: another balanced tree laid out like fn_005bc161_tree, with an
// empty allocator at +0x00, the key compare at +0x01 and _Multi at +0x0c;
// this is its _Tree(_Parg, _Marg, _Al) constructor.
struct fn_005bc3de_tree
{
	struct node;
	struct _Pr
	{
	};
	struct _A
	{
	};

	fn_005bc3de_tree(const _Pr &_Parg, bool _Marg, const _A &_Al);
	// HYPOTHESIS: _Init().
	void FUN_0042511e();

	_A allocator;
	_Pr key_compare;
	node *_Head;
	node *_Nil;
	bool _Multi;
	unsigned int _Size;
};

// FUNCTION: STARFLEETOP 0x005bc3de
fn_005bc3de_tree::fn_005bc3de_tree(const _Pr &_Parg, bool _Marg, const _A &_Al)
	: allocator(_Al), key_compare(_Parg), _Multi(_Marg)
{
	FUN_0042511e();
}

// FUNCTION: STARFLEETOP 0x005bc404
fn_005bc161_tree::iterator fn_005bc161_tree::FUN_005bc404(iterator _F, iterator _L)
{
	if (size() == 0 || _F != begin() || _L != end())
		{while (_F != _L)
			FUN_0060ac82(_F++);
		return (_F); }
	else
		{FUN_005bcdc1(_Head->_Parent);
		_Head->_Parent = _Nil, _Size = 0;
		_Head->_Left = _Head, _Head->_Right = _Head;
		return (begin()); }
}

// HYPOTHESIS: another set over the same tree type (its insert(_V) is again
// 0x0057f946); this is its insert(_X).
struct fn_005bc478_set
{
	fn_005bc324_tree _Tr;

	fn_005bc324_tree::_Pairib FUN_005bc478(const int &_X);
};

// FUNCTION: STARFLEETOP 0x005bc478
fn_005bc324_tree::_Pairib fn_005bc478_set::FUN_005bc478(const int &_X)
{
	fn_005bc324_tree::_Pairib _Ans = _Tr.FUN_0057f946(_X);
	return (fn_005bc324_tree::_Pairib(_Ans.first, _Ans.second));
}

// FUNCTION: STARFLEETOP 0x005bc49b
fn_005bc161_tree::iterator fn_005bc161_tree::FUN_005bc49b(iterator _F, iterator _L)
{
	if (size() == 0 || _F != begin() || _L != end())
		{while (_F != _L)
			FUN_0060ac82(_F++);
		return (_F); }
	else
		{FUN_005bcdf1(_Head->_Parent);
		_Head->_Parent = _Nil, _Size = 0;
		_Head->_Left = _Head, _Head->_Right = _Head;
		return (begin()); }
}

// HYPOTHESIS: a 0x2c-byte record of a database id, a hex position and two
// ints; 0x005bc50f is its member-wise copy constructor.
struct fn_005bc50f_record
{
	nDatabase::tID m00;
	tHexPoint m10;
	int m24;
	int m28;

	fn_005bc50f_record(const fn_005bc50f_record &other);
};

// FUNCTION: STARFLEETOP 0x005bc50f
fn_005bc50f_record::fn_005bc50f_record(const fn_005bc50f_record &other)
	: m00(other.m00), m10(other.m10), m24(other.m24), m28(other.m28)
{
}

// FUNCTION: STARFLEETOP 0x005bc570
void fn_005bc161_tree::FUN_005bc570(node *_X)
{
	for (node *_Y = _X; _Y != _Nil; _X = _Y)
		{FUN_005bc570(_Y->_Right);
		_Y = _Y->_Left;
		operator delete(_X); }
}

// HYPOTHESIS: a balanced tree of database ids laid out like
// fn_005bc3de_tree, its value at +0x0c of a node and ordered by
// tID::operator<; this is its insert(_V).
struct fn_005bc5a0_tree
{
	struct node
	{
		node *_Left;
		node *_Parent;
		node *_Right;
		nDatabase::tID _Value;
	};
	class iterator
	{
	public:
		iterator() {}
		iterator(node *_P) : _Ptr(_P) {}
		// HYPOTHESIS: the iterator's decrement.
		void FUN_005bd1f0();
		iterator &operator--() { FUN_005bd1f0(); return (*this); }
		bool operator==(const iterator &_X) const { return (_Ptr == _X._Ptr); }
		node *_Ptr;
	};
	struct _Pairib
	{
		_Pairib(const iterator &_V1, const bool &_V2) : first(_V1), second(_V2) {}
		iterator first;
		bool second;
	};

	char m00[0x4];
	node *_Head;
	node *_Nil;
	bool _Multi;
	unsigned int _Size;

	iterator begin() { return (iterator(_Head->_Left)); }
	// HYPOTHESIS: _Insert(_X, _Y, _V).
	iterator FUN_005bd0a6(node *_X, node *_Y, const nDatabase::tID &_V);
	_Pairib FUN_005bc5a0(const nDatabase::tID &_V);
};

// FUNCTION: STARFLEETOP 0x005bc5a0
fn_005bc5a0_tree::_Pairib fn_005bc5a0_tree::FUN_005bc5a0(const nDatabase::tID &_V)
{
	node *_X = _Head->_Parent;
	node *_Y = _Head;
	bool _Ans = true;
	while (_X != _Nil)
		{_Y = _X;
		_Ans = _V < _X->_Value;
		_X = _Ans ? _X->_Left : _X->_Right; }
	if (_Multi)
		return (_Pairib(FUN_005bd0a6(_X, _Y, _V), true));
	iterator _P = iterator(_Y);
	if (!_Ans)
		;
	else if (_P == begin())
		return (_Pairib(FUN_005bd0a6(_X, _Y, _V), true));
	else
		--_P;
	if (_P._Ptr->_Value < _V)
		return (_Pairib(FUN_005bd0a6(_X, _Y, _V), true));
	return (_Pairib(_P, false));
}

// FUNCTION: STARFLEETOP 0x005bc645
fn_005bc1d5_tree::iterator fn_005bc1d5_tree::FUN_005bc645(iterator _P)
{
	node *_X;
	node *_Y = (_P++)._Ptr;
	node *_Z = _Y;
	if (_Y->_Left == _Nil)
		_X = _Y->_Right;
	else if (_Y->_Right == _Nil)
		_X = _Y->_Left;
	else
		_Y = _Min(_Y->_Right), _X = _Y->_Right;
	if (_Y != _Z)
		{_Z->_Left->_Parent = _Y;
		_Y->_Left = _Z->_Left;
		if (_Y == _Z->_Right)
			_X->_Parent = _Y;
		else
			{_X->_Parent = _Y->_Parent;
			_Y->_Parent->_Left = _X;
			_Y->_Right = _Z->_Right;
			_Z->_Right->_Parent = _Y; }
		if (_Root() == _Z)
			_Root() = _Y;
		else if (_Z->_Parent->_Left == _Z)
			_Z->_Parent->_Left = _Y;
		else
			_Z->_Parent->_Right = _Y;
		_Y->_Parent = _Z->_Parent;
		std::swap(_Y->_Color, _Z->_Color);
		_Y = _Z; }
	else
		{_X->_Parent = _Y->_Parent;
		if (_Root() == _Z)
			_Root() = _X;
		else if (_Z->_Parent->_Left == _Z)
			_Z->_Parent->_Left = _X;
		else
			_Z->_Parent->_Right = _X;
		if (_Lmost() != _Z)
			;
		else if (_Z->_Right == _Nil)
			_Lmost() = _Z->_Parent;
		else
			_Lmost() = _Min(_X);
		if (_Rmost() != _Z)
			;
		else if (_Z->_Left == _Nil)
			_Rmost() = _Z->_Parent;
		else
			_Rmost() = _Max(_X); }
	if (_Y->_Color == 1)
		{while (_X != _Root() && _X->_Color == 1)
			if (_X == _X->_Parent->_Left)
				{node *_W = _X->_Parent->_Right;
				if (_W->_Color == 0)
					{_W->_Color = 1;
					_X->_Parent->_Color = 0;
					FUN_00556fd0(_X->_Parent);
					_W = _X->_Parent->_Right; }
				if (_W->_Left->_Color == 1
					&& _W->_Right->_Color == 1)
					{_W->_Color = 0;
					_X = _X->_Parent; }
				else
					{if (_W->_Right->_Color == 1)
						{_W->_Left->_Color = 1;
						_W->_Color = 0;
						FUN_00558526(_W);
						_W = _X->_Parent->_Right; }
					_W->_Color = _X->_Parent->_Color;
					_X->_Parent->_Color = 1;
					_W->_Right->_Color = 1;
					FUN_00556fd0(_X->_Parent);
					break; }}
			else
				{node *_W = _X->_Parent->_Left;
				if (_W->_Color == 0)
					{_W->_Color = 1;
					_X->_Parent->_Color = 0;
					FUN_00558526(_X->_Parent);
					_W = _X->_Parent->_Left; }
				if (_W->_Right->_Color == 1
					&& _W->_Left->_Color == 1)
					{_W->_Color = 0;
					_X = _X->_Parent; }
				else
					{if (_W->_Left->_Color == 1)
						{_W->_Right->_Color = 1;
						_W->_Color = 0;
						FUN_00556fd0(_W);
						_W = _X->_Parent->_Left; }
					_W->_Color = _X->_Parent->_Color;
					_X->_Parent->_Color = 1;
					_W->_Left->_Color = 1;
					FUN_00558526(_X->_Parent);
					break; }}
		_X->_Color = 1; }
	(&_Y->_Value)->~fn_005bc091_entry();
	operator delete(_Y);
	--_Size;
	return (_P);
}

// FUNCTION: STARFLEETOP 0x005bc8b0
void fn_005bc1d5_tree::FUN_005bc8b0(node *_X)
{
	for (node *_Y = _X; _Y != _Nil; _X = _Y)
		{FUN_005bc8b0(_Y->_Right);
		_Y = _Y->_Left;
		(&_X->_Value)->~fn_005bc091_entry();
		operator delete(_X); }
}

// FUNCTION: STARFLEETOP 0x005bc8e8
void fn_005bc1d5_tree::FUN_005bc8e8()
{
	_Nil = _Buynode(0, 1);
	_Nil->_Isnil = 1;
	_Nil->_Left = 0, _Nil->_Right = 0;
	_Head = _Buynode(_Nil, 0);
	_Lmost() = _Head, _Rmost() = _Head;
	_Size = 0;
}

// HYPOTHESIS: a 32-byte element of another STL vector; 0x005aae47 is its
// destructor and 0x005ab4d9 its assignment.
struct fn_005bc938_elem
{
	char m00[0x20];

	void FUN_005aae47();
	fn_005bc938_elem &FUN_005ab4d9(const fn_005bc938_elem &_X);
};

// HYPOTHESIS: the library's _Construct(_P, _V) for this element.
void FUN_005ab4ac(fn_005bc938_elem *_P, const fn_005bc938_elem &_V);

// HYPOTHESIS: an STL vector of 32-byte elements laid out like
// fn_005bc018_vector.
struct fn_005bc938_vector
{
	char m00;
	fn_005bc938_elem *_First;
	fn_005bc938_elem *_Last;
	fn_005bc938_elem *_End;

	unsigned int size() const
		{return (_First == 0 ? 0 : _Last - _First); }
	void _Destroy(fn_005bc938_elem *_F, fn_005bc938_elem *_L)
		{for (; _F != _L; ++_F)
			_F->FUN_005aae47(); }
	fn_005bc938_elem *_Ucopy(const fn_005bc938_elem *_F, const fn_005bc938_elem *_L,
		fn_005bc938_elem *_P)
		{for (; _F != _L; ++_P, ++_F)
			FUN_005ab4ac(_P, *_F);
		return (_P); }
	void _Ufill(fn_005bc938_elem *_F, unsigned int _N, const fn_005bc938_elem &_X)
		{for (; 0 < _N; --_N, ++_F)
			FUN_005ab4ac(_F, _X); }
	// HYPOTHESIS: the library's allocate, fill and copy_backward, inlined.
	fn_005bc938_elem *_Allocate(int _N, fn_005bc938_elem *)
		{if (_N < 0)
			_N = 0;
		return ((fn_005bc938_elem *)operator new((unsigned int)_N * sizeof (fn_005bc938_elem))); }
	void _Fill(fn_005bc938_elem *_F, fn_005bc938_elem *_L, const fn_005bc938_elem &_X)
		{for (; _F != _L; ++_F)
			_F->FUN_005ab4d9(_X); }
	fn_005bc938_elem *_Copy_backward(fn_005bc938_elem *_F, fn_005bc938_elem *_L,
		fn_005bc938_elem *_X)
		{while (_F != _L)
			(--_X)->FUN_005ab4d9(*--_L);
		return (_X); }
	void FUN_005bc938(fn_005bc938_elem *_P, unsigned int _M, const fn_005bc938_elem &_X);
};

// HYPOTHESIS: the STL vector's insert(_P, _M, _X).
// FUNCTION: STARFLEETOP 0x005bc938
void fn_005bc938_vector::FUN_005bc938(fn_005bc938_elem *_P, unsigned int _M, const fn_005bc938_elem &_X)
{
	if ((unsigned int)(_End - _Last) < _M)
		{unsigned int _N = size() + (_M < size() ? size() : _M);
		fn_005bc938_elem *_S = _Allocate(_N, (fn_005bc938_elem *)0);
		fn_005bc938_elem *_Q = _Ucopy(_First, _P, _S);
		_Ufill(_Q, _M, _X);
		_Ucopy(_P, _Last, _Q + _M);
		_Destroy(_First, _Last);
		operator delete(_First);
		_End = _S + _N;
		_Last = _S + size() + _M;
		_First = _S; }
	else if ((unsigned int)(_Last - _P) < _M)
		{_Ucopy(_P, _Last, _P + _M);
		_Ufill(_Last, _M - (_Last - _P), _X);
		_Fill(_P, _Last, _X);
		_Last += _M; }
	else if (0 < _M)
		{_Ucopy(_Last - _M, _Last, _Last);
		_Copy_backward(_P, _Last - _M, _Last);
		_Fill(_P, _P + _M, _X);
		_Last += _M; }
}

// HYPOTHESIS: the STL vector's insert(_P, _M, _X).
// FUNCTION: STARFLEETOP 0x005bcb5b
void fn_005bc2a0_vector::FUN_005bcb5b(fn_005bc2cd_elem *_P, unsigned int _M, const fn_005bc2cd_elem &_X)
{
	if ((unsigned int)(_End - _Last) < _M)
		{unsigned int _N = size() + (_M < size() ? size() : _M);
		fn_005bc2cd_elem *_S = _Allocate(_N, (fn_005bc2cd_elem *)0);
		fn_005bc2cd_elem *_Q = _Ucopy(_First, _P, _S);
		_Ufill(_Q, _M, _X);
		_Ucopy(_P, _Last, _Q + _M);
		_Destroy(_First, _Last);
		operator delete(_First);
		_End = _S + _N;
		_Last = _S + size() + _M;
		_First = _S; }
	else if ((unsigned int)(_Last - _P) < _M)
		{_Ucopy(_P, _Last, _P + _M);
		_Ufill(_Last, _M - (_Last - _P), _X);
		_Fill(_P, _Last, _X);
		_Last += _M; }
	else if (0 < _M)
		{_Ucopy(_Last - _M, _Last, _Last);
		_Copy_backward(_P, _Last - _M, _Last);
		_Fill(_P, _P + _M, _X);
		_Last += _M; }
}

// FUNCTION: STARFLEETOP 0x005bcd91
void fn_005bc161_tree::FUN_005bcd91(node *_X)
{
	for (node *_Y = _X; _Y != _Nil; _X = _Y)
		{FUN_005bcd91(_Y->_Right);
		_Y = _Y->_Left;
		operator delete(_X); }
}

// FUNCTION: STARFLEETOP 0x005bcdc1
void fn_005bc161_tree::FUN_005bcdc1(node *_X)
{
	for (node *_Y = _X; _Y != _Nil; _X = _Y)
		{FUN_005bcdc1(_Y->_Right);
		_Y = _Y->_Left;
		operator delete(_X); }
}

// FUNCTION: STARFLEETOP 0x005bcdf1
void fn_005bc161_tree::FUN_005bcdf1(node *_X)
{
	for (node *_Y = _X; _Y != _Nil; _X = _Y)
		{FUN_005bcdf1(_Y->_Right);
		_Y = _Y->_Left;
		operator delete(_X); }
}

// HYPOTHESIS: the STL vector's insert(_P, _M, _X).
// FUNCTION: STARFLEETOP 0x005bce21
void fn_005bce21_vector::FUN_005bce21(fn_005bb6fc_elem *_P, unsigned int _M, const fn_005bb6fc_elem &_X)
{
	if ((unsigned int)(_End - _Last) < _M)
		{unsigned int _N = size() + (_M < size() ? size() : _M);
		fn_005bb6fc_elem *_S = _Allocate(_N, (fn_005bb6fc_elem *)0);
		fn_005bb6fc_elem *_Q = _Ucopy(_First, _P, _S);
		_Ufill(_Q, _M, _X);
		_Ucopy(_P, _Last, _Q + _M);
		_Destroy(_First, _Last);
		operator delete(_First);
		_End = _S + _N;
		_Last = _S + size() + _M;
		_First = _S; }
	else if ((unsigned int)(_Last - _P) < _M)
		{_Ucopy(_P, _Last, _P + _M);
		_Ufill(_Last, _M - (_Last - _P), _X);
		_Fill(_P, _Last, _X);
		_Last += _M; }
	else if (0 < _M)
		{_Ucopy(_Last - _M, _Last, _Last);
		_Copy_backward(_P, _Last - _M, _Last);
		_Fill(_P, _P + _M, _X);
		_Last += _M; }
}
