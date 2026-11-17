// Unit:      unsorted/fn_0056F000.cpp
// Functions: 0x0056f041 to 0x0056ffe3, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

#include <typeinfo>
#include "Common/Thread/Optex.h"
#include "Common/DynamicAllocator.h"
#include "Common/Exceptions/Validate.h"
#include "Common/Database/DatabaseObject.h"

class tServCharacter;

// HYPOTHESIS: the member destroyed by 0x00571a7a is a VC6 vector of 32-bit values, the same
// destructor tPanelMgr.cpp gives its vectors (declared only there, as fn_00571a69_vector); this
// declaration carries only the destructor until that type moves to a shared header.
struct fn_00571a7a_allocator {};
struct fn_00571a7a_vector {
	fn_00571a7a_vector(const fn_00571a7a_allocator &al = fn_00571a7a_allocator()) : m_allocator(al), m_first(0), m_last(0), m_end(0) {}
	~fn_00571a7a_vector();
	void fn_00571a69(void *const &value);
	void **begin() { return m_first; }
	void **end() { return m_last; }
	unsigned int size() const { return m_first == 0 ? 0 : m_last - m_first; }
	fn_00571a7a_allocator m_allocator;
	void **m_first;
	void **m_last;
	void **m_end;
};

// HYPOTHESIS: the function sets the vtable of nDatabase::tQuery<tServCharacter> and destroys a
// member at +0x4 through 0x00571a7a; it is taken to be that class's destructor. The SDK's
// tQuery has AddEval and operator() after the destructor, in that order.
namespace nDatabase {
	template <class T> class tQuery {
	public:
		virtual ~tQuery();
		virtual void fn_0056f04f(void *value);
		virtual bool operator()(const T *value) const;
		fn_00571a7a_vector m_results;
	};
}

// FUNCTION: STARFLEETOP 0x0056f041
nDatabase::tQuery<tServCharacter>::~tQuery()
{
}

// FUNCTION: STARFLEETOP 0x0056f04f
void nDatabase::tQuery<tServCharacter>::fn_0056f04f(void *value)
{
	m_results.fn_00571a69(value);
}

// HYPOTHESIS: a database class (0x0056f05f queries the database for it); its members are not
// recovered.
class tServCharacter : public nDatabase::tDatabaseObject {};

using nDatabase::tDatabaseObject;

// Inline in the SDK; Optex.h declares it only. Inline here, the guard's release after Leave()
// folds away as it does in the original.
inline tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->Leave();
	}
}

// HYPOTHESIS: the result container, likely std::vector<tServCharacter>; its push_back is out of
// line at 0x005406d7.
class fn_005406d7_results {
public:
	void push_back(const tServCharacter &value);
};

// HYPOTHESIS: std::map<iAllocatorID, std::vector<tDatabaseObject*> >; its operator[] is out of
// line at 0x005719df.
struct fn_005719df_map {
	fn_00571a7a_vector &fn_005719df(const long &key);
};

// HYPOTHESIS: 0x0056f05f is the SDK's member template nDatabase::tDatabase::LockingQueryDB,
// instantiated for tServCharacter: its assertion names Common/DataBase/Database.h, line 210, and
// slot 10 of the owner's vtable is MultiLock. VC6 cannot instantiate that member template here
// without its caller, so this class stands in for tDatabase (fAccess at +0x4, fTypeIndex at
// +0x18) until the header and the caller are rebuilt. The #line below gives the assertion the
// header's line; its file name stays this unit's (with a file name the build fails).
class fn_0056f05f_owner {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void MultiLock(fn_00571a7a_vector *objects, bool mustLockAll);
	void fn_0056f05f(const nDatabase::tQuery<tServCharacter> &query, fn_005406d7_results *result, bool mustLockAll);
	tOptex fAccess;
	fn_005719df_map fTypeIndex;
};

