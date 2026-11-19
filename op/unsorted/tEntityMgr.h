//	tEntityMgr.h: tEntityMgr, the entity manager (the object at 0x008e1060).
// Evidence:  class (the SDK's method names; original file unknown)

#ifndef tEntityMgr_h
#define tEntityMgr_h 1

#include "unsorted/tEntity.h"

class tSFCStream;
class tEntity_Astral;
class tEntity_Weapon;
class tObfuscatedNumber;
class tObject_00423831_entity;
class tObject_00423a6a_entity;

class tEntityMgr {
public:
    enum {
        kMaxNumberofEntitiesInTheGame = 16384,
        kMaxNumberofServerControlledEntities = 1024,
        kMaxNumberofPlayerControlledEntities = 2560,
        kMaxNumberofPlayers = 6,
        kMaxNumberofAstralEntities = 16
    };

    tEntityMgr();
    ~tEntityMgr();

    // The only virtual the SDK header declares (0x00424a8f, vtable slot 0);
    // its signature there is (tSFCStream &s, int32u mode).
    virtual void mArchive(tSFCStream &s, unsigned long mode);

    void fn_00423598(int32 value);
    int32 fn_004235ac(int32 flags);
    void fn_004235cd(int8u flags, int32 value);
    iTruth fn_004235fa(int32 id);
    int32 fn_0042370f(int32 id);
    iTruth fn_00423735(int32 id);
    void mAddEntity(tEntity *pNew);
    void mAddAstralEntity(tEntity_Astral *pNew, int index);
    void fn_004237b0(tID id);
    tEntity *fn_004249c0(tEntity *pEntity);
    tObject_00423a6a_entity *fn_004249d3(tID id);
    // HYPOTHESIS: looks an entity up by ID (0x00423831 uses the result as
    // tObject_00423831_entity, declared further down).
    tObject_00423831_entity *fn_0042496f(tID id);
    void fn_00423831();
    void mSetPlayerNumber(const int player);
    tID mGetNextPlayerID(void);
    tID mGetNextHostID(void);
    void mMakeContectWithAnyPhysicalObject(int32 ContactID);
    iTruth mDetectCollision(tEntity *pEntity1, tEntity *pEntity2, iTruth ForceCollisionCheck = kFalse);
    iTruth mDetectProximityCollision(tEntity *pEntity1, tEntity *pEntity2, float ProximityRadius, iTruth ForceCollisionCheck = kFalse);
    // HYPOTHESIS: maps a listed entity to the object told about a contact
    // (0x00423a6a calls its slot 24 with the contact ID).
    tObject_00423a6a_entity *fn_00424a24(tEntity *pEntity);
    // HYPOTHESIS: looks an entity up by ID, falling back to a walk of the
    // entity list for an entity that carries the ID at 0x1bc.
    tEntity *fn_00424a37(tID id);
    // HYPOTHESIS: returns the first entity of the list linked through fPrev,
    // as 0x00413f59 walks it.
    tEntity *fn_0055f269();
    // HYPOTHESIS: the SDK's mDirectionToOtherEntity(From, Target), by its
    // signature and its result being stored in fLastCollisionDir.
    tAngle fn_004245e1(tEntity *pFrom, tEntity *pTarget);
    tAngle mDirectionToPoint(tEntity *From, tPoint3 location);
    float mGetDistanceFromEntities(const tID EntID1, const tID EntID2);
    // HYPOTHESIS: 0x004246e4, the SDK's overload taking two entities; it returns
    // a float and directly follows the ID overload.
    float mGetDistanceFromEntities(const tEntity *pEntity1, const tEntity *pEntity2);
    float mGetDistanceFromEntityToPoint(const tEntity *pEntity, const tPoint3 &location);
    float mGetDistanceFromEntityToPoint(const tID EntID, const tPoint3 &location);
    iTruth mMarkIDasControledByPlayer(const tID kID);
    tEntity *mGetPlayersControledEntity(void);
    tEntity_Weapon *mCastToWeapon(tEntity *pEntity);
    tEntity *mFindID(const tID kIDToFind) const
    {
        if (kIDToFind >= 0 && kIDToFind < kMaxNumberofEntitiesInTheGame)
            return fIDLookup[kIDToFind];
        else
            return NULL;
    }

protected:
    int32 fPlayerIDBase;
    int32 fPlayer;
    tEntity *fEntities;
    tEntity *fTail;
    iTruth fHost;
    int fNextAstralIndexFree;
    tID fNextPlayerID;
    tID fNextHostID;
    tID fPlayersControledShip;
    tObfuscatedNumber *fEntityStrength;
    tAngle fLastCollisionDir;
    tEntity *fIDLookup[kMaxNumberofEntitiesInTheGame];
    tEntity_Astral *fAstralEntityArray[kMaxNumberofAstralEntities];
    int8u fFreshnessID[kMaxNumberofEntitiesInTheGame];
};

#endif	// tEntityMgr_h
