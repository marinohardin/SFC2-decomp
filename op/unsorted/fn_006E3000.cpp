// Functions of the 0x006e3000 page; original file unknown.

#include <windows.h>
#include <list>
#include <map>
#include <string>

// HYPOTHESIS: a scoped critical-section lock that counts how often it holds the
// section; the destructor leaves it once per count.
class tLock_006e3000
{
public:
	tLock_006e3000(CRITICAL_SECTION* section);
	~tLock_006e3000();

	CRITICAL_SECTION* mSection;
	int mCount;
};

inline tLock_006e3000::tLock_006e3000(CRITICAL_SECTION* section)
{
	mSection = section;
	::EnterCriticalSection(mSection);
	mCount = 1;
}

inline tLock_006e3000::~tLock_006e3000()
{
	while (mCount != 0) {
		::LeaveCriticalSection(mSection);
		--mCount;
	}
}

// HYPOTHESIS: an interlocked reference-counted object; slot 0 of its vtable is
// the deleting destructor.
class tObject_006e3000_shared
{
public:
	virtual ~tObject_006e3000_shared();
	void fn_006e08f0(long value);
	tObject_006e3000_shared* fn_addref();
	void fn_release();

	long mRefCount;
	char m08[0x20];
	long m28;
};

// HYPOTHESIS: the add half of the reference count; the exe inlines it.
inline tObject_006e3000_shared* tObject_006e3000_shared::fn_addref()
{
	::InterlockedIncrement(&mRefCount);
	return this;
}

// HYPOTHESIS: the release half of the reference count; the exe inlines it.
inline void tObject_006e3000_shared::fn_release()
{
	if (::InterlockedDecrement(&mRefCount) <= 0)
		delete this;
}

// HYPOTHESIS: the smart pointer that holds a tObject_006e3000_shared.
class tRef_006e3000
{
public:
	tRef_006e3000(tObject_006e3000_shared* object);
	~tRef_006e3000();
	tObject_006e3000_shared* operator->() const;

	tObject_006e3000_shared* mPtr;
};

inline tRef_006e3000::tRef_006e3000(tObject_006e3000_shared* object)
{
	mPtr = object ? object->fn_addref() : 0;
}

inline tRef_006e3000::~tRef_006e3000()
{
	mPtr->fn_release();
}

inline tObject_006e3000_shared* tRef_006e3000::operator->() const
{
	return mPtr;
}

class tObject_006e3000_entry;

typedef std::multimap<unsigned long, tObject_006e3000_entry*> tTimerMap_006e3000;
typedef std::map<long, long> tIndexMap_006e3000;

// HYPOTHESIS: one timed entry; m04 is its place in the owner's second map.
class tObject_006e3000_entry
{
public:
	void fn_005d6a66();

	tObject_006e3000_shared* mObject;
	tIndexMap_006e3000::iterator m04;
};

class tObject_006e3000
{
public:
	void FUN_006e3000();
	unsigned long FUN_006e31b0();

	char m00[0x18];
	CRITICAL_SECTION mSection;
	tTimerMap_006e3000 mTimers;
	long m40;
	tIndexMap_006e3000 m44;
	char m54[0x8];
	tTimerMap_006e3000 m5c;
	char m6c[0x1c];
	unsigned long mLastTick;
};

// FUNCTION: STARFLEETOP 0x006e3000
void tObject_006e3000::FUN_006e3000()
{
	tLock_006e3000 lock(&mSection);
	unsigned long now = ::GetTickCount();
	if (now < mLastTick) {
		for (tTimerMap_006e3000::iterator i = mTimers.begin(); i != mTimers.end(); ++i)
			(*i).second->mObject->fn_006e08f0((*i).second->mObject->m28);
		mTimers.swap(m5c);
		m5c.clear();
	}
	mLastTick = now;
	tTimerMap_006e3000::iterator it = mTimers.begin();
	while (it != mTimers.end() && (*it).first <= mLastTick) {
		tRef_006e3000 object((*it).second->mObject);
		object->fn_006e08f0(object->m28);
		tObject_006e3000_entry* entry = (*it).second;
		tTimerMap_006e3000::iterator next = it;
		mTimers.erase(next++);
		m44.erase(entry->m04);
		entry->fn_005d6a66();
		delete entry;
		it = next;
	}
}