// FUNCTION: STARFLEETOP 0x0056f05f
void fn_0056f05f_owner::fn_0056f05f(const nDatabase::tQuery<tServCharacter> &query, fn_005406d7_results *result, bool mustLockAll)
{
	tOptex::tGuard guard(&fAccess);
	long allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(tServCharacter));
#line 210
	validate(allocID != kInvalidAllocatorID);
	fn_00571a7a_vector &objects = fTypeIndex.fn_005719df(allocID);
	bool lockFailed = false;
	fn_00571a7a_vector holdResult;
	for (void **i = objects.begin(); i != objects.end() && !lockFailed; i++) {
		tServCharacter *pObject = static_cast<tServCharacter *>((tDatabaseObject *)*i);
		if (query(pObject)) {
			pObject->AddReference();
			holdResult.fn_00571a69((void *)pObject);
		}
	}
	guard.Leave();
	if (holdResult.size() > 0) {
		MultiLock(&holdResult, mustLockAll);
		for (void **lockedObject = holdResult.begin(); lockedObject != holdResult.end(); lockedObject++) {
			if (*lockedObject) {
				tServCharacter *pObject = static_cast<tServCharacter *>((tDatabaseObject *)*lockedObject);
				if (!pObject->IsDead())
					result->push_back(*pObject);
				pObject->RemoveReference();
			}
		}
	}
}

// HYPOTHESIS: a second database object type (0x160 bytes, vtable slot 9 called on each
// collected copy); the type and its name are not recovered.
class fn_0056f19b_object {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	char fn_0056f19b_pad[0x160 - 4];
};

// HYPOTHESIS: the result container for 0x0056f19b; push_back is out of line, the container
// ends at +0x8 with sizeof(element) == 0x160.
class fn_0056f19b_results {
public:
	void push_back(const fn_0056f19b_object &value);
	fn_0056f19b_object &back() { return *(m_last - 1); }
	void *m_allocator;
	fn_0056f19b_object *m_first;
	fn_0056f19b_object *m_last;
};

// HYPOTHESIS: unlocked variant of the database query of 0x0056f05f, for another object type.
class fn_0056f19b_owner {
public:
	virtual void vfunction0();
	void fn_0056f19b(const nDatabase::tQuery<fn_0056f19b_object> &query, fn_0056f19b_results *result);
	tOptex fAccess;
	fn_005719df_map fTypeIndex;
};

#line 176
// FUNCTION: STARFLEETOP 0x0056f19b
void fn_0056f19b_owner::fn_0056f19b(const nDatabase::tQuery<fn_0056f19b_object> &query, fn_0056f19b_results *result)
{
	tOptex::tGuard guard(&fAccess);
	long allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(fn_0056f19b_object));
#line 183
	validate(allocID != kInvalidAllocatorID);
	fn_00571a7a_vector &objects = fTypeIndex.fn_005719df(allocID);
	void **i = objects.begin();
	for (; i != objects.end(); i++) {
		fn_0056f19b_object *pObject = (fn_0056f19b_object *)*i;
		if (query(pObject)) {
			result->push_back(*pObject);
			result->back().vfunction9();
		}
	}
	guard.Leave();
}

class tShip;

// FUNCTION: STARFLEETOP 0x0056f265
nDatabase::tQuery<tShip>::~tQuery()
{
}

// FUNCTION: STARFLEETOP 0x0056f273
void nDatabase::tQuery<tShip>::fn_0056f04f(void *value)
{
	m_results.fn_00571a69(value);
}

#include "Common/Database/DatabaseID.h"

// HYPOTHESIS: a query evaluator holding a pointer to member (address and this adjustment at
// +8/+0xc) and an ID at +0x10; the original calls the member on the argument and compares.
// HYPOTHESIS: a two-base class gives the 8-byte pointer to member the exe uses.
struct fn_0056f283_base_a {
	int m_a;
};
struct fn_0056f283_base_b {
	int m_b;
};
class fn_0056f283_target : public fn_0056f283_base_a, public fn_0056f283_base_b {
};

typedef const nDatabase::tID *(fn_0056f283_target::*fn_0056f283_member)();

