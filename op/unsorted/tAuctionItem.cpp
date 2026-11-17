#include <string>

#include "Common/Database/DatabaseObject.h"
#include "Common/Database/DatabaseID.h"
// MATCH-ONLY: the string writer is called out of line (0x0042a116); see the header's note.
#define DATASTORE_OUT_OF_LINE_WRITE
#include "Common/DataStore/DataStoreOutput.h"
#include "Common/DataStore/DataStoreInput.h"
#include "unsorted/tShip.h"

// HYPOTHESIS: layout read from the constructor; the fields between the named ones are not
// identified yet.
class tAuctionItem : public nDatabase::tDatabaseObject
{
public:
	tAuctionItem();
	// HYPOTHESIS: the double is the item's price or bid; the ship is read from the call order.
	tAuctionItem( double arg5, tShip* ship );
	// HYPOTHESIS: the original declares no destructor. The compiler-generated one (0x00544c24)
	// stores no vtable; an implicit destructor builds byte for byte, but the harness cannot claim
	// a function with no source line, so this declared one (which stores the vtable) stands in.
	virtual ~tAuctionItem();
	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	virtual void SQLFillInsertStream( std::stringstream* SQL, bool lastOne = false );
	virtual void SQLFillUpdateStream( std::stringstream* SQL, bool lastOne = false );
	virtual const char* SQLTableName() const;
	virtual const char* SQLFieldNames() const;

	// HYPOTHESIS: the two integer arguments' meaning is unknown; fn_00544d20 stores their
	// product with the double at +0x88 (fild, so arg4 is a signed int).
	void fn_00544d20( const nDatabase::tID& id, std::string name, int arg3, int arg4, double arg5 );
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

// FUNCTION: STARFLEETOP 0x00544b5c
tAuctionItem::tAuctionItem()
{
	fn_00544d20( nDatabase::tID(), "", 0, 0, 1.0 );
}

// FUNCTION: STARFLEETOP 0x00544c08
// FUNCTION: STARFLEETOP 0x00544c24
tAuctionItem::~tAuctionItem()
{
}

// FUNCTION: STARFLEETOP 0x00544c79
tAuctionItem::tAuctionItem( double arg5, tShip* ship )
{
	fn_00544d20( ship->vfunction12(), ship->fn_00546296(), ship->fn_00410223(), ship->fn_00410223(), arg5 );
}

// FUNCTION: STARFLEETOP 0x00544d20
void tAuctionItem::fn_00544d20( const nDatabase::tID& id, std::string name, int arg3, int arg4, double arg5 )
{
	fBool_48 = false;
	fString_4c = name;
	fID_5c = id;
	fInt_6c = arg3;
	fInt_70 = arg4;
	fDouble_78 = arg5;
	fInt_80 = 0;
	fInt_84 = 0;
	fInt_88 = arg4 * arg5;
	fInt_9c = 0;
	fInt_a0 = 0;
}

// FUNCTION: STARFLEETOP 0x00544db7
void tAuctionItem::StreamOut( nDataStore::tOutput& output ) const
{
	tDatabaseObject::StreamOut( output );

	output << fBool_48;
	output << fString_4c << fID_5c << fInt_6c << fInt_70 << fDouble_78 << fInt_80 << fInt_84 << fInt_88 << fID_8c << fInt_9c << fInt_a0;
}

// FUNCTION: STARFLEETOP 0x00544e66
void tAuctionItem::StreamIn( nDataStore::tInput& input )
{
	tDatabaseObject::StreamIn( input );

	input >> fBool_48;
	input >> fString_4c >> fID_5c >> fInt_6c >> fInt_70 >> fDouble_78 >> fInt_80 >> fInt_84 >> fInt_88 >> fID_8c >> fInt_9c >> fInt_a0;
}

// The two tID::GetObjectID calls reach 0x006fd600, a body the linker shares with another class's
// one-line accessor; the analysis names that address after the other class.
// FUNCTION: STARFLEETOP 0x00544f0f
void tAuctionItem::SQLFillInsertStream( std::stringstream* SQL, bool lastOne )
{
	tDatabaseObject::SQLFillInsertStream( SQL );

	*SQL << fBool_48 << ", '" << fn_006fc200( fString_4c ) << "', " << fID_5c.GetObjectID() << ", " << fInt_6c << ", " << fInt_70 << ", " << fDouble_78 << ", " << fInt_80 << ", " << fInt_84 << ", " << fInt_88 << ", " << fID_8c.GetObjectID() << ", " << fInt_9c << ", " << fInt_a0 << " )";
}

// FUNCTION: STARFLEETOP 0x00545084
void tAuctionItem::SQLFillUpdateStream( std::stringstream* SQL, bool lastOne )
{
	tDatabaseObject::SQLFillUpdateStream( SQL, lastOne );

	*SQL << "BiddingHasBegun=" << fBool_48 << ", ItemName='" << fn_006fc200( fString_4c ) << "', ItemID=" << fID_5c.GetObjectID() << ", ItemInfo=" << fInt_6c << ", AuctionValue=" << fInt_70 << ", AuctionRate=" << fDouble_78 << ", TurnOpened=" << fInt_80 << ", TurnToClose=" << fInt_84 << ", CurrentBid=" << fInt_88 << ", BidOwnerID=" << fID_8c.GetObjectID() << ", TurnBidMade=" << fInt_9c << ", BidMaximum=" << fInt_a0;
}

// FUNCTION: STARFLEETOP 0x00545221
const char* tAuctionItem::SQLTableName() const
{
	return "BidItem";
}

// HYPOTHESIS: slot 9 of the table is SQLFieldNames, the virtual that follows SQLTableName in
// tDatabaseObject (as in tMetaMapDesc). The text (at 0x0085c284) is read from the exe.
// FUNCTION: STARFLEETOP 0x00545227
const char* tAuctionItem::SQLFieldNames() const
{
	return " ( ID, Locked, LockID, BiddingHasBegun, ItemName, ItemID, ItemInfo, AuctionValue, "
		"AuctionRate, TurnOpened, TurnToClose, CurrentBid, BidOwnerID, TurnBidMade, BidMaximum ) ";
}