// FUNCTION: STARFLEETOP 0x006e31b0
unsigned long tObject_006e3000::FUN_006e31b0()
{
	tLock_006e3000 lock(&mSection);
	unsigned long wait = 9 - mLastTick;
	if (m40 != 0) {
		tTimerMap_006e3000::iterator first = mTimers.begin();
		wait = (*first).first <= mLastTick ? 0 : (*first).first - mLastTick;
	}
	return wait;
}

class tRef_006e15b0;

// HYPOTHESIS: a growable buffer; m10 is the block, m08 its capacity and m0c
// the bytes in use.
class tObject_006e3240
{
public:
	void FUN_006e3240();
	void FUN_006e3270();
	void FUN_006e32f0(int param_1);
	void FUN_006e3320(unsigned long param_1);
	void FUN_006e3370(unsigned long param_1);
	void FUN_006e3390(int param_1);
	void FUN_006e33b0(const void* param_1, int param_2);
	void FUN_006e33f0(unsigned char param_1);
	void FUN_006e3400(bool param_1);
	void FUN_006e3420(unsigned short param_1);
	void FUN_006e3430(unsigned long param_1);
	void FUN_006e3440(unsigned long param_1, unsigned char param_2);
	// HYPOTHESIS: the string is VC6's std::string (data at +4, length at +8,
	// c_str falling back to the shared empty string).
	void FUN_006e3480(const std::string& param_1, unsigned char param_2);
	// HYPOTHESIS: the wide form of FUN_006e3480 (std::wstring, two bytes per
	// character).
	void FUN_006e34c0(const std::wstring& param_1, unsigned char param_2);
	void FUN_006e3500(int param_1, const void* param_2, int param_3);
	void FUN_006e3530(int param_1, unsigned char param_2);
	void FUN_006e3550(int param_1, unsigned short param_2);
	tRef_006e15b0 FUN_006e3570();

	char m00[0x4];
	unsigned char m04;
	char m05[0x3];
	unsigned long m08;
	unsigned long m0c;
	char* m10;
};

// FUNCTION: STARFLEETOP 0x006e3240
void tObject_006e3240::FUN_006e3240()
{
	delete m10;
	m10 = 0;
	m0c = 0;
	m08 = 0;
	if (m04 > 0)
		FUN_006e3390(m04);
}

// FUNCTION: STARFLEETOP 0x006e3270
void tObject_006e3240::FUN_006e3270()
{
	m10 = 0;
	FUN_006e3240();
}

// FUNCTION: STARFLEETOP 0x006e32f0
void tObject_006e3240::FUN_006e32f0(int param_1)
{
	unsigned long needed = m0c + param_1;
	if (needed <= m08)
		return;
	int size = m08;
	if (size < 0x20)
		size = 0x20;
	while (size < (int)needed)
		size *= 2;
	FUN_006e3320(size);
}

// FUNCTION: STARFLEETOP 0x006e3320
void tObject_006e3240::FUN_006e3320(unsigned long param_1)
{
	if (m08 < param_1) {
		char* block = new char[param_1];
		memcpy(block, m10, m0c);
		delete m10;
		m10 = block;
		m08 = param_1;
	}
}

// FUNCTION: STARFLEETOP 0x006e3370
void tObject_006e3240::FUN_006e3370(unsigned long param_1)
{
	FUN_006e3320(param_1);
	m0c = param_1;
}

// FUNCTION: STARFLEETOP 0x006e3390
void tObject_006e3240::FUN_006e3390(int param_1)
{
	FUN_006e32f0(param_1);
	m0c += param_1;
}

// FUNCTION: STARFLEETOP 0x006e33b0
void tObject_006e3240::FUN_006e33b0(const void* param_1, int param_2)
{
	FUN_006e32f0(param_2);
	memcpy(m10 + m0c, param_1, param_2);
	m0c += param_2;
}

// FUNCTION: STARFLEETOP 0x006e33f0
void tObject_006e3240::FUN_006e33f0(unsigned char param_1)
{
	FUN_006e33b0(&param_1, 1);
}

// FUNCTION: STARFLEETOP 0x006e3400
void tObject_006e3240::FUN_006e3400(bool param_1)
{
	unsigned char value = param_1 ? 1 : 0;
	FUN_006e33b0(&value, 1);
}

