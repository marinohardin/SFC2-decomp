// tEntityMgr

#include "unsorted/tEntity.h"
#include "Source/entity/entity_physical.h"
#include "Tiger/tcglue.h"

// HYPOTHESIS: the bearing of a direction (x, y) as an angle; unsorted/tSfcApp3d.cpp
// and unsorted/tEntityControlShip.cpp declare it the same way inside those units.
short fn_0069afc0(float x, float y);

typedef int32 tID;

class tSFCStream;

// From the SDK's sfcstreams.h.
extern void ArchiveInt32TM(int32 &num, tSFCStream &s, int32u mode, char *text);
extern void ArchiveIntegerTM(int32 size, void *num, tSFCStream &s, int32u mode, char *text);
extern void ArchiveTitleTM(char *title, tSFCStream &s, int32u mode);
extern void ArchiveSeperatorTM(tSFCStream &s, int32u mode);
extern void IncrementStreamTab(tSFCStream &s);
extern void DecrementStreamTab(tSFCStream &s);

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

// FUNCTION: STARFLEETOP 0x00423537
tEntityMgr::tEntityMgr()
{
    fPlayerIDBase = 0;
    fPlayer = -1;
    fHost = kFalse;
    fEntities = NULL;
    fTail = NULL;
    fNextHostID = 0;
    fNextPlayerID = kMaxNumberofServerControlledEntities;
    clib::memset(fIDLookup, 0, sizeof(fIDLookup));
    clib::memset(fAstralEntityArray, 0, sizeof(fAstralEntityArray));
    fPlayersControledShip = -1;
    fLastCollisionDir = 0;
    fNextAstralIndexFree = 0;
    fEntityStrength = NULL;
}

// FUNCTION: STARFLEETOP 0x00423591
tEntityMgr::~tEntityMgr()
{
}

// HYPOTHESIS: the key tObfuscatedNumber adds to the value it stores; its
// meaning is not known.
extern int32 gValue_0083bb28;

// FUNCTION: STARFLEETOP 0x00423598
// HYPOTHESIS: an inline setter on fEntityStrength; tObfuscatedNumber's layout
// is not known, so its first word is written as the stored (keyed) value.
void tEntityMgr::fn_00423598(int32 value)
{
    *(int32 *)fEntityStrength = gValue_0083bb28 + value;
}

// HYPOTHESIS: a tEntityMgr method (it tests the word at 0x28, fEntityStrength
// in this layout); the argument looks like a 16-bit flag word whose bit 0x80
// is set or cleared depending on bit 0x4 and whether that member is set. The
// masks are written as 0xff7f and its complement, which is what the constants
// in the code are.
// FUNCTION: STARFLEETOP 0x004235ac
int32 tEntityMgr::fn_004235ac(int32 flags)
{
    if (fEntityStrength != NULL) {
        if (flags & 0x4)
            flags |= ~0xff7f;
        else
            flags &= 0xff7f;
    } else {
        if (flags & 0x4)
            flags &= 0xff7f;
        else
            flags |= ~0xff7f;
    }
    return flags;
}

// HYPOTHESIS: a tEntityMgr method (it writes through the word at 0x28) that
// stores the value as the setter at 0x00423598 does (probably that inline
// setter, expanded here) only when bits 0x80 and 0x4 of a byte-sized flags
// argument compare equal as bytes.
// FUNCTION: STARFLEETOP 0x004235cd
void tEntityMgr::fn_004235cd(int8u flags, int32 value)
{
    if ((int8u)(flags & 0x80) == (int8u)(flags & 0x4))
        *(int32 *)fEntityStrength = gValue_0083bb28 + value;
}

// HYPOTHESIS: the original is a compiler-generated exception-handling stub
// (loads a handler table at 0x0075c044 and jumps to the frame handler); it has
// no source of its own. This empty body is a placeholder, not the original code.
// FUNCTION: STARFLEETOP 0x004235f0
void fn_004235f0()
{
}

// FUNCTION: STARFLEETOP 0x004235fa
// HYPOTHESIS: 0x004235f0 and 0x004235fa are one function. The 10 bytes at
// 0x004235f0 are its EH prolog ("mov eax, <EH function info>; call
// __EH_prolog"), which the analysis split off. Written whole, the function
// compiles to the original's instructions except for the frame size (0x30
// against 0x38), but its symbol then starts 10 bytes before the address
// compared and scores 0%, so the head is still left out here and this function
// is incomplete. The head, in the order the original runs it:
//   if (fEntityStrength != NULL) {
//       s = the object the pointer at 0x0090be9c refers to;
//       if ((unsigned)((double)s->slot6() * <double at 0x00797220>
//                      / (double)<unsigned at s+0x20>) < 1) {
//           node = <tNetControl at 0x008f5b98>.fn_004314f5(
//               this->fn_0042370f(*(int32 *)fEntityStrength - gValue_0083bb28));
//           if (node != NULL) {
//               tFormatStr text;                    // 0x0062c790, 8 bytes
//               tString name((char *)node + 4);     // 0x00623120
//               text.mTag(<char[] at 0x0083cc04>, name);    // 0x0062d110
//               <object the pointer at 0x0096908c refers to>->slot3(&text,
//                   "ModifiedData", <char[] at 0x007980e0>, <float at 0x00795620>);
//           }
//           fEntityStrength = NULL;
//       }
//   }
// The tail is an ID-range test (true for any ID below 0xa00 on the host, or a
// player ID in this manager's block of 0xa00 IDs); its last test is computed as
// an int (push 1 / pop eax, xor eax, eax) and returned as the byte.
iTruth tEntityMgr::fn_004235fa(int32 id)
{
    if (fHost && id < 0xa00)
        return kTrue;
    return id >= fPlayerIDBase && id < fPlayerIDBase + 0xa00;
}

