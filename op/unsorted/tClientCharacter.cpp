// Unit:      unsorted/tClientCharacter.cpp
// Functions: 0x004295da to 0x0042a165, 20 in game/UNITS.tsv
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

// HYPOTHESIS: tHexPoint (dtor 0x005b6a36) is 0x14 bytes; its members are not recovered.
class tHexPoint {
public:
	tHexPoint();
	~tHexPoint();
	tHexPoint &operator=(const tHexPoint &other);
	int m_00;
	int m_04;
	int m_08;
	int m_0c;
	int m_10;
};

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

// HYPOTHESIS: a 0x14-byte container type with an out-of-line destructor at 0x004296c8, used
// as the member at +0xe4 and inside the two members below.
// HYPOTHESIS: the SDK types the first constructor argument tThreadState::eUse
// (kUseThreadObject = 1); tThreadState is declared only inside another unit, so it is an int
// here until that class has a shared header.
class tOptex {
public:
	class tGuard {
	public:
		tGuard(tOptex *optex);
		~tGuard();
		void Enter();
		void Leave();

	protected:
		bool fNeedToLeave;
		tOptex *fOptex;
	};

	tOptex(int use = 1, unsigned int spinCount = 4000);
	~tOptex();
	void CommonConstructor(int use, unsigned int spinCount, bool unicode, const void *pName);
	void fn_00429717();
	bool TryEnter();
	void fn_004297fa();
	void fn_0042984d();
	int m_00;
	char m_04;
	char m_05;
	HANDLE m_08;
	HANDLE m_0c;
	void *m_10;
};

inline tOptex::tOptex(int use, unsigned int spinCount)
{
	CommonConstructor(use, spinCount, false, NULL);
}

inline tOptex::tGuard::tGuard(tOptex *optex)
	: fOptex(optex)
{
	Enter();
}

inline void tOptex::tGuard::Enter()
{
	fNeedToLeave = true;
	fOptex->fn_00429717();
}

inline void tOptex::tGuard::Leave()
{
	fOptex->fn_004297fa();
	fNeedToLeave = false;
}

// The SDK's tSingleAccessWrapper: exclusive access to a pointer. The members at +0xb4 and
// +0xcc are two instances; the constructor (0x0048589c) and IsValid (0x0058f829) are shared
// by both, Delete is 0x0042cc05 for the first and 0x005ff84a for the second.
// The SDK's tAccessPointer: locked access to the pointer of a tSingleAccessWrapper.
template <typename tnPtrType, typename tnPtrGuard>
class tAccessPointer {
public:
	tAccessPointer(tAccessPointer<tnPtrType, tnPtrGuard> &copy);
	~tAccessPointer();
	void Done();
	tnPtrType &operator*();

protected:
	mutable tOptex fAccess;
	tnPtrType *fPointer;
	const tnPtrGuard *fPointerGuard;
};

template <typename tnPtrType, typename tnPtrGuard>
inline tAccessPointer<tnPtrType, tnPtrGuard>::~tAccessPointer()
{
	Done();
}

template <typename tPtrType>
class tSingleAccessWrapper {
public:
	typedef tAccessPointer<tPtrType, tSingleAccessWrapper> iPtr;
	typedef tAccessPointer<const tPtrType, tSingleAccessWrapper> iConstPtr;

	tSingleAccessWrapper();
	tSingleAccessWrapper(tPtrType *pointer);
	~tSingleAccessWrapper();
	void SetPointer(tPtrType *copy);
	void Delete();
	iPtr Lock();
	bool IsValid() const;

protected:
	tPtrType *fpPointer;
	mutable tOptex fPointerOptex;
};

template <typename tPtrType>
inline tSingleAccessWrapper<tPtrType>::~tSingleAccessWrapper()
{
	Delete();
}

template <typename tPtrType>
inline void tSingleAccessWrapper<tPtrType>::SetPointer(tPtrType *copy)
{
	Delete();

	fpPointer = copy;
}

// HYPOTHESIS: the 0xd0-byte object the member at +0xcc points to; its constructor at
// 0x005391b0 takes one argument, passed as 0. Its members are not recovered.
class tObject_005391b0 {
public:
	tObject_005391b0(int arg = 0);
	char m_00[0xd0];
};

#include "Common/Database/DatabaseID.h"