// FUNCTION: STARFLEETOP 0x006e3420
void tObject_006e3240::FUN_006e3420(unsigned short param_1)
{
	FUN_006e33b0(&param_1, 2);
}

// FUNCTION: STARFLEETOP 0x006e3430
void tObject_006e3240::FUN_006e3430(unsigned long param_1)
{
	FUN_006e33b0(&param_1, 4);
}

// FUNCTION: STARFLEETOP 0x006e3440
void tObject_006e3240::FUN_006e3440(unsigned long param_1, unsigned char param_2)
{
	switch (param_2) {
	case 1:
		FUN_006e33f0((unsigned char)param_1);
		break;
	case 2:
		FUN_006e3420((unsigned short)param_1);
		break;
	case 4:
		FUN_006e3430(param_1);
		break;
	}
}

// FUNCTION: STARFLEETOP 0x006e3480
void tObject_006e3240::FUN_006e3480(const std::string& param_1, unsigned char param_2)
{
	FUN_006e3440(param_1.size(), param_2);
	FUN_006e33b0(param_1.c_str(), param_1.size());
}

// FUNCTION: STARFLEETOP 0x006e34c0
void tObject_006e3240::FUN_006e34c0(const std::wstring& param_1, unsigned char param_2)
{
	FUN_006e3440(param_1.size(), param_2);
	FUN_006e33b0(param_1.c_str(), param_1.size() * 2);
}

// FUNCTION: STARFLEETOP 0x006e3500
void tObject_006e3240::FUN_006e3500(int param_1, const void* param_2, int param_3)
{
	memcpy(m10 + param_1, param_2, param_3);
}

// FUNCTION: STARFLEETOP 0x006e3530
void tObject_006e3240::FUN_006e3530(int param_1, unsigned char param_2)
{
	FUN_006e3500(param_1, &param_2, 1);
}

// FUNCTION: STARFLEETOP 0x006e3550
void tObject_006e3240::FUN_006e3550(int param_1, unsigned short param_2)
{
	FUN_006e3500(param_1, &param_2, 2);
}

// HYPOTHESIS: the reference-counted byte block FUN_006e3570 hands out; its
// constructor at 0x006e15b0 takes the data, the length and an ownership flag.
class tObject_006e15b0
{
public:
	tObject_006e15b0(const void* data, unsigned long length, bool owned);
	tObject_006e15b0* fn_addref();

	char m00[0x4];
	long mRefCount;
	char m08[0x8];
};

// HYPOTHESIS: the add half of the reference count; the exe inlines it.
inline tObject_006e15b0* tObject_006e15b0::fn_addref()
{
	::InterlockedIncrement(&mRefCount);
	return this;
}

// HYPOTHESIS: the smart pointer returned by value; the exe inlines its
// constructor.
class tRef_006e15b0
{
public:
	tRef_006e15b0(tObject_006e15b0* object);
	~tRef_006e15b0();

	tObject_006e15b0* mPtr;
};

inline tRef_006e15b0::tRef_006e15b0(tObject_006e15b0* object)
{
	mPtr = object ? object->fn_addref() : 0;
}

// FUNCTION: STARFLEETOP 0x006e3570
tRef_006e15b0 tObject_006e3240::FUN_006e3570()
{
	switch (m04) {
	case 1:
		*(unsigned char*)m10 = (unsigned char)m0c;
		break;
	case 2:
		{ unsigned short v = (unsigned short)m0c; *(unsigned short*)m10 = v; }
		break;
	case 4:
		{ unsigned long v = m0c; *(unsigned long*)m10 = v; }
		break;
	}
	char* data = m10;
	unsigned long length = m0c;
	FUN_006e3270();
	return new tObject_006e15b0(data, length, true);
}

// HYPOTHESIS: the element type of the list FUN_006e3640 frees; its destructor
// is at 0x005d6a66.
class tObject_005d6a66
{
public:
	~tObject_005d6a66();
};

// HYPOTHESIS: FUN_006e3640 is VC6's std::list destructor inlined into the
// destructor of a class whose first member is the list.
class tObject_006e3640
{
public:
	~tObject_006e3640();
	std::list<tObject_005d6a66>::iterator FUN_006e3750(std::list<tObject_005d6a66>::iterator param_1);

	std::list<tObject_005d6a66> mList;
};

