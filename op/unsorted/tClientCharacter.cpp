// Unit:      unsorted/tClientCharacter.cpp
// Functions: 0x004295da to 0x0042a165, 20 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include <windows.h>
#include <string>
#include <vector>
#include <functional>
class tObject_00429f17;
// HYPOTHESIS: nDataStore::tStreamable (dtor 0x006f8ed0) is a polymorphic base 0xc bytes long.
// MATCH-ONLY: tBuffer::Write and tOutput::operator<<(const std::string &) are defined below,
// out of line, instead of in Common/DataStore (see DataStoreBuffer.h).
#define DATASTORE_OUT_OF_LINE_WRITE
#include "Common/DataStore/Streamable.h"







#include "Common/Object.h"














namespace nDataStore {



}
#include "Common/DataStore/DataStoreBuffer.h"
namespace nDataStore {









}

namespace OSSPECIFIC {
extern LONG (*OS_InterlockedCompareExchange)(LPLONG Destination, LONG Exchange, LONG Comperand);
}

#include "unsorted/tHexPoint.h"












// HYPOTHESIS: a 0x14-byte member type with an out-of-line destructor at 0x0042ade3.
// HYPOTHESIS: a tree-like container: the constructor copies an empty allocator object to +0
// and an empty comparison object to +1, clears the flag at +0xc and calls 0x0042baaf on
// itself. It is not the compiler's own std::map (that one keeps its flag at +8 and is 0x10
// bytes); the key and element types, and so the real allocator and comparison types, are not
// recovered.
class tObject_0042ade3 {
public:
	tObject_0042ade3(const std::less<int> &pred = std::less<int>(), const std::allocator<int> &al = std::allocator<int>());

	~tObject_0042ade3();
	void fn_0042baaf();
	std::allocator<int> m_00;
	std::less<int> m_01;
	int m_04;
	int m_08;
	bool m_0c;
	int m_10;
};

inline tObject_0042ade3::tObject_0042ade3(const std::less<int> &pred, const std::allocator<int> &al)
	: m_00(al)
	, m_01(pred)
	, m_0c(false)
{
	fn_0042baaf();
}

#include "Common/Thread/Optex.h"
// The lines below up to tSingleAccessWrapper stay blank so the code keeps its line numbers.




























// The SDK's tSingleAccessWrapper: exclusive access to a pointer. The members at +0xb4 and
// +0xcc are two instances; the constructor (0x0048589c) and IsValid (0x0058f829) are shared
// by both, Delete is 0x0042cc05 for the first and 0x005ff84a for the second.
#include "Common/Thread/SingleAccessWrapper.h"






















































// HYPOTHESIS: the 0xd0-byte object the member at +0xcc points to; its constructor at
// 0x005391b0 takes one argument, passed as 0. Its members are not recovered.
class tObject_005391b0 {
public:
	tObject_005391b0(int arg = 0);
	tObject_005391b0 &operator=(const tObject_005391b0 &other);
	char m_00[0xd0];
};

#include "Common/Database/DatabaseID.h"







// HYPOTHESIS: the object 0x00429d06 copies a tClientCharacter from; its class is not recovered,
// its accessors are declared from the calls: the virtual function at +0x30 returns the object's
// database identifier, the rest are plain members. 0x00413c68 is shared, through
// identical code folding, with an unrelated getter; operator* (0x00604259) and Done
// (0x005f4352) likewise serve both the const and the non-const tAccessPointer.
class tObject_00543551 {
public:
	virtual void vfunction00();
	virtual void vfunction04();
	virtual void vfunction08();
	virtual void vfunction0c();
	virtual void vfunction10();
	virtual void vfunction14();
	virtual void vfunction18();
	virtual void vfunction1c();
	virtual void vfunction20();
	virtual void vfunction24();
	virtual void vfunction28();
	virtual void vfunction2c();
	virtual const nDatabase::tID *vfunction30() const;
	std::string fn_00543551() const;
	std::string fn_005433a7() const;
	std::string fn_00543459() const;
	int fn_00543495() const;
	int fn_005434a3() const;
	int fn_00413c68() const;
	int fn_0054349c() const;
	int fn_0054a8c8() const;
	int fn_0054a61d() const;
	tHexPoint fn_005434b1() const;
	int fn_00543514() const;
	const tHexPoint &fn_005435c3() const;
	const tHexPoint &fn_005434d6() const;
	int fn_0054312b() const;
	bool fn_005433e8() const;
	tSingleAccessWrapper<std::vector<int> >::iConstPtr fn_005434f7() const;
};

