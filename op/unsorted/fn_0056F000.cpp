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
