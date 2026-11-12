// tEntityControlShip

#include "unsorted/tEntity.h"
#include <math.h>

// HYPOTHESIS: the base class of every entity controller; its constructor
// (STARFLEETOP 0x004218c2) takes the controlled entity. Declared here until
// tEntityControl's own unit lands.
class tEntityControl {
public:
    tEntityControl(tEntity *entity);
    virtual ~tEntityControl();
    // HYPOTHESIS: slots 1 to 5 of tEntityControlShip's vtable (0x00797f68);
    // slots 2 to 5 hold functions of other classes, so they are taken to be
    // inherited. Named by the addresses in that vtable; signatures unknown.
    virtual void fn_00422711();
    virtual void fn_004088d6();
    virtual void fn_00436663();
    virtual void fn_0043666d();
    virtual void fn_00407d08();
};

// HYPOTHESIS: returns whether the third argument lies between the first two;
// 0x00421e76 calls it as (-22, 22, value).
bool fn_00415450(short param_1, short param_2, short param_3);

class tEntityControlShip;

// HYPOTHESIS: an entry handed out by tObject_004230b9 (STARFLEETOP
// 0x0042310b), seen only through the fields 0x00421b42 and 0x00421bbe fill in.
class tObject_0042310b_entry {
public:
    int m_00;
    float m_04;
    // HYPOTHESIS: a point; 0x00421c5b copies it whole from a by-value
    // argument, 0x00421bbe one component at a time.
    tPoint3 m_08;
    short m_14;
    char pad_16[0x02];
    int m_18;
    int m_1c;
    char m_20;
    char pad_21[0x07];
    double m_28;
    double m_30;
};

// HYPOTHESIS: a 12-byte member object with a constructor at
// STARFLEETOP 0x004230b9 and a destructor; its class is not yet known.
class tObject_004230b9 {
public:
    tObject_004230b9();
    ~tObject_004230b9();
    tObject_0042310b_entry *fn_0042310b(tEntityControlShip *owner);
    void fn_00423142();
    void fn_00423165(tObject_0042310b_entry *entry);
    void fn_00423232(tObject_0042310b_entry *entry);
    // HYPOTHESIS: the first entry of the list; 0x00421bbe reads its m_00.
    tObject_0042310b_entry *m_00;
    int m_04;
    int m_08;
};

// HYPOTHESIS: what slot 27 of the ship's object at 0xad4 returns; 0x00421ea9
// reads only the float at 0x18 and compares it with 10.
class tObject_00421ea9_part {
public:
    char pad_00[0x18];
    float m_18;
};

// HYPOTHESIS: the object held at 0xad4 of the ship; only slot 27 (offset
// 0x6c) is called, without arguments, returning a pointer.
class tObject_00421ea9_member {
public:
    virtual void vfunction0();
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
    virtual void vfunction25();
    virtual void vfunction26();
    virtual tObject_00421ea9_part *vfunction27();
};

// HYPOTHESIS: returns the angle of the direction (x, y), in 0x2000 units to
// the turn, as 0x00421ea9 masks it.
short fn_0069afc0(float x, float y);

