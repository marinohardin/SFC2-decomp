#include "Common/Database/DatabaseID.h"
#include "unsorted/tMetaMapHex.h"
#include <vector>
#include <map>
#include "Common/Thread/Optex.h"
#include "unsorted/fn_0042b0fd_owner.h"
#include <string>
#include <time.h>

// The SDK's inline tOptex destructor, the same body tClientCharacter.cpp claims as the game's
// copy (0x004296c8). With its body in view the compiler sets no unwind state before calling it
// from an inlined tAccessPointer destructor, as the original does at 0x0054372f.
// HYPOTHESIS: it belongs in Common/Thread/Optex.h, shared by both units, once they can move.
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

// HYPOTHESIS: a map from a hex's two coordinates to the hex point. Like the maps tMetaMapHex.h
// declares, it is not this compiler's own std::map (0x10 bytes) but a 0x14-byte tree with a nil
// node of its own at +8, the multi flag at +0xc and the size at +0x10. Only the members the
// rebuilt functions call are declared: the index operator 0x0042b004, the destructor 0x0042b077,
// the tree's initialisation 0x0042b346 and the copy of another tree's nodes 0x0042b2e4.
struct fn_0042b004_map
{
	typedef std::pair< long, long > key_type;
	struct node;

	explicit fn_0042b004_map( const std::less< key_type >& pred = std::less< key_type >(), const std::allocator< tHexPoint >& al = std::allocator< tHexPoint >() )
		: m_alloc( al ), m_comp( pred ), m_multi( false ) { fn_0042b346(); }
	fn_0042b004_map( const fn_0042b004_map& other )
		: m_alloc( other.m_alloc ), m_comp( other.m_comp ), m_multi( other.m_multi ) { fn_0042b346(); fn_0042b2e4( other ); }
	~fn_0042b004_map();
	void fn_0042b346();
	void fn_0042b2e4( const fn_0042b004_map& other );
	tHexPoint& operator[]( const key_type& key );

	std::allocator< tHexPoint > m_alloc;
	std::less< key_type > m_comp;
	char	m_pad[2];
	node*	m_head;
	node*	m_nil;
	bool	m_multi;
	unsigned int	m_size;
};

// HYPOTHESIS: 0x005419e4 is a bool query on the 0x4c-byte elements of the vector the member
// at +0xf0 guards; the element type is not recovered.
// The analysis names its constructor 0x0054185f and destructor 0x0042a863 tShipCache.
// HYPOTHESIS: the constructor's argument is the object 0x0054385d also reads a tID from
// through its virtual function at +0x30.
class fn_0054385d_source;
class fn_005419e4_entry
{
public:
	fn_005419e4_entry(fn_0054385d_source *source);
	fn_005419e4_entry &operator=(const fn_005419e4_entry &other);
	fn_005419e4_entry(const fn_005419e4_entry &other);
	~fn_005419e4_entry();
	bool fn_005419e4() const;
	nDatabase::tID fn_005419ed() const;
	char m_00[0x4c];
};

// HYPOTHESIS: the object an entry is built from; its virtual function at +0x30 returns its
// database identifier, as with the class tClientCharacter.cpp declares as tObject_00543551,
// which is probably the same class (declared there only, so not shared yet).
class fn_0054385d_source
{
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
	virtual const nDatabase::tID &vfunction30() const;
};

// HYPOTHESIS: the member at +0xf0 is the SDK's tSingleAccessWrapper< std::vector< X > > and
// the lock below its iConstPtr (the SDK's tAccessPointer). Both templates are declared only
// inside unsorted/tClientCharacter.cpp, so they are written here under names of their own
// until they move to a shared header: Lock 0x0042c709, operator* 0x00604259, Done 0x005f4352.
class fn_0042c709_ptr
{
public:
	~fn_0042c709_ptr();
	void Done();
	const std::vector< fn_005419e4_entry > &operator*();

