// Functions of page 0x0060c000 with no unit or class evidence; original file unknown.

#include <memory>
#include <stdio.h>

#include "Tiger/tstring.h"
#include "unsorted/fn_0090bed8_type.h"
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
int fn_0060e11e();
// HYPOTHESIS: stores a text under a name; also declared in the GameConnections state's unit.
void fn_004d3179(const char *name, const tString &value);

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
