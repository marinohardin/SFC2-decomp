#include "Common/Database/DatabaseID.h"
#include "unsorted/tHexPoint.h"

// FUNCTION: STARFLEETOP 0x005bc012
int __fastcall FUN_005bc012(int *param_1)
{
	return *param_1 - 0x2c;
}

// HYPOTHESIS: a 24-byte element whose destructor is 0x005bb6fc.
struct fn_005bb6fc_elem
{
	char m00[0x18];

	void FUN_005bb6fc();
};

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

// HYPOTHESIS: takes the owner's +8 field, a flag and a value; the callee
// pops its own arguments.
void __stdcall FUN_005bce21(int param_1, int param_2, int param_3);

// HYPOTHESIS: owner object holding a handle at +8.
struct fn_005bc04b_owner
{
	int m00;
	int m04;
	int m08;

	void FUN_005bc04b(int param_1);
};

// FUNCTION: STARFLEETOP 0x005bc04b
void fn_005bc04b_owner::FUN_005bc04b(int param_1)
{
	FUN_005bce21(m08, 1, param_1);
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
	// HYPOTHESIS: erase(iterator).
	iterator FUN_005bc645(iterator _P);
	// HYPOTHESIS: _Erase(node).
	void FUN_005bc8b0(node *_X);
	iterator FUN_005bc1d5(iterator _F, iterator _L);
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

// HYPOTHESIS: a 20-byte element with a virtual destructor in vtable slot 0;
// 0x005b69b0 is its assignment operator.
struct fn_005bc249_elem
{
	virtual ~fn_005bc249_elem();
	fn_005bc249_elem &FUN_005b69b0(const fn_005bc249_elem &_X);

	char m04[0x10];
};

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

// HYPOTHESIS: a 44-byte element of another STL vector.
struct fn_005bc2a0_elem
{
	char m00[0x2c];
};

// HYPOTHESIS: an STL vector of 44-byte elements laid out like
// fn_005bc018_vector.
struct fn_005bc2a0_vector
{
	char m00;
	fn_005bc2a0_elem *_First;
	fn_005bc2a0_elem *_Last;
	fn_005bc2a0_elem *_End;

	// HYPOTHESIS: insert(_P, _M, _X).
	void FUN_005bcb5b(fn_005bc2a0_elem *_P, unsigned int _M, const fn_005bc2a0_elem &_X);
	fn_005bc2a0_elem *FUN_005bc2a0(fn_005bc2a0_elem *_P, const fn_005bc2a0_elem &_X);
};

// HYPOTHESIS: the STL vector's insert(_P, _X).
// FUNCTION: STARFLEETOP 0x005bc2a0
fn_005bc2a0_elem *fn_005bc2a0_vector::FUN_005bc2a0(fn_005bc2a0_elem *_P, const fn_005bc2a0_elem &_X)
{
	fn_005bc2a0_elem *_F = _First;
	FUN_005bcb5b(_P, 1, _X);
	return (_First + (_P - _F));
}

// HYPOTHESIS: a 44-byte element with a virtual destructor in vtable slot 0;
// 0x005b676a is its assignment operator.
struct fn_005bc2cd_elem
{
	virtual ~fn_005bc2cd_elem();
	fn_005bc2cd_elem &FUN_005b676a(const fn_005bc2cd_elem &_X);

	char m04[0x28];
};

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
