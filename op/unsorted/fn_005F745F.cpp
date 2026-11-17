// Unit:      unsorted/fn_005F745F.cpp
// Functions: 0x005f745f to 0x005f7fd4, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
#include <string>
#include "Common/Thread/Optex.h"

// MATCH-ONLY: inline in the SDK, as in unsorted/tClientCharacter.cpp, which claims its
// out-of-line copy (0x00429a18); the header cannot define it while that unit does.
inline tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->Leave();
	}
}

// HYPOTHESIS: the object at +0x04 of fn_005f745f_object; 0x005f77c6 takes one argument whose
// type is not known.
// HYPOTHESIS: the queued entry: 0x18 bytes on the stack, its copy constructor is 0x005f7526
// and its destructor inlines to the string's _Tidy (0x005da20a); +0x10 and +0x14 are not known.
struct fn_005f74a6_entry
{
	fn_005f74a6_entry( const fn_005f74a6_entry& other );
	fn_005f74a6_entry( const std::string& value, int a, int b );

	std::string field_00;
	int field_10;
	int field_14;
};

// HYPOTHESIS: a queue of entries: 0x005f77a7 reads the last entry, 0x005f7897 destroys and drops it.
class fn_005f77c6_object
{
public:
	void FUN_005f77c6( const fn_005f74a6_entry& value );
	fn_005f74a6_entry& FUN_005f77a7();
	void FUN_005f7897();
	~fn_005f77c6_object();
	// HYPOTHESIS: frees the block map; the original calls 0x005d97c1, a body shared with the
	// queue in unsorted/fn_005D913F.cpp, whose class is declared only in that unit.
	void FUN_005d97c1();
	void FUN_005f7afe( unsigned int count ); // HYPOTHESIS: grows the block map
	// HYPOTHESIS: reports a length error (the analysis names it for length_error)
	void FUN_005f7f20();

	byte field_00[0x04];
	fn_005f74a6_entry** field_04; // HYPOTHESIS: the map of blocks of 170 entries
	unsigned int field_08; // HYPOTHESIS: the map size
	unsigned int field_0c; // HYPOTHESIS: the index of the first entry
	unsigned int field_10; // HYPOTHESIS: the entry count; 0x005f76d5 tests it
};

// HYPOTHESIS: a position in fn_005f77c6_object: an index and the owner; 0x005f77a7 builds one
// past the last entry and steps it back, 0x005f7c17 reads the entry it points at.
class fn_005f7c17_iterator
{
public:
	fn_005f7c17_iterator( int index, fn_005f77c6_object* owner )
		: fIndex( index ), fOwner( owner )
	{
	}
	fn_005f7c17_iterator& operator-=( int n )
	{
		fIndex -= n;
		return *this;
	}
	fn_005f7c17_iterator operator-( int n ) const
	{
		fn_005f7c17_iterator tmp = *this;
		return tmp -= n;
	}
	fn_005f74a6_entry& FUN_005f7c17();

	int fIndex;
	fn_005f77c6_object* fOwner;
};

class fn_005f7961_object;

// HYPOTHESIS: the queued value: one string, copied by a copy constructor the original calls
// out of line (0x0055fe12); its destructor inlines to _Tidy (0x005da20a).
struct fn_0055fe12_value
{
	fn_0055fe12_value( const fn_0055fe12_value& other );
	fn_0055fe12_value( const std::string& value, int a );
	~fn_0055fe12_value();

	std::string field_00;
	int field_10;
};

// HYPOTHESIS: a position in fn_005f7961_object: an index and the owner; 0x005f7db6
// reads the value it points at.
class fn_005f7db6_iterator
{
public:
	fn_005f7db6_iterator( unsigned int index, fn_005f7961_object* owner )
		: fIndex( index ), fOwner( owner )
	{
	}
	fn_0055fe12_value& FUN_005f7db6();

	unsigned int fIndex;
	fn_005f7961_object* fOwner;
};

