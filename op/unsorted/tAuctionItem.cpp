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
	// HYPOTHESIS: places a bid; returns 0 when refused, 1 when outbid, 2 when the bidder leads.
	int fn_0054522d( const nDatabase::tID& bidder, int arg2, int32u bid, nDatabase::tID& owner, int32u* outBid );
	// HYPOTHESIS: small accessors read from their callers; 0x004b94dc, 0x00420f2f and 0x00420f3c
	// are bodies the linker shares with other classes' one-line members.
	bool fn_00545401() const;
	void fn_00545405( bool value );
	bool fn_00545560( int arg );
	int32u fn_004b94dc() const;
	const nDatabase::tID& fn_005455bb() const;
	int32u fn_005455c2() const;
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

// FUNCTION: STARFLEETOP 0x0054522d
int tAuctionItem::fn_0054522d( const nDatabase::tID& bidder, int arg2, int32u bid, nDatabase::tID& owner, int32u* outBid )
{
	// MATCH-ONLY: a named tID in its own block; a temporary copies from the constructor's
	// return register, the original copies from the stack and destroys the tID at once.
	{
		nDatabase::tID none( 0 );
		owner = none;
	}

	if ( !bidder.IsValid() || fn_00545560( arg2 ) || ( fn_00545401() && bid <= fn_004b94dc() ) )
		return 0;

	if ( fn_00545401() )
	{
		if ( bid < fn_005455c2() )
		{
			if ( !( bidder != fn_005455bb() ) )
				return 0;
			fn_005455e3( bid + 1 );
			owner = fn_005455bb();
		}
		else if ( bid == fn_005455c2() )
		{
			fn_005455e3( bid );
		}
		else
		{
			if ( bidder != fn_005455bb() )
				fn_005455e3( fn_005455c2() + 1 );
			owner = fn_005455bb();
			*outBid = fn_005455c2();
			// The original's layout: the two "return 1" cases share one exit and this case runs on
			// into the code below.
			goto lead;
		}
		return 1;
	}

lead:
	fn_00545405( true );
	fn_005455f0( bidder );
	fn_00420f2f( arg2 );
	fn_00420f3c( bid );
	return 2;
}

// HYPOTHESIS: withdraws the leading bid unless the given bidder holds it.
// FUNCTION: STARFLEETOP 0x00545385
bool tAuctionItem::fn_00545385( const nDatabase::tID& bidder )
{
	if ( !fn_005455bb().IsValid() || fn_005455bb() == bidder )
		return false;

	fn_005455f0( nDatabase::tID() );
	fn_00420f2f( 0 );
	fn_00420f3c( 0 );
	return true;
}

// FUNCTION: STARFLEETOP 0x00545401
bool tAuctionItem::fn_00545401() const
{
	return fBool_48;
}

// FUNCTION: STARFLEETOP 0x00545405
void tAuctionItem::fn_00545405( bool value )
{
	fBool_48 = value;
}

// HYPOTHESIS: true when the given bidder holds the leading bid.
// FUNCTION: STARFLEETOP 0x0054540f
bool tAuctionItem::fn_0054540f( const nDatabase::tID& bidder )
{
	return bidder.IsValid() && fn_005455bb().IsValid() && bidder == fn_005455bb();
}

// FUNCTION: STARFLEETOP 0x00545451
int32u tAuctionItem::fn_00545451( int arg1, const nDatabase::tID& bidder )
{
	if ( fn_0054540f( bidder ) )
		return fn_005454c1( arg1, bidder ) - fn_005455c2();

	return fn_005454c1( arg1, nDatabase::tID() );
}

// HYPOTHESIS: the three bid multipliers are doubles at fixed exe addresses.
extern double g_007b0c20;
extern double g_007b0c28;
extern double g_007b0c30;

// FUNCTION: STARFLEETOP 0x005454c1
int32u tAuctionItem::fn_005454c1( int arg1, const nDatabase::tID& bidder )
{
	int32u bid;
	if ( fn_0054540f( bidder ) )
		bid = fn_005455c2();
	else
		bid = fn_004b94dc();

	switch ( arg1 )
	{
	case 0:
		if ( !fn_00545401() )
			bid = fn_004b94dc();
		break;
	case 2:
		bid += 5;
		break;
	case 3:
		bid += 10;
		break;
	case 4:
		bid = bid * g_007b0c20;
		break;
	case 5:
		bid = bid * g_007b0c28;
		break;
	case 6:
		bid = bid * g_007b0c30;
		break;
	default:
		bid += 1;
		break;
	}

	return bid;
}