	mutable tOptex fAccess;
	const std::vector< fn_005419e4_entry > *fPointer;
	const void *fPointerGuard;
};

inline fn_0042c709_ptr::~fn_0042c709_ptr()
{
	Done();
}

// HYPOTHESIS: the wrapper's non-const lock (the SDK's iPtr); it shares Lock 0x0042c709,
// operator* 0x00604259 and Done 0x005f4352 with the const one through identical code folding.
class fn_0042c709_iptr
{
public:
	~fn_0042c709_iptr();
	void Done();
	std::vector< fn_005419e4_entry > &operator*();

	mutable tOptex fAccess;
	std::vector< fn_005419e4_entry > *fPointer;
	const void *fPointerGuard;
};

inline fn_0042c709_iptr::~fn_0042c709_iptr()
{
	Done();
}

class fn_0042c709_wrapper
{
public:
	fn_0042c709_iptr Lock();
	fn_0042c709_ptr Lock() const;

	std::vector< fn_005419e4_entry > *fpPointer;
	mutable tOptex fPointerOptex;
};

// HYPOTHESIS: the object at +0x14c of the class below is a tHexPoint: 0x00543ab1 assigns it
// with tHexPoint's assignment, and 0x005b6a5c sits among tHexPoint's members.








// HYPOTHESIS: a member of an unidentified class that returns a one-bit flag kept in
// bit 8 of the dword at +0x114, read as a bit-field (`shr eax, 8; and al, 1`).
// HYPOTHESIS: 0x005430d3 works on the same object (it sits next to the bit-field
// accessors); +0x138 is a count it lowers without going below zero.
// HYPOTHESIS: +0xd4 and +0xd8 are the begin and end of a container of tIDs whose
// object starts at +0xd0 (0x005430fa walks them).
class fn_005430ad_owner
{
public:
	bool fn_005430ad();
	void fn_005430b9(bool flag);
	void fn_005430d3(int amount);
	fn_0042b0fd_owner *fn_005430f3();
	bool fn_005430fa(const nDatabase::tID &id);
	int fn_0054312b();
	bool fn_00543132();
	bool fn_0054313e();
	fn_0042b004_map fn_00543149();
	tHexPoint fn_005434b1() const;
	int fn_005434cf();
	tHexPoint *fn_005434d6();
	fn_0042c709_ptr fn_005434dd() const;
	fn_0042c709_ptr fn_005434f7() const;
	int fn_00543514();
	nDatabase::tID fn_0054351b() const;
	std::string fn_00543551() const;
	std::string fn_00543587() const;
	tHexPoint *fn_005435c3();
	bool fn_005435ca(std::string text);
	void fn_0054360a();
	void fn_0054361f();
	void fn_00543634();
	void fn_00543649(bool flag);
	void fn_00543671(bool flag);
	void fn_0054368b(bool flag);
	void fn_005436a5(bool flag);
	bool fn_005436bf(std::string text);
	bool fn_00543702(int value);
	bool fn_00543711(int value);
	bool fn_00543720(int value);
	bool fn_0054372f(fn_0054385d_source *source);
	void fn_005437a5(const nDatabase::tID &id);
	void fn_0054385d(fn_0054385d_source *source);
	bool fn_00543944(const nDatabase::tID &id) const;
	bool fn_005439e4(int value);
	bool fn_005439f3(int value);
	bool fn_00543a02(int value);
	bool fn_00543a11(float value);
	bool fn_00543a20(const tHexPoint &point);
	void fn_00543aa4(int value);
	void fn_00543ab1(const tHexPoint &point);
	void fn_00543ac3(const nDatabase::tID &id);
	bool fn_00543ae2(int value);
	bool fn_00543294() const;
	int fn_0054330e(const nDatabase::tID &id);
	void fn_0054334a(int flags);
	int fn_00543357(const nDatabase::tID &id);
	std::string fn_005433a7() const;
	bool fn_005433dd();
	bool fn_005433e8();
	bool fn_005433f4();
	bool fn_00543400();
	bool fn_0054340c();
	bool fn_00543437();
	void fn_00543445();
	int fn_00543455();
	std::string fn_00543459() const;
	int fn_00543495();
	int fn_0054349c();
	int fn_005434a3();
	float fn_005434aa();

