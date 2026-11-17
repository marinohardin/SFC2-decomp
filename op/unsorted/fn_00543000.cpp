#include "Common/Database/DatabaseID.h"
#include "unsorted/tMetaMapHex.h"
#include <vector>
#include <map>
#include "Common/Thread/Optex.h"
#include "unsorted/fn_0042b0fd_owner.h"
#include <string>

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
class fn_005419e4_entry
{
public:
	bool fn_005419e4() const;
	char m_00[0x4c];
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

class fn_0042c709_wrapper
{
public:
	fn_0042c709_ptr Lock() const;

	std::vector< fn_005419e4_entry > *fpPointer;
	mutable tOptex fPointerOptex;
};

// HYPOTHESIS: the object at +0x14c of the class below; 0x005b6a5c is a member taking two
// arguments, both passed as 0 by 0x00543445. The class is not recovered.
class fn_005b6a5c_owner
{
public:
	void fn_005b6a5c(int a, int b);
};

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
	fn_005b6a5c_owner *fn_005434d6();
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
	int m058;
	char m_pad5c[0x74];
	fn_0042b0fd_owner m0d0;
	std::string m0e0;
	fn_0042c709_wrapper m0f0;
	int m108;
	char m_pad10c[0x4];
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
	char m_pad13c[0xc];
	int m148;
	fn_005b6a5c_owner m14c;
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
	unsigned int v = *(unsigned int*)(m_pad10c + 0x8);
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
fn_005b6a5c_owner *fn_005430ad_owner::fn_005434d6()
{
	return &m14c;
}
