//	MapObject.h: the kinds of object a mission map cell holds, as the SDK's MapObject.h declares
//	them.
#ifndef MapObject_h
#define MapObject_h

enum eMapObjectType
{
	kObjectEmptySpace,
	kObjectNebula,
	kObjectBlackHole,
	kObjectFissure,
	kObjectOrganian,
	kObjectPulsar,
	kObjectWormHole,
	kObjectSun,
	kObjectPlanet,
	kObjectAsteroid,
	kObjectDustCloud,
	kObjectCorpse,
	kObjectEnemyBase,
	kObjectPlayerBase,
	kObjectEnemyShip,
	kObjectAlliedShip,
	kObjectPlayerShip,
	kObjectShipStart,
	kObjectShipEnd,
	kObjectOffMapEdge,
	kObjectAssigned,
	kObjectIonStorm,
	kMaxObjects
};

// HYPOTHESIS: the SDK's class ends with a std::string fSpecRecord after fTacticalMapIcon; it is
// left out until matched code reads it, so this header does not pull in <string>.
class tMapObjectType
{
public:
	eMapObjectType		fObjectType;
	long				fSubType;
	eMapObjectType		fTacticalMapIcon;
};

#endif