	char m_pad[0x48];
	std::string m048;
	time_t m058;
	fn_0042c709_wrapper m05c;
	std::string m074;
	std::string m084;
	char m_pad94[0x4];
	// HYPOTHESIS: 0x00543a20 resets +0xac with tHexPoint's assignment, so +0x98..+0xc0 holds
	// two tHexPoints.
	tHexPoint m098;
	tHexPoint m0ac;
	nDatabase::tID m0c0;
	fn_0042b0fd_owner m0d0;
	std::string m0e0;
	fn_0042c709_wrapper m0f0;
	int m108;
	int m10c;
	int m110;
	unsigned int m114_0 : 1;
	unsigned int m114_1 : 1;
	unsigned int m114_2 : 1;
	unsigned int m114_3 : 1;
	unsigned int m114_4 : 1;
	unsigned int m114_5 : 2;
	unsigned int m114_7 : 1;
	unsigned int m114_8 : 1;
	unsigned int m114_9 : 1;
	unsigned int m114_10 : 1;
	float m118;
	tHexPoint m11c;
	int m130;
	int m134;
	int m138;
	int m13c;
	char m_pad140[0x4];
	int m144;
	int m148;
	tHexPoint m14c;
};

// FUNCTION: STARFLEETOP 0x005430ad
bool fn_005430ad_owner::fn_005430ad()
{
	return m114_8 ? true : false;
}

// FUNCTION: STARFLEETOP 0x005430b9
void fn_005430ad_owner::fn_005430b9(bool flag)
{
	if (flag)
		m114_8 = 1;
	else
		m114_8 = 0;
}

// FUNCTION: STARFLEETOP 0x005430d3
void fn_005430ad_owner::fn_005430d3(int amount)
{
	if (amount < m138)
		m138 -= amount;
	else
		m138 = 0;
}

// HYPOTHESIS: 0x005430f3 works on the same object; it returns the address of a
// member embedded at +0xd0 whose type is not known yet.
// FUNCTION: STARFLEETOP 0x005430f3
fn_0042b0fd_owner *fn_005430ad_owner::fn_005430f3()
{
	return &m0d0;
}

// FUNCTION: STARFLEETOP 0x005430fa
bool fn_005430ad_owner::fn_005430fa(const nDatabase::tID &id)
{
	for (nDatabase::tID *entry = m0d0.m_04; entry != m0d0.m_08; entry++)
		if (*entry == id)
			return true;
	return false;
}

// HYPOTHESIS: the dword at +0x134 is returned as an int; its type is not known yet.
// FUNCTION: STARFLEETOP 0x0054312b
int fn_005430ad_owner::fn_0054312b()
{
	return m134;
}

// HYPOTHESIS: a second one-bit flag, bit 9 of the dword at +0x114.
// FUNCTION: STARFLEETOP 0x00543132
bool fn_005430ad_owner::fn_00543132()
{
	return m114_9 ? true : false;
}

// HYPOTHESIS: a third one-bit flag, bit 7 of the dword at +0x114. The original
// tests it as `test byte ptr [ecx+0x114], 0x80; setne al`; every source form tried
// (bit-field, mask, byte cast) compiles to a shift instead.
// FUNCTION: STARFLEETOP 0x0054313e
bool fn_005430ad_owner::fn_0054313e()
{
	// HYPOTHESIS: byte-level read of the flag; shr remains instead of test/setne.
	return (((unsigned char *)this)[0x114] & 0x80) ? true : false;
}

