// Functions of page 0x0060c000 with no unit or class evidence; original file unknown.

#include <memory>
#include <stdio.h>
#include <vector>

#include "Source/network/NetMsgClass.h"
#include "Tiger/tstring.h"
#include "unsorted/fn_0090bed8_type.h"
#include "unsorted/tNetControl.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/tObject_0096ed60.h"

// HYPOTHESIS: the auto_ptr's pointee; the out-of-line ~auto_ptr at 0x00607364 deletes it
// through vtable slot 0, so it has a virtual destructor. Its name is unknown.
class C_00c3a7c0
{
public:
    virtual ~C_00c3a7c0();
};

// HYPOTHESIS: a static auto_ptr at 0x00c3a7c0 (the bool _Owns, then the pointer); name unknown.
// The compiler's code for it: the initializer pair at 0x0060c14f (call, jmp), the inline
// constructor expanded at 0x0060c159, the atexit registration at 0x0060c168 and the atexit
// destructor at 0x0060c174.
// FUNCTION: STARFLEETOP 0x0060c168
// FUNCTION: STARFLEETOP 0x0060c159
std::auto_ptr<C_00c3a7c0> g_00c3a7c0;

class C_0060c486;

// HYPOTHESIS: the object the pointer at 0x00c3a80c holds; 0x0060c720 takes a line of text.
class C_00c3a80c
{
public:
    C_00c3a80c();
    ~C_00c3a80c();
    void fn_0060c68a(int entry);
    void fn_0060c6d5(int entry);
    void fn_0060c720(tString &text);
    void fn_0060c486(C_0060c486 *owner);
    void fn_0060c51d(bool debriefing);
    C_0060c486 *fn_0060c675(C_0060c486 *next, C_0060c486 *node);
    void fn_0060c76b(tString &text);
    // HYPOTHESIS: five owners the destructor frees through 0x0060c486.
    C_0060c486 *m_00;
    C_0060c486 *m_04;
    C_0060c486 *m_08;
    C_0060c486 *m_0c;
    C_0060c486 *m_10;
    // HYPOTHESIS: the links of the list headed at 0x00c3a810.
    C_00c3a80c *m_14;
    C_00c3a80c *m_18;
};

extern C_00c3a80c *g_00c3a80c;
extern C_00c3a80c *g_00c3a810;

// HYPOTHESIS: text lookups by key; 0x004cbdae also substitutes a value. Signatures unknown.
void fn_004cbd87(const char *key, int flags, tString &out);
void fn_004cbdae(const char *key, int flags, const char *tag, const tString &value, tString &out);
// HYPOTHESIS: the object 0x0060e11e returns; 0x0060cb1b looks a team up in it through
// 0x0052a2b5 and reads the record's +0x184.
class C_0052a2b5
{
public:
    char m_00[0x184];
    int m_184;
};
class C_0060e11e
{
public:
    C_0052a2b5 *fn_0052a2b5(int team);
};
C_0060e11e *fn_0060e11e();

#include "unsorted/fn_004D3143.h"

// HYPOTHESIS: the class of 0x0060c1c7 and 0x0060c3e1; its name is unknown.
class C_0060c1c7
{
public:
    void fn_0060c1c7();
    void fn_0060c3e1(int entry);
};

// FUNCTION: STARFLEETOP 0x0060c1c7
// HYPOTHESIS: reports the end of a mission: prestige, mission time, then every entry.
void C_0060c1c7::fn_0060c1c7()
{
    tString text;
    C_0052ccf9 *record = gObject_0096ed60.fn_0052ccf9();
    if (record) {
        int kind = record->m_1c;
        if (kind == 0 || kind == 1) {
            int a = gObject_0096ed60.fn_0052ce40();
            int gained = gObject_0096ed60.fn_0052ce32();
            int bonus = gObject_0096ed60.fn_0052ce39();
            gObject_0092deb4->vfunction106(bonus + gained);
            fn_004cbd87("BonusPrestige", 0, text);
            text.mAppendInt32s(bonus, 1);
            g_00c3a80c->fn_0060c720(text);
            fn_004cbd87("PrestigeGained", 0, text);
            text.mAppendInt32s(gained, 1);
            g_00c3a80c->fn_0060c720(text);
            gObject_0092deb4->vfunction42(record->m_fc);
            gObject_0092deb4->vfunction40(record->m_100);
            gObject_0092deb4->vfunction46(record->m_108);
            gObject_0092deb4->vfunction49(record->m_3c0);
            gObject_0092deb4->vfunction44(a);
        } else if (kind == 2) {
            fn_0060e11e();
        }
    }
    int seconds = g_0090bed8.fn_0045d50a();
    char buffer[100];
    sprintf(buffer, "%2.2i:%2.2i:%2.2i", seconds / 3600, seconds % 3600 / 60, seconds % 60);
    fn_004cbdae("MissionTime", 0, "Time", buffer, text);
    g_00c3a80c->fn_0060c720(text);
    int count = gObject_0096ed60.fn_0052ce1a();
    while (count--)
        fn_0060c3e1(gObject_0096ed60.fn_0052ce67(gObject_0096ed60.fn_0052ce21(count)));
}