// HYPOTHESIS: the object 0x00429d10 copies a tClientCharacter from; its class is not recovered,
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
	tClientCharacter(int arg);
	virtual ~tClientCharacter();
	tClientCharacter &fn_00429857(const tClientCharacter &other);
	void fn_00429d06(int arg);
	void fn_00429d10(const tObject_00543551 *source);
	void fn_00429f17(tObject_00429f17 *stream);
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

// HYPOTHESIS: the function really starts at 0x004295d0 with `mov eax, <handler>; call
// __EH_prolog`; the analysis boundary at 0x004295da skips those 10 bytes, so the score is
// measured against a misaligned original. Past the prolog the rebuilt code is instruction for
// instruction the original.
// FUNCTION: STARFLEETOP 0x004295da
tClientCharacter::~tClientCharacter()
{
	m_e4.fn_00429717();
	m_e4.fn_004297fa();
}

// HYPOTHESIS: m_10 is a block either mapped from the file mapping m_0c or allocated on the
// heap; when m_05 is set it holds a critical section at +0x10.
// FUNCTION: STARFLEETOP 0x004296c8
tOptex::~tOptex()
{
	if (m_00) {
		if (m_05 && m_10)
			DeleteCriticalSection((CRITICAL_SECTION *)((char *)m_10 + 0x10));
		if (m_08)
			CloseHandle(m_08);
		if (m_0c == 0) {
			operator delete(m_10);
		} else {
			UnmapViewOfFile(m_10);
			CloseHandle(m_0c);
		}
	}
}

// HYPOTHESIS: acquires the lock: a critical section at m_10+0x10 when m_05 is set, otherwise
// a recursive lock in the shared block m_10 (+4 lock count, +8 owner thread, +0xc recursion
// count) backed by the wait handle m_08. 0x00429785 returns true when no lock is needed.
// FUNCTION: STARFLEETOP 0x00429717
void tOptex::fn_00429717()
{
	if (m_00) {
		if (m_05) {
			EnterCriticalSection((CRITICAL_SECTION *)((char *)m_10 + 0x10));
		} else if (!TryEnter()) {
			DWORD thread = GetCurrentThreadId();
			if (InterlockedIncrement((LONG *)((char *)m_10 + 4)) == 1) {
				*(DWORD *)((char *)m_10 + 8) = thread;
				*(int *)((char *)m_10 + 0xc) = 1;
			} else if (*(DWORD *)((char *)m_10 + 8) == thread) {
				++*(int *)((char *)m_10 + 0xc);
			} else {
				WaitForSingleObject(m_08, INFINITE);
				*(DWORD *)((char *)m_10 + 8) = thread;
				*(int *)((char *)m_10 + 0xc) = 1;
			}
		}
	}
}

// HYPOTHESIS: spins up to the shared block's count at +0 trying to take the lock count at +4
// from 0 to 1, or re-enters when this thread already owns it; never used with m_05 set.
// FUNCTION: STARFLEETOP 0x00429785
bool tOptex::TryEnter()
{
	if (m_00 == 0)
		return false;

	if (m_05)
		return false;

	DWORD threadId = GetCurrentThreadId();

	bool thisThreadOwnsTheOptex = false;
	DWORD spinCount = *(DWORD *)m_10;

	do {
		thisThreadOwnsTheOptex =
			(0 == (*(OSSPECIFIC::OS_InterlockedCompareExchange))((LONG *)((char *)m_10 + 4), 1, 0));

		if (thisThreadOwnsTheOptex) {
			*(DWORD *)((char *)m_10 + 8) = threadId;
			*(int *)((char *)m_10 + 0xc) = 1;
		} else {
			if (*(DWORD *)((char *)m_10 + 8) == threadId) {
				InterlockedIncrement((LONG *)((char *)m_10 + 4));
				++*(int *)((char *)m_10 + 0xc);
				thisThreadOwnsTheOptex = TRUE;
			}
		}
	} while (!thisThreadOwnsTheOptex && (spinCount-- > 0));

	return thisThreadOwnsTheOptex;
}