// HYPOTHESIS: the object at +0x18 of fn_005f745f_object, a queue of strings; 0x005f7961 takes
// one argument whose type is not known, 0x005f7902 drops the front.
class fn_005f7961_object
{
public:
	void FUN_005f7961( const fn_0055fe12_value& value );
	void FUN_005f7902();
	void FUN_005f7c9d( unsigned int count ); // HYPOTHESIS: grows the block map
	~fn_005f7961_object();
	void FUN_005f7c4b(); // HYPOTHESIS: destroys and drops the last value
	// HYPOTHESIS: frees the block map; the same shared body as in fn_005f77c6_object.
	void FUN_005d97c1();

	byte field_00[0x04];
	fn_0055fe12_value** field_04; // HYPOTHESIS: the map of blocks of 204 values
	unsigned int field_08; // HYPOTHESIS: the map size
	unsigned int field_0c; // HYPOTHESIS: the index of the first value
	unsigned int field_10; // HYPOTHESIS: the value count; 0x005f76f8 tests it
};

// HYPOTHESIS: the 4-byte value queued at +0x2c of fn_005f745f_object; 0x005f767d returns it
// through a hidden pointer, so it is not a plain int.
struct fn_005f7a4b_value
{
	fn_005f7a4b_value& operator=( const fn_005f7a4b_value& other );

	int field_00;
};

class fn_005f7a6a_object;

// HYPOTHESIS: a position in fn_005f7a6a_object, as fn_005f7c17_iterator; 0x005f7efa reads the
// value it points at.
class fn_005f7efa_iterator
{
public:
	fn_005f7efa_iterator( int index, fn_005f7a6a_object* owner )
		: fIndex( index ), fOwner( owner )
	{
	}
	fn_005f7efa_iterator& operator-=( int n )
	{
		fIndex -= n;
		return *this;
	}
	fn_005f7efa_iterator operator-( int n ) const
	{
		fn_005f7efa_iterator tmp = *this;
		return tmp -= n;
	}
	fn_005f7a4b_value& FUN_005f7efa();

	int fIndex;
	fn_005f7a6a_object* fOwner;
};

// HYPOTHESIS: the object at +0x2c of fn_005f745f_object; 0x005f7a6a takes its argument by
// reference, its type is not known; 0x005f7a4b reads the last value, 0x005f7aeb drops one.
class fn_005f7a6a_object
{
public:
	void FUN_005f7a6a( const int& value );
	fn_005f7a4b_value& FUN_005f7a4b();
	void FUN_005f7aeb();
	void FUN_005f7dea( unsigned int count ); // HYPOTHESIS: grows the block map

	byte field_00[0x04];
	fn_005f7a4b_value** field_04; // HYPOTHESIS: the map of blocks
	unsigned int field_08; // HYPOTHESIS: the map size
	unsigned int field_0c; // HYPOTHESIS: the index of the first value
	unsigned int field_10; // HYPOTHESIS: the value count; 0x005f771b tests it
};

// HYPOTHESIS: a container guarded by the lock at +0x40; only the members used here are placed.
class fn_005f745f_object
{
public:
	void FUN_005f745f( const fn_005f74a6_entry& value );
	fn_005f74a6_entry FUN_005f74a6();
	void FUN_005f755c( const fn_0055fe12_value& value );
	fn_0055fe12_value FUN_005f75a3();
	void FUN_005f7635( int value );
	fn_005f7a4b_value FUN_005f767d();
	bool FUN_005f76d5();
	bool FUN_005f76f8();
	bool FUN_005f771b();

	byte field_00[0x04];
	fn_005f77c6_object field_04;
	fn_005f7961_object field_18;
	fn_005f7a6a_object field_2c;
	tOptex field_40;
};