// HYPOTHESIS: this class is the one tClientCharacter.cpp declares as tObject_00543551 (both
// call fn_005434b1 and fn_0054312b). The function maps this hex and its six neighbours by their
// two coordinates.
// FUNCTION: STARFLEETOP 0x00543149
fn_0042b004_map fn_005430ad_owner::fn_00543149()
{
	tMetaMapHex hex( false );
	hex.fn_0053a49c( fn_005434b1() );
	std::vector< tHexPoint > points = hex.fn_00539ff1( -1, -1 );
	points.push_back( hex.fn_00539f9c() );
	fn_0042b004_map result;
	for ( std::vector< tHexPoint >::iterator it = points.begin(); it != points.end(); it++ )
		result[ std::pair< long, long >( it->fn_0055f269(), it->fn_005e8f21() ) ] = *it;
	return result;
}

// FUNCTION: STARFLEETOP 0x00543294
bool fn_005430ad_owner::fn_00543294() const
{
	fn_0042c709_ptr list = m0f0.Lock();
	for ( std::vector< fn_005419e4_entry >::const_iterator it = (*list).begin(); it != (*list).end(); it++ )
		if ( it->fn_005419e4() )
			return true;
	return false;
}

// FUNCTION: STARFLEETOP 0x0054330e
int fn_005430ad_owner::fn_0054330e(const nDatabase::tID &id)
{
	if ( !fn_005430fa( id ) )
		m0d0.fn_0042add2( id );
	return m0d0.size();
}

// HYPOTHESIS: +0x134 holds bit flags; this sets the given ones.
// FUNCTION: STARFLEETOP 0x0054334a
void fn_005430ad_owner::fn_0054334a(int flags)
{
	m134 |= flags;
}

// FUNCTION: STARFLEETOP 0x00543357
int fn_005430ad_owner::fn_00543357(const nDatabase::tID &id)
{
	for (nDatabase::tID *entry = m0d0.m_04; entry != m0d0.m_08; entry++)
		if (*entry == id)
		{
			m0d0.fn_00544725( entry );
			break;
		}
	return m0d0.size();
}

// HYPOTHESIS: +0x48 holds a std::string returned by copy (the allocator byte, the
// string's reset 0x005da20a and its assign 0x00403b49).
// FUNCTION: STARFLEETOP 0x005433a7
std::string fn_005430ad_owner::fn_005433a7() const
{
	return m048;
}

// HYPOTHESIS: a fourth one-bit flag, bit 1 of the dword at +0x114.
// FUNCTION: STARFLEETOP 0x005433dd
bool fn_005430ad_owner::fn_005433dd()
{
	return m114_1 ? true : false;
}

// HYPOTHESIS: a fifth one-bit flag, bit 2 of the dword at +0x114.
// FUNCTION: STARFLEETOP 0x005433e8
bool fn_005430ad_owner::fn_005433e8()
{
	return m114_2 ? true : false;
}

// HYPOTHESIS: a sixth one-bit flag, bit 3 of the dword at +0x114.
// FUNCTION: STARFLEETOP 0x005433f4
bool fn_005430ad_owner::fn_005433f4()
{
	return m114_3 ? true : false;
}

// HYPOTHESIS: a seventh one-bit flag, bit 10 of the dword at +0x114.
// FUNCTION: STARFLEETOP 0x00543400
bool fn_005430ad_owner::fn_00543400()
{
	return m114_10 ? true : false;
}

// FUNCTION: STARFLEETOP 0x0054340c
bool fn_005430ad_owner::fn_0054340c()
{
	return !fn_005433e8() && !fn_005433f4() && !fn_00543400();
}

// FUNCTION: STARFLEETOP 0x00543437
bool fn_005430ad_owner::fn_00543437()
{
	unsigned int v = *(unsigned int*)((char*)&m10c + 0x8);
	unsigned char r = ~v;
	r >>= 4;
	r &= 1;
	return r;
}

// FUNCTION: STARFLEETOP 0x00543445
void fn_005430ad_owner::fn_00543445()
{
	m14c.fn_005b6a5c(0, 0);
}