// FUNCTION: STARFLEETOP 0x0060c3e1
void C_0060c1c7::fn_0060c3e1(int entry)
{
    if (entry)
        g_00c3a80c->fn_0060c68a(entry);
}

// FUNCTION: STARFLEETOP 0x0060c3fa
// HYPOTHESIS: 0x00c3a810 heads a list of these objects whose destructor (0x0060c4aa) unlinks
// each from it; the loop deletes them all, then drops the pointer at 0x00c3a80c.
void fn_0060c3fa()
{
    C_00c3a80c *entry;
    while ((entry = g_00c3a810) != 0)
        delete entry;
    g_00c3a80c = 0;
}

// FUNCTION: STARFLEETOP 0x0060c41e
// HYPOTHESIS: creates a new object, makes it current and pushes it on the list at 0x00c3a810.
void fn_0060c41e()
{
    g_00c3a80c = new C_00c3a80c;
    g_00c3a80c->m_14 = g_00c3a810;
    g_00c3a80c->m_18 = 0;
    if (g_00c3a810)
        g_00c3a810->m_18 = g_00c3a80c;
    g_00c3a810 = g_00c3a80c;
}

// HYPOTHESIS: a node of a singly linked list, next at +0x04; deleting a node unlinks it, so
// the loop below empties the list. Slot 1 returns the node's text (0x0060c51d). Name unknown.
class C_0060c486
{
public:
    virtual ~C_0060c486();
    virtual tString &vfunction1();
    C_0060c486 *m_04;
    C_0060c486 *m_08;
};

// HYPOTHESIS: a 0x3c-byte line of text (RTTI name); 0x0060c68a builds one with (5, entry) and
// links it as a C_0060c486. The meaning of the constructor's arguments is unknown.
class tStringMessage : public C_0060c486
{
public:
    tStringMessage(int kind, int entry);
    tStringMessage(int kind, tString &text);
    char m_0c[0x30];
};

// FUNCTION: STARFLEETOP 0x0060c486
// HYPOTHESIS: a member of C_00c3a80c that does not use this; its destructor passes this in ecx.
void C_00c3a80c::fn_0060c486(C_0060c486 *owner)
{
    if (owner) {
        while (owner->m_04)
            delete owner->m_04;
        delete owner;
    }
}

// FUNCTION: STARFLEETOP 0x0060c4aa
// HYPOTHESIS: unlinks itself from the list headed at 0x00c3a810, then frees its five owners.
C_00c3a80c::~C_00c3a80c()
{
    if (m_14)
        m_14->m_18 = m_18;
    if (!m_18)
        g_00c3a810 = m_14;
    else
        m_18->m_14 = m_14;
    fn_0060c486(m_00);
    fn_0060c486(m_04);
    fn_0060c486(m_08);
    fn_0060c486(m_0c);
    fn_0060c486(m_10);
}

// FUNCTION: STARFLEETOP 0x0060c504
C_00c3a80c::C_00c3a80c()
{
    m_14 = 0;
    m_18 = 0;
    m_00 = 0;
    m_04 = 0;
    m_08 = 0;
    m_0c = 0;
    m_10 = 0;
}

// FUNCTION: STARFLEETOP 0x0060c51d
// HYPOTHESIS: publishes the briefing (or debriefing) texts: title, the two lists of lines and
// the personal data.
void C_00c3a80c::fn_0060c51d(bool debriefing)
{
    tString text("");
    if (fn_0060e11e()) {
        tString title(gObject_0096ed60.fn_0052cd2f());
        fn_004d3179(debriefing ? "DebriefingMissionTitle" : "MissionTitle", title);
    }
    C_0060c486 *line;
    for (line = m_08; line; line = line->m_04) {
        text += line->vfunction1();
        text += iBasicStringElement('\n');
    }
    fn_004d3179(debriefing ? "DebriefingText" : "BriefingText", text);
    text = "";
    for (line = m_10; line; line = line->m_04) {
        text += line->vfunction1();
        text += iBasicStringElement('\n');
    }
    fn_004d3179(debriefing ? "DebriefingMapArea" : "MapArea", text);
    text = "";
    fn_0060c76b(text);
    fn_004d3179(debriefing ? "DebriefingPersonalData" : "PersonalData", text);
}

