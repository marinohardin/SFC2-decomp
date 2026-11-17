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