// HYPOTHESIS: the dword at +0x58 is returned as an int; its type is not known yet.
// FUNCTION: STARFLEETOP 0x00543455
int fn_005430ad_owner::fn_00543455()
{
	return m058;
}

// HYPOTHESIS: +0xe0 holds a second std::string, returned by copy like +0x48.
// FUNCTION: STARFLEETOP 0x00543459
std::string fn_005430ad_owner::fn_00543459() const
{
	return m0e0;
}

// HYPOTHESIS: the dword at +0x108 is returned as an int; its type is not known yet.
// FUNCTION: STARFLEETOP 0x00543495
int fn_005430ad_owner::fn_00543495()
{
	return m108;
}

// HYPOTHESIS: the dword at +0x130 is returned as an int; its type is not known yet.
// FUNCTION: STARFLEETOP 0x0054349c
int fn_005430ad_owner::fn_0054349c()
{
	return m130;
}

// HYPOTHESIS: the dword at +0x110 is returned as an int; its type is not known yet.
// FUNCTION: STARFLEETOP 0x005434a3
int fn_005430ad_owner::fn_005434a3()
{
	return m110;
}

// HYPOTHESIS: the dword at +0x118 is a float (read with fld).
// FUNCTION: STARFLEETOP 0x005434aa
float fn_005430ad_owner::fn_005434aa()
{
	return m118;
}

// FUNCTION: STARFLEETOP 0x005434b1
tHexPoint fn_005430ad_owner::fn_005434b1() const
{
	return m11c;
}

// HYPOTHESIS: the dword at +0x148 is returned as an int; its type is not known yet.
// FUNCTION: STARFLEETOP 0x005434cf
int fn_005430ad_owner::fn_005434cf()
{
	return m148;
}

// FUNCTION: STARFLEETOP 0x005434d6
tHexPoint *fn_005430ad_owner::fn_005434d6()
{
	return &m14c;
}

// HYPOTHESIS: +0x5c is a second guarded vector of the same wrapper type as +0xf0 (both lock
// through 0x0042c709); its element type is not recovered.
// FUNCTION: STARFLEETOP 0x005434dd
fn_0042c709_ptr fn_005430ad_owner::fn_005434dd() const
{
	return m05c.Lock();
}

// FUNCTION: STARFLEETOP 0x005434f7
fn_0042c709_ptr fn_005430ad_owner::fn_005434f7() const
{
	return m0f0.Lock();
}

// FUNCTION: STARFLEETOP 0x00543514
int fn_005430ad_owner::fn_00543514()
{
	return m144;
}

// The copy writes the tStreamable vtable, then tID's (0x00798244): the member at +0xc0 is a tID.
// FUNCTION: STARFLEETOP 0x0054351b
nDatabase::tID fn_005430ad_owner::fn_0054351b() const
{
	return m0c0;
}

// HYPOTHESIS: +0x74 holds a third std::string, returned by copy like +0x48.
// FUNCTION: STARFLEETOP 0x00543551
std::string fn_005430ad_owner::fn_00543551() const
{
	return m074;
}

// HYPOTHESIS: +0x84 holds a fourth std::string, returned by copy like +0x74.
// FUNCTION: STARFLEETOP 0x00543587
std::string fn_005430ad_owner::fn_00543587() const
{
	return m084;
}

// HYPOTHESIS: returns the address of a member embedded at +0x98 whose type is not known yet.
// FUNCTION: STARFLEETOP 0x005435c3
tHexPoint *fn_005430ad_owner::fn_005435c3()
{
	return &m098;
}

// HYPOTHESIS: the setter for the string at +0x48 (0x005433a7 is its getter); it takes the
// string by value (ret 0x10) and always reports success.
// FUNCTION: STARFLEETOP 0x005435ca
bool fn_005430ad_owner::fn_005435ca(std::string text)
{
	m048 = text;
	return true;
}

