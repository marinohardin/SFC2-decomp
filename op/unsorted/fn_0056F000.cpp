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
struct fn_00571a7a_vector {
	fn_00571a7a_vector() : m_first(0), m_last(0), m_end(0) {}
	~fn_00571a7a_vector();
	void fn_00571a69(void *const &value);
	char m_allocator;
	void **m_first;
	void **m_last;
	void **m_end;
};

// HYPOTHESIS: the function sets the vtable of nDatabase::tQuery<tServCharacter> and destroys a
// member at +0x4 through 0x00571a7a; it is taken to be that class's destructor.
namespace nDatabase {
	template <class T> class tQuery {
	public:
		virtual ~tQuery();
		void fn_0056f04f(void *value);
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

// HYPOTHESIS: the owner of 0x0056f05f: an optex at +0x4 and a map at +0x18 from allocator id to a
// list of database objects; the class and its members are not recovered.
class tServCharacter {};

using nDatabase::tDatabaseObject;

// MATCH-ONLY: the original inlined tOptex::tGuard here; this copy has the same members and an
// inline constructor and destructor.
struct fn_0056f05f_guard {
	fn_0056f05f_guard(tOptex *optex)
	{
		fNeedToLeave = true;
		fOptex = optex;
		fOptex->Enter();
	}
	~fn_0056f05f_guard()
	{
		if (fNeedToLeave)
			fOptex->Leave();
	}
	void Leave()
	{
		fOptex->Leave();
		fNeedToLeave = false;
	}
	bool fNeedToLeave;
	tOptex *fOptex;
};

class fn_0056f05f_filter {
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual bool vfunction2(tDatabaseObject *object);
};

class fn_0056f05f_sink {
public:
	void fn_005406d7(tDatabaseObject *object);
};

struct fn_0056f05f_map {
	fn_00571a7a_vector &fn_005719df(const long &key);
};

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
	virtual void vfunction10(fn_00571a7a_vector &found, int arg);
	void fn_0056f05f(fn_0056f05f_filter *filter, fn_0056f05f_sink *sink, int arg);
	tOptex m_optex;
	fn_0056f05f_map m_map;
};

// FUNCTION: STARFLEETOP 0x0056f05f
void fn_0056f05f_owner::fn_0056f05f(fn_0056f05f_filter *filter, fn_0056f05f_sink *sink, int arg)
{
	fn_0056f05f_guard guard(&m_optex);
	long allocID = tDynamicAllocator::Instance()->mGetAllocatorID(typeid(tServCharacter));
#line 210
	validate(allocID != -1);
	fn_00571a7a_vector &list = m_map.fn_005719df(allocID);
	fn_00571a7a_vector found;
	for (void **it = list.m_first; it != list.m_last; it++) {
		tDatabaseObject *object = (tDatabaseObject *)*it;
		if (filter->vfunction2(object)) {
			object->AddReference();
			found.fn_00571a69((void *)object);
		}
	}
	guard.Leave();
	if (found.m_first != 0 && found.m_last - found.m_first != 0) {
		vfunction10(found, arg);
		for (void **it = found.m_first; it != found.m_last; it++) {
			tDatabaseObject *object = (tDatabaseObject *)*it;
			if (object) {
				if (!object->IsDead())
					sink->fn_005406d7(object);
				object->RemoveReference();
			}
		}
	}
}
