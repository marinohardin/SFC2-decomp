#include <string>

#include "Common/Database/DatabaseObject.h"
#include "Common/Database/DatabaseID.h"
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

	// HYPOTHESIS: the two integer arguments' meaning is unknown; fn_00544d20 stores their
	// product with the double at +0x88 (fild, so arg4 is a signed int).
	void fn_00544d20( const nDatabase::tID& id, std::string name, int arg3, int arg4, double arg5 );

protected:
	bool fBool_48;
	std::string fString_4c;
	nDatabase::tID fID_5c;
	int fInt_6c;
	int fInt_70;
	double fDouble_78;
	int fInt_80;
	int fInt_84;
	int fInt_88;
	nDatabase::tID fID_8c;
	int fInt_9c;
	int fInt_a0;
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