// HYPOTHESIS: clears the two-bit field at bits 5-6 of the dword at +0x114 and sets bit 4.
// FUNCTION: STARFLEETOP 0x0054360a
void fn_005430ad_owner::fn_0054360a()
{
	m114_5 = 0;
	m114_4 = 1;
}

// HYPOTHESIS: sets the two-bit field at bits 5-6 of the dword at +0x114 to 1 and clears bit 4.
// FUNCTION: STARFLEETOP 0x0054361f
void fn_005430ad_owner::fn_0054361f()
{
	m114_4 = 0;
	m114_5 = 1;
}

// HYPOTHESIS: sets the two-bit field at bits 5-6 of the dword at +0x114 to 2 and clears bit 4.
// FUNCTION: STARFLEETOP 0x00543634
void fn_005430ad_owner::fn_00543634()
{
	m114_4 = 0;
	m114_5 = 2;
}

// HYPOTHESIS: stamps +0x58 with the current time (0x007390fa is the CRT's time) and sets or
// clears bit 1 of the dword at +0x114.
// FUNCTION: STARFLEETOP 0x00543649
void fn_005430ad_owner::fn_00543649(bool flag)
{
	time(&m058);
	if (flag)
		m114_1 = 1;
	else
		m114_1 = 0;
}

// HYPOTHESIS: sets or clears bit 2 of the dword at +0x114 (read by fn_005433e8).
// FUNCTION: STARFLEETOP 0x00543671
void fn_005430ad_owner::fn_00543671(bool flag)
{
	if (flag)
		m114_2 = 1;
	else
		m114_2 = 0;
}

// HYPOTHESIS: sets or clears bit 3 of the dword at +0x114 (read by fn_005433f4).
// FUNCTION: STARFLEETOP 0x0054368b
void fn_005430ad_owner::fn_0054368b(bool flag)
{
	if (flag)
		m114_3 = 1;
	else
		m114_3 = 0;
}

// HYPOTHESIS: sets or clears bit 10 of the dword at +0x114 (bit 2 of the byte at +0x115).
// FUNCTION: STARFLEETOP 0x005436a5
void fn_005430ad_owner::fn_005436a5(bool flag)
{
	if (flag)
		m114_10 = 1;
	else
		m114_10 = 0;
}

// HYPOTHESIS: the setter for the string at +0xe0 (0x00543459 is its getter), shaped like 0x005435ca.
// FUNCTION: STARFLEETOP 0x005436bf
bool fn_005430ad_owner::fn_005436bf(std::string text)
{
	m0e0 = text;
	return true;
}

// HYPOTHESIS: the setter for the dword at +0x108 (0x00543495 is its getter); always reports success.
// FUNCTION: STARFLEETOP 0x00543702
bool fn_005430ad_owner::fn_00543702(int value)
{
	m108 = value;
	return true;
}

// HYPOTHESIS: the setter for the dword at +0x130 (paired with its getter above); always reports success.
// FUNCTION: STARFLEETOP 0x00543711
bool fn_005430ad_owner::fn_00543711(int value)
{
	m130 = value;
	return true;
}

// HYPOTHESIS: the setter for the dword at +0x10c; always reports success.
// FUNCTION: STARFLEETOP 0x00543720
bool fn_005430ad_owner::fn_00543720(int value)
{
	m10c = value;
	return true;
}

// HYPOTHESIS: appends an entry built from the argument to the guarded vector at +0xf0.
// FUNCTION: STARFLEETOP 0x0054372f
bool fn_005430ad_owner::fn_0054372f(fn_0054385d_source *source)
{
	(*m0f0.Lock()).push_back(fn_005419e4_entry(source));
	return true;
}

