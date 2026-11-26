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
