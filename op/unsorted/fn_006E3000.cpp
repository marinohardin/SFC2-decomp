// Functions of the 0x006e3000 page; original file unknown.

#include <windows.h>
#include <map>

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

	char m00[0x18];
	CRITICAL_SECTION mSection;
	tTimerMap_006e3000 mTimers;
	char m40[0x4];
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
