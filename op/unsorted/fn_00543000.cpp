#include "Common/Database/DatabaseID.h"
#include "unsorted/tMetaMapHex.h"
#include <vector>
#include <map>
#include "Common/Thread/Optex.h"
#include "unsorted/fn_0042b0fd_owner.h"

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
	bool fn_00543294() const;
	int fn_0054330e(const nDatabase::tID &id);

	char m_pad[0xd0];
	fn_0042b0fd_owner m0d0;
	char m0e0[0x10];
	fn_0042c709_wrapper m0f0;
	char m108[0xc];
	unsigned int m114_0 : 7;
	unsigned int m114_7 : 1;
	unsigned int m114_8 : 1;
	unsigned int m114_9 : 1;
	char m_pad118[0x1c];
	int m134;
	int m138;
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