struct fn_0056f283_owner {
	int m_0;
	int m_4;
	fn_0056f283_member m_member;
	const nDatabase::tID *m_id;
	bool operator()(fn_0056f283_target *target);
};

// FUNCTION: STARFLEETOP 0x0056f283
bool fn_0056f283_owner::operator()(fn_0056f283_target *target)
{
	const nDatabase::tID *id = m_id;
	const nDatabase::tID &other = *id;
	const nDatabase::tID *current = (target->*m_member)();
	return current->operator==(other) ? true : false;
}

// HYPOTHESIS: tShip is a database class; its members are not recovered.
class tShip : public nDatabase::tDatabaseObject {
public:
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	char tShip_pad[0x434 - 0x48];
};

// HYPOTHESIS: the result container, likely std::vector<tShip>; its push_back is out of line.
class fn_0056f2a4_results {
public:
	void push_back(const tShip &value);
};

// HYPOTHESIS: 0x0056f2a4 is the same LockingQueryDB member template as 0x0056f05f,
// instantiated for tShip; this stand-in for tDatabase mirrors fn_0056f05f_owner.
class fn_0056f2a4_owner {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void MultiLock(fn_00571a7a_vector *objects, bool mustLockAll);
	void fn_0056f2a4(const nDatabase::tQuery<tShip> &query, fn_0056f2a4_results *result, bool mustLockAll);
	tOptex fAccess;
	fn_005719df_map fTypeIndex;
};

// FUNCTION: STARFLEETOP 0x0056f2a4
void fn_0056f2a4_owner::fn_0056f2a4(const nDatabase::tQuery<tShip> &query, fn_0056f2a4_results *result, bool mustLockAll)
{
	tOptex::tGuard guard(&fAccess);
	long allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(tShip));
#line 210
	validate(allocID != kInvalidAllocatorID);
	fn_00571a7a_vector &objects = fTypeIndex.fn_005719df(allocID);
	bool lockFailed = false;
	fn_00571a7a_vector holdResult;
	for (void **i = objects.begin(); i != objects.end() && !lockFailed; i++) {
		tShip *pObject = static_cast<tShip *>((tDatabaseObject *)*i);
		if (query(pObject)) {
			pObject->AddReference();
			holdResult.fn_00571a69((void *)pObject);
		}
	}
	guard.Leave();
	if (holdResult.size() > 0) {
		MultiLock(&holdResult, mustLockAll);
		for (void **lockedObject = holdResult.begin(); lockedObject != holdResult.end(); lockedObject++) {
			if (*lockedObject) {
				tShip *pObject = static_cast<tShip *>((tDatabaseObject *)*lockedObject);
				if (!pObject->IsDead())
					result->push_back(*pObject);
				pObject->RemoveReference();
			}
		}
	}
}

// HYPOTHESIS: unlocked variant of the database query for a third object type, like 0x0056f19b;
// the type and its container are not recovered.
class fn_0056f3e0_results {
public:
	void push_back(const tShip &value);
	tShip &back() { return *(m_last - 1); }
	void *m_allocator;
	tShip *m_first;
	tShip *m_last;
};

class fn_0056f3e0_owner {
public:
	virtual void vfunction0();
	void fn_0056f3e0(const nDatabase::tQuery<tShip> &query, fn_0056f3e0_results *result);
	tOptex fAccess;
	fn_005719df_map fTypeIndex;
};

#line 328
// FUNCTION: STARFLEETOP 0x0056f3e0
void fn_0056f3e0_owner::fn_0056f3e0(const nDatabase::tQuery<tShip> &query, fn_0056f3e0_results *result)
{
	tOptex::tGuard guard(&fAccess);
	long allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(tShip));
	// MATCH-ONLY: the assertion names the original's Database.h (the body is that header's
	// member template); the named #line gives __FILE__ that header until it is rebuilt there.
#line 183 "../Common/Database/Database.h"
	validate(allocID != kInvalidAllocatorID);
