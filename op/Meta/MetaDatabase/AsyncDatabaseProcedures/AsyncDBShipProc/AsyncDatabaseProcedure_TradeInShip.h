#if !defined( AsyncDatabaseProcedure_TradeInShip_h )
#define AsyncDatabaseProcedure_TradeInShip_h

#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBProcedure.h"
#include "Meta/MetaDatabase/StoredProcedures/ShipProcedures/SellShipRequest.h"
#include "Common/Database/DatabaseID.h"
#include "unsorted/tServCharacter.h"

namespace nAsyncDatabaseProcedure
{

// HYPOTHESIS: RTTI names the class; members are those 0x0056964b uses.
class tTradeInShip : public tAsyncDatabaseProcedure
{
public:
	virtual ~tTradeInShip();

protected:
	void fn_0056964b( const nDataStore::tBuffer* pBuffer );

	// HYPOTHESIS: the address the character update is sent to, read as a plain pointer.
	nSwitch::tAddress* m_48;
	nStoredProcedureArguments::tSellShipRequest fRequest;
	// HYPOTHESIS: a copy of the character after the trade-in.
	tServCharacter m_c4;
	// HYPOTHESIS: the ID of the ship traded in.
	nDatabase::tID m_224;
};

}

#endif