// FUNCTION: STARFLEETOP 0x0042370f
// HYPOTHESIS: maps an entity ID to the player that owns it: player IDs start
// after the server-controlled block, one block of player-controlled IDs each;
// on the host the first block maps to this manager's player. The result is
// one more than the zero-based player index.
int32 tEntityMgr::fn_0042370f(int32 id)
{
    int32 player = id - kMaxNumberofServerControlledEntities;
    if (player < kMaxNumberofPlayerControlledEntities && fHost == kTrue)
        player = fPlayer;
    else
        player /= kMaxNumberofPlayerControlledEntities;
    return player + 1;
}

// FUNCTION: STARFLEETOP 0x00423735
// HYPOTHESIS: true when the ID falls in the player-controlled block (the IDs
// after the server-controlled ones), computed as fn_0042370f does.
iTruth tEntityMgr::fn_00423735(int32 id)
{
    if (id - kMaxNumberofServerControlledEntities < kMaxNumberofPlayerControlledEntities)
        return kTrue;
    return kFalse;
}

// HYPOTHESIS: the object at 0x008f5170 is told of every entity added; its
// class and the meaning of the second argument are not known.
// HYPOTHESIS: it also keeps entities in lists by map cell: fn_004259bd turns a
// position into two indices and fn_00425726 returns the list for them.
class tObject_00425726_list;

#include "unsorted/tObject_008f5170.h"







// HYPOTHESIS: the list fn_00425726 returns, seen only through 0x00423a6a: its
// first word is the head of a chain of (entity, next) nodes.
class tObject_00425726_node {
public:
    tEntity *fEntity;
    tObject_00425726_node *fNext;
};

class tObject_00425726_list {
public:
    tObject_00425726_node *fHead;
};

// HYPOTHESIS: the object fn_00424a24 returns; only slot 24 (offset 0x60) is
// called, with a contact entity's ID.
class tObject_00423a6a_entity
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10();
    virtual void vfunction11();
    virtual void vfunction12();
    virtual void vfunction13();
    virtual void vfunction14();
    virtual void vfunction15();
    virtual void vfunction16();
    virtual void vfunction17();
    virtual void vfunction18();
    virtual void vfunction19();
    virtual void vfunction20();
    virtual void vfunction21();
    virtual void vfunction22();
    virtual void vfunction23();
    virtual void vfunction24();
    virtual void vfunction25(tID aContactID);

    // HYPOTHESIS: offsets 0x00423b3b adds to the entity's current and last
    // positions before sweeping it.
    tPoint3 fn_004130d0();
    tPoint3 fn_004130f1();
};

extern tObject_008f5170 gObject_008f5170;

// FUNCTION: STARFLEETOP 0x00423749
// HYPOTHESIS: the SDK's mAddEntity: pushes the entity on the front of the list
// (fEntities, linked through the words at 0x08 and 0x0c, which tEntity.h names
// fPrev and fField0C; here 0x08 links towards the tail and 0x0c towards the
// head) and records it in the ID table.
void tEntityMgr::mAddEntity(tEntity *pNew)
{
    if (pNew != NULL) {
        if (fEntities == NULL)
            fTail = pNew;
        if (fEntities != NULL)
            fEntities->fField0C = pNew;
        pNew->fPrev = fEntities;
        pNew->fField0C = NULL;
        fEntities = pNew;
        fIDLookup[pNew->fID] = pNew;
        gObject_008f5170.fn_004257f5(pNew, kTrue);
    }
}

// FUNCTION: STARFLEETOP 0x00423784
// HYPOTHESIS: the SDK's mAddAstralEntity (same arguments; it stores into
// fAstralEntityArray at a checked index and advances fNextAstralIndexFree,
// which becomes -1 once the array is full).
void tEntityMgr::mAddAstralEntity(tEntity_Astral *pNew, int index)
{
    if (pNew != NULL && index >= 0 && index < kMaxNumberofAstralEntities) {
        fNextAstralIndexFree++;
        if (fNextAstralIndexFree >= kMaxNumberofAstralEntities)
            fNextAstralIndexFree = -1;
        fAstralEntityArray[index] = pNew;
    }
}

// HYPOTHESIS: the object 0x005edc3b returns is the tMatchedMission record
// (see the netcode notes); it is told about the entity 0x004249c0 maps the
// removed one to.
class tMatchedMission {
public:
    void fn_005ee5d4(tEntity *pEntity);
};

tMatchedMission *fn_005edc3b();

