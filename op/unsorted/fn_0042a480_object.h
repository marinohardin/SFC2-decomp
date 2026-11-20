//	fn_0042a480_object.h: the object whose members unsorted/fn_0042A000.cpp holds; the logon
//	information procedure reads its database identifier (0x0042a9ba).
// Evidence:  calls (original name and file unknown)

#if !defined( fn_0042a480_object_h )
#define fn_0042a480_object_h

#include <string>
#include <map>
#include <vector>
#include "Common/BaseTypes.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/SingleAccessWrapper.h"
#include "Common/Database/DatabaseID.h"
#include "unsorted/tHexPoint.h"
#include "unsorted/tServCharacter.h"

// HYPOTHESIS: the 0xd0-byte object tClientCharacter wraps at +0xcc (tObject_005391b0 in
// unsorted/tClientCharacter.cpp); only the member 0x0042a8b7 calls is placed, its argument and
// result types are guesses.
class fn_005395b2_object
{
public:
	void FUN_005395b2( int value );
};

// HYPOTHESIS: this is tClientCharacter (the string at +0x0c and the lock at +0xe4 match its
// layout), declared only inside unsorted/tClientCharacter.cpp; only the members used here are
// placed. 0x0042a535 and 0x0042a5ad assign strings at +0x1c and +0x30 under the same lock.
// 0x0042ac56 calls 0x0042abe4 on the object 0x0042a73d passes, so the flag and map members
// below belong to the same class.
class fn_0042a480_object
{
public:
	bool FUN_0042a403( int bit );
	// HYPOTHESIS: the adjacent byte getter at 0x0042a41a reads the same object.
	bool FUN_0042a41a();
	bool FUN_0042a421( int key0, int key1 );
	bool FUN_0042a45f();
	void FUN_0042a480();
	void FUN_0042a4f7( std::string value );
	void FUN_0042a535( const std::string& value );
	bool FUN_0042a587( int value );
	void FUN_0042a5ad( const std::string& value );
	void FUN_0042a5ff( int value );
	void FUN_0042a623( int value );
	void FUN_0042a647( int value );
	void FUN_0042a66b( int value );
	void FUN_0042a68f( tHexPoint value );
	void FUN_0042a6ea( const tHexPoint& value );
	void FUN_0042a6fc( bool value );
	void FUN_0042a709( int key0, int key1 );
	void FUN_0042a73d();
	// HYPOTHESIS: sets the whole flag word that 0x0042a403 tests.
	void FUN_0042a7b2( unsigned long flags );
	void FUN_0042a7bf( const std::vector< fn_0054185f_ship >& ships );
	void FUN_0042a8b7( int value );
	std::string FUN_0042a92f();
	const std::string& FUN_0042a99a();
	db::tID FUN_0042a9ba();
	const std::string& FUN_0042aa0b();
	int FUN_0042aa2b();
	int FUN_0042aa4b();
	int FUN_0042aa6b();
	int FUN_0042aa8b();
	int FUN_0042aaab();
	int FUN_0042aacb();
	tSingleAccessWrapper< std::vector< tServCharacter::tShipCache > >::iPtr FUN_0042aaeb();
	tSingleAccessWrapper< fn_005395b2_object >::iPtr FUN_0042ab08();
	void FUN_0042ab25( std::vector< db::tID >& ids );
	tHexPoint FUN_0042abe4();
	const tHexPoint& FUN_0042ac36();
	// HYPOTHESIS: the hex points around the one 0x0042abe4 returns, keyed by the two numbers
	// 0x0055f269 and 0x005e8f21 read from each; the map's values are assigned by tHexPoint's
	// operator= (0x005b69b0).
	std::map< std::pair< int, int >, tHexPoint > FUN_0042ac56();
	int FUN_0042ada1();

	byte field_00[0x0c];
	std::string field_0c;
	std::string field_1c;
	int field_2c;
	std::string field_30;
	int field_40;
	int field_44;
	int field_48;
	int field_4c;
	int field_50;
	int field_54;
	int field_58;
	tHexPoint field_5c;
	// HYPOTHESIS: a second hex point, sized by the gap to +0x84; 0x0042ac36 returns it.
	tHexPoint field_70;
	tHexPoint field_84;
	// HYPOTHESIS: a flag word tested by bit index.
	unsigned long field_98;
	// HYPOTHESIS: the byte flag that 0x0042a41a reads at +0x9c.
	bool field_9c;
	// HYPOTHESIS: a map from an 8-byte key to a byte flag; the callee 0x0042b1d3 is its find, the
	// node's value sits at +0x14 and end() is the head pointer at +0xa4. The key type is a guess.
	std::map< std::pair< int, int >, bool > field_a0;
	// HYPOTHESIS: the original's maps are 0x14 bytes (0x0042ac56 copies _Multi at +0x0c), as in the
	// SDK's STL, whose tree also holds a _Nil pointer; this unit's 0x10-byte map leaves this gap.
	byte field_b0[0xb4 - 0xb0];
	// HYPOTHESIS: the wrapped ship cache vector; its Lock is 0x0042c709.
	tSingleAccessWrapper< std::vector< tServCharacter::tShipCache > > field_b4;
	// HYPOTHESIS: tClientCharacter's wrapper at +0xcc of tObject_005391b0, declared only inside
	// unsorted/tClientCharacter.cpp; 0x0042a8b7 locks it and calls 0x005395b2 on the object.
	tSingleAccessWrapper< fn_005395b2_object > field_cc;
	tOptex field_e4;
};

#endif