// HYPOTHESIS: member types and the trivially destroyed fields between them are inferred from
// the destructor only.
class tClientCharacter : public nDataStore::tStreamable {
public:
	tClientCharacter(const tClientCharacter &other);
	tClientCharacter(const tObject_00543551 *source);
	virtual ~tClientCharacter();
	tClientCharacter &fn_0042984d(const tClientCharacter &other);
	void fn_00429d06(const tObject_00543551 *source);
	void fn_00429f0d(tObject_00429f17 *stream);
	void StreamIn(nDataStore::tInput &input);
	std::string m_0c;
	std::string m_1c;
	int m_2c;
	std::string m_30;
	int m_40;
	int m_44;
	int m_48;
	int m_4c;
	int m_50;
	int m_54;
	int m_58;
	tHexPoint m_5c;
	tHexPoint m_70;
	tHexPoint m_84;
	int m_98;
	int m_9c;
	tObject_0042ade3 m_a0;
	tSingleAccessWrapper<std::vector<int> > m_b4; // HYPOTHESIS: element type not recovered
	tSingleAccessWrapper<tObject_005391b0> m_cc;
	tOptex m_e4;
};

// The calls to tOptex::~tOptex go to the game's copy (0x004296c8); the SDK's own copy is
// 0x0070f100, and the harness holds one symbol to one address, so this stays short of exact.
// FUNCTION: STARFLEETOP 0x004295d0
tClientCharacter::~tClientCharacter()
{
	m_e4.Enter();
	m_e4.Leave();
}

// The game's out-of-line copies of the SDK's inline tOptex members.
// FUNCTION: STARFLEETOP 0x004296c8
inline tOptex::~tOptex()
{
	if (fUse != tThreadState::kIgnoreThreadObject) {
		if (fUseCriticalSections && fpSharedInfo != NULL)
			::DeleteCriticalSection(&fpSharedInfo->fCriticalSection);
		if (fEvent != NULL)
			::CloseHandle(fEvent);
		if (fFileMapping == NULL) {
			delete fpSharedInfo;
		} else {
			::UnmapViewOfFile(fpSharedInfo);
			::CloseHandle(fFileMapping);
		}
	}
}