// FUNCTION: STARFLEETOP 0x006e3640
tObject_006e3640::~tObject_006e3640()
{
}

// HYPOTHESIS: the second tree of the timer table; FUN_006e36a0 only hands its
// iterator to erase(iterator) at 0x005d9441.
struct fn_005d9441_tree
{
	struct node;
	struct value_type;
	class iterator
	{
	public:
		iterator() {}
		iterator(node *_P) : _Ptr(_P) {}
		value_type &operator*() const;
		node *_Ptr;
	};

	iterator FUN_005d9441(iterator _P);
	// HYPOTHESIS: insert(const value_type&).
	std::pair<iterator, bool> FUN_006e3f40(const value_type& _X);

	char m00[0x4];
	node *_Head;
	node *_Nil;
	char m0c[0x4];
	unsigned int _Size;
};

// HYPOTHESIS: the entry FUN_006e36a0 frees after calling 0x006dbc40 on it; m04
// is its place in the second tree. It may be tObject_006e3000_entry.
class tObject_006dbc40
{
public:
	tObject_006dbc40(tObject_006e3000_shared* const& object, fn_005d9441_tree::iterator where);
	void FUN_006dbc40();

	tObject_006e3000_shared* m00;
	fn_005d9441_tree::iterator m04;
};

inline tObject_006dbc40::tObject_006dbc40(tObject_006e3000_shared* const& object, fn_005d9441_tree::iterator where)
	: m00(object ? object->fn_addref() : 0), m04(where)
{
}

// HYPOTHESIS: the reference the second tree holds as its key.
class tRef_006e38a0
{
public:
	tRef_006e38a0(tObject_006e3000_shared* object);
	~tRef_006e38a0();

	tObject_006e3000_shared* mPtr;
};

inline tRef_006e38a0::tRef_006e38a0(tObject_006e3000_shared* object)
{
	mPtr = object ? object->fn_addref() : 0;
}

inline tRef_006e38a0::~tRef_006e38a0()
{
	if (mPtr != 0)
		mPtr->fn_release();
}

// HYPOTHESIS: a balanced tree of the game's STL, with a nil node per tree and
// the nil flag in each node, keyed by a 4-byte value.
struct fn_0060ac82_tree
{
	struct node
	{
		node *_Left;
		node *_Parent;
		node *_Right;
		struct value
		{
			value(const unsigned long& _V1, tObject_006dbc40* const& _V2)
				: first(_V1), second(_V2) {}
			unsigned long first;
			tObject_006dbc40 *second;
		} _Value;
		char _Color;
		char _Isnil;
	};
	class iterator : public std::_Bidit<node::value, int>
	{
	public:
		iterator() {}
		iterator(node *_P) : _Ptr(_P) {}
		node::value &operator*() const { return (_Ptr->_Value); }
		iterator &operator++()
			{if (!_Ptr->_Right->_Isnil)
				_Ptr = _Min(_Ptr->_Right);
			else
				{node *_P;
				while (_Ptr == (_P = _Ptr->_Parent)->_Right)
					_Ptr = _P;
				if (_Ptr->_Right != _P)
					_Ptr = _P; }
			return (*this); }
		iterator operator++(int) { iterator _Tmp = *this; ++*this; return (_Tmp); }
		iterator &operator--() { _Dec(); return (*this); }
		void _Dec()
			{if (_Ptr->_Color == 0
				&& _Ptr->_Parent->_Parent == _Ptr)
				_Ptr = _Ptr->_Right;
			else if (!_Ptr->_Left->_Isnil)
				_Ptr = _Max(_Ptr->_Left);
			else
				{node *_P;
				while (_Ptr == (_P = _Ptr->_Parent)->_Left)
					_Ptr = _P;
				_Ptr = _P; }}
		bool operator==(const iterator& _X) const { return (_Ptr == _X._Ptr); }
		bool operator!=(const iterator& _X) const { return (!(*this == _X)); }
		node *_Mynode() const { return (_Ptr); }
		node *_Ptr;
	};

	typedef std::pair<iterator, bool> _Pairib;

	std::less<unsigned long> key_compare;
	char m01[0x3];
	node *_Head;
	node *_Nil;
	char m0c[0x4];
	unsigned int _Size;