// FUNCTION: STARFLEETOP 0x004237b0
// HYPOTHESIS: the counterpart of mAddEntity: unlinks the entity with this ID
// from the list, tells the objects that track entities, clears its slot in the
// ID table and calls its second virtual. The upper bound is tested with jg,
// so the ID kMaxNumberofEntitiesInTheGame itself passes the test.
void tEntityMgr::fn_004237b0(tID id)
{
    if (id >= 0 && id <= kMaxNumberofEntitiesInTheGame) {
        tEntity *pEntity = fIDLookup[id];
        if (pEntity != NULL) {
            tEntity *pNext = (tEntity *)pEntity->fField0C;
            tEntity *pPrev = pEntity->fPrev;
            if (pNext != NULL)
                pNext->fPrev = pPrev;
            else
                fEntities = pPrev;
            if (pPrev != NULL)
                pPrev->fField0C = pNext;
            else
                fTail = pNext;
            tMatchedMission *pMission = fn_005edc3b();
            if (pMission != NULL) {
                tEntity *pOwner = fn_004249c0(pEntity);
                if (pOwner != NULL)
                    pMission->fn_005ee5d4(pOwner);
            }
            gObject_008f5170.fn_004258de(pEntity, 2, kTrue);
            fIDLookup[id] = NULL;
            pEntity->vfunction2();
        }
    }
}

#include "unsorted/tObject_0040ce21.h"










#include "unsorted/tObject_0092deb4.h"




























































































// HYPOTHESIS: the entity 0x0042496f looks up, seen only through what is used
// here: slot 23 returns a fraction (scaled by 100 and subtracted from 100),
// the team at 0x60 and a buffer at 0x87d, probably a name.
class tObject_00423831_entity
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10();
    virtual void vfunction11();
    virtual void vfunction12();
    virtual void vfunction13();
    virtual void vfunction14();
    virtual void vfunction15();
    virtual void vfunction16();
    virtual void vfunction17();
    virtual void vfunction18();
    virtual void vfunction19();
    virtual void vfunction20();
    virtual void vfunction21();
    virtual void vfunction22();
    virtual void vfunction23();
    virtual float vfunction24();

    char pad_04[0x5c];
    tID fTeam;                  // +0x60
    char pad_64[0x819];
    char m_87d[1];              // +0x87d
};

#include "unsorted/tObject_0090be60.h"









// HYPOTHESIS: the object at 0x0096ed60; fn_0052ca48 takes a text and a flag
// and returns a value passed on to 0x0092deb4's slot 74.
class tObject_0096ed60
{
public:
    int32 fn_0052ca48(char *aText, int32 aFlag);
};

extern tObject_0096ed60 gObject_0096ed60;

// FUNCTION: STARFLEETOP 0x00423831
// HYPOTHESIS: removes and deletes every entity (the marker's text is "Clean
// up time!"), reporting the state of each one it can look up as it goes.
void tEntityMgr::fn_00423831()
{
    static tObject_0040ce21 marker("Clean up time!");

    tEntity *pEntity = fEntities;
    fPlayersControledShip = -1;
    fEntities = NULL;
    fNextAstralIndexFree = 0;
    clib::memset(fAstralEntityArray, 0, sizeof(fAstralEntityArray));
    int32 value = gObject_0092deb4->vfunction33();

    while (pEntity != NULL) {
        tEntity *pNext = pEntity->fPrev;
        tObject_00423831_entity *pFound = fn_0042496f(pEntity->fID);
        if (pFound != NULL) {
            int32 damage = (int32)(100.0 - pFound->vfunction24() * 100.0);
            tID id = pEntity->fID;
            int32 index = gObject_0092deb4->vfunction14(id);
            if (index >= 0) {
                gObject_0092deb4->vfunction82(damage, index);
            } else if (damage > 70 && damage < 100) {
                int32 relation = gObject_0090be60->fn_00427581(value, pFound->fTeam);
                if (gObject_0092deb4->vfunction7() == 0 || gObject_0092deb4->vfunction7() == 1) {
                    if (relation == 6 || relation == 5)
                        gObject_0092deb4->vfunction73(2, gObject_0096ed60.fn_0052ca48(pFound->m_87d, 1));
                }
            }
        }
        tMatchedMission *pMission = fn_005edc3b();
        if (pMission != NULL) {
            tEntity *pOwner = fn_004249c0(pEntity);
            if (pOwner != NULL)
                pMission->fn_005ee5d4(pOwner);
        }
        gObject_008f5170.fn_004258de(pEntity, 2, kTrue);
        fIDLookup[pEntity->fID] = NULL;
        pEntity->vfunction2();
        pEntity = pNext;
    }
}

// FUNCTION: STARFLEETOP 0x004239ba
// HYPOTHESIS: the SDK's mSetPlayerNumber (same signature); player numbers are
// one-based, and each player's IDs start after the server-controlled block.
void tEntityMgr::mSetPlayerNumber(const int player)
{
    fPlayer = player - 1;
    fNextPlayerID = fPlayer * kMaxNumberofPlayerControlledEntities + kMaxNumberofServerControlledEntities;
    fPlayerIDBase = fNextPlayerID;
}

