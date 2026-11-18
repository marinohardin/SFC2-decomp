// Unit:      unsorted/fn_00571000.cpp
// Functions: 0x00571018 to 0x00571e33, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)
#include <string>
#include <typeinfo>
#include "Common/Thread/Optex.h"
#include "Common/DynamicAllocator.h"
#include "Common/Exceptions/Validate.h"
#include "Common/Database/DatabaseID.h"
#include "Common/Database/DatabaseObject.h"
// Inline in the SDK; Optex.h declares it only.
inline tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->Leave();
	}
}

// HYPOTHESIS: the database object type this instantiation of the SDK's member template
// tDatabase::AssignObjectToDB handles (virtual slots 0x2c SetLockID, 0x30 GetID, 0x3c
// AttachToDB); its name and members are not recovered.
class fn_00571018_object : public tAllocatedClass {
public:
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void SetLockID(long lockID);
	virtual const nDatabase::tID& GetID();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void AttachToDB(const nDatabase::tID& id);
	fn_00571018_object& operator=(const fn_00571018_object& other);
};

// HYPOTHESIS: the database's storage; slot 4 is AddToStorage.
class fn_00571018_storage {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual nDatabase::tID AddToStorage(fn_00571018_object* pObject);
};

// HYPOTHESIS: the callee 0x00705900 is nDatabase::tDatabase::AddObjectToDBIndex(tDatabaseObject*,
// iAllocatorID); tDatabase is declared only inside Common/Database/FileDatabaseMgr.cpp, so no
// second copy is made here and the symbol differs until it moves to a header.
// HYPOTHESIS: stands in for nDatabase::tDatabase (fAccess at +0x4, fDatabaseStorage at +0x40)
// until its header is rebuilt; VC6 cannot instantiate the SDK's member template here without
// its caller.
class fn_00571018_owner {
public:
	virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2(); virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5(); virtual void vfunction6(); virtual void vfunction7(); virtual long vfunction8(long a); virtual void vfunction9();
	void AddObjectToDBIndex(fn_00571018_object* pActualObject, iAllocatorID allocID); void fn_0054e4dc(long a);
	void fn_00571018(fn_00571018_object* pObject);
	void fn_0057118e(fn_00571018_object* pObject);
	void fn_00571304(fn_00571018_object* pObject);
	void fn_0057147a(fn_00571018_object* pObject);
	void fn_005715f0(long a, fn_00571018_owner* b);
	tOptex fAccess;
	char fn_00571018_pad[0x40 - 4 - sizeof(tOptex)];
	fn_00571018_storage* fDatabaseStorage;
};

// FUNCTION: STARFLEETOP 0x00571018
void fn_00571018_owner::fn_00571018(fn_00571018_object* pObject)
{
	tOptex::tGuard guard(&fAccess);
	// MATCH-ONLY: the original releases the guard explicitly after the tID temporary is gone
	// (no state reset before Leave); a block round the body reproduces that.
	{
	// MATCH-ONLY: the assertions name the original's Database.h (this is that header's member
	// template); the named #line gives __FILE__ that header, restored after it.
#line 152 "../Common/Database/Database.h"
	validate(pObject);
	iAllocatorID allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(*pObject));
#line 154
	validate(allocID != kInvalidAllocatorID);
	fn_00571018_object* pActualObject = static_cast<fn_00571018_object*>(tDynamicAllocator::Instance()->mAllocateClass(allocID));
#line 157
	validate(pActualObject);
	*pActualObject = *pObject;
#line 160
	validate(pActualObject->GetID().IsValid() == false);
#line 161 "fn_00571000.cpp"
	const nDatabase::tID& newID = fDatabaseStorage->AddToStorage(pActualObject);
	pActualObject->AttachToDB(newID);
	pActualObject->SetLockID(0);

	AddObjectToDBIndex(pActualObject, allocID);

	*pObject = *pActualObject;
	}
	guard.Leave();
}
#line 102
// HYPOTHESIS: a third instantiation of the same member template, for another database object type.
// FUNCTION: STARFLEETOP 0x00571304
void fn_00571018_owner::fn_00571304(fn_00571018_object* pObject)
{
	tOptex::tGuard guard(&fAccess);
	{
	// MATCH-ONLY: the assertions name the original's Database.h (this is that header's member
	// template); the named #line gives __FILE__ that header, restored after it.
#line 152 "../Common/Database/Database.h"
	validate(pObject);
	iAllocatorID allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(*pObject));
#line 154
	validate(allocID != kInvalidAllocatorID);
	fn_00571018_object* pActualObject = static_cast<fn_00571018_object*>(tDynamicAllocator::Instance()->mAllocateClass(allocID));
#line 157
	validate(pActualObject);
	*pActualObject = *pObject;
#line 160
	validate(pActualObject->GetID().IsValid() == false);
#line 161 "fn_00571000.cpp"
	const nDatabase::tID& newID = fDatabaseStorage->AddToStorage(pActualObject);
	pActualObject->AttachToDB(newID);

	pActualObject->SetLockID(0);

	AddObjectToDBIndex(pActualObject, allocID);

	*pObject = *pActualObject;
	}
	guard.Leave();
}

