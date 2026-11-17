#include <string>

#include "Common/Database/DatabaseObject.h"
#include "Common/Database/DatabaseID.h"

// HYPOTHESIS: layout read from the constructor; the fields between the named ones are not
// identified yet.
class tAuctionItem : public nDatabase::tDatabaseObject
{
public:
	tAuctionItem();
	// HYPOTHESIS: the original declares no destructor. The compiler-generated one (0x00544c24)
	// stores no vtable; an implicit destructor builds byte for byte, but the harness cannot claim
	// a function with no source line, so this declared one (which stores the vtable) stands in.
	virtual ~tAuctionItem();

	// HYPOTHESIS: the two integer arguments' types and meaning are unknown.
	void fn_00544d20( const nDatabase::tID& id, std::string name, int arg3, int arg4, double arg5 );

protected:
	char fPad_48[0x4];
	std::string fString_4c;
	nDatabase::tID fID_5c;
	char fPad_6c[0x20];
	nDatabase::tID fID_8c;
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