// FUNCTION: STARFLEETOP 0x004239d6
// HYPOTHESIS: the SDK's mGetNextPlayerID: hands out the next free ID in this
// player's block, wrapping inside the block, or -1 when every ID is taken.
tID tEntityMgr::mGetNextPlayerID(void)
{
    tID start = fNextPlayerID;
    tID id = start;
    while (mFindID(id) != NULL) {
        id++;
        if (id >= fPlayerIDBase + kMaxNumberofPlayerControlledEntities)
            id = fPlayerIDBase;
        if (id == start)
            return -1;
    }
    fNextPlayerID = id + 1;
    if (fNextPlayerID >= fPlayerIDBase + kMaxNumberofPlayerControlledEntities)
        fNextPlayerID = fPlayerIDBase;
    return id;
}

// FUNCTION: STARFLEETOP 0x00423a26
// HYPOTHESIS: the SDK's mGetNextHostID: as mGetNextPlayerID, inside the
// server-controlled block that starts at ID 0.
tID tEntityMgr::mGetNextHostID(void)
{
    tID start = fNextHostID;
    tID id = start;
    while (mFindID(id) != NULL) {
        id++;
        if (id >= kMaxNumberofServerControlledEntities)
            id = 0;
        if (id == start)
            return -1;
    }
    fNextHostID = id + 1;
    if (fNextHostID >= kMaxNumberofServerControlledEntities)
        fNextHostID = 0;
    return id;
}

// FUNCTION: STARFLEETOP 0x00423a6a
// HYPOTHESIS: the SDK's mMakeContectWithAnyPhysicalObject (the marker's text is
// "Touch them all!"): finds the first entity listed for the contact entity's map
// cell that it collides with, and tells that one about the contact.
void tEntityMgr::mMakeContectWithAnyPhysicalObject(int32 ContactID)
{
    tEntity *pContact = mFindID(ContactID);
    if (pContact != NULL) {
        tPoint3 pos = pContact->fCurrPos;
        static tObject_0040ce21 marker("Touch them all!");

        int32 column;
        int32 row;
        gObject_008f5170.fn_004259bd(pos.x, pos.y, &column, &row);
        tObject_00425726_list *pList = gObject_008f5170.fn_00425726(column, row);
        if (pList != NULL) {
            for (tObject_00425726_node *pNode = pList->fHead;
                 pNode != NULL && pNode->fEntity != NULL;
                 pNode = pNode->fNext) {
                if (mDetectCollision(pNode->fEntity, pContact)) {
                    tObject_00423a6a_entity *pTouched = fn_00424a24(pNode->fEntity);
                    if (pTouched != NULL)
                        pTouched->vfunction25(ContactID);
                    break;
                }
            }
        }
    }
}

// HYPOTHESIS: a function pointer at 0x008ac574 that stores the square root of
// its first argument through the second (unsorted/tSfcApp3d.cpp declares it
// the same way inside that unit).
extern void (*g_p008ac574)(float, float *);