// FUNCTION: STARFLEETOP 0x0060c675
// HYPOTHESIS: a member of C_00c3a80c that does not use this (ret 8, like 0x0060c486); it links
// node before next, so a node keeps its previous link at +0x08.
C_0060c486 *C_00c3a80c::fn_0060c675(C_0060c486 *next, C_0060c486 *node)
{
    node->m_04 = next;
    if (next)
        next->m_08 = node;
    return node;
}

// FUNCTION: STARFLEETOP 0x0060c68a
// HYPOTHESIS: adds a line for entry at the head of the list at +0x08.
void C_00c3a80c::fn_0060c68a(int entry)
{
    m_08 = fn_0060c675(m_08, new tStringMessage(5, entry));
}

// FUNCTION: STARFLEETOP 0x0060c6d5
// HYPOTHESIS: adds a line of kind 6 for entry at the head of the list at +0x10.
void C_00c3a80c::fn_0060c6d5(int entry)
{
    m_10 = fn_0060c675(m_10, new tStringMessage(6, entry));
}

// FUNCTION: STARFLEETOP 0x0060c720
// HYPOTHESIS: adds a line of kind 7 holding text at the head of the list at +0x0c.
void C_00c3a80c::fn_0060c720(tString &text)
{
    m_0c = fn_0060c675(m_0c, new tStringMessage(7, text));
}

// FUNCTION: STARFLEETOP 0x0060c76b
// HYPOTHESIS: appends the lines of the list at +0x0c to text, one per line.
void C_00c3a80c::fn_0060c76b(tString &text)
{
    for (C_0060c486 *line = m_0c; line; line = line->m_04) {
        text += line->vfunction1();
        text += iBasicStringElement('\n');
    }
}

// HYPOTHESIS: the class at +0x24 of tObject_0090b730, built by 0x00472518 (0x704 bytes); name and
// members unknown.
class C_00472518
{
public:
    C_00472518();
    ~C_00472518();
    C_00472518 &operator=(const C_00472518 &other);
    // HYPOTHESIS: the layout follows the copy at 0x0060c892; member meanings are unknown.
    int m_00;
    int m_04;
    int m_08;
    int m_0c;
    tString m_10[6];
    int m_100;
    int m_104[6];
    int m_11c[6];
    int m_134[6];
    int m_14c[6];
    tString m_164[18];
    tString m_434[18];
};

// HYPOTHESIS: the class whose destructor is 0x0060cf03; name and members unknown.
class C_0060cf03
{
public:
    ~C_0060cf03();
};

// HYPOTHESIS: the class of the vector's elements, whose non-virtual destructor is 0x004729a6;
// name and members unknown.
// HYPOTHESIS: an entry of the six at +0x40, one per ship (0x0060cb1b sends the strings and the
// id as a tNetMsg_kShipScriptDescription when m_00 is set).
struct C_004729a6_ship
{
    bool m_00;
    int m_04;
    tString m_08;
    tString m_30;
    int m_58;
};

// HYPOTHESIS: the class of the vector's elements, whose non-virtual destructor is 0x004729a6;
// name unknown. 0x0060cb1b reads a team description from it.
class C_004729a6
{
public:
    ~C_004729a6();
    char m_00[0x2c];
    int m_2c;
    int m_30;
    int m_34;
    int m_38;
    int m_3c;
    C_004729a6_ship m_40[6];
};

// HYPOTHESIS: the field names follow the SDK's tTeamScriptDescription, whose order the stores
// at 0x0060cb1b follow.
struct tNetMsg_kTeamScriptDescription: public tNetMsg
{
    virtual const tNetMsg::tMapItem *mGetMap(void) const;
    int fTeam;
    int fRace;
    iTruth fHumanControlled;
    int fTeamActorID;
    int fTeamTag;
};

// HYPOTHESIS: the field names follow the SDK's tShipScriptDescription; the constructor is
// 0x0045c242.
struct tNetMsg_kShipScriptDescription: public tNetMsg
{
    tNetMsg_kShipScriptDescription();
    virtual const tNetMsg::tMapItem *mGetMap(void) const;
    int fTeamID;
    tString fClassName;
    int fClassType;
    tString fCustomName;
    int fMetaDatabaseID;
};

