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
	void AddObjectToDBIndex(fn_00571018_object* pActualObject, iAllocatorID allocID);
	void fn_00571018(fn_00571018_object* pObject);
	void fn_0057118e(fn_00571018_object* pObject);
	void fn_00571304(fn_00571018_object* pObject);
	void fn_0057147a(fn_00571018_object* pObject); void fn_005715f0(long a, fn_00571018_owner* b); void fn_0054e4dc(long a);
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
#line 152
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
#line 152
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
#line 152
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
#line 152
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
	fn_00571734_point fn_00571734_pt;	bool fn_00571786(int a); };
// MATCH-ONLY: kept compact so the unit ends before line 303 (0x0057147a's first line record).
// FUNCTION: STARFLEETOP 0x00571734
fn_00571734_owner::fn_00571734_owner(long a, long b, fn_00571734_point p)
	: fn_00571676_base(a, b), fn_00571734_pt(p)
{}
// FUNCTION: STARFLEETOP 0x00571786
bool fn_00571734_owner::fn_00571786(int a)
{ typedef fn_00571734_point (fn_005716e0_target::*fn_get_t)(); bool same = (((fn_005716e0_target*)(fn_00571676_b + a))->*(*(fn_get_t*)&fn_00571676_a))() == fn_00571734_pt; return same ? true : false; }
class fn_00715ac0_object { public: char fn_00715ac0(long v); }; class fn_005717d6_owner : public fn_00571676_base { public: long fn_005717d6_v; bool fn_005717d6(int a); };
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