#line 184 "fn_0056F000.cpp"
	const fn_00571a7a_vector &objects = fTypeIndex.fn_005719df(allocID);
	// MATCH-ONLY: the SDK iterates begin() to end(); through the inline accessors the build
	// swaps esi and edi, the fields read directly give the original's registers.
	for (void *const *i = objects.m_first; i != objects.m_last; i++) {
		const tShip *pObject = static_cast<const tShip *>((tDatabaseObject *)*i);
		if (query(pObject)) {
			result->push_back(*pObject);
			result->back().vfunction9();
		}
	}
	guard.Leave();
}

// HYPOTHESIS: the type behind the pointer at +8 and the compared field at +0x10 are not recovered.
class fn_0056f4aa_base_a {
public:
	int a;
};

class fn_0056f4aa_base_b {
public:
	int b;
};

class fn_0056f4aa_target : public fn_0056f4aa_base_a, public fn_0056f4aa_base_b {
public:
	int fn_getter();
};

class fn_0056f4aa_owner {
public:
	bool fn_0056f4aa(fn_0056f4aa_target *object);
	int m0;
	int m4;
	int (fn_0056f4aa_target::*m8)();
	int m10;
};

#line 377
// FUNCTION: STARFLEETOP 0x0056f4aa
bool fn_0056f4aa_owner::fn_0056f4aa(fn_0056f4aa_target *object)
{
	bool result = (object->*m8)() == m10;
	return result;
}

class tAuctionItem;

// HYPOTHESIS: the destructor of nDatabase::tQuery<tAuctionItem>, as 0x0056f041 is for tServCharacter.
// FUNCTION: STARFLEETOP 0x0056f4c1
nDatabase::tQuery<tAuctionItem>::~tQuery()
{
}

// FUNCTION: STARFLEETOP 0x0056f4cf
void nDatabase::tQuery<tAuctionItem>::fn_0056f04f(void *value)
{
	m_results.fn_00571a69(value);
}

// HYPOTHESIS: tAuctionItem is a database class; its members are not recovered.
class tAuctionItem : public nDatabase::tDatabaseObject {
public:
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	char tAuctionItem_pad[0xa8 - 0x48];
};

// HYPOTHESIS: the result container, likely std::vector<tAuctionItem>; its push_back is out of line.
class fn_0056f4df_results {
public:
	void push_back(const tAuctionItem &value);
	tAuctionItem &back() { return *(m_last - 1); }
	void *m_allocator;
	tAuctionItem *m_first;
	tAuctionItem *m_last;
};

// HYPOTHESIS: 0x0056f4df is the same LockingQueryDB member template as 0x0056f05f,
// instantiated for tAuctionItem; this stand-in for tDatabase mirrors fn_0056f05f_owner.
class fn_0056f4df_owner {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void MultiLock(fn_00571a7a_vector *objects, bool mustLockAll);
	void fn_0056f4df(const nDatabase::tQuery<tAuctionItem> &query, fn_0056f4df_results *result, bool mustLockAll);
	tOptex fAccess;
	fn_005719df_map fTypeIndex;
};

// FUNCTION: STARFLEETOP 0x0056f4df
void fn_0056f4df_owner::fn_0056f4df(const nDatabase::tQuery<tAuctionItem> &query, fn_0056f4df_results *result, bool mustLockAll)
{
	tOptex::tGuard guard(&fAccess);
	long allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(tAuctionItem));
	// MATCH-ONLY: the assertion names the original's Database.h (the body is that header's
	// member template); the named #line gives __FILE__ that header until it is rebuilt there.