// HYPOTHESIS: the controlled ship, seen only through the three shorts
// this constructor copies.
class tObject_00421939_ship {
public:
    // HYPOTHESIS: five arguments, as 0x00421d83 pushes them; their types and
    // the return type are not yet known.
    void fn_004102fe(int param_1, int param_2, int param_3, int param_4, int param_5);
    // HYPOTHESIS: five arguments, as 0x00421ea9 pushes them.
    void fn_004102d9(int param_1, int param_2, int param_3, int param_4, int param_5);
    // HYPOTHESIS: called by 0x004222e8 with the ship still in ecx, its
    // result tested as a bool.
    bool fn_00419c87();
    // HYPOTHESIS: the entity's virtuals; only slot 54 (offset 0xd8) is called,
    // by 0x0042222a, without arguments, returning a float.
    virtual void vfunction0();
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
    virtual void vfunction25();
    virtual void vfunction26();
    virtual void vfunction27();
    virtual void vfunction28();
    virtual void vfunction29();
    virtual void vfunction30();
    virtual void vfunction31();
    virtual void vfunction32();
    virtual void vfunction33();
    virtual void vfunction34();
    virtual void vfunction35();
    virtual void vfunction36();
    virtual void vfunction37();
    virtual void vfunction38();
    virtual void vfunction39();
    virtual void vfunction40();
    virtual void vfunction41();
    virtual void vfunction42();
    virtual void vfunction43();
    virtual void vfunction44();
    virtual void vfunction45();
    virtual void vfunction46();
    virtual void vfunction47();
    virtual void vfunction48();
    virtual void vfunction49();
    virtual void vfunction50();
    virtual void vfunction51();
    virtual void vfunction52();
    virtual void vfunction53();
    virtual float vfunction54();
    char pad_04[0x18];
    // HYPOTHESIS: the entity's fID; 0x00421ea9 compares it with an entity ID.
    int fID;
    char pad_20[0x14];
    // HYPOTHESIS: the ship is the controlled tEntity; these are its
    // fCurrPos and fLastPos.
    tPoint3 fCurrPos;
    tPoint3 fLastPos;
    char pad_4c[0x44];
    short m_90;
    short m_92;
    short m_94;
    char pad_96[0x7de];
    int m_874;
    char pad_878[0x2c];
    // HYPOTHESIS: a turn rate that 0x004222e8 reads, and the turn it sets.
    float m_8a4;
    float m_8a8;
    // HYPOTHESIS: two rates that 0x0042258d sets.
    float m_8ac;
    float m_8b0;
    char pad_8b4[0x10];
    // HYPOTHESIS: a float scale that 0x0042222a multiplies into its result.
    float m_8c4;
    char pad_8c8[0x04];
    tPoint3 m_8cc;
    tPoint3 m_8d8;
    char pad_8e4[0x15c];
    int m_a40;
    char pad_a44[0x90];
    tObject_00421ea9_member *m_ad4;
};

// HYPOTHESIS: a global block of four shorts and 32 floats that every new
// ship controller resets.
class tObject_008e0f54 {
public:
    short m_00;
    short m_02;
    short m_04;
    short m_06;
    float m_08[32];
};

extern tObject_008e0f54 g_008e0f54;

class tEntityControlShip : public tEntityControl {
public:
    tEntityControlShip(tEntity *entity);
    virtual ~tEntityControlShip();
    // Slot 1 of the vtable at 0x00797f68.
    virtual void fn_00422711();
    // Virtuals in the order of the vtable at 0x00797f68, from slot 6.
    virtual void fn_00421db6();
    virtual void fn_0042258d();
    virtual void fn_00422a36();
    virtual void fn_00422cbe();
    virtual void fn_00422187(tPoint3 &point);
    virtual void fn_00421e76(tObject_0042310b_entry *entry);
    virtual void fn_00421ea9(tObject_0042310b_entry *entry);
    virtual void fn_004220d5(tObject_0042310b_entry *entry);
    virtual float fn_0042222a(float param_1, float *param_2);
    virtual void fn_004221f5(tObject_0042310b_entry *entry);
    virtual void fn_004222e8(short param_1, int param_2);
    virtual void fn_00421b42(short param_1, int param_2);
    virtual void fn_00421bbe(tPoint3 param_1);
    virtual void fn_00421c5b(tPoint3 param_1);
    virtual void fn_00421d0d(int param_1);
    virtual void fn_00421c99(int param_1);
    void fn_00421d83(int param_1);
    // HYPOTHESIS: returns the turn towards the entry's heading or point, in
    // 0x2000 units; it sets only ax, so the result is a short.
    short fn_00422157(tObject_0042310b_entry *entry);
    // HYPOTHESIS: the turn towards a point; 0x00422157 passes the entry's m_08.
    short fn_004221a3(tPoint3 &point);
    int fn_00423025(tObject_0042310b_entry *entry);
    void fn_00422214();
    // HYPOTHESIS: members called with this by 0x00422711; signatures unknown.
    void fn_00422737();
    void fn_00422edb();