// FUNCTION: STARFLEETOP 0x005f745f
void fn_005f745f_object::FUN_005f745f( const fn_005f74a6_entry& value )
{
	tOptex::tGuard guard( &field_40 );

	field_04.FUN_005f77c6( value );

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x005f74a6
fn_005f74a6_entry fn_005f745f_object::FUN_005f74a6()
{
	tOptex::tGuard guard( &field_40 );

	fn_005f74a6_entry value( field_04.FUN_005f77a7() );
	field_04.FUN_005f7897();

	return value;
}

// HYPOTHESIS: the member-wise copy the compiler would generate, written out to be claimed.
// FUNCTION: STARFLEETOP 0x005f7526
fn_005f74a6_entry::fn_005f74a6_entry( const fn_005f74a6_entry& other )
	: field_00( other.field_00 ), field_10( other.field_10 ), field_14( other.field_14 )
{
}

// FUNCTION: STARFLEETOP 0x005f755c
void fn_005f745f_object::FUN_005f755c( const fn_0055fe12_value& value )
{
	tOptex::tGuard guard( &field_40 );

	field_18.FUN_005f7961( value );

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x005f75a3
fn_0055fe12_value fn_005f745f_object::FUN_005f75a3()
{
	tOptex::tGuard guard( &field_40 );

	fn_005f7db6_iterator front( field_18.field_0c, &field_18 );
	fn_0055fe12_value value( front.FUN_005f7db6() );
	field_18.FUN_005f7902();

	return value;
}

// HYPOTHESIS: the destructor the compiler would generate; the original has an out-of-line copy
// for the unwind of 0x005f75a3.
// FUNCTION: STARFLEETOP 0x005f762d
inline fn_0055fe12_value::~fn_0055fe12_value()
{
}

// FUNCTION: STARFLEETOP 0x005f7635
void fn_005f745f_object::FUN_005f7635( int value )
{
	tOptex::tGuard guard( &field_40 );

	field_2c.FUN_005f7a6a( value );

	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x005f767d
fn_005f7a4b_value fn_005f745f_object::FUN_005f767d()
{
	// As in 0x005f74a6, the original sets no unwind state before releasing the guard; this form
	// does, which is the remaining difference.
	tOptex::tGuard guard( &field_40 );

	fn_005f7a4b_value value( field_2c.FUN_005f7a4b() );
	field_2c.FUN_005f7aeb();

	return value;
}

// FUNCTION: STARFLEETOP 0x005f76d5
bool fn_005f745f_object::FUN_005f76d5()
{
	field_40.Enter();
	bool empty = field_04.field_10 == 0;
	field_40.Leave();
	return empty;
}

// FUNCTION: STARFLEETOP 0x005f76f8
bool fn_005f745f_object::FUN_005f76f8()
{
	field_40.Enter();
	bool empty = field_18.field_10 == 0;
	field_40.Leave();
	return empty;
}

// FUNCTION: STARFLEETOP 0x005f771b
bool fn_005f745f_object::FUN_005f771b()
{
	field_40.Enter();
	bool empty = field_2c.field_10 == 0;
	field_40.Leave();
	return empty;
}

// HYPOTHESIS: builds an entry from a string and the two values at +0x10 and +0x14.
// FUNCTION: STARFLEETOP 0x005f773e
fn_005f74a6_entry::fn_005f74a6_entry( const std::string& value, int a, int b )
	: field_00( value ), field_10( a ), field_14( b )
{
}

// HYPOTHESIS: builds a value from a string and the value at +0x10.
// FUNCTION: STARFLEETOP 0x005f7776
fn_0055fe12_value::fn_0055fe12_value( const std::string& value, int a )
	: field_00( value ), field_10( a )
{
}

// FUNCTION: STARFLEETOP 0x005f77a7
fn_005f74a6_entry& fn_005f77c6_object::FUN_005f77a7()
{
	return (fn_005f7c17_iterator( field_10 + field_0c, this ) - 1).FUN_005f7c17();
}

// FUNCTION: STARFLEETOP 0x005f77c6
void fn_005f77c6_object::FUN_005f77c6( const fn_005f74a6_entry& value )
{
	if ( (field_0c + field_10) % 170 == 0 && field_08 <= (field_10 + 170) / 170 ) {
		FUN_005f7afe( 1 );
	}
	unsigned int off = field_0c + field_10;
	unsigned int block = off / 170;
	if ( field_08 <= block ) {
		block -= field_08;
	}
	if ( field_04[block] == 0 ) {
		field_04[block] = (fn_005f74a6_entry*) operator new( 170 * sizeof( fn_005f74a6_entry ) );
	}
	unsigned int index = off % 170;
	try {
		++field_10;
		fn_005f74a6_entry* p = field_04[block] + index;
		new( p ) fn_005f74a6_entry( value );
	} catch ( ... ) {
		// HYPOTHESIS: the original's catch funclet (0x005f7886) is 17 bytes, the size of a member
		// call through this, not of this decrement; its callee is not known.
		--field_10;
		throw;
	}
}

// FUNCTION: STARFLEETOP 0x005f7897
void fn_005f77c6_object::FUN_005f7897()
{
	if ( field_10 != 0 ) {
		unsigned int off = field_0c + field_10 - 1;
		unsigned int block = off / 170;
		if ( field_08 <= block ) {
			block -= field_08;
		}
		fn_005f74a6_entry* p = field_04[block] + off % 170;
		p->~fn_005f74a6_entry();
		if ( --field_10 == 0 ) {
			field_0c = 0;
		}
	}
}

// FUNCTION: STARFLEETOP 0x005f78e9
fn_005f77c6_object::~fn_005f77c6_object()
{
	while ( field_10 != 0 ) {
		FUN_005f7897();
	}
	FUN_005d97c1();
}

// FUNCTION: STARFLEETOP 0x005f7902
void fn_005f7961_object::FUN_005f7902()
{
	if ( field_10 != 0 ) {
		fn_0055fe12_value* p = field_04[field_0c / 204] + field_0c % 204;
		p->~fn_0055fe12_value();
		if ( field_08 * 204 <= ++field_0c ) {
			field_0c = 0;
		}
		if ( --field_10 == 0 ) {
			field_0c = 0;
		}
	}
}

// FUNCTION: STARFLEETOP 0x005f7961
void fn_005f7961_object::FUN_005f7961( const fn_0055fe12_value& value )
{
	if ( (field_0c + field_10) % 204 == 0 && field_08 <= (field_10 + 204) / 204 ) {
		FUN_005f7c9d( 1 );
	}
	unsigned int off = field_0c + field_10;
	unsigned int block = off / 204;
	if ( field_08 <= block ) {
		block -= field_08;
	}
	if ( field_04[block] == 0 ) {
		field_04[block] = (fn_0055fe12_value*) operator new( 204 * sizeof( fn_0055fe12_value ) );
	}
	unsigned int index = off % 204;
	try {
		++field_10;
		fn_0055fe12_value* p = field_04[block] + index;
		new( p ) fn_0055fe12_value( value );
	} catch ( ... ) {
		// HYPOTHESIS: as in 0x005f77c6, the original's catch funclet (0x005f7a21) is not known.
		--field_10;
		throw;
	}
}

// FUNCTION: STARFLEETOP 0x005f7a32
fn_005f7961_object::~fn_005f7961_object()
{
	while ( field_10 != 0 ) {
		FUN_005f7c4b();
	}
	FUN_005d97c1();
}

// FUNCTION: STARFLEETOP 0x005f7a4b
fn_005f7a4b_value& fn_005f7a6a_object::FUN_005f7a4b()
{
	return (fn_005f7efa_iterator( field_10 + field_0c, this ) - 1).FUN_005f7efa();
}

// FUNCTION: STARFLEETOP 0x005f7a6a
void fn_005f7a6a_object::FUN_005f7a6a( const int& value )
{
	if ( (field_0c + field_10) % 1024 == 0 && field_08 <= (field_10 + 1024) / 1024 ) {
		FUN_005f7dea( 1 );
	}
	unsigned int off = field_0c + field_10;
	unsigned int block = off / 1024;
	if ( field_08 <= block ) {
		block -= field_08;
	}
	if ( field_04[block] == 0 ) {
		field_04[block] = (fn_005f7a4b_value*) operator new( 1024 * sizeof( fn_005f7a4b_value ) );
	}
	++field_10;
	int* p = (int*) ( field_04[block] + off % 1024 );
	new( p ) int( value );
}

// FUNCTION: STARFLEETOP 0x005f7aeb
void fn_005f7a6a_object::FUN_005f7aeb()
{
	if ( field_10 != 0 ) {
		if ( --field_10 == 0 ) {
			field_0c = 0;
		}
	}
}

// MATCH-ONLY: the library's uninitialized copy and fill, which the original inlined.
inline fn_005f74a6_entry** fn_005f7afe_copy( fn_005f74a6_entry** first, fn_005f74a6_entry** last, fn_005f74a6_entry** dest )
{
	for ( ; first != last; ++dest, ++first ) {
		*dest = *first;
	}
	return dest;
}

// MATCH-ONLY: as fn_005f7afe_copy.
inline void fn_005f7afe_fill( fn_005f74a6_entry** dest, unsigned int n, fn_005f74a6_entry* value )
{
	for ( ; 0 < n; --n, ++dest ) {
		*dest = value;
	}
}

// FUNCTION: STARFLEETOP 0x005f7afe
void fn_005f77c6_object::FUN_005f7afe( unsigned int count )
{
	if ( 0x101010 - field_08 < count ) {
		FUN_005f7f20();
	}
	unsigned int inc = field_08 / 2;
	if ( inc < 8 ) {
		inc = 8;
	}
	if ( count < inc && field_08 <= 0x101010 - inc ) {
		count = inc;
	}
	unsigned int boff = field_0c / 170;
	int bytes = ( field_08 + count ) * sizeof( fn_005f74a6_entry* );
	if ( bytes < 0 ) {
		bytes = 0;
	}
	fn_005f74a6_entry** newmap = (fn_005f74a6_entry**) operator new( bytes );
	fn_005f74a6_entry** p = fn_005f7afe_copy( field_04 + boff, field_04 + field_08, newmap + boff );
	if ( boff <= count ) {
		p = fn_005f7afe_copy( field_04, field_04 + boff, p );
		fn_005f7afe_fill( p, count - boff, 0 );
		fn_005f7afe_fill( newmap, boff, 0 );
	} else {
		fn_005f7afe_copy( field_04, field_04 + count, p );
		p = fn_005f7afe_copy( field_04 + count, field_04 + boff, newmap );
		fn_005f7afe_fill( p, count, 0 );
	}
	operator delete( field_04 );
	field_08 += count;
	field_04 = newmap;
}

// FUNCTION: STARFLEETOP 0x005f7c17
fn_005f74a6_entry& fn_005f7c17_iterator::FUN_005f7c17()
{
	unsigned int off = fIndex;
	unsigned int block = off / 170;
	unsigned int index = off - block * 170;
	if ( fOwner->field_08 <= block ) {
		block -= fOwner->field_08;
	}
	return fOwner->field_04[block][index];
}

// FUNCTION: STARFLEETOP 0x005f7c4b
void fn_005f7961_object::FUN_005f7c4b()
{
	if ( field_10 != 0 ) {
		unsigned int off = field_0c + field_10 - 1;
		unsigned int block = off / 204;
		if ( field_08 <= block ) {
			block -= field_08;
		}
		fn_0055fe12_value* p = field_04[block] + off % 204;
		p->~fn_0055fe12_value();
		if ( --field_10 == 0 ) {
			field_0c = 0;
		}
	}
}