// HYPOTHESIS: removes every entry of the guarded vector at +0xf0 whose tID equals the argument.
// FUNCTION: STARFLEETOP 0x005437a5
void fn_005430ad_owner::fn_005437a5(const nDatabase::tID &id)
{
	fn_0042c709_iptr list = m0f0.Lock();
	std::vector< fn_005419e4_entry >::iterator it = (*list).begin();
	while ( it != (*list).end() )
	{
		if ( it->fn_005419ed() == id )
			it = (*list).erase( it );
		else
			it++;
	}
	list.Done();
}

// HYPOTHESIS: replaces the first entry of the guarded vector at +0xf0 whose tID is the
// argument's with a new entry built from the argument.
// FUNCTION: STARFLEETOP 0x0054385d
void fn_005430ad_owner::fn_0054385d(fn_0054385d_source *source)
{
	fn_0042c709_iptr list = m0f0.Lock();
	std::vector< fn_005419e4_entry >::iterator it;
	for ( it = (*list).begin(); it != (*list).end(); it++ )
		if ( it->fn_005419ed() == source->vfunction30() )
		{
			fn_005419e4_entry entry( source );
			*it = entry;
			break;
		}
	// MATCH-ONLY: the original calls operator* once more after the loop and drops the result;
	// the statement that did so is not recovered.
	(*list).end();
	list.Done();
}

// FUNCTION: STARFLEETOP 0x00543944
bool fn_005430ad_owner::fn_00543944(const nDatabase::tID &id) const
{
	fn_0042c709_ptr list = m0f0.Lock();
	for ( std::vector< fn_005419e4_entry >::const_iterator it = (*list).begin(); it != (*list).end(); it++ )
		if ( it->fn_005419ed() == id )
			return true;
	return false;
}

// HYPOTHESIS: the setter for the dword at +0x138; always reports success.
// FUNCTION: STARFLEETOP 0x005439e4
bool fn_005430ad_owner::fn_005439e4(int value)
{
	m138 = value;
	return true;
}

// HYPOTHESIS: the setter for the dword at +0x13c; always reports success.
// FUNCTION: STARFLEETOP 0x005439f3
bool fn_005430ad_owner::fn_005439f3(int value)
{
	m13c = value;
	return true;
}

// HYPOTHESIS: the setter for the dword at +0x110; always reports success.
// FUNCTION: STARFLEETOP 0x00543a02
bool fn_005430ad_owner::fn_00543a02(int value)
{
	m110 = value;
	return true;
}

// HYPOTHESIS: the setter for the float at +0x118; always reports success.
// FUNCTION: STARFLEETOP 0x00543a11
bool fn_005430ad_owner::fn_00543a11(float value)
{
	m118 = value;
	return true;
}

// HYPOTHESIS: the setter for the point at +0x11c; a new point resets the one at +0xac.
// FUNCTION: STARFLEETOP 0x00543a20
bool fn_005430ad_owner::fn_00543a20(const tHexPoint &point)
{
	if (point != fn_005434b1())
	{
		m11c = point;
		m0ac = tHexPoint(-1, -1);
	}
	return true;
}

// HYPOTHESIS: the setter for the dword at +0x148, whose getter sits earlier in this unit.
// FUNCTION: STARFLEETOP 0x00543aa4
void fn_005430ad_owner::fn_00543aa4(int value)
{
	m148 = value;
}

// FUNCTION: STARFLEETOP 0x00543ab1
void fn_005430ad_owner::fn_00543ab1(const tHexPoint &point)
{
	m14c = point;
}

// HYPOTHESIS: the setter for the tID at +0xc0, whose getter is 0x0054351b.
// FUNCTION: STARFLEETOP 0x00543ac3
void fn_005430ad_owner::fn_00543ac3(const nDatabase::tID &id)
{
	m0c0 = id;
}

// HYPOTHESIS: the setter for the dword at +0x144, whose getter sits earlier in this unit;
// always reports success.
// FUNCTION: STARFLEETOP 0x00543ae2
bool fn_005430ad_owner::fn_00543ae2(int value)
{
	m144 = value;
	return true;
}