    tObject_00421939_ship *m_04;
    int m_08;
    int m_0c;
    double m_10;
    // HYPOTHESIS: four points copied from the ship by 0x00421db6.
    tPoint3 m_18;
    tPoint3 m_24;
    tPoint3 m_30;
    tPoint3 m_3c;
    short m_48;
    short m_4a;
    short m_4c;
    short m_4e;
    tObject_004230b9 m_50;
    char m_5c;
    char m_5d;
    short m_5e;
    double m_60;
};

// FUNCTION: STARFLEETOP 0x0042192f
tEntityControlShip::tEntityControlShip(tEntity *entity)
    : tEntityControl(entity)
{
    m_04 = (tObject_00421939_ship *)entity;
    m_10 = -1.0;
    m_18.x = 0.0f;
    m_18.y = 0.0f;
    m_18.z = 0.0f;
    m_24.x = 0.0f;
    m_24.y = 0.0f;
    m_24.z = 0.0f;
    m_30.x = 0.0f;
    m_30.y = 0.0f;
    m_30.z = 0.0f;
    m_3c.x = 0.0f;
    m_3c.y = 0.0f;
    m_3c.z = 0.0f;
    // HYPOTHESIS: reset before being copied from the ship just below.
    m_4a = 0;
    m_4c = 0;
    m_4e = 0;
    m_48 = 0;
    m_5c = 0;
    m_5d = 0;
    m_08 = 0;
    m_0c = 0;
    m_4a = m_04->m_92;
    m_4c = m_04->m_90;
    m_4e = m_04->m_94;
    g_008e0f54.m_00 = 0xe3;
    g_008e0f54.m_02 = 0x400;
    g_008e0f54.m_04 = 0x800;
    g_008e0f54.m_06 = 0x1000;
    g_008e0f54.m_08[0] = 0.0f;
    g_008e0f54.m_08[1] = 0.0f;
    g_008e0f54.m_08[2] = 0.0f;
    g_008e0f54.m_08[3] = 0.0f;
    g_008e0f54.m_08[4] = 0.0f;
    g_008e0f54.m_08[5] = 0.0f;
    g_008e0f54.m_08[6] = 0.0f;
    g_008e0f54.m_08[7] = 0.0f;
    g_008e0f54.m_08[8] = 0.0f;
    g_008e0f54.m_08[9] = 0.0f;
    g_008e0f54.m_08[10] = 0.0f;
    g_008e0f54.m_08[11] = 0.0f;
    g_008e0f54.m_08[12] = 0.0f;
    g_008e0f54.m_08[13] = 0.0f;
    g_008e0f54.m_08[14] = 0.0f;
    g_008e0f54.m_08[15] = 0.0f;
    g_008e0f54.m_08[16] = 0.0f;
    g_008e0f54.m_08[17] = 0.0f;
    g_008e0f54.m_08[18] = 0.0f;
    g_008e0f54.m_08[19] = 0.0f;
    g_008e0f54.m_08[20] = 0.0f;
    g_008e0f54.m_08[21] = 0.0f;
    g_008e0f54.m_08[22] = 0.0f;
    g_008e0f54.m_08[23] = 0.0f;
    g_008e0f54.m_08[24] = 0.0f;
    g_008e0f54.m_08[25] = 0.0f;
    g_008e0f54.m_08[26] = 0.0f;
    g_008e0f54.m_08[27] = 0.0f;
    g_008e0f54.m_08[28] = 0.0f;
    g_008e0f54.m_08[29] = 0.0f;
    g_008e0f54.m_08[30] = 0.0f;
    g_008e0f54.m_08[31] = 0.0f;
    m_60 = 0.0;
}