// FUNCTION: STARFLEETOP 0x00423b3b
// HYPOTHESIS: the SDK's mDetectCollision(tEntity*, tEntity*, iTruth): checks
// the boxes swept by both entities since their last positions, then steps
// both along their paths until they come within the sum of their radii; on a
// hit, moves both to the touching positions and records the direction. The
// marker's text is taken to be the string after "Touch them all!".
iTruth tEntityMgr::mDetectCollision(tEntity *pEntity1, tEntity *pEntity2, iTruth ForceCollisionCheck)
{
    static tObject_0040ce21 marker("What BS!");

    if (pEntity1 != pEntity2 && pEntity1 != NULL && pEntity2 != NULL &&
        (ForceCollisionCheck || (pEntity1->fProperties & 8) || (pEntity2->fProperties & 8))) {
    float radius1 = pEntity1->vfunction14();
    float radius2 = pEntity2->vfunction14();
    if (radius1 > 1e-15f && radius2 > 1e-14f) {

    tPoint3 curr1 = pEntity1->fCurrPos;
    tPoint3 last1 = pEntity1->fLastPos;
    tPoint3 curr2 = pEntity2->fCurrPos;
    tPoint3 last2 = pEntity2->fLastPos;

    tEntity *pOwner1 = fn_004249c0(pEntity1);
    tObject_00423a6a_entity *pObject1 = fn_00424a24(pEntity1);
    if (pObject1 != NULL) {
        curr1 += pObject1->fn_004130d0();
        last1 += pObject1->fn_004130f1();
    }
    tEntity *pOwner2 = fn_004249c0(pEntity2);
    tObject_00423a6a_entity *pObject2 = fn_00424a24(pEntity2);
    if (pObject2 != NULL) {
        curr2 += pObject2->fn_004130d0();
        last2 += pObject2->fn_004130f1();
    }

    float minX1, maxX1, minY1, maxY1;
    float minX2, maxX2, minY2, maxY2;
    if (curr1.x < last1.x) {
        minX1 = curr1.x - radius1;
        maxX1 = last1.x + radius1;
    } else {
        maxX1 = curr1.x + radius1;
        minX1 = last1.x - radius1;
    }
    if (curr1.y < last1.y) {
        minY1 = curr1.y - radius1;
        maxY1 = last1.y + radius1;
    } else {
        maxY1 = curr1.y + radius1;
        minY1 = last1.y - radius1;
    }
    if (curr2.x < last2.x) {
        minX2 = curr2.x - radius2;
        maxX2 = last2.x + radius2;
    } else {
        maxX2 = curr2.x + radius2;
        minX2 = last2.x - radius2;
    }
    // HYPOTHESIS: the original tests the first entity's Y here, not the
    // second's.
    if (curr1.y < last1.y) {
        minY2 = curr2.y - radius2;
        maxY2 = last2.y + radius2;
    } else {
        maxY2 = curr2.y + radius2;
        minY2 = last2.y - radius2;
    }

    tPoint3 path1 = curr1 - last1;
    float length1 = path1 * path1;
    tPoint3 path2 = curr2 - last2;
    float length2 = path2 * path2;

    if (length2 >= length1) {
        if (minX1 > maxX2 || maxX1 < minX2 || minY1 > maxY2 || maxY1 < minY2)
            return kFalse;
    } else {
        if (minX2 > maxX1 || maxX2 < minX1 || minY2 > maxY1 || maxY2 < minY1)
            return kFalse;
    }

    path1.mNormalize();
    path2.mNormalize();

    float root = 0.0f;
    if (length1 != 0.0f) {
        g_p008ac574(length1, &root);
        length1 = root;
    }
    if (length2 != 0.0f) {
        g_p008ac574(length2, &root);
        length2 = root;
    }
    float longest = length2 >= length1 ? length2 : length1;
    float stepSize = (radius1 <= radius2 ? radius1 : radius2) * 0.5f;
    // HYPOTHESIS: the original writes the division twice (it keeps both
    // operands on the FPU stack across it). It also stores the quotient to
    // steps before the comparison, which this form does not reproduce.
    float steps = longest / stepSize < 1.0f ? 1.0f : longest / stepSize;
    float fraction = 1.0f / steps;
    tPoint3 step1 = path1 * (fraction * length1);
    tPoint3 step2 = path2 * (fraction * length2);
    float reach = radius2 + radius1;
    reach = reach * reach;

    for (int i = 0; i < steps; i++) {
        last1 = last1 + step1;
        last2 = last2 + step2;
        tPoint3 gap = last1 - last2;
        if (gap * gap <= reach) {
            if (pOwner1 != NULL) {
                pOwner1->fn_00413112(tPoint3(0.0f, 0.0f, last1.z));
                last1.z = 0.0f;
            }
            pEntity1->fn_00417d3f(last1);
            if (pOwner2 != NULL) {
                pOwner2->fn_00413112(tPoint3(0.0f, 0.0f, last2.z));
                last2.z = 0.0f;
            }
            pEntity2->fn_00417d3f(last2);
            fLastCollisionDir = fn_004245e1(pEntity2, pEntity1);
            return kTrue;
        }
    }
    }
    }
    return kFalse;
}