#line 136
// HYPOTHESIS: a second instantiation of the same member template as 0x00571018, for another
// database object type; names are not recovered.
// FUNCTION: STARFLEETOP 0x0057118e
void fn_00571018_owner::fn_0057118e(fn_00571018_object* pObject)
{
	tOptex::tGuard guard(&fAccess);
	{
	// MATCH-ONLY: the assertions name the original's Database.h (this is that header's member
	// template); the named #line gives __FILE__ that header, restored after it.
#line 152 "../Common/Database/Database.h"
	validate(pObject);
	iAllocatorID allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(*pObject));
#line 154
	validate(allocID != kInvalidAllocatorID);
	fn_00571018_object* pActualObject = static_cast<fn_00571018_object*>(tDynamicAllocator::Instance()->mAllocateClass(allocID));
#line 157
	validate(pActualObject);
	*pActualObject = *pObject;
#line 160
	validate(pActualObject->GetID().IsValid() == false);
#line 161 "fn_00571000.cpp"
	const nDatabase::tID& newID = fDatabaseStorage->AddToStorage(pActualObject);
	pActualObject->AttachToDB(newID);

	pActualObject->SetLockID(0);

	AddObjectToDBIndex(pActualObject, allocID);

	*pObject = *pActualObject;
	}
	guard.Leave();
}

#line 170
// HYPOTHESIS: a fourth instantiation of the same member template, for another database object type.
// FUNCTION: STARFLEETOP 0x0057147a
void fn_00571018_owner::fn_0057147a(fn_00571018_object* pObject)
{
	tOptex::tGuard guard(&fAccess);
	{
	// MATCH-ONLY: the assertions name the original's Database.h (this is that header's member
	// template); the named #line gives __FILE__ that header, restored after it.
#line 152 "../Common/Database/Database.h"
	validate(pObject);
	iAllocatorID allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(*pObject));
#line 154
	validate(allocID != kInvalidAllocatorID);
	fn_00571018_object* pActualObject = static_cast<fn_00571018_object*>(tDynamicAllocator::Instance()->mAllocateClass(allocID));
#line 157
	validate(pActualObject);
	*pActualObject = *pObject;
#line 160
	validate(pActualObject->GetID().IsValid() == false);
#line 161 "fn_00571000.cpp"
	const nDatabase::tID& newID = fDatabaseStorage->AddToStorage(pActualObject);
	pActualObject->AttachToDB(newID);

	pActualObject->SetLockID(0);

	AddObjectToDBIndex(pActualObject, allocID);

	*pObject = *pActualObject;
	}
	guard.Leave();
}

#line 202
// FUNCTION: STARFLEETOP 0x005715f0
// HYPOTHESIS: the second parameter is another object of the same stand-in class; its type,
// the callee 0x0054e4dc and the virtual slots 8 and 9 are not recovered.
void fn_00571018_owner::fn_005715f0(long a, fn_00571018_owner* b)
{
	tOptex::tGuard guard(&fAccess);
	{
	long r = vfunction8(a);
	b->fn_0054e4dc(r);
	b->vfunction9();
	}
	guard.Leave();
}

// HYPOTHESIS: unrecovered class; +8 holds a callback taking (+0xc + a) in ecx, whose char
// result is compared with the byte at +0x10. The __fastcall pointer only models that call.
class fn_00571648_owner {
public:
	bool fn_00571648(int a);
	bool fn_0057165f(int a);
	int fn_00571648_pad[2];
	char (__fastcall *fn_00571648_func)(int);
	int fn_00571648_base;
	char fn_00571648_byte;
};

// FUNCTION: STARFLEETOP 0x00571648
bool fn_00571648_owner::fn_00571648(int a)
{
	bool same = fn_00571648_func(fn_00571648_base + a) == fn_00571648_byte;
	return same;
}