// HYPOTHESIS: releases the lock taken by 0x00429717.
// FUNCTION: STARFLEETOP 0x004297fa
void tOptex::fn_004297fa()
{
	if (m_00) {
		if (m_05) {
			LeaveCriticalSection((CRITICAL_SECTION *)((char *)m_10 + 0x10));
		} else if (--*(int *)((char *)m_10 + 0xc) > 0) {
			InterlockedDecrement((LONG *)((char *)m_10 + 4));
		} else {
			*(DWORD *)((char *)m_10 + 8) = 0;
			if (InterlockedDecrement((LONG *)((char *)m_10 + 4)) > 0)
				SetEvent(m_08);
		}
	}
}

// HYPOTHESIS: these 10 bytes (`mov eax, <handler>; call __EH_prolog`) are the exception prolog
// of the function that follows; the analysis boundary split it off, so this cannot be matched
// as a function of its own. Left as an empty placeholder until the boundary is corrected.
// FUNCTION: STARFLEETOP 0x0042984d
void tOptex::fn_0042984d()
{
}

// HYPOTHESIS: this is really tClientCharacter::operator=(const tClientCharacter &) (copy of
// the +0xc/+0x1c/+0x30 strings, +0x5c/+0x70/+0x84 hex points, +0xb4/+0xcc locked members,
// guarded by the tOptex at +0xe4 through 0x00429717/0x004297fa); its exception prolog is the
// 10 bytes at 0x0042984d. The callees at 0x00405.. (string assign, tHexPoint copy, etc.) have
// no recovered types yet, so only an empty placeholder is recorded here.
// HYPOTHESIS: scoped holder of the tOptex at +0xe4; its constructor and destructor are the
// calls to 0x00429717 and 0x004297fa.
class tOptexHolder {
public:
	tOptexHolder(tOptex *optex)
		: m_optex(optex)
	{
		m_optex->fn_00429717();
	}
	~tOptexHolder() { m_optex->fn_004297fa(); }
	tOptex *m_optex;
};

// HYPOTHESIS: the two locked members (+0xb4, +0xcc) are copied through callees whose types are
// not recovered, so that part is left out until they are.
// FUNCTION: STARFLEETOP 0x00429857
tClientCharacter &tClientCharacter::fn_00429857(const tClientCharacter &other)
{
	tOptexHolder holder(&m_e4);

	m_0c.assign(other.m_0c);
	m_1c.assign(other.m_1c);
	m_2c = other.m_2c;
	m_30.assign(other.m_30);
	m_40 = other.m_40;
	m_44 = other.m_44;
	m_48 = other.m_48;
	m_4c = other.m_4c;
	m_50 = other.m_50;
	m_54 = other.m_54;
	m_5c = other.m_5c;
	m_70 = other.m_70;
	m_84 = other.m_84;
	m_98 = other.m_98;
	*(char *)&m_9c = *(char *)&other.m_9c;
	return *this;
}

// HYPOTHESIS: inline in the SDK header; written without `inline` here so that it is emitted
// while nothing in this unit uses a tGuard yet (see the note on 0x00429a30).
// FUNCTION: STARFLEETOP 0x00429a18
tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->fn_004297fa();
	}
}

// HYPOTHESIS: these 10 bytes (`mov eax, <handler>; call __EH_prolog`) are the exception prolog
// of the function that follows (its unwind funclets at 0x0075c538..0x0075c5c5 name this
// address as their parent); the analysis boundary split it off, so this cannot be matched as a
// function of its own. Left as an empty placeholder until the boundary is corrected.
// FUNCTION: STARFLEETOP 0x00429a26
void fn_00429a26()
{
}

// FUNCTION: STARFLEETOP 0x00429a30
// HYPOTHESIS: copy constructor: default-constructs the members, makes sure both guarded
// pointers hold an object and ends with *this = other (0x00429857). The 10-byte exception
// prolog at 0x00429a26 precedes the analysis boundary, so the score is measured against a
// misaligned original and says nothing about the body.
// HYPOTHESIS: read from the original instructions, the full body is the one below, with
// tOptex::tGuard::~tGuard inline. Built that way the function is the original's 386 bytes
// plus the 10-byte prolog, but the misaligned comparison scores it 0%, so only the last
// statement but one is written until the boundary is corrected:
//
//	tOptex::tGuard guard(&m_e4);
//	if (!m_b4.IsValid()) {
//		m_b4.SetPointer(new std::vector<int>);
//	}
//	if (!m_cc.IsValid()) {
//		m_cc.SetPointer(new tObject_005391b0);
//	}
//	fn_00429857(other);
//	guard.Leave();
tClientCharacter::tClientCharacter(const tClientCharacter &other)
{
	fn_00429857(other);
}