	static node *_Max(node *_P) { while (!_P->_Right->_Isnil) _P = _P->_Right; return (_P); }
	static node *_Min(node *_P) { while (!_P->_Left->_Isnil) _P = _P->_Left; return (_P); }
	iterator begin() { return (iterator(_Head->_Left)); }
	iterator end() { return (iterator(_Head)); }
	static node *&_Left(node *_P) { return (_P->_Left); }
	static node *&_Parent(node *_P) { return (_P->_Parent); }
	static node *&_Right(node *_P) { return (_P->_Right); }
	struct _Kfn
	{
		const unsigned long &operator()(const node::value& _X) const { return (_X.first); }
	};
	static node::value &_Value(node *_P) { return (_P->_Value); }
	static const unsigned long &_Key(node *_P) { return (_Kfn()(_Value(_P))); }
	node *&_Root() const { return (_Parent(_Head)); }
	node *&_Rmost() const { return (_Right(_Head)); }
	unsigned int size() const { return (_Size); }
	_Pairib insert(const node::value& _V)
		{node *_X = _Root();
		node *_Y = _Head;
		bool _Ans = true;
		while (_X != _Nil)
			{_Y = _X;
			_Ans = key_compare(_Kfn()(_V), _Key(_X));
			_X = _Ans ? _Left(_X) : _Right(_X); }
		if (m0c[0])
			return (_Pairib(FUN_006e4360(_X, _Y, _V), true));
		iterator _P = iterator(_Y);
		if (!_Ans)
			;
		else if (_P == begin())
			return (_Pairib(FUN_006e4360(_X, _Y, _V), true));
		else
			--_P;
		if (key_compare(_Key(_P._Mynode()), _Kfn()(_V)))
			return (_Pairib(FUN_006e4360(_X, _Y, _V), true));
		return (_Pairib(_P, false)); }
	// HYPOTHESIS: insert(iterator, const value_type&).
	iterator FUN_006e39e0(iterator _P, const node::value& _V);
	// HYPOTHESIS: _Insert(x, y, value).
	iterator FUN_006e4360(node *_X, node *_Y, const node::value& _V);
	// HYPOTHESIS: erase(iterator).
	iterator FUN_0060ac82(iterator _P);
	// HYPOTHESIS: find(key).
	iterator FUN_006d3a60(const unsigned long& _Kv);
	// HYPOTHESIS: insert(iterator, const value_type&).
	iterator FUN_006e3cf0(iterator _P, const node::value& _X);
};

struct fn_005d9441_tree::value_type
{
	value_type(tObject_006e3000_shared* const& _V1, const fn_0060ac82_tree::iterator& _V2)
		: first(_V1), second(_V2) {}
	tRef_006e38a0 first;
	fn_0060ac82_tree::iterator second;
};

struct fn_005d9441_tree::node
{
	node *_Left;
	node *_Parent;
	node *_Right;
	value_type _Value;
	char _Color;
	char _Isnil;
};

inline fn_005d9441_tree::value_type &fn_005d9441_tree::iterator::operator*() const
{
	return (_Ptr->_Value);
}

// HYPOTHESIS: the timer table that tObject_006e3000 holds from +0x30; the
// second tree starts at +0x14 as its m44 does.
class tObject_006e36a0
{
public:
	fn_0060ac82_tree::iterator FUN_006e36a0(fn_0060ac82_tree::iterator param_1);
	fn_0060ac82_tree::iterator FUN_006e3790(fn_0060ac82_tree::iterator param_1, fn_0060ac82_tree::iterator param_2);

	fn_0060ac82_tree mTimers;
	fn_005d9441_tree m14;
};

// FUNCTION: STARFLEETOP 0x006e36a0
fn_0060ac82_tree::iterator tObject_006e36a0::FUN_006e36a0(fn_0060ac82_tree::iterator param_1)
{
	tObject_006dbc40* entry = (*param_1).second;
	mTimers.FUN_0060ac82(param_1++);
	// MATCH-ONLY: the copy loads m04 before the return slot's address.
	fn_005d9441_tree::iterator where = entry->m04;
	m14.FUN_005d9441(where);
	entry->FUN_006dbc40();
	delete entry;
	return param_1;
}