// FUNCTION: STARFLEETOP 0x0042402d
// HYPOTHESIS: the SDK's mDetectProximityCollision(tEntity*, tEntity*, float,
// iTruth), by its signature: mDetectCollision with the second entity's radius
// grown by ten times the proximity radius (the float at 0x007962f8 holds 10
// and is taken to be the literal). The marker's text is taken to be the
// string at 0x0083dce4.
iTruth tEntityMgr::mDetectProximityCollision(tEntity *pEntity1, tEntity *pEntity2, float ProximityRadius, iTruth ForceCollisionCheck)
{
    static tObject_0040ce21 marker("What BS!");
    // HYPOTHESIS: declared at function scope; the original gives it a stack
    // slot of its own instead of reusing ProximityRadius's.
    float root;

    if (pEntity1 != pEntity2 && pEntity1 != NULL && pEntity2 != NULL &&
        (ForceCollisionCheck || (pEntity1->fProperties & 8) || (pEntity2->fProperties & 8))) {
    float radius1 = pEntity1->vfunction14();
    float radius2 = pEntity2->vfunction14();
    if (radius1 > 1e-15f && radius2 > 1e-14f) {

    ProximityRadius *= 10.0f;
    radius2 += ProximityRadius;

    tPoint3 curr1 = pEntity1->fCurrPos;
    tPoint3 last1 = pEntity1->fLastPos;
    tPoint3 curr2 = pEntity2->fCurrPos;
    tPoint3 last2 = pEntity2->fLastPos;

    tEntity *pOwner1 = fn_004249c0(pEntity1);
    tObject_00423a6a_entity *pObject1 = fn_00424a24(pEntity1);
    if (pObject1 != NULL) {
        curr1 += pObject1->fn_004130d0();
        last1 += pObject1->fn_004130f1();
    }
    tEntity *pOwner2 = fn_004249c0(pEntity2);
    tObject_00423a6a_entity *pObject2 = fn_00424a24(pEntity2);
    if (pObject2 != NULL) {
        curr2 += pObject2->fn_004130d0();
        last2 += pObject2->fn_004130f1();
    }

    float minX1, maxX1, minY1, maxY1;
    float minX2, maxX2, minY2, maxY2;
    if (curr1.x < last1.x) {
        minX1 = curr1.x - radius1;
        maxX1 = last1.x + radius1;
    } else {
        maxX1 = curr1.x + radius1;
        minX1 = last1.x - radius1;
    }
    if (curr1.y < last1.y) {
        minY1 = curr1.y - radius1;
        maxY1 = last1.y + radius1;
    } else {
        maxY1 = curr1.y + radius1;
        minY1 = last1.y - radius1;
    }
    if (curr2.x < last2.x) {
        minX2 = curr2.x - radius2;
        maxX2 = last2.x + radius2;
    } else {
        maxX2 = curr2.x + radius2;
        minX2 = last2.x - radius2;
    }
    // HYPOTHESIS: the original tests the first entity's Y here, not the
    // second's, as mDetectCollision does.
    if (curr1.y < last1.y) {
        minY2 = curr2.y - radius2;
        maxY2 = last2.y + radius2;
    } else {
        maxY2 = curr2.y + radius2;
        minY2 = last2.y - radius2;
    }

    tPoint3 path1 = curr1 - last1;
    float length1 = path1 * path1;
    tPoint3 path2 = curr2 - last2;
    float length2 = path2 * path2;

    if (length2 >= length1) {
        if (minX1 > maxX2 || maxX1 < minX2 || minY1 > maxY2 || maxY1 < minY2)
            return kFalse;
    } else {
        if (minX2 > maxX1 || maxX2 < minX1 || minY2 > maxY1 || maxY2 < minY1)
            return kFalse;
    }

    path1.mNormalize();
    path2.mNormalize();

    root = 0.0f;
    if (length1 != 0.0f) {
        g_p008ac574(length1, &root);
        length1 = root;
    }
    if (length2 != 0.0f) {
        g_p008ac574(length2, &root);
        length2 = root;
    }
    float longest = length2 >= length1 ? length2 : length1;
    float stepSize = (radius1 <= radius2 ? radius1 : radius2) * 0.5f;
    // HYPOTHESIS: as in mDetectCollision, the original stores the quotient to
    // steps before the comparison, which this form does not reproduce.
    float steps = longest / stepSize < 1.0f ? 1.0f : longest / stepSize;
    float fraction = 1.0f / steps;
    tPoint3 step1 = path1 * (fraction * length1);
    tPoint3 step2 = path2 * (fraction * length2);
    float reach = radius2 + radius1;
    reach = reach * reach;

    for (int i = 0; i < steps; i++) {
        last1 = last1 + step1;
        last2 = last2 + step2;
        tPoint3 gap = last1 - last2;
        if (gap * gap <= reach) {
            if (pOwner1 != NULL) {
                pOwner1->fn_00413112(tPoint3(0.0f, 0.0f, last1.z));
                last1.z = 0.0f;
            }
            pEntity1->fn_00417d3f(last1);
            if (pOwner2 != NULL) {
                pOwner2->fn_00413112(tPoint3(0.0f, 0.0f, last2.z));
                last2.z = 0.0f;
            }
            pEntity2->fn_00417d3f(last2);
            fLastCollisionDir = fn_004245e1(pEntity2, pEntity1);
            return kTrue;
        }
    }
    }
    }
    return kFalse;
}

// FUNCTION: STARFLEETOP 0x0042452c
// HYPOTHESIS: the SDK's mDirectionToPoint(tEntity*, tPoint3), by its signature
// (an entity and a point by value, a tAngle back; the marker's text is "Where
// is here?"): the bearing of the point from the entity, relative to the
// physical entity's heading (the short at 0x90).
tAngle tEntityMgr::mDirectionToPoint(tEntity *From, tPoint3 location)
{
    tAngle facing = 0;
    static tObject_0040ce21 marker("Where is here?");

    // HYPOTHESIS: 0x004249c0 is the SDK's mCastToPhysical; it is declared above
    // as returning a tEntity, so the result is cast to reach the heading.
    tPhysicalEntity *pPhysical = (tPhysicalEntity *)fn_004249c0(From);
    if (pPhysical != NULL)
        facing = pPhysical->field_0x90;
    tPoint3 position = From->fCurrPos;
    tPoint3 direction;
    direction = location - position;
    direction.mNormalize();
    return (facing - (fn_0069afc0(direction.x, direction.y) & 0x1fff)) & 0x1fff;
}

// FUNCTION: STARFLEETOP 0x004245e1
tAngle tEntityMgr::fn_004245e1(tEntity *pFrom, tEntity *pTarget)
{
    tAngle facing = 0;
    // HYPOTHESIS: the marker's text is the string that follows "Where is here?"
    // in the exe's data, as this function follows mDirectionToPoint.
    static tObject_0040ce21 marker("Can't get there from here!");

    tPhysicalEntity *pPhysical = (tPhysicalEntity *)fn_004249c0(pFrom);
    if (pPhysical != NULL)
        facing = pPhysical->field_0x90;
    tPoint3 position = pFrom->fCurrPos;
    tPoint3 target = pTarget->fCurrPos;
    tPoint3 direction;
    direction = target - position;
    direction.mNormalize();
    return (facing - (fn_0069afc0(direction.x, direction.y) & 0x1fff)) & 0x1fff;
}

