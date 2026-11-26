#include "unsorted/tCampaignInfo.h"

// FUNCTION: STARFLEETOP 0x00549d84
const char *tCampaignInfo::SQLTableName() const
{
    return "CampaignInfo";
}

// FUNCTION: STARFLEETOP 0x00549d8a
const char *tCampaignInfo::SQLFieldNames() const
{
    return " ( ID, Locked, LockID, TemplateName, Description, EarlyMapName, MidMapName, LateMapName, MissionListSize, MissionList, RaceListSize, RaceList, CampaignEra, CampaignDifficulty, TriggerPrestige, TriggerMission, CampaignName, CharacterName, CharacterRace, ServerPassword, CurrentTimeSize, CurrentTime, PlayersCurrent, PlayersMax, PlayersLoggedOnCurrent, PlayersLoggedOnMax, ServerVersion, ValidClientVersion ) ";
}
