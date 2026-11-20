#include "unsorted/tTeamSlot.h"
#include "unsorted/tServCharacter.h"
#include "unsorted/fn_005430ad_owner.h"
#include "Common/Exceptions/Validate.h"

// HYPOTHESIS: fills the slot from a character: whether it is not flagged, its ID, the value
// 0x00543495 reads, and the IDs of its ship caches. The character's members 0x00543495 and
// 0x005434f7 are declared on the record unsorted/fn_00543000.cpp rebuilds, which reads the same
// layout as tServCharacter; they are reached through a cast until the two classes are one.
// The lock's tOptex destructor goes to the game's copy (0x004296c8); the SDK's own copy is
// 0x0070f100, and the harness holds one symbol to one address, so this stays short of exact.
// FUNCTION: STARFLEETOP 0x00541499
void tTeamSlot::fn_00541499( tServCharacter* character )
{
#line 47
	validate( character );
	validate( character->GetID().IsValid() );

	field_0x18 = !character->fn_0054340c();
	field_0x1c = character->GetID();
	field_0x14 = ((fn_005430ad_owner*)character)->fn_00543495();
	field_0x2c.clear();

	fn_0042c709_ptr list = ((fn_005430ad_owner*)character)->fn_005434f7();
	for ( std::vector< fn_005419e4_entry >::const_iterator it = (*list).begin(); it != (*list).end(); it++ )
		field_0x2c.fn_0042add2( it->fn_005419ed() );
}