// FUNCTION: STARFLEETOP 0x00421b42
void tEntityControlShip::fn_00421b42(short param_1, int param_2)
{
    fn_00421d83(0);
    tObject_0042310b_entry *entry = m_50.fn_0042310b(this);
    entry->m_00 = 0;
    entry->m_20 = 0;
    entry->m_14 = param_1;
    entry->m_1c = param_2;
    int index = m_04->m_a40 - 1;
    if (index < 0) {
        index = 0;
    }
    // HYPOTHESIS: the 32 floats are a table of four per row, one row per
    // value of the ship's field 0xa40.
    entry->m_28 = g_008e0f54.m_08[index * 4 + fn_00423025(entry)];
    entry->m_30 = 0.0;
    if (param_1 == -1) {
        m_50.fn_00423142();
    } else {
        m_50.fn_00423165(entry);
    }
}

// FUNCTION: STARFLEETOP 0x00421bbe
void tEntityControlShip::fn_00421bbe(tPoint3 param_1)
{
    fn_00421d83(0);
    tObject_0042310b_entry *entry = m_50.fn_0042310b(this);
    entry->m_00 = 1;
    entry->m_08.x = param_1.x;
    entry->m_08.y = param_1.y;
    entry->m_08.z = param_1.z;
    entry->m_04 = 10.0f;
    entry->m_18 = gObject_008e1060.fn_004239d6();
    entry->m_1c = -1;
    entry->m_20 = 0;
    int index = m_04->m_a40 - 1;
    if (index < 0) {
        index = 0;
    }
    entry->m_28 = g_008e0f54.m_08[index * 4 + fn_00423025(entry)];
    entry->m_30 = 0.0;
    if (m_50.m_00 && m_50.m_00->m_00 == 1) {
        m_50.fn_00423232(entry);
    } else {
        m_50.fn_00423165(entry);
    }
}

// FUNCTION: STARFLEETOP 0x00421c5b
void tEntityControlShip::fn_00421c5b(tPoint3 param_1)
{
    tObject_0042310b_entry *entry = m_50.fn_0042310b(this);
    entry->m_00 = 4;
    entry->m_08 = param_1;
    entry->m_04 = 5.0f;
    entry->m_1c = -1;
    m_50.fn_00423165(entry);
}

// FUNCTION: STARFLEETOP 0x00421c99
void tEntityControlShip::fn_00421c99(int param_1)
{
    fn_00421d83(2);
    tObject_0042310b_entry *entry = m_50.fn_0042310b(this);
    entry->m_00 = 2;
    entry->m_08.mSet(0.0f, 0.0f, 0.0f);
    entry->m_20 = 0;
    entry->m_1c = param_1;
    entry->m_04 = 0.0f;
    int index = m_04->m_a40 - 1;
    if (index < 0) {
        index = 0;
    }
    entry->m_28 = g_008e0f54.m_08[index * 4 + fn_00423025(entry)];
    entry->m_30 = 0.0;
    m_50.fn_00423165(entry);
}

// FUNCTION: STARFLEETOP 0x00421d0d
void tEntityControlShip::fn_00421d0d(int param_1)
{
    fn_00421d83(1);
    tObject_0042310b_entry *entry = m_50.fn_0042310b(this);
    entry->m_00 = 3;
    entry->m_08.mSet(0.0f, 0.0f, 0.0f);
    entry->m_20 = 0;
    entry->m_1c = param_1;
    entry->m_04 = 0.0f;
    int index = m_04->m_a40 - 1;
    if (index < 0) {
        index = 0;
    }
    entry->m_28 = g_008e0f54.m_08[index * 4 + fn_00423025(entry)];
    entry->m_30 = 0.0;
    m_50.fn_00423165(entry);
}

// FUNCTION: STARFLEETOP 0x00421d83
void tEntityControlShip::fn_00421d83(int param_1)
{
    if (m_08 == 1 && m_50.m_00 != 0) {
        m_04->fn_004102fe(0x1000, m_50.m_00->m_1c, 0, 0, 0);
    }
    m_08 = param_1;
    m_0c = param_1;
}

// FUNCTION: STARFLEETOP 0x00421db6
void tEntityControlShip::fn_00421db6()
{
    m_18 = m_04->m_8cc;
    m_24 = m_04->m_8d8;
    m_30 = m_04->fCurrPos;
    m_3c = m_04->fLastPos;
    m_4a = m_04->m_92;
    m_4c = m_04->m_90;
    m_4e = m_04->m_94;
}