struct tNetMsg_kTeamReady: public tNetMsg
{
    virtual const tNetMsg::tMapItem *mGetMap(void) const;
    int fTeam;
};

// HYPOTHESIS: the class 0x0060c7dc constructs; 0x0060ca83 stores a table entry at +0x18. It is
// the class of the global at 0x0090b730: other units call 0x0060ca42..0x0060cac6 on that global
// (unsorted/tObject_0090b730.h), and 0x0060cac6 reaches the vector through it.
class tObject_0090b730
{
public:
    tObject_0090b730();
    ~tObject_0090b730();
    void fn_0060ca20();
    void fn_0060ca42(int value);
    int fn_0060ca4f();
    bool fn_0060ca56();
    bool fn_0060ca6c();
    char *fn_0060ca77();
    bool fn_0060ca7d();
    void fn_0060ca83(int index);
    void fn_0060ca94(int value);
    int fn_0060ca9e();
    void fn_0060cac6();
    void fn_0060cb1b();
    void fn_0060c883(const C_00472518 &value);
    C_00472518 &fn_0060c97e();
    char *fn_0060c982(const char *name);
    C_0060cf03 *m_00;
    int m_04;
    std::vector<C_004729a6 *> m_08;
    int m_18;
    int m_1c;
    bool m_20;
    C_00472518 m_24;
    int m_728;
};

// FUNCTION: STARFLEETOP 0x0060c7dc
tObject_0090b730::tObject_0090b730()
    : m_00(0), m_04(0), m_18(0), m_1c(1), m_20(true)
{
    m_728 = 0;
    fn_0060ca83(3);
}

// FUNCTION: STARFLEETOP 0x0060c841
tObject_0090b730::~tObject_0090b730()
{
    fn_0060ca20();
}

// HYPOTHESIS: forwards to the member at +0x24 (add ecx, 0x24); 0x0060c892 is that class's.
// FUNCTION: STARFLEETOP 0x0060c883
void tObject_0090b730::fn_0060c883(const C_00472518 &value)
{
    m_24 = value;
}

// FUNCTION: STARFLEETOP 0x0060c892
C_00472518 &C_00472518::operator=(const C_00472518 &other)
{
    int i;
    m_00 = other.m_00;
    m_04 = other.m_04;
    m_08 = other.m_08;
    m_0c = other.m_0c;
    for (i = 0; i < 6; i++)
        m_10[i] = other.m_10[i];
    m_100 = other.m_100;
    for (i = 0; i < 6; i++)
        m_104[i] = other.m_104[i];
    for (i = 0; i < 6; i++)
        m_11c[i] = other.m_11c[i];
    for (i = 0; i < 6; i++)
        m_134[i] = other.m_134[i];
    for (i = 0; i < 6; i++)
        m_14c[i] = other.m_14c[i];
    for (i = 0; i < 18; i++)
        m_164[i] = other.m_164[i];
    for (i = 0; i < 18; i++)
        m_434[i] = other.m_434[i];
    return *this;
}

// HYPOTHESIS: returns the member at +0x24 (lea eax, [ecx+0x24]); reference or pointer is unknown.
// FUNCTION: STARFLEETOP 0x0060c97e
C_00472518 &tObject_0090b730::fn_0060c97e()
{
    return m_24;
}

// HYPOTHESIS: finds name among the three strings of each row of m_434 (6 rows of 3) and returns
// the matching row's string from m_10.
// FUNCTION: STARFLEETOP 0x0060c982
char *tObject_0090b730::fn_0060c982(const char *name)
{
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 3; j++) {
            tString s(m_24.m_434[i * 3 + j]);
            if (stricmp(name, s.mCharStr()) == 0)
                return m_24.m_10[i].mCharStr();
        }
    }
    return 0;
}

// HYPOTHESIS: deletes the object at +0x00, whose class has the non-virtual destructor
// 0x0060cf03, and clears +0x00 and +0x04.
// FUNCTION: STARFLEETOP 0x0060ca20
void tObject_0090b730::fn_0060ca20()
{
    if (m_00 != 0) {
        delete m_00;
        m_00 = 0;
        m_04 = 0;
    }
}

// FUNCTION: STARFLEETOP 0x0060ca42
void tObject_0090b730::fn_0060ca42(int value)
{
    m_728 = value;
}

// FUNCTION: STARFLEETOP 0x0060ca4f
int tObject_0090b730::fn_0060ca4f()
{
    return m_728;
}

