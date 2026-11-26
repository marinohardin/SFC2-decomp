#include "unsorted/tServCharacter.h"

// FUNCTION: STARFLEETOP 0x00544719
const char* tServCharacter::SQLTableName() const
{
	return "ServCharacter";
}

// FUNCTION: STARFLEETOP 0x0054471f
const char* tServCharacter::SQLFieldNames() const
{
	return " ( ID, Locked, LockID, WONLogon, LastLoggedOn, MissionsPlayedVectorSize, MissionsPlayedVector, IPAddress, "
		"NextMissionTitle, NextMissionScore, HomeWorldLocationX, HomeWorldLocationY, LastMustPlayLocationX, "
		"LastMustPlayLocationY, PreparedMissionsID, OpenShipBidsSize, OpenShipBids, CharacterName, "
		"ShipCacheVectorSize, ShipCacheVector, CharacterRace, CharacterRank, CharacterPoliticalControl, Flags, "
		"CharacterLastBattleResult, CharacterLocationX, CharacterLocationY, CharacterRating, Medals, "
		"CharacterCurrentPrestige, CharacterLifetimePrestige, CharacterBattlesPlayed, MissionSlotIdx, "
		"MoveCompletes, MoveDestinationX, MoveDestinationY ) ";
}

// HYPOTHESIS: the element class of this vector is not known: 0x10 bytes with a virtual
// destructor, a dword at +4, a byte at +8 and a dword at +0xc, which the assignment copies.
class fn_00544725_elem
{
public:
	virtual ~fn_00544725_elem();

	int m_04;
	char m_08;
	int m_0c;
};

#include <vector>

template<> std::vector< fn_00544725_elem >::iterator std::vector< fn_00544725_elem >::erase( iterator _P );

// HYPOTHESIS: single erase of the vector of the elements above: the elements after it are
// assigned down one place, the last is destroyed and the end moves back.
// FUNCTION: STARFLEETOP 0x00544725
template<>
std::vector< fn_00544725_elem >::iterator std::vector< fn_00544725_elem >::erase( iterator _P )
{
	fn_00544725_elem *last = _Last;
	fn_00544725_elem *dest = _P;
	for ( fn_00544725_elem *src = _P + 1; src != last; ++dest, ++src )
		*dest = *src;
	fn_00544725_elem *e = _Last;
	for ( fn_00544725_elem *p = e - 1; p != e; ++p )
		p->~fn_00544725_elem();
	--_Last;
	return _P;
}

// MATCH-ONLY: a user so that the specialization above is emitted out of line; the original's
// callers of 0x00544725 are in other units.
void fn_00544725_user( std::vector< fn_00544725_elem > *v )
{
	v->erase( v->begin() );
}