// FUNCTION: STARFLEETOP 0x0057165f
bool fn_00571648_owner::fn_0057165f(int a)
{
	bool less = ((int (__fastcall *)(int))fn_00571648_func)(fn_00571648_base + a) < *(int*)&fn_00571648_byte;
	return less;
}
// HYPOTHESIS: unrecovered class pair: a polymorphic base (vtable 0x007b2ba0) whose destructor
// adds no unwind state, two longs at +8 and +0xc, and in the derived class (vtable 0x007b2b98)
// a std::string at +0x10 copied from the by-value parameter. The base's virtual slots are not
// recovered.
class fn_00571676_base {
public:
	fn_00571676_base(long a, long b) : fn_00571676_a(a), fn_00571676_b(b) {}
	virtual void vfunction0();
	int fn_00571676_base_pad;
	long fn_00571676_a;
	long fn_00571676_b;
};
class fn_00571676_owner : public fn_00571676_base {
public:
	fn_00571676_owner(long a, long b, std::string s);
	std::string fn_00571676_str;
	bool fn_005716e0(int a);
};

// FUNCTION: STARFLEETOP 0x00571676
fn_00571676_owner::fn_00571676_owner(long a, long b, std::string s)
	: fn_00571676_base(a, b), fn_00571676_str(s)
{
}
// HYPOTHESIS: stand-in for the callee's object type; the call is a thiscall.
class fn_005716e0_target {
public:
	std::string fn_get();
};
// FUNCTION: STARFLEETOP 0x005716e0
// HYPOTHESIS: the long at +8 is a thiscall function pointer returning a std::string, applied to
// the long at +0xc plus the argument.
bool fn_00571676_owner::fn_005716e0(int a)
{
	typedef std::string (fn_005716e0_target::*fn_get_t)();
	if ((((fn_005716e0_target*)(fn_00571676_b + a))->*(*(fn_get_t*)&fn_00571676_a))() == fn_00571676_str)
		return true;
	return false;
}
// HYPOTHESIS: stands in for tHexPoint (0x14 bytes, copy constructor 0x005b6973, destructor 0x005b6a36), declared only inside unsorted/tClientCharacter.cpp; the symbols differ until it moves to a header.
class fn_00571734_point {
public:	fn_00571734_point(const fn_00571734_point &other);
	~fn_00571734_point();	bool operator==(const fn_00571734_point &other) const;
	int m_00[5]; };
// HYPOTHESIS: a second class on the base of 0x00571676 (vtable 0x007b2ba8), holding a tHexPoint at +0x10 copied from the by-value parameter.
class fn_00571734_owner : public fn_00571676_base {
public:	fn_00571734_owner(long a, long b, fn_00571734_point p);
	fn_00571734_point fn_00571734_pt;
	bool fn_00571786(int a);
};
// FUNCTION: STARFLEETOP 0x00571734
fn_00571734_owner::fn_00571734_owner(long a, long b, fn_00571734_point p)
	: fn_00571676_base(a, b), fn_00571734_pt(p)
{}
// FUNCTION: STARFLEETOP 0x00571786
bool fn_00571734_owner::fn_00571786(int a)
{ typedef fn_00571734_point (fn_005716e0_target::*fn_get_t)(); bool same = (((fn_005716e0_target*)(fn_00571676_b + a))->*(*(fn_get_t*)&fn_00571676_a))() == fn_00571734_pt; return same ? true : false; } class fn_00715ac0_object { public: char fn_00715ac0(long v); };
class fn_005717d6_owner : public fn_00571676_base {
public:	long fn_005717d6_v;
	bool fn_005717d6(int a);
};
// FUNCTION: STARFLEETOP 0x005717d6
// HYPOTHESIS: the getter returns an object pointer and 0x00715ac0 is a thiscall on it.
bool fn_005717d6_owner::fn_005717d6(int a) { long v = fn_005717d6_v; typedef fn_00715ac0_object* (fn_005716e0_target::*fn_t)(); fn_00715ac0_object* o = (((fn_005716e0_target*)(fn_00571676_b + a))->*(*(fn_t*)&fn_00571676_a))(); char r = o->fn_00715ac0(v); bool same = r != 0; return same ? true : false; }
#include <map>
#include <vector>
// HYPOTHESIS: a count query on the database: the owner's map (at +0x18) lists the objects of
// one type; slot 2 of the query object is its predicate. Types and names are not recovered.
class fn_005717f7_object {
public:
	virtual void vfunction0();
};
class fn_005717f7_query {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual bool operator()(fn_005717f7_object* pObject);
};
struct fn_005717f7_vector {
	~fn_005717f7_vector();
	void* m_allocator;
	void** m_first;
	void** m_last;
	void** m_end;
};
// HYPOTHESIS: the per-type index is a std::map, allocator id to object pointers; 0x005719df is its operator[].
typedef std::map<long, std::vector<fn_005717f7_object*> > fn_005717f7_index;
struct fn_005717f7_map : public fn_005717f7_index {
	fn_005717f7_vector& fn_005719df(const long& key);
};
class fn_005717f7_owner {
public:
	virtual void vfunction0();
	long fn_005717f7(fn_005717f7_query& query);
	tOptex fAccess;
	// the map follows the optex directly (+0x18)
	fn_005717f7_map fTypeIndex;
};
#line 340
// FUNCTION: STARFLEETOP 0x005717f7
long fn_005717f7_owner::fn_005717f7(fn_005717f7_query& query)
{
	tOptex::tGuard guard(&fAccess);
	long count = 0;
	long allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(fn_005717f7_object));
	// MATCH-ONLY: the assertion names the original's Database.h (the body is that header's
	// member template); the named #line gives __FILE__ that header until it is rebuilt there.