// FUNCTION: STARFLEETOP 0x0060ca56
bool tObject_0090b730::fn_0060ca56()
{
    if (m_728 == 4 || m_728 == 5)
        return true;
    return false;
}

// FUNCTION: STARFLEETOP 0x0060ca6c
bool tObject_0090b730::fn_0060ca6c()
{
    if (m_728 == 1)
        return true;
    return false;
}

// HYPOTHESIS: 0x0087bfc4 holds a pointer to the version string "2.5.5.2".
extern char *g_0087bfc4;

// FUNCTION: STARFLEETOP 0x0060ca77
char *tObject_0090b730::fn_0060ca77()
{
    return g_0087bfc4;
}

extern bool g_0087bfdc;

// FUNCTION: STARFLEETOP 0x0060ca7d
bool tObject_0090b730::fn_0060ca7d()
{
    return g_0087bfdc;
}

// HYPOTHESIS: a table of int values at 0x0087bfe4, indexed by the argument.
extern int g_0087bfe4[];

// FUNCTION: STARFLEETOP 0x0060ca83
void tObject_0090b730::fn_0060ca83(int index)
{
    m_18 = g_0087bfe4[index];
}

// FUNCTION: STARFLEETOP 0x0060ca94
void tObject_0090b730::fn_0060ca94(int value)
{
    m_18 = value;
}

// HYPOTHESIS: returns which band of the g_0087bfe4 table m_18 falls in (0..3).
// FUNCTION: STARFLEETOP 0x0060ca9e
int tObject_0090b730::fn_0060ca9e()
{
    int value = m_18;
    if (value >= g_0087bfe4[2])
        return (value >= g_0087bfe4[3]) + 2;
    return value >= g_0087bfe4[1];
}

// The global at 0x0090b730, the object of this class that tObject_0090b730.h declares for
// other units; this function reaches its vector at +0x08 through the global, not through this.

extern tObject_0090b730 gObject_0090b730;

// HYPOTHESIS: deletes every element of the vector from the last down, erasing each, then erases
// the whole range.
// FUNCTION: STARFLEETOP 0x0060cac6
void tObject_0090b730::fn_0060cac6()
{
    while (!gObject_0090b730.m_08.empty()) {
        delete gObject_0090b730.m_08.back();
        gObject_0090b730.m_08.erase(gObject_0090b730.m_08.end() - 1);
    }
    gObject_0090b730.m_08.clear();
}

// HYPOTHESIS: sends every team's description, its ships' descriptions and a ready message to
// the other machines, deleting each team from the last down, then empties the vector.
// FUNCTION: STARFLEETOP 0x0060cb1b
void tObject_0090b730::fn_0060cb1b()
{
    while (!gObject_0090b730.m_08.empty()) {
        C_004729a6 *team = gObject_0090b730.m_08.back();
        C_0060e11e *teams = fn_0060e11e();
        int id = team->m_2c;
        C_0052a2b5 *record = teams->fn_0052a2b5(id);
        tNetMsg_kTeamScriptDescription teamMsg;
        teamMsg.fTeam = id;
        teamMsg.fHumanControlled = false;
        teamMsg.fTeamActorID = team->m_30;
        teamMsg.fRace = team->m_38;
        teamMsg.fTeamTag = record->m_184;
        gObject_0096ed60.fn_0052c64e(id, team->m_38);
        gNetControl.mQueueMsg(&teamMsg, tNetMsg::kTeamScriptDescription, -1, -1.0);
        for (int i = 0; i < 6; i++) {
            if (team->m_40[i].m_00) {
                tNetMsg_kShipScriptDescription shipMsg;
                shipMsg.fTeamID = id;
                shipMsg.fClassName = team->m_40[i].m_08;
                shipMsg.fClassType = gObject_0096ed60.fn_0052ca48(shipMsg.fClassName.mCharStr());
                shipMsg.fCustomName = team->m_40[i].m_30;
                shipMsg.fMetaDatabaseID = team->m_40[i].m_58;
                gNetControl.mQueueMsg(&shipMsg, tNetMsg::kShipScriptDescription, -1, -1.0);
            }
        }
        tNetMsg_kTeamReady readyMsg;
        readyMsg.fTeam = id;
        gNetControl.mQueueMsg(&readyMsg, tNetMsg::kTeamReady, gNetControl.fn_004324ab(), -1, -1.0);
        delete team;
        gObject_0090b730.m_08.erase(gObject_0090b730.m_08.end() - 1);
    }
    gObject_0090b730.m_08.erase(gObject_0090b730.m_08.begin(), gObject_0090b730.m_08.end());
}