// FUNCTION: STARFLEETOP 0x004246ab
// HYPOTHESIS: the SDK's mGetDistanceFromEntities(const tID, const tID): looks
// both IDs up and passes the entities to the overload that follows it.
float tEntityMgr::mGetDistanceFromEntities(const tID EntID1, const tID EntID2)
{
    tEntity *pEntity1 = mFindID(EntID1);
    tEntity *pEntity2 = mFindID(EntID2);
    return mGetDistanceFromEntities(pEntity1, pEntity2);
}

// FUNCTION: STARFLEETOP 0x004246e4
// HYPOTHESIS: the distance between the two entities' edges: the distance
// between their positions less both radii, never below zero; -1 when either
// entity is missing. The marker's text is taken to be the string after "Can't
// get there from here!".
float tEntityMgr::mGetDistanceFromEntities(const tEntity *pEntity1, const tEntity *pEntity2)
{
    float root;
    static tObject_0040ce21 marker("How long is it?");

    if (pEntity1 != NULL && pEntity2 != NULL) {
        tPoint3 pos1 = pEntity1->fCurrPos;
        tPoint3 pos2 = pEntity2->fCurrPos;
        tPoint3 diff = pos1 - pos2;
        float distance = diff * diff;
        if (distance != 0.0f) {
            g_p008ac574(distance, &root);
            if (root != 0.0f)
                distance = root;
            else
                distance = 0.0f;
        } else {
            distance = 0.0f;
        }
        // HYPOTHESIS: vfunction14 is the entity's radius, as in mDetectCollision;
        // the SDK passes the entities as const, so the casts drop it.
        distance -= ((tEntity *)pEntity1)->vfunction14();
        distance -= ((tEntity *)pEntity2)->vfunction14();
        if (distance < 0.0f)
            return 0.0f;
        return distance;
    }
    return -1.0f;
}

// FUNCTION: STARFLEETOP 0x004247dd
// HYPOTHESIS: the SDK's ID overload of mGetDistanceFromEntityToPoint: looks the
// ID up and passes the entity to the overload that follows it (0x00424801).
float tEntityMgr::mGetDistanceFromEntityToPoint(const tID EntID, const tPoint3 &location)
{
    return mGetDistanceFromEntityToPoint(mFindID(EntID), location);
}

// FUNCTION: STARFLEETOP 0x00424801
float tEntityMgr::mGetDistanceFromEntityToPoint(const tEntity *pEntity, const tPoint3 &location)
{
    static tObject_0040ce21 marker("No really, How long is it?");

    if (pEntity == NULL)
        return -1.0f;
    tPoint3 position = pEntity->fCurrPos;
    tPoint3 diff = position - location;
    float squared = diff * diff;
    float root = 0.0f;
    if (squared != 0.0f)
        g_p008ac574(squared, &root);
    // HYPOTHESIS: vfunction14 is the entity's radius, as in the two-entity
    // overload; the SDK passes the entity as const, so the cast drops it.
    float distance = root - ((tEntity *)pEntity)->vfunction14();
    if (distance < 0.0f)
        return 0.0f;
    return distance;
}

// HYPOTHESIS: a dword at 0x0090b670 compared with 1 before the player's ship
// may change; its meaning is not known.
extern int32 gValue_0090b670;