// HYPOTHESIS: a member of the list's owner that erases one entry and returns
// the next, as FUN_006e36a0 does for its tree; the original reloads the next
// node inside the inlined erase, so the iterator is advanced before it.
// FUNCTION: STARFLEETOP 0x006e3750
std::list<tObject_005d6a66>::iterator tObject_006e3640::FUN_006e3750(std::list<tObject_005d6a66>::iterator param_1)
{
	mList.erase(param_1++);
	return param_1;
}

// HYPOTHESIS: erase(first, last) of the timer table, with FUN_006e36a0's body
// written out for each step.
// FUNCTION: STARFLEETOP 0x006e3790
fn_0060ac82_tree::iterator tObject_006e36a0::FUN_006e3790(fn_0060ac82_tree::iterator param_1, fn_0060ac82_tree::iterator param_2)
{
	while (param_1 != param_2)
	{
		// MATCH-ONLY: the original keeps &m14 in a register across the loop.
		fn_005d9441_tree* tree = &m14;
		fn_0060ac82_tree::iterator it = param_1;
		tObject_006dbc40* entry = (*it).second;
		mTimers.FUN_0060ac82(it++);
		fn_005d9441_tree::iterator where = entry->m04;
		tree->FUN_005d9441(where);
		entry->FUN_006dbc40();
		delete entry;
		param_1 = it;
	}
	return (param_1);
}

// HYPOTHESIS: an object holding a tree of the same layout as the timer table
// and a flag at +0x28; FUN_006e3830 copies another one into it entry by entry,
// FUN_006e38a0 taking each key by reference and its value.
class tObject_006e3830
{
public:
	void FUN_006e3830(tObject_006e3830& param_1);
	bool FUN_006e38a0(const unsigned long& param_1, tObject_006e3000_shared* const& param_2);

	fn_0060ac82_tree mTree;
	fn_005d9441_tree m14;
	char m28;
};

// FUNCTION: STARFLEETOP 0x006e3830
void tObject_006e3830::FUN_006e3830(tObject_006e3830& param_1)
{
	m28 = param_1.m28;
	for (fn_0060ac82_tree::iterator it = param_1.mTree.begin(); it != param_1.mTree.end(); ++it)
		FUN_006e38a0((*it).first, (*it).second->m00);
}

// HYPOTHESIS: m28 allows several entries under one key; each entry also goes
// into the second tree, keyed by its object, which points back at it.
// FUNCTION: STARFLEETOP 0x006e38a0
bool tObject_006e3830::FUN_006e38a0(const unsigned long& param_1, tObject_006e3000_shared* const& param_2)
{
	fn_0060ac82_tree::iterator where = mTree.FUN_006d3a60(param_1);
	if (!m28 && where != mTree.end())
		return false;
	std::pair<fn_005d9441_tree::iterator, bool> result = m14.FUN_006e3f40(fn_005d9441_tree::value_type(param_2, mTree.end()));
	if (!result.second)
		return false;
	tObject_006dbc40* entry = new tObject_006dbc40(param_2, result.first);
	fn_0060ac82_tree::iterator inserted = mTree.FUN_006e3cf0(where, fn_0060ac82_tree::node::value(param_1, entry));
	(*result.first).second = inserted;
	return true;
}

// HYPOTHESIS: the SDK STL's _Tree::insert(iterator, const value_type&), with
// FUN_006e4360 as _Insert; it passes _Head, not the hint, as _X.
// FUNCTION: STARFLEETOP 0x006e39e0
fn_0060ac82_tree::iterator fn_0060ac82_tree::FUN_006e39e0(iterator _P, const node::value& _V)
{
	if (size() == 0)
		;
	else if (_P == begin())
		{if (key_compare(_Kfn()(_V), _Key(_P._Mynode())))
			return (FUN_006e4360(_Head, _P._Mynode(), _V)); }
	else if (_P == end())
		{if (key_compare(_Key(_Rmost()), _Kfn()(_V)))
			return (FUN_006e4360(_Nil, _Rmost(), _V)); }
	else
		{iterator _Pb = _P;
		if (key_compare(_Key((--_Pb)._Mynode()), _Kfn()(_V))
			&& key_compare(_Kfn()(_V), _Key(_P._Mynode())))
			{if (_Right(_Pb._Mynode()) == _Nil)
				return (FUN_006e4360(_Nil, _Pb._Mynode(), _V));
			else
				return (FUN_006e4360(_Head, _P._Mynode(), _V)); }}
	return (insert(_V).first);
}