// HYPOTHESIS: as at 0x00429a26, these 10 bytes (`mov eax, 0x0075c673; call __EH_prolog`) are
// the exception prolog of the function that follows, split off by the analysis boundary, so they
// cannot be matched as a function of their own. Left as an empty placeholder until the boundary
// is corrected.
// FUNCTION: STARFLEETOP 0x00429bb2
void fn_00429bb2()
{
}

// HYPOTHESIS: a one-argument constructor: builds the members, gives both guarded pointers a new
// object and, under the lock at +0xe4, passes its argument to 0x00429d06. The argument's type
// is not recovered (one dword). Its 10-byte exception prolog is 0x00429bb2, split off by the
// analysis boundary, so the score is measured against a misaligned original. Past the prolog
// the rebuilt body is the original's except that the original inlines tOptex::tGuard::~tGuard
// (here it is out of line, see 0x00429a18), which also saves one stack slot.
// FUNCTION: STARFLEETOP 0x00429bbc
tClientCharacter::tClientCharacter(int arg)
	: m_b4(new std::vector<int>)
	, m_cc(new tObject_005391b0)
{
	tOptex::tGuard guard(&m_e4);
	fn_00429d06(arg);
	guard.Leave();
}

// HYPOTHESIS: 0x00429bbc calls this address, so it is a real entry point, but its 10 bytes are
// only the exception prolog (`mov eax, 0x0075c6d0; call __EH_prolog`); the body continues at
// 0x00429d10, past the analysis boundary. Left empty until the boundary is corrected, as at
// 0x00429a26 and 0x00429bb2.
// FUNCTION: STARFLEETOP 0x00429d06
void tClientCharacter::fn_00429d06(int arg)
{
}

// HYPOTHESIS: 0x00429d10 is the body of 0x00429d06 past its 10-byte exception prolog: under the
// lock at +0xe4 it copies every field from the object passed in, through its accessors. The
// argument's class is not recovered; it is declared here from the calls alone. Built this way,
// the function is the prolog plus the body, so the score is measured against a misaligned
// original, as at 0x004295da.
// FUNCTION: STARFLEETOP 0x00429d10
void tClientCharacter::fn_00429d10(const tObject_00543551 *source)
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

// HYPOTHESIS: as at 0x00429a26, 0x00429bb2 and 0x00429d06, these 10 bytes (`mov eax,
// 0x0075c704; call __EH_prolog`) are the exception prolog of the function whose body follows
// at 0x00429f17, split off by the analysis boundary, so they cannot be matched as a function of
// their own. The analysis places it in tClientCharacter's vtable (slot 2); the body is not
// recovered. Left as an empty placeholder until the boundary is corrected.
// FUNCTION: STARFLEETOP 0x00429f0d
void fn_00429f0d()
{
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

// HYPOTHESIS: 0x00429f17 is the body of 0x00429f0d past its 10-byte exception prolog. Read from
// the original instructions: under the lock at +0xe4 it takes the locked pointers at +0xcc and
// +0xb4, writes the fields at +0x1c, +0x2c, +0x30, +0x40..+0x4c to a stream through the
// argument (written here as one dword stream class, not recovered) and ends by passing the
// fields at +0x50..+0x84 on to one callee. The callees have no recovered types, so only the
// part that is certain is written: the lock and the two locked accesses. The score is measured
// against a misaligned original, as at 0x00429d10.
// FUNCTION: STARFLEETOP 0x00429f17
void tClientCharacter::fn_00429f17(tObject_00429f17 *stream)
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
// HYPOTHESIS: as at 0x00429f0d, these 10 bytes (`mov eax, 0x0075c738; call __EH_prolog`) are the
// exception prolog of the function whose body follows, split off by the analysis boundary, so
// they cannot be matched as a function of their own. The analysis places it in
// tClientCharacter's vtable (slot 3); the body is not recovered. Left as an empty placeholder
// until the boundary is corrected.
// FUNCTION: STARFLEETOP 0x0042a165
void fn_0042a165()
{
}