// FUNCTION: STARFLEETOP 0x004248b5
// HYPOTHESIS: the SDK's mMarkIDasControledByPlayer(const tID), the first of the
// SDK's targeting functions, by its single ID and its use of
// fPlayersControledShip. Slot 0x40 is taken to mark the new ship and slot 0x3c
// to release the old one. The original calls 0x004235f0, the head of
// fn_004235fa that the analysis split off.
iTruth tEntityMgr::mMarkIDasControledByPlayer(const tID kID)
{
    iTruth result = kTrue;

    if (!fn_004235fa(kID) && kID >= 0 && gValue_0090b670 == kTrue)
        return kFalse;

    if (fPlayersControledShip < 0) {
        fPlayersControledShip = kID;
    } else if (kID < 0) {
        tEntity *pOld = mFindID(fPlayersControledShip);
        if (pOld != NULL && fn_004235fa(pOld->fID))
            pOld->vfunction16();
        fPlayersControledShip = kID;
    } else if (kID != fPlayersControledShip) {
        tEntity *pNew = (tEntity *)fn_0042496f(kID);
        if (pNew != NULL) {
            pNew->vfunction17();
            tEntity *pOld = (tEntity *)fn_0042496f(fPlayersControledShip);
            if (pOld != NULL && fn_004235fa(pOld->fID))
                pOld->vfunction16();
            fPlayersControledShip = kID;
        } else {
            result = kFalse;
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x00424946
// HYPOTHESIS: the SDK's mGetPlayersControledEntity, which follows
// mMarkIDasControledByPlayer in the header; it looks up fPlayersControledShip.
tEntity *tEntityMgr::mGetPlayersControledEntity(void)
{
    return mFindID(fPlayersControledShip);
}

// FUNCTION: STARFLEETOP 0x0042495c
// HYPOTHESIS: the SDK's mCastToWeapon(tEntity *), which follows the
// player-ship functions in the header; bit 4 of fClassification is taken to
// mark a weapon, as entity_physical.cpp tests it before treating an entity as
// one. tEntity_Weapon is declared only inside entity_physical.cpp, so it is
// forward-declared here and the cast assumes it derives from tEntity at
// offset 0.
tEntity_Weapon *tEntityMgr::mCastToWeapon(tEntity *pEntity)
{
    if (pEntity != NULL && (pEntity->fClassification & 4))
        return (tEntity_Weapon *)pEntity;
    return NULL;
}

// FUNCTION: STARFLEETOP 0x0042496f
tObject_00423831_entity *tEntityMgr::fn_0042496f(tID id)
{
    static tObject_0040ce21 marker("Ship Entity for Sale.");
    // HYPOTHESIS: the SDK pairs a lookup by ID with mCastToPhysical (0x004249c0).
    return (tObject_00423831_entity *)fn_004249c0(mFindID(id));
}

// FUNCTION: STARFLEETOP 0x004249c0
// HYPOTHESIS: the SDK's mCastToPhysical(tEntity *), as the callers above use
// it; bit 2 of fClassification is taken to mark a physical entity, as bit 4
// marks a weapon in mCastToWeapon.
tEntity *tEntityMgr::fn_004249c0(tEntity *pEntity)
{
    if (pEntity != NULL && (pEntity->fClassification & 2))
        return pEntity;
    return NULL;
}

// FUNCTION: STARFLEETOP 0x004249d3
// HYPOTHESIS: the SDK's mGetPhysicalEntity(const tID), which pairs a lookup by
// ID with a cast as 0x0042496f does; the cast here is 0x00424a24.
tObject_00423a6a_entity *tEntityMgr::fn_004249d3(tID id)
{
    static tObject_0040ce21 marker("Physical objects here.");
    return fn_00424a24(mFindID(id));
}

// FUNCTION: STARFLEETOP 0x00424a24
// HYPOTHESIS: a cast like mCastToPhysical (0x004249c0); bit 1 of
// fClassification is taken to mark the kind of entity it returns.
tObject_00423a6a_entity *tEntityMgr::fn_00424a24(tEntity *pEntity)
{
    if (pEntity != NULL && (pEntity->fClassification & 1))
        return (tObject_00423a6a_entity *)pEntity;
    return NULL;
}

// HYPOTHESIS: the entity 0x00424a37 matches while walking the entity list,
// seen only through what is used there: slot 17 returns a kind (0x1e is the
// one sought) and the 4-byte member at 0x1bc holds an ID compared with the
// one looked up.
class tObject_00424a37_entity
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10();
    virtual void vfunction11();
    virtual void vfunction12();
    virtual void vfunction13();
    virtual void vfunction14();
    virtual void vfunction15();
    virtual void vfunction16();
    virtual void vfunction17();
    virtual int vfunction18();

    char pad_04[0x1b8];
    tID field_0x1bc;            // +0x1bc
};

// FUNCTION: STARFLEETOP 0x00424a37
// HYPOTHESIS: looks an entity up by ID; when the table has no entry for an ID
// in range, walks the entity list (through fPrev, as 0x00413f59 does) for an
// entity of classification 1 and kind 0x1e whose member at 0x1bc holds the ID.
tEntity *tEntityMgr::fn_00424a37(tID id)
{
    tEntity *pEntity = mFindID(id);
    if (pEntity == NULL && id >= 0 && id < kMaxNumberofEntitiesInTheGame) {
        for (pEntity = fn_0055f269(); pEntity != NULL; pEntity = pEntity->fPrev) {
            if (pEntity->fClassification == 1
                && ((tObject_00424a37_entity *)pEntity)->vfunction18() == 0x1e
                && ((tObject_00424a37_entity *)pEntity)->field_0x1bc == id)
                break;
        }
    }
    return pEntity;
}

// FUNCTION: STARFLEETOP 0x00424a8f
void tEntityMgr::mArchive(tSFCStream &s, unsigned long mode)
{
    ArchiveSeperatorTM(s, mode);
    ArchiveTitleTM("Entity Manager:", s, mode);
    IncrementStreamTab(s);
    ArchiveInt32TM(fPlayerIDBase, s, mode, "fPlayerIDBase");
    ArchiveInt32TM(fPlayer, s, mode, "fPlayer");
    ArchiveIntegerTM(sizeof(fLastCollisionDir), &fLastCollisionDir, s, mode, "fLastCollisionDir");
    ArchiveIntegerTM(sizeof(fNextPlayerID), &fNextPlayerID, s, mode, "fNextPlayerID");
    ArchiveIntegerTM(sizeof(fNextHostID), &fNextHostID, s, mode, "fNextHostID");
    ArchiveIntegerTM(sizeof(fPlayersControledShip), &fPlayersControledShip, s, mode, "fPlayersControledShip");
    DecrementStreamTab(s);
}