#line 210 "../Common/Database/Database.h"
	validate(allocID != kInvalidAllocatorID);
	fn_00571a7a_vector &objects = fTypeIndex.fn_005719df(allocID);
	bool lockFailed = false;
	fn_00571a7a_vector holdResult;
	for (void **i = objects.begin(); i != objects.end() && !lockFailed; i++) {
		tAuctionItem *pObject = static_cast<tAuctionItem *>((tDatabaseObject *)*i);
		if (query(pObject)) {
			pObject->AddReference();
			holdResult.fn_00571a69((void *)pObject);
		}
	}
	guard.Leave();
	if (holdResult.size() > 0) {
		MultiLock(&holdResult, mustLockAll);
		for (void **lockedObject = holdResult.begin(); lockedObject != holdResult.end(); lockedObject++) {
			if (*lockedObject) {
				tAuctionItem *pObject = static_cast<tAuctionItem *>((tDatabaseObject *)*lockedObject);
				if (!pObject->IsDead())
					result->push_back(*pObject);
				pObject->RemoveReference();
			}
		}
	}
}

// HYPOTHESIS: unlocked variant of the database query for tAuctionItem, like 0x0056f3e0;
// the result container is the one of 0x0056f4df.
class fn_0056f61b_owner {
public:
	virtual void vfunction0();
	void fn_0056f61b(const nDatabase::tQuery<tAuctionItem> &query, fn_0056f4df_results *result);
	tOptex fAccess;
	fn_005719df_map fTypeIndex;
};

#line 482
// FUNCTION: STARFLEETOP 0x0056f61b
void fn_0056f61b_owner::fn_0056f61b(const nDatabase::tQuery<tAuctionItem> &query, fn_0056f4df_results *result)
{
	tOptex::tGuard guard(&fAccess);
	long allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(tAuctionItem));
#line 183 "../Common/Database/Database.h"
	validate(allocID != kInvalidAllocatorID);
#line 489 "fn_0056F000.cpp"
	const fn_00571a7a_vector &objects = fTypeIndex.fn_005719df(allocID);
	for (void *const *i = objects.m_first; i != objects.m_last; i++) {
		const tAuctionItem *pObject = static_cast<const tAuctionItem *>((tDatabaseObject *)*i);
		if (query(pObject)) {
			result->push_back(*pObject);
			result->back().vfunction9();
		}
	}
	guard.Leave();
}

class tMetaMapHex;

// HYPOTHESIS: the destructor of nDatabase::tQuery<tMetaMapHex>, as 0x0056f041 is for tServCharacter.
// FUNCTION: STARFLEETOP 0x0056f6e5
nDatabase::tQuery<tMetaMapHex>::~tQuery()
{
}

// FUNCTION: STARFLEETOP 0x0056f6f3
void nDatabase::tQuery<tMetaMapHex>::fn_0056f04f(void *value)
{
	m_results.fn_00571a69(value);
}

// HYPOTHESIS: tMetaMapHex is a database class; its members are not recovered.
class tMetaMapHex : public nDatabase::tDatabaseObject {
public:
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	char tMetaMapHex_pad[0xd0 - 0x48];
};

// HYPOTHESIS: the result container for tMetaMapHex, as for 0x0056f4df.
class fn_0056f703_results {
public:
	void push_back(const tMetaMapHex &value);
	tMetaMapHex &back() { return *(m_last - 1); }
	void *m_allocator;
	tMetaMapHex *m_first;
	tMetaMapHex *m_last;
};
typedef fn_0056f703_results fn_0056f83f_results;

// HYPOTHESIS: 0x0056f703 is the same LockingQueryDB member template as 0x0056f05f,
// instantiated for tMetaMapHex; this stand-in for tDatabase mirrors fn_0056f05f_owner.
class fn_0056f703_owner {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void MultiLock(fn_00571a7a_vector *objects, bool mustLockAll);
	void fn_0056f703(const nDatabase::tQuery<tMetaMapHex> &query, fn_0056f703_results *result, bool mustLockAll);
	tOptex fAccess;
	fn_005719df_map fTypeIndex;
};

// FUNCTION: STARFLEETOP 0x0056f703
void fn_0056f703_owner::fn_0056f703(const nDatabase::tQuery<tMetaMapHex> &query, fn_0056f703_results *result, bool mustLockAll)
{
	tOptex::tGuard guard(&fAccess);
	long allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(tMetaMapHex));
