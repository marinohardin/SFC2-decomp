//	tAuctionItem.h: tAuctionItem.
// Evidence:  class (RTTI; original file unknown)

#ifndef tAuctionItem_h
#define tAuctionItem_h 1

#include <string>
#include "Common/Database/DatabaseObject.h"
#include "Common/Database/DatabaseID.h"

class tShip;

// HYPOTHESIS: layout read from the constructor; the fields between the named ones are not
// identified yet.
class tAuctionItem : public nDatabase::tDatabaseObject
{
public:
	tAuctionItem();
	// HYPOTHESIS: the double is the item's price or bid; the ship is read from the call order.
	tAuctionItem( double arg5, tShip* ship );
	// MATCH-ONLY: the original declares no destructor; its implicit one is 0x00544c24, left
	// unclaimed. This declared one is kept only so the scalar deleting destructor 0x00544c08
	// has a destructor definition to be claimed on.
	virtual ~tAuctionItem();
	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );
	virtual void SQLFillInsertStream( std::stringstream* SQL, bool lastOne = false );
	virtual void SQLFillUpdateStream( std::stringstream* SQL, bool lastOne = false );
	virtual const char* SQLTableName() const;
	// The exe's vtable holds 0x00545227 in tDatabaseObject's SQLFieldNames slot; the slots
	// after it are the base's, so the class adds no virtual of its own.
	virtual const char* SQLFieldNames() const;
	// HYPOTHESIS: the two integer arguments' meaning is unknown; fn_00544d20 stores their
	// product with the double at +0x88 (fild, so arg4 is a signed int).
	void fn_00544d20( const nDatabase::tID& id, std::string name, int arg3, int arg4, double arg5 );
	// HYPOTHESIS: places a bid; returns 0 when refused, 1 when outbid, 2 when the bidder leads.
	int fn_0054522d( const nDatabase::tID& bidder, int arg2, int32u bid, nDatabase::tID& owner, int32u* outBid );
	// HYPOTHESIS: small accessors read from their callers; 0x004b94dc, 0x00420f2f and 0x00420f3c
	// are bodies the linker shares with other classes' one-line members.
	bool fn_00545401() const;
	void fn_00545405( bool value );
	bool fn_00545560( int arg );
	std::string fn_00545581() const;
	const nDatabase::tID& fn_005455b7() const;
	int32u fn_004b94dc() const;
	const nDatabase::tID& fn_005455bb() const;
	int32u fn_005455c2() const;
	void fn_005455c9( int32u value );
	void fn_005455d6( int32u value );
	void fn_005455e3( int32u value );
	void fn_005455f0( const nDatabase::tID& id );
	void fn_00420f2f( int value );
	void fn_00420f3c( int32u value );
	bool fn_00545385( const nDatabase::tID& bidder );
	// HYPOTHESIS: returns bool; the caller 0x00545451 tests al.
	bool fn_0054540f( const nDatabase::tID& bidder );
	// HYPOTHESIS: the first argument's type and meaning are unknown; it is passed through to
	// fn_005454c1.
	int32u fn_00545451( int arg1, const nDatabase::tID& bidder );
	int32u fn_005454c1( int arg1, const nDatabase::tID& bidder );
protected:
	bool fBool_48;
	std::string fString_4c;
	nDatabase::tID fID_5c;
	int32s fInt_6c;
	int32s fInt_70;
	double fDouble_78;
	int32u fInt_80;
	int32u fInt_84;
	int32u fInt_88;
	nDatabase::tID fID_8c;
	int32u fInt_9c;
	int32u fInt_a0;
};

#endif