#line 306 "../Common/Database/Database.h"
	validate(allocID != kInvalidAllocatorID);
#line 349 "fn_00571000.cpp"
	fn_005717f7_vector& objects = fTypeIndex.fn_005719df(allocID);
	for (void** i = objects.m_first; i != objects.m_last; i++) {
		fn_005717f7_object* pObject = (fn_005717f7_object*)*i;
		if (query(pObject))
			count++;
	}
	guard.Leave();
	return count;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x0056eee7 is the
// destructor body, flag bit 0 asks for operator delete.
class fn_005718ab_object {
public:
	void fn_0056eee7();
	fn_005718ab_object* fn_005718ab(unsigned char flags);
};
#line 369
// FUNCTION: STARFLEETOP 0x005718ab
fn_005718ab_object* fn_005718ab_object::fn_005718ab(unsigned char flags)
{
	fn_0056eee7();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x0056f041 is the
// destructor body, flag bit 0 asks for operator delete.
class fn_005718c7_object {
public:
	void fn_0056f041();
	fn_005718c7_object* fn_005718c7(unsigned char flags);
};
#line 389
// FUNCTION: STARFLEETOP 0x005718c7
fn_005718c7_object* fn_005718c7_object::fn_005718c7(unsigned char flags)
{
	fn_0056f041();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x0056f265 is the
// destructor body, flag bit 0 asks for operator delete.
class fn_005718e3_object {
public:
	void fn_0056f265();
	fn_005718e3_object* fn_005718e3(unsigned char flags);
};

// FUNCTION: STARFLEETOP 0x005718e3
fn_005718e3_object* fn_005718e3_object::fn_005718e3(unsigned char flags)
{
	fn_0056f265();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x0056f4c1 is the
// destructor body, flag bit 0 asks for operator delete.
class fn_005718ff_object {
public:
	void fn_0056f4c1();
	fn_005718ff_object* fn_005718ff(unsigned char flags);
};

// FUNCTION: STARFLEETOP 0x005718ff
fn_005718ff_object* fn_005718ff_object::fn_005718ff(unsigned char flags)
{
	fn_0056f4c1();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x0056f6e5 is the
// destructor body, flag bit 0 asks for operator delete.
class fn_0057191b_object {
public:
	void fn_0056f6e5();
	fn_0057191b_object* fn_0057191b(unsigned char flags);
};

// FUNCTION: STARFLEETOP 0x0057191b
fn_0057191b_object* fn_0057191b_object::fn_0057191b(unsigned char flags)
{
	fn_0056f6e5();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x0056f9b9 is the
// destructor body, flag bit 0 asks for operator delete.
class fn_00571937_object {
public:
	void fn_0056f9b9();
	fn_00571937_object* fn_00571937(unsigned char flags);
};

// FUNCTION: STARFLEETOP 0x00571937
fn_00571937_object* fn_00571937_object::fn_00571937(unsigned char flags)
{
	fn_0056f9b9();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x0056fbd7 is the
// destructor body, flag bit 0 asks for operator delete.
class fn_00571953_object {
public:
	void fn_0056fbd7();
	fn_00571953_object* fn_00571953(unsigned char flags);
};

// FUNCTION: STARFLEETOP 0x00571953
fn_00571953_object* fn_00571953_object::fn_00571953(unsigned char flags)
{
	fn_0056fbd7();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x0056fcb9 is the
// destructor body, flag bit 0 asks for operator delete.
class fn_0057196f_object {
public:
	void fn_0056fcb9();
	fn_0057196f_object* fn_0057196f(unsigned char flags);
};

// FUNCTION: STARFLEETOP 0x0057196f
fn_0057196f_object* fn_0057196f_object::fn_0057196f(unsigned char flags)
{
	fn_0056fcb9();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x0056fedd is the
// destructor body, flag bit 0 asks for operator delete.
class fn_0057198b_object {
public:
	void fn_0056fedd();
	fn_0057198b_object* fn_0057198b(unsigned char flags);
};

// FUNCTION: STARFLEETOP 0x0057198b
fn_0057198b_object* fn_0057198b_object::fn_0057198b(unsigned char flags)
{
	fn_0056fedd();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x0056ffc5 is the
// destructor body, flag bit 0 asks for operator delete.
class fn_005719a7_object {
public:
	void fn_0056ffc5();
	fn_005719a7_object* fn_005719a7(unsigned char flags);
};

// FUNCTION: STARFLEETOP 0x005719a7
fn_005719a7_object* fn_005719a7_object::fn_005719a7(unsigned char flags)
{
	fn_0056ffc5();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the body of a scalar deleting destructor, written as a function: 0x00570309 is the
// destructor body, flag bit 0 asks for operator delete.
class fn_005719c3_object {
public:
	void fn_00570309();
	fn_005719c3_object* fn_005719c3(unsigned char flags);
};

// FUNCTION: STARFLEETOP 0x005719c3
fn_005719c3_object* fn_005719c3_object::fn_005719c3(unsigned char flags)
{
	fn_00570309();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// FUNCTION: STARFLEETOP 0x005719df
fn_005717f7_vector& fn_005717f7_map::fn_005719df(const long& key)
{
	iterator it = insert(value_type(key, std::vector<fn_005717f7_object*>())).first;
	// MATCH-ONLY: the struct above stands for the std::vector it has the layout of.
	return *(fn_005717f7_vector*)&(*it).second;
}
class fn_00571a58_inner;
class fn_00571e33_elem;
class fn_00571a58_owner {
public:	char fn_00571a58_pad[4];
	fn_00571a58_inner* fn_00571a58_first;
	fn_00571a58_inner* fn_00571a58_ptr;
	fn_00571a58_inner* fn_00571a58_limit;
	void fn_00571a58(int arg);
	void fn_00571a69(int arg);
	void fn_00571a9a(int arg);
	void fn_00571aab(int arg);
	void fn_00571abc(int arg);
	unsigned int size() const;
	void fn_00571bda(fn_00571a58_inner* pos, unsigned int m, const fn_00571a58_inner& x);
	void fn_00571e33(fn_00571e33_elem* pos, unsigned int m, const fn_00571e33_elem& x);
	void fn_00571acd(int arg);
};
#line 573
// FUNCTION: STARFLEETOP 0x00571a58
void fn_00571a58_owner::fn_00571a58(int arg)
{
	fn_00571bda(fn_00571a58_ptr, 1, *(fn_00571a58_inner*)arg);
}

// HYPOTHESIS: a forwarding member like 0x00571a58; 0x005de471 takes the object at +8, a constant 1 and the argument.
void __stdcall fn_005de471(fn_00571a58_inner* inner, int flag, int arg);

// FUNCTION: STARFLEETOP 0x00571a69
void fn_00571a58_owner::fn_00571a69(int arg)
{
	fn_005de471(fn_00571a58_ptr, 1, arg);
}

// HYPOTHESIS: the out-of-line destructor of the std::vector the struct above stands for; the body
// is the library's ~vector with the pointer elements' empty destroy loop written out.
// FUNCTION: STARFLEETOP 0x00571a7a
fn_005717f7_vector::~fn_005717f7_vector()
{
	for (void** i = m_first; i != m_last; ++i)
		;
	::operator delete(m_first);
	m_first = 0, m_last = 0, m_end = 0;
}

// HYPOTHESIS: a forwarding member like 0x00571a58; the vector at 0x00571e33 has 0x98-byte elements.
// (the call below passes the object at +8, a constant 1 and the argument.)

// FUNCTION: STARFLEETOP 0x00571a9a
void fn_00571a58_owner::fn_00571a9a(int arg)
{
	fn_00571e33((fn_00571e33_elem*)fn_00571a58_ptr, 1, *(fn_00571e33_elem*)arg);
}

// HYPOTHESIS: a forwarding member like 0x00571a58; 0x005720aa takes the object at +8, a constant 1 and the argument.
void __stdcall fn_005720aa(fn_00571a58_inner* inner, int flag, int arg);

// FUNCTION: STARFLEETOP 0x00571aab
void fn_00571a58_owner::fn_00571aab(int arg)
{
	fn_005720aa(fn_00571a58_ptr, 1, arg);
}

// HYPOTHESIS: a forwarding member like 0x00571a58; 0x005722e0 takes the object at +8, a constant 1 and the argument.
void __stdcall fn_005722e0(fn_00571a58_inner* inner, int flag, int arg);

// FUNCTION: STARFLEETOP 0x00571abc
void fn_00571a58_owner::fn_00571abc(int arg)
{
	fn_005722e0(fn_00571a58_ptr, 1, arg);
}

// HYPOTHESIS: a forwarding member like 0x00571a58; 0x0057253a takes the object at +8, a constant 1 and the argument.
void __stdcall fn_0057253a(fn_00571a58_inner* inner, int flag, int arg);

// FUNCTION: STARFLEETOP 0x00571acd
void fn_00571a58_owner::fn_00571acd(int arg)
{
	fn_0057253a(fn_00571a58_ptr, 1, arg);
}

// HYPOTHESIS: the destructor of the map's value_type (long key, vector at +4), which destroys the vector.
struct fn_00571ade_pair {
	long first;
	fn_005717f7_vector second;
	~fn_00571ade_pair();
};

// FUNCTION: STARFLEETOP 0x00571ade
fn_00571ade_pair::~fn_00571ade_pair()
{
}

// HYPOTHESIS: the library's tree node (left, parent, right, key at +0xc) and the unique-key
// insert of the map above; the iterator decrement is 0x0042dcfb, the node insertion 0x005727a2.
struct fn_00571ae6_node {
	fn_00571ae6_node* left;
	fn_00571ae6_node* parent;
	fn_00571ae6_node* right;
	long key;
};
struct fn_00571ae6_iter {
	fn_00571ae6_node* node;
	void fn_0042dcfb();
};
struct fn_00571ae6_result {
	fn_00571ae6_node* node;
	bool inserted;
	fn_00571ae6_result(fn_00571ae6_node* n, bool i) : node(n), inserted(i) {}
};
struct fn_00571ae6_tree {
	int pad;
	fn_00571ae6_node* head;
	fn_00571ae6_node* nil;
	bool multi;
	fn_00571ae6_result fn_00571ae6(const long* value);
	fn_00571ae6_iter fn_005727a2(fn_00571ae6_node* x, fn_00571ae6_node* y, const long* value);
};

// FUNCTION: STARFLEETOP 0x00571ae6
fn_00571ae6_result fn_00571ae6_tree::fn_00571ae6(const long* value)
{
	fn_00571ae6_node* x = head->parent;
	fn_00571ae6_node* y = head;
	bool addLeft = true;
	while (x != nil) {
		y = x;
		addLeft = *value < x->key;
		x = addLeft ? x->left : x->right;
	}
	if (multi)
		return fn_00571ae6_result(fn_005727a2(x, y, value).node, true);
	fn_00571ae6_iter p;
	p.node = y;
	if (addLeft) {
		if (p.node == head->left)
			return fn_00571ae6_result(fn_005727a2(x, y, value).node, true);
		p.fn_0042dcfb();
	}
	if (p.node->key < *value)
		return fn_00571ae6_result(fn_005727a2(x, y, value).node, true);
	return fn_00571ae6_result(p.node, false);
}

void* __cdecl operator new(unsigned int);

struct fn_00571b81_vector {
	char alloc;
	long** first;
	long** last;
	long** limit;
	fn_00571b81_vector(const fn_00571b81_vector& x);
	int size() const
	{
		if (first == 0)
			return 0;
		return last - first;
	}
};

// MATCH-ONLY: the allocator's clamp, inlined by the original
inline long** fn_00571b81_allocate(int n)
{
	if (n < 0)
		n = 0;
	return (long**)operator new((unsigned int)n * 4);
}

// MATCH-ONLY: the uninitialized copy, inlined by the original
inline long** fn_00571b81_copy(long** f, long** l, long** d)
{
	for (; f != l; ++d, ++f)
		if (d)
			*d = *f;
	return d;
}

// FUNCTION: STARFLEETOP 0x00571b81
fn_00571b81_vector::fn_00571b81_vector(const fn_00571b81_vector& x)
{
	// HYPOTHESIS: a vector of pointers copy constructor
	alloc = x.alloc;
	first = fn_00571b81_allocate(x.size());
	last = fn_00571b81_copy(x.first, x.last, first);
	limit = last;
}

// HYPOTHESIS: the element of the vector at 0x00571a58: a database object of 0x4c bytes with one
// long at 0x48; its copy constructor is 0x0057291e and its assignment goes through 0x006fa870.
class fn_00571a58_inner : public nDatabase::tDatabaseObject {
public:
	fn_00571a58_inner& operator=(const fn_00571a58_inner& other);
	long fn_00571a58_48;
};

inline fn_00571a58_inner& fn_00571a58_inner::operator=(const fn_00571a58_inner& other)
{
	*(nDatabase::tDatabaseObject*)this = other;
	fn_00571a58_48 = other.fn_00571a58_48;
	return *this;
}

inline unsigned int fn_00571a58_owner::size() const
{
	return fn_00571a58_first == 0 ? 0 : fn_00571a58_ptr - fn_00571a58_first;
}

// HYPOTHESIS: the library's _Construct for this element type, out of line (cdecl).
void __cdecl fn_0057291e(fn_00571a58_inner* p, const fn_00571a58_inner& v);

// MATCH-ONLY: the allocator's clamp, inlined by the original
inline fn_00571a58_inner* fn_00571bda_allocate(int n)
{
	if (n < 0)
		n = 0;
	return (fn_00571a58_inner*)operator new((unsigned int)n * sizeof(fn_00571a58_inner));
}

// MATCH-ONLY: the library's uninitialized copy, inlined by the original
inline fn_00571a58_inner* fn_00571bda_ucopy(fn_00571a58_inner* f, fn_00571a58_inner* l, fn_00571a58_inner* d)
{
	for (; f != l; ++d, ++f)
		fn_0057291e(d, *f);
	return d;
}

// MATCH-ONLY: the library's uninitialized fill, inlined by the original
inline void fn_00571bda_ufill(fn_00571a58_inner* f, unsigned int n, const fn_00571a58_inner& x)
{
	for (; n > 0; --n, ++f)
		fn_0057291e(f, x);
}

// MATCH-ONLY: the library's fill, inlined by the original
inline void fn_00571bda_fill(fn_00571a58_inner* f, fn_00571a58_inner* l, const fn_00571a58_inner& x)
{
	for (; f != l; ++f)
		*f = x;
}

// MATCH-ONLY: the library's destroy loop, inlined by the original
inline void fn_00571bda_destroy(fn_00571a58_inner* f, fn_00571a58_inner* l)
{
	for (; f != l; ++f)
		f->~fn_00571a58_inner();
}

// MATCH-ONLY: the library's copy_backward, inlined by the original
inline void fn_00571bda_copy_backward(fn_00571a58_inner* f, fn_00571a58_inner* l, fn_00571a58_inner* d)
{
	while (f != l)
		*--d = *--l;
}

// FUNCTION: STARFLEETOP 0x00571bda
// HYPOTHESIS: the library's vector::insert(iterator, size_type, const T&), with the helpers inlined.
void fn_00571a58_owner::fn_00571bda(fn_00571a58_inner* pos, unsigned int m, const fn_00571a58_inner& x)
{
	fn_00571a58_inner* last = fn_00571a58_ptr;
	if ((unsigned int)(fn_00571a58_limit - last) < m) {
		unsigned int cnt = size() + (m < size() ? size() : m);
		fn_00571a58_inner* s = fn_00571bda_allocate(cnt);
		fn_00571a58_inner* q = fn_00571bda_ucopy(fn_00571a58_first, pos, s);
		fn_00571bda_ufill(q, m, x);
		fn_00571bda_ucopy(pos, fn_00571a58_ptr, q + m);
		fn_00571bda_destroy(fn_00571a58_first, fn_00571a58_ptr);
		operator delete(fn_00571a58_first);
		fn_00571a58_limit = s + cnt;
		fn_00571a58_ptr = s + size() + m;
		fn_00571a58_first = s;
	} else if ((unsigned int)(fn_00571a58_ptr - pos) < m) {
		fn_00571bda_ucopy(pos, fn_00571a58_ptr, pos + m);
		fn_00571bda_ufill(fn_00571a58_ptr, m - (fn_00571a58_ptr - pos), x);
		fn_00571bda_fill(pos, fn_00571a58_ptr, x);
		fn_00571a58_ptr += m;
	} else if (0 < m) {
		fn_00571bda_ucopy(fn_00571a58_ptr - m, fn_00571a58_ptr, fn_00571a58_ptr);
		fn_00571bda_copy_backward(pos, fn_00571a58_ptr - m, fn_00571a58_ptr);
		fn_00571bda_fill(pos, pos + m, x);
		fn_00571a58_ptr += m;
	}
}

// HYPOTHESIS: the element of the vector at 0x00571e33: a 0x98-byte object with a virtual destructor;
// its copy constructor is 0x00572965 and its assignment is 0x0053edc7.
class fn_00571e33_elem {
public:
	virtual ~fn_00571e33_elem();
	fn_00571e33_elem& operator=(const fn_00571e33_elem& other);
	char fn_00571e33_pad[0x94];
};

void __cdecl fn_00572965(fn_00571e33_elem* p, const fn_00571e33_elem& v);

// MATCH-ONLY: the allocator's clamp, inlined by the original
inline fn_00571e33_elem* fn_00571e33_allocate(int n)
{
	if (n < 0)
		n = 0;
	return (fn_00571e33_elem*)operator new((unsigned int)n * sizeof(fn_00571e33_elem));
}

// MATCH-ONLY: the library's uninitialized copy, inlined by the original
inline fn_00571e33_elem* fn_00571e33_ucopy(fn_00571e33_elem* f, fn_00571e33_elem* l, fn_00571e33_elem* d)
{
	for (; f != l; ++d, ++f)
		fn_00572965(d, *f);
	return d;
}

// MATCH-ONLY: the library's uninitialized fill, inlined by the original
inline void fn_00571e33_ufill(fn_00571e33_elem* f, unsigned int n, const fn_00571e33_elem& x)
{
	for (; n > 0; --n, ++f)
		fn_00572965(f, x);
}

// MATCH-ONLY: the library's fill, inlined by the original
inline void fn_00571e33_fill(fn_00571e33_elem* f, fn_00571e33_elem* l, const fn_00571e33_elem& x)
{
	for (; f != l; ++f)
		*f = x;
}

// MATCH-ONLY: the library's destroy loop, inlined by the original
inline void fn_00571e33_destroy(fn_00571e33_elem* f, fn_00571e33_elem* l)
{
	for (; f != l; ++f)
		f->~fn_00571e33_elem();
}

// MATCH-ONLY: the library's copy_backward, inlined by the original
inline void fn_00571e33_copy_backward(fn_00571e33_elem* f, fn_00571e33_elem* l, fn_00571e33_elem* d)
{
	while (f != l)
		*--d = *--l;
}

// MATCH-ONLY: the library's size(), inlined by the original
inline unsigned int fn_00571e33_size(fn_00571a58_owner* v)
{
	return v->fn_00571a58_first == 0 ? 0 : (fn_00571e33_elem*)v->fn_00571a58_ptr - (fn_00571e33_elem*)v->fn_00571a58_first;
}

// FUNCTION: STARFLEETOP 0x00571e33
// HYPOTHESIS: the library's vector::insert(iterator, size_type, const T&) for the 0x98-byte element.
void fn_00571a58_owner::fn_00571e33(fn_00571e33_elem* pos, unsigned int m, const fn_00571e33_elem& x)
{
	fn_00571e33_elem* last = (fn_00571e33_elem*)fn_00571a58_ptr;
	if ((unsigned int)((fn_00571e33_elem*)fn_00571a58_limit - last) < m) {
		unsigned int grow = (fn_00571a58_first != 0 && m < fn_00571e33_size(this)) ? fn_00571e33_size(this) : m;
		unsigned int cnt = grow + fn_00571e33_size(this);
		fn_00571e33_elem* s = fn_00571e33_allocate(cnt);
		fn_00571e33_elem* q = fn_00571e33_ucopy((fn_00571e33_elem*)fn_00571a58_first, pos, s);
		fn_00571e33_ufill(q, m, x);
		fn_00571e33_ucopy(pos, (fn_00571e33_elem*)fn_00571a58_ptr, q + m);
		fn_00571e33_destroy((fn_00571e33_elem*)fn_00571a58_first, (fn_00571e33_elem*)fn_00571a58_ptr);
		operator delete(fn_00571a58_first);
		fn_00571a58_limit = (fn_00571a58_inner*)(s + cnt);
		fn_00571a58_ptr = (fn_00571a58_inner*)(s + fn_00571e33_size(this) + m);
		fn_00571a58_first = (fn_00571a58_inner*)s;
	} else if ((unsigned int)((fn_00571e33_elem*)fn_00571a58_ptr - pos) < m) {
		fn_00571e33_ucopy(pos, (fn_00571e33_elem*)fn_00571a58_ptr, pos + m);
		fn_00571e33_ufill((fn_00571e33_elem*)fn_00571a58_ptr, m - ((fn_00571e33_elem*)fn_00571a58_ptr - pos), x);
		fn_00571e33_fill(pos, (fn_00571e33_elem*)fn_00571a58_ptr, x);
		fn_00571a58_ptr = (fn_00571a58_inner*)((fn_00571e33_elem*)fn_00571a58_ptr + m);
	} else if (0 < m) {
		fn_00571e33_ucopy((fn_00571e33_elem*)fn_00571a58_ptr - m, (fn_00571e33_elem*)fn_00571a58_ptr, (fn_00571e33_elem*)fn_00571a58_ptr);
		fn_00571e33_copy_backward(pos, (fn_00571e33_elem*)fn_00571a58_ptr - m, (fn_00571e33_elem*)fn_00571a58_ptr);
		fn_00571e33_fill(pos, pos + m, x);
		fn_00571a58_ptr = (fn_00571a58_inner*)((fn_00571e33_elem*)fn_00571a58_ptr + m);
	}
}