#line 210 "../Common/Database/Database.h"
	validate(allocID != kInvalidAllocatorID);
	fn_00571a7a_vector &objects = fTypeIndex.fn_005719df(allocID);
	bool lockFailed = false;
	fn_00571a7a_vector holdResult;
	for (void **i = objects.begin(); i != objects.end() && !lockFailed; i++) {
		tMetaMapHex *pObject = static_cast<tMetaMapHex *>((tDatabaseObject *)*i);
		if (query(pObject)) {
			pObject->AddReference();
			holdResult.fn_00571a69((void *)pObject);
		}
	}
	guard.Leave();
	if (holdResult.size() > 0) {
		MultiLock(&holdResult, mustLockAll);
		for (void **lockedObject = holdResult.begin(); lockedObject != holdResult.end(); lockedObject++) {
			if (*lockedObject) {
				tMetaMapHex *pObject = static_cast<tMetaMapHex *>((tDatabaseObject *)*lockedObject);
				if (!pObject->IsDead())
					result->push_back(*pObject);
				pObject->RemoveReference();
			}
		}
	}
}

// HYPOTHESIS: unlocked variant of the database query for tMetaMapHex, like 0x0056f61b.
class fn_0056f83f_owner {
public:
	virtual void vfunction0();
	void fn_0056f83f(const nDatabase::tQuery<tMetaMapHex> &query, fn_0056f83f_results *result);
	tOptex fAccess;
	fn_005719df_map fTypeIndex;
};

// FUNCTION: STARFLEETOP 0x0056f83f
void fn_0056f83f_owner::fn_0056f83f(const nDatabase::tQuery<tMetaMapHex> &query, fn_0056f83f_results *result)
{
	tOptex::tGuard guard(&fAccess);
	long allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(tMetaMapHex));
#line 183 "../Common/Database/Database.h"
	validate(allocID != kInvalidAllocatorID);
#line 602 "fn_0056F000.cpp"
	const fn_00571a7a_vector &objects = fTypeIndex.fn_005719df(allocID);
	for (void *const *i = objects.m_first; i != objects.m_last; i++) {
		const tMetaMapHex *pObject = static_cast<const tMetaMapHex *>((tDatabaseObject *)*i);
		if (query(pObject)) {
			result->push_back(*pObject);
			result->back().vfunction9();
		}
	}
	guard.Leave();
}
#line 615 "fn_0056F000.cpp"

// HYPOTHESIS: a locked lookup through vfunction8 whose result is added to a polymorphic container.
class fn_0056f909_result {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	void fn_005395b2(void *object);
};

class fn_0056f909_owner {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void *vfunction8(void *key);
	void fn_0056f909(void *key, fn_0056f909_result *result);
	tOptex fAccess;
};

// FUNCTION: STARFLEETOP 0x0056f909
void fn_0056f909_owner::fn_0056f909(void *key, fn_0056f909_result *result)
{
	tOptex::tGuard guard(&fAccess);
	result->fn_005395b2(vfunction8(key));
	result->vfunction9();
	guard.Leave();
}

// HYPOTHESIS: same locked lookup as fn_0056f909, on another owner and result class.
class fn_0056f961_result {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	void fn_0045d230(void *object);
};

class fn_0056f961_owner {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void *vfunction8(void *key);
	void fn_0056f961(void *key, fn_0056f961_result *result);
	tOptex fAccess;
};

// FUNCTION: STARFLEETOP 0x0056f961
void fn_0056f961_owner::fn_0056f961(void *key, fn_0056f961_result *result)
{
	tOptex::tGuard guard(&fAccess);
	result->fn_0045d230(vfunction8(key));
	result->vfunction9();
	guard.Leave();
}

class tMetaMapDesc;

// HYPOTHESIS: the destructor of nDatabase::tQuery<tMetaMapDesc>, as 0x0056f041 is for tServCharacter.
// FUNCTION: STARFLEETOP 0x0056f9b9
nDatabase::tQuery<tMetaMapDesc>::~tQuery()
{
}
