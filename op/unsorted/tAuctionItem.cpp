#include <string>

#include "Common/Database/DatabaseObject.h"
#include "Common/Database/DatabaseID.h"
// MATCH-ONLY: the string writer is called out of line (0x0042a116); see the header's note.
#define DATASTORE_OUT_OF_LINE_WRITE
#include "Common/DataStore/DataStoreOutput.h"
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
	int32s fInt_80;
	int32s fInt_84;
	int32s fInt_88;
	nDatabase::tID fID_8c;
	int32s fInt_9c;
	int32s fInt_a0;
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