// HYPOTHESIS: the analysis puts this function at 0x00421e45, but the exe's
// bytes show it begins at 0x00421e3b with the EH prolog; from 0x00421e45 on
// the original and this build agree byte for byte.
// FUNCTION: STARFLEETOP 0x00421e3b
tEntityControlShip::~tEntityControlShip()
{
}

// FUNCTION: STARFLEETOP 0x00421e76
void tEntityControlShip::fn_00421e76(tObject_0042310b_entry *entry)
{
    short value = fn_00422157(entry);
    if (!fn_00415450(-22, 22, value)) {
        fn_004222e8(value, 0);
    } else {
        fn_004222e8(0, 0);
    }
}

// FUNCTION: STARFLEETOP 0x00421ea9
// HYPOTHESIS: steers the ship to a point beside the entry's target, 45 degrees
// round from the line between them, and slows it once it is there.
void tEntityControlShip::fn_00421ea9(tObject_0042310b_entry *entry)
{
    tEntity *target = (tEntity *)gObject_008e1060.fn_0042496f(entry->m_1c);
    if (target) {
        tPoint3 point(0.0f, 0.0f, 0.0f);
        tObject_00421ea9_part *part = m_04->m_ad4->vfunction27();
        tPoint3 offset = m_30 - target->fCurrPos;
        offset.mNormalize();
        float range = target->vfunction14() * 0.1f + 5.0;
        offset = offset * range;
        static float s_sin = (float)sin(kPi * -0.25);
        static float s_cos = (float)cos(kPi * -0.25);
        point.x = s_cos * offset.x - s_sin * offset.y;
        point.y = s_cos * offset.y + s_sin * offset.x;
        point = point + target->fCurrPos;
        tPoint3 delta = point - m_30;
        float distance = delta.mLength();
        if (distance <= range) {
            m_04->fn_004102d9(0x800, entry->m_1c, 0, 0, 0);
        }
        int playerID = gObject_008e1060.fn_00525081();
        if (distance <= 20.0 && part->m_18 >= 10.0 && playerID == m_04->fID) {
            // HYPOTHESIS: the original queues a tNetMsg_kAdjustSpeed (vftable
            // 0x007971a4) with the speed 10.0f here, through
            // gNetControl.mQueueMsg(&msg, 1, playerID, 250.0); tNetMsg is
            // declared only inside Source/entity/entity_physical.cpp, so the
            // message waits for a shared header.
        }
        delta.mNormalize();
        int angle = fn_0069afc0(delta.x, delta.y) & 0x1fff;
        m_48 = m_4c;
        int turn = (m_48 - angle) & 0x1fff;
        entry->m_14 = angle;
        fn_004222e8(turn, 0);
    } else {
        fn_00421b42(m_48, 0);
    }
}

// FUNCTION: STARFLEETOP 0x004220d5
// HYPOTHESIS: turns the ship towards the entry's target.
void tEntityControlShip::fn_004220d5(tObject_0042310b_entry *entry)
{
    tEntity *target = (tEntity *)gObject_008e1060.fn_0042496f(entry->m_1c);
    if (target) {
        tPoint3 delta = target->fCurrPos - m_30;
        delta.mNormalize();
        int angle = fn_0069afc0(delta.x, delta.y) & 0x1fff;
        m_48 = m_4c;
        int turn = (m_48 - angle) & 0x1fff;
        entry->m_14 = angle;
        fn_004222e8(turn, 0);
    } else {
        fn_00421b42(m_48, 0);
    }
}