// FUNCTION: STARFLEETOP 0x00429717
inline void tOptex::Enter()
{
	if (fUse != tThreadState::kIgnoreThreadObject) {
		if (fUseCriticalSections) {
			::EnterCriticalSection(&fpSharedInfo->fCriticalSection);
		} else if (!TryEnter()) {
			int32u threadId = ::GetCurrentThreadId();
			if (::InterlockedIncrement(&fpSharedInfo->fLockCount) == 1) {
				fpSharedInfo->fThreadId = threadId;
				fpSharedInfo->fRecurseCount = 1;
			} else if (fpSharedInfo->fThreadId == threadId) {
				++fpSharedInfo->fRecurseCount;
			} else {
				::WaitForSingleObject(fEvent, INFINITE);
				fpSharedInfo->fThreadId = threadId;
				fpSharedInfo->fRecurseCount = 1;
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x00429785
inline bool tOptex::TryEnter()
{
	if (fUse == tThreadState::kIgnoreThreadObject)
		return false;

	if (fUseCriticalSections)
		return false;

	int32u threadId = ::GetCurrentThreadId();

	bool thisThreadOwnsTheOptex = false;
	int32u spinCount = fpSharedInfo->fSpinCount;

	do {
		thisThreadOwnsTheOptex =
			(0 == (*(OSSPECIFIC::OS_InterlockedCompareExchange))(&fpSharedInfo->fLockCount, 1, 0));

		if (thisThreadOwnsTheOptex) {
			fpSharedInfo->fThreadId = threadId;
			fpSharedInfo->fRecurseCount = 1;
		} else {
			if (fpSharedInfo->fThreadId == threadId) {
				::InterlockedIncrement(&fpSharedInfo->fLockCount);
				++fpSharedInfo->fRecurseCount;
				thisThreadOwnsTheOptex = TRUE;
			}
		}
	} while (!thisThreadOwnsTheOptex && (spinCount-- > 0));

	return thisThreadOwnsTheOptex;
}

// FUNCTION: STARFLEETOP 0x004297fa
inline void tOptex::Leave()
{
	if (fUse != tThreadState::kIgnoreThreadObject) {
		if (fUseCriticalSections) {
			::LeaveCriticalSection(&fpSharedInfo->fCriticalSection);
		} else if (--fpSharedInfo->fRecurseCount > 0) {
			::InterlockedDecrement(&fpSharedInfo->fLockCount);
		} else {
			fpSharedInfo->fThreadId = 0;
			if (::InterlockedDecrement(&fpSharedInfo->fLockCount) > 0)
				::SetEvent(fEvent);
		}
	}
}

// Inline in the SDK; defined before its first use so that the functions below inline it. The
// out-of-line copy (0x00429a18) is the one their unwind code calls, emitted after 0x0042984d.
// FUNCTION: STARFLEETOP 0x00429a18
inline tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->Leave();
	}
}

// HYPOTHESIS: a copy assignment (it returns *this), likely the class's operator=; the name is
// not recovered. Under the lock at +0xe4 it copies every field but +0x58, the two locked
// members through the SDK's `*Lock() = *copy.Lock()` idiom.
// FUNCTION: STARFLEETOP 0x0042984d
tClientCharacter &tClientCharacter::fn_0042984d(const tClientCharacter &other)
{
	tOptex::tGuard guard(&m_e4);

	m_0c = other.m_0c;
	m_1c = other.m_1c;
	m_2c = other.m_2c;
	m_30 = other.m_30;
	m_40 = other.m_40;
	m_44 = other.m_44;
	m_48 = other.m_48;
	m_4c = other.m_4c;
	m_50 = other.m_50;
	m_54 = other.m_54;
	m_5c = other.m_5c;
	m_70 = other.m_70;
	m_84 = other.m_84;
	*m_b4.Lock() = *other.m_b4.Lock();
	*m_cc.Lock() = *other.m_cc.Lock();
	m_98 = other.m_98;
	*(char *)&m_9c = *(char *)&other.m_9c;

	guard.Leave();
	return *this;
}

// HYPOTHESIS: copy constructor: default-constructs the members, makes sure both guarded
// pointers hold an object and ends with *this = other (0x0042984d).
// FUNCTION: STARFLEETOP 0x00429a26
tClientCharacter::tClientCharacter(const tClientCharacter &other)
{
	tOptex::tGuard guard(&m_e4);
	if (!m_b4.IsValid()) {
		m_b4.SetPointer(new std::vector<int>);
	}
	if (!m_cc.IsValid()) {
		m_cc.SetPointer(new tObject_005391b0);
	}
	fn_0042984d(other);
	guard.Leave();
}

// HYPOTHESIS: a constructor from the object 0x00429d06 copies: builds the members, gives both
// guarded pointers a new object and, under the lock at +0xe4, copies the fields through
// 0x00429d06.
// FUNCTION: STARFLEETOP 0x00429bb2
tClientCharacter::tClientCharacter(const tObject_00543551 *source)
	: m_b4(new std::vector<int>)
	, m_cc(new tObject_005391b0)
{
	tOptex::tGuard guard(&m_e4);
	fn_00429d06(source);
	guard.Leave();
}

// HYPOTHESIS: under the lock at +0xe4 it copies every field from the object passed in, through
// its accessors. The argument's class is not recovered; it is declared here from the calls
// alone.
// FUNCTION: STARFLEETOP 0x00429d06
void tClientCharacter::fn_00429d06(const tObject_00543551 *source)
{
	tOptex::tGuard guard(&m_e4);

	m_0c = source->fn_00543551();
	m_1c = source->fn_005433a7();
	m_2c = source->vfunction30()->GetObjectID();
	m_30 = source->fn_00543459();
	m_40 = source->fn_00543495();
	m_44 = source->fn_005434a3();
	m_48 = source->fn_00413c68();
	m_4c = source->fn_0054349c();
	m_50 = source->fn_0054a8c8();
	m_54 = source->fn_0054a61d();
	m_5c = source->fn_005434b1();
	m_58 = source->fn_00543514();
	m_5c = source->fn_005434b1();
	m_70 = source->fn_005435c3();
	m_84 = source->fn_005434d6();
	m_98 = source->fn_0054312b();
	*(char *)&m_9c = source->fn_005433e8();
	*m_b4.Lock() = *source->fn_005434f7();

	guard.Leave();
}

// HYPOTHESIS: the stream 0x00429f17 writes to: the original loads a writer from +8 of it and
// calls a method taking a buffer and a byte count. Neither class is recovered.
class tObject_00429f17_writer {
public:
	void Write(const void *data, int size);
};

class tObject_00429f17 {
public:
	int m_00;
	int m_04;
	tObject_00429f17_writer *m_08;
};

// HYPOTHESIS: an honest partial of 0x00429f0d. Read from the original instructions: under the
// lock at +0xe4 it takes the locked pointers at +0xcc and +0xb4, writes the fields at +0x1c,
// +0x2c, +0x30, +0x40..+0x4c to a stream through the argument (written here as one dword
// stream class, not recovered) and ends by passing the fields at +0x50..+0x84 on to one callee.
// The callees have no recovered types, so only the part that is certain is written: the lock
// and the two locked accesses. The body covers only part of the function, hence its 1.066%
// score.
// FUNCTION: STARFLEETOP 0x00429f0d
void tClientCharacter::fn_00429f0d(tObject_00429f17 *stream)
{
	tOptex::tGuard guard(&m_e4);

	int value = m_2c;
	stream->m_08->Write(&value, 4);
	value = m_40;
	stream->m_08->Write(&value, 4);
	value = m_44;
	stream->m_08->Write(&value, 4);
	value = m_48;
	stream->m_08->Write(&value, 4);
	value = m_4c;
	stream->m_08->Write(&value, 4);

	guard.Leave();
}
namespace nDataStore {
// FUNCTION: STARFLEETOP 0x0042a0fc
void tBuffer::Write( const byte* data, int32u size )
{
	fBuffer.insert(fBuffer.end(), data, data + size);
}
}
#include "Common/DataStore/DataStoreOutput.h"
namespace nDataStore {
// MATCH-ONLY: Write and operator<< are declared inline in Common/DataStore, so the compiler
// emits their out-of-line copies (0x0042a0fc, 0x0042a116) only for a use it cannot inline;
// these pointers are that use.
void (tBuffer::*gEmitBufferWrite)( const byte*, int32u ) = &tBuffer::Write;
tOutput& (tOutput::*gEmitOutputString)( const std::string& ) = &tOutput::operator<<;














// FUNCTION: STARFLEETOP 0x0042a116
tOutput& tOutput::operator<<( const std::string& string )
{
	int32u size = string.size();
	Write((const byte *)&size, sizeof(size));

	Write((const byte *)string.data(), size);

	return *this;
}
}
#include "Common/DataStore/DataStoreInput.h"
namespace nDataStore {
// HYPOTHESIS: the reader of the list at +0xb4 (0x0042c863); its element type is not recovered.
tInput &operator>>(tInput &input, std::vector<int> &list);
}

// HYPOTHESIS: StreamIn, read from the original instructions: under the lock at +0xe4, holding
// both guarded pointers, it reads the fields back in the order 0x00429f17 writes them, the
// tStreamable at +8 of the object at +0xcc among them. tHexPoint (0x14 bytes) and that object
// are not recovered as tStreamable classes, so they are cast here. The original inlines the
// lock's guard destructor (out of line in this unit, see 0x00429a18) and the list's operator*.
// FUNCTION: STARFLEETOP 0x0042a165
void tClientCharacter::StreamIn(nDataStore::tInput &input)
{
	tOptex::tGuard guard(&m_e4);
	tSingleAccessWrapper<tObject_005391b0>::iPtr object = m_cc.Lock();
	tObject_005391b0 *pObject = &*object;
	tSingleAccessWrapper<std::vector<int> >::iPtr list = m_b4.Lock();
	std::vector<int> &v = *list;

	nDataStore::tInput &in = input >> m_0c >> m_1c;
	in.Read((byte *)&m_2c, sizeof(m_2c));
	nDataStore::tInput &in2 = in >> m_30;
	in2.Read((byte *)&m_40, sizeof(m_40));
	in2.Read((byte *)&m_44, sizeof(m_44));
	in2.Read((byte *)&m_48, sizeof(m_48));
	in2.Read((byte *)&m_4c, sizeof(m_4c));
	in2 >> (int32s &)m_50 >> (int32s &)m_54 >> (int32s &)m_58
		>> *(nDataStore::tStreamable *)&m_5c >> *(nDataStore::tStreamable *)&m_70
		>> *(nDataStore::tStreamable *)&m_84 >> v
		>> *(nDataStore::tStreamable *)((char *)pObject + 8)
		>> (int32s &)m_98 >> *(bool *)&m_9c;

}
