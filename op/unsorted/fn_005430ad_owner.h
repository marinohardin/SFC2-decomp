//	fn_005430ad_owner.h: the record unsorted/fn_00543000.cpp rebuilds and the types its members
//	hand out, shared with the units that call them.

#if !defined( fn_005430ad_owner_h )
#define fn_005430ad_owner_h

#include "Common/Database/DatabaseObject.h"
#include "Common/Database/DatabaseID.h"
#include "unsorted/tMetaMapHex.h"
#include <vector>
#include <map>
#include "Common/Thread/Optex.h"
#include "unsorted/fn_0042b0fd_owner.h"
#include <string>
#include <time.h>

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
class fn_005430ad_owner : public nDatabase::tDatabaseObject
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
	// HYPOTHESIS: an amount 0x00568b2e caps a repair cost by, compared unsigned.
	unsigned int fn_0054a8c8() const;
	bool fn_005439e4(int value);
	bool fn_005439f3(int value);
	bool fn_00543a02(int value);
	bool fn_00543a11(float value);
	bool fn_00543a20(const tHexPoint &point);
	void fn_00543aa4(int value);
	void fn_00543ab1(const tHexPoint &point);
	void fn_00543ac3(const nDatabase::tID &id);
	bool fn_00543ae2(int value);
	bool fn_00543af1(std::string text);
	bool fn_00543b31(int value);
	bool fn_00543b40(const tHexPoint &point);
	bool fn_00543b54(const tHexPoint &point);
	bool fn_00543b68(std::string text);
	bool fn_00543bab(int amount);
	bool fn_00543bba();
	void fn_00543bc3(bool flag);
	void fn_00543bdd(bool flag);
	// HYPOTHESIS: the exe's slot 6 of this class (0x00543c70) is SQLFillInsertStream.
	virtual void SQLFillInsertStream( std::stringstream* SQL, bool lastOne = false );
	// HYPOTHESIS: a member of tDatabaseObject (0x006fc200) that returns a string for a string
	// value; the SQL text writes every string field through it, between quotes.
	std::string fn_006fc200( std::string value );
	bool fn_00543294() const;
	int fn_0054330e(const nDatabase::tID &id);
	void fn_0054334a(int flags);
	int fn_00543357(const nDatabase::tID &id);
	std::string fn_005433a7() const;
	// 0x005433dd, 0x005433e8, 0x005433f4, 0x00543400, 0x0054340c and 0x00543445 are tServCharacter's
	// (AsyncDBSingle calls them on that class); they read this layout through a cast.



	bool fn_00543437();

	int fn_00543455();
	std::string fn_00543459() const;
	int fn_00543495();
	int fn_0054349c();
	int fn_005434a3();
	float fn_005434aa();

	std::string m048;
	time_t m058;
	fn_0042c709_wrapper m05c;
	std::string m074;
	std::string m084;
	int m094;
	// HYPOTHESIS: 0x00543a20 resets +0xac with tHexPoint's assignment, so +0x98..+0xc0 holds
	// two tHexPoints.
	tHexPoint m098;
	tHexPoint m0ac;
	nDatabase::tID m0c0;
	fn_0042b0fd_owner m0d0;
	std::string m0e0;
	fn_0042c709_wrapper m0f0;
	unsigned int m108;
	unsigned int m10c;
	unsigned int m110;
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
	long m134;
	int m138;
	int m13c;
	int m140;
	long m144;
	long m148;
	tHexPoint m14c;
};

#endif