// FUNCTION: STARFLEETOP 0x00422157
short tEntityControlShip::fn_00422157(tObject_0042310b_entry *entry)
{
    if (entry) {
        if (entry->m_00 == 1) {
            return fn_004221a3(entry->m_08);
        }
        m_48 = m_4c;
        short turn = m_48 - entry->m_14;
        return turn & 0x1fff;
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x00422187
// HYPOTHESIS: turns the ship towards a point.
void tEntityControlShip::fn_00422187(tPoint3 &point)
{
    fn_004222e8(fn_004221a3(point), 0);
}

// FUNCTION: STARFLEETOP 0x004221a3
short tEntityControlShip::fn_004221a3(tPoint3 &point)
{
    tPoint3 delta = point - m_30;
    delta.mNormalize();
    int angle = fn_0069afc0(delta.x, delta.y) & 0x1fff;
    m_48 = m_4c;
    return (m_48 - angle) & 0x1fff;
}

// FUNCTION: STARFLEETOP 0x004221f5
void tEntityControlShip::fn_004221f5(tObject_0042310b_entry *entry)
{
    fn_004222e8((m_4c - entry->m_14) & 0x1fff, entry->m_1c);
}

// FUNCTION: STARFLEETOP 0x00422214
void tEntityControlShip::fn_00422214()
{
    fn_004222e8((m_4c - m_48) & 0x1fff, 0);
}

// HYPOTHESIS: a global object at 0x0090bed8; only its method 0x0051bc19,
// which returns a float, is called here.
class tObject_0090bed8 {
public:
    float fn_0051bc19();
    char pad_00[0x08];
    // HYPOTHESIS: a fraction that 0x004222e8 keeps only inside [0, 0.25).
    float m_08;
};

extern tObject_0090bed8 gObject_0090bed8;

// HYPOTHESIS: 33 factors from 0.2 rising to 1.0, indexed by a ratio that
// 0x0042222a computes.
extern float g_0083db4c[33];

// FUNCTION: STARFLEETOP 0x0042222a
float tEntityControlShip::fn_0042222a(float param_1, float *param_2)
{
    float scale = m_04->m_8c4;
    float divisor = m_04->vfunction54();
    if (divisor == 0.0f) {
        divisor = 1.0f;
    }
    int index = (int)(param_1 / (divisor * 10.0f));
    float factor;
    if (m_5c) {
        factor = 1.0f;
    } else if (index < 33 && index >= 0) {
        factor = g_0083db4c[index];
    } else {
        factor = 0.0f;
    }
    // HYPOTHESIS: 8192 / 360 converts degrees to the 0x2000 units of a turn.
    float value = gObject_0090bed8.fn_0051bc19() * factor * scale * (8192.0 / 360.0);
    float result;
    if (m_5c) {
        result = (short)value * 6.0f;
    } else {
        result = (short)value;
    }
    if (param_2) {
        if (m_5c) {
            *param_2 = factor * 5.0f;
        } else {
            *param_2 = factor;
        }
    }
    return result;
}

// HYPOTHESIS: clamps the short it is given the address of; called by
// 0x004222e8, which cleans the stack after it.
void fn_00422565(short *param_1);

// FUNCTION: STARFLEETOP 0x004222e8
// HYPOTHESIS: sets the ship's turn for a heading error of param_1, in 0x2000
// units to the turn; param_2 forces the direction when it is not zero.
void tEntityControlShip::fn_004222e8(short param_1, int param_2)
{
    float turn = 0.0f;
    float rate = m_04->m_8a4;
    int index = m_04->m_a40 - 1;
    float factor = rate * 0.0032258064f;
    if (index < 0) {
        index = 0;
    }
    short limit;
    switch (index) {
    case 0:
        if (m_04->fn_00419c87()) {
            limit = 0x1c7;
            factor *= 6.5f;
        } else if (m_04->m_874 == 1) {
            limit = 0x1c7;
            factor *= 4.5f;
        } else {
            limit = 0x1c7;
            factor *= 1.5f;
        }
        break;
    case 1:
        limit = 0x1c7;
        factor *= 1.4f;
        break;
    case 2:
        limit = 0x155;
        factor *= 1.2f;
        break;
    case 3:
        limit = 0xe3;
        break;
    case 4:
        factor *= 0.8f;
        limit = 0xe3;
        break;
    default:
        factor *= 0.6f;
        limit = 0xe3;
        break;
    }
    if (m_5c) {
        factor *= 1.5;
    }
    float maxTurn = fn_0042222a(rate, NULL);
    m_5e = 0;
    if (param_1 != 0 && maxTurn != 0.0f) {
        float fraction = gObject_0090bed8.m_08;
        if (fraction < 0.0f || fraction >= 0.25f) {
            fraction = 0.0f;
        }
        if ((param_1 <= 0x1000 && param_2 == 0) || param_2 < 0) {
            turn = -maxTurn;
            m_5e = (short)(factor * 455.0f);
            if (param_1 < limit) {
                turn *= 0.75f;
                if (param_1 < 0x9f) {
                    turn *= 0.75f;
                }
                if (-turn < 227.0f) {
                    turn = -227.0f;
                }
                m_5e = 0;
            }
            if (-(fraction * turn) > param_1 && fraction != 0.0f) {
                short amount = -param_1;
                fn_00422565(&amount);
            }
        } else {
            turn = maxTurn;
            m_5e = (short)(factor * -455.0f);
            if (0x2000 - param_1 < limit) {
                turn *= 0.75f;
                if (0x2000 - param_1 < 0x9f) {
                    turn *= 0.75f;
                }
                if (turn < 227.0f) {
                    turn = 227.0f;
                }
                m_5e = 0;
            }
            if (fraction * turn > 0x2000 - param_1 && fraction != 0.0f) {
                short amount = 0x2000 - param_1;
                fn_00422565(&amount);
            }
        }
    } else {
        m_5e = 0;
    }
    m_04->m_8a8 = turn;
}

// FUNCTION: STARFLEETOP 0x00422565
void fn_00422565(short *param_1)
{
    if (*param_1 < 0) {
        if (-*param_1 <= 0x40) {
            *param_1 = -0x20;
        }
    } else if (*param_1 <= 0x40) {
        *param_1 = 0x20;
    }
}

// HYPOTHESIS: sets the ship's two rates towards the headings it is still
// turning through, each limited by a step that depends on the ship's field
// 0xa40, in 0x2000 units to the turn.
// FUNCTION: STARFLEETOP 0x0042258d
void tEntityControlShip::fn_0042258d()
{
    float fraction = gObject_0090bed8.m_08;
    short diff = (m_4a - m_5e) & 0x1fff;
    if (fraction < 0.0f || fraction >= 0.25) {
        fraction = 0.0f;
    }
    int index = m_04->m_a40 - 1;
    if (index < 0) {
        index = 0;
    }
    float step;
    switch (index) {
    case 0:
        if (m_04->fn_00419c87()) {
            step = 9102.0f;
        } else if (m_04->m_874 == 1) {
            step = 8192.0f;
        } else {
            step = 7281.0f;
        }
        break;
    case 1:
        step = 6371.0f;
        break;
    case 2:
        step = 5461.0f;
        break;
    case 3:
        step = 4551.0f;
        break;
    case 4:
        step = 3640.0f;
        break;
    default:
        step = 2730.0f;
        break;
    }
    if (m_5c) {
        step *= 2.0f;
    }
    if (diff != 0) {
        if (diff < 0x1000) {
            if (diff >= step * fraction) {
                m_04->m_8b0 = -step;
            } else {
                m_04->m_8b0 = -diff;
            }
        } else {
            if (diff >= step * fraction) {
                m_04->m_8b0 = step;
            } else {
                m_04->m_8b0 = diff;
            }
        }
    }
    short turn = m_4e;
    if (turn != 0) {
        if (turn < 0x1000) {
            if (turn >= step * fraction) {
                m_04->m_8ac = -step;
            } else {
                m_04->m_8ac = -turn;
            }
        } else {
            // HYPOTHESIS: the original tests diff here, not turn.
            if (diff >= step * fraction) {
                m_04->m_8ac = step;
            } else {
                m_04->m_8ac = turn;
            }
        }
    }
}

// FUNCTION: STARFLEETOP 0x00422711
void tEntityControlShip::fn_00422711()
{
    fn_00421db6();
    fn_0042258d();
    fn_00422a36();
    fn_00422737();
    fn_00422edb();
}
