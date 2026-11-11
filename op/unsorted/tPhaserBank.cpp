#include "unsorted/fn_0040ce21_type.h"
#include "unsorted/fn_004e20f4_field2c_type.h"
#include "unsorted/fn_004e2895_field148_type.h"
#include "unsorted/fn_004e2b02_ship_type.h"
#include "unsorted/fn_0090be9c_type.h"
#include "unsorted/tBaseWeapon.h"

// HYPOTHESIS: the stream archive helpers of the SDK's sfcstreams, called as
// (value, stream, mode, field name); which sibling each address is has not been confirmed.
void fn_0040e9df(int &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040eafc(bool &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040eb35(float &aValue, tSFCStream &s, unsigned mode, const char *aText);

// HYPOTHESIS: looks a name up and returns an id, as tCloakSystem calls it with "Cloak".
int __cdecl fn_00453c60(const char* name, int a2, int a3);

// HYPOTHESIS: the same global object as tCloakSystem uses at 0x0090be60 (see
// game/Source/entity/entity_physical.cpp); it belongs in a shared header once one exists.
// Only the members 0x004e2b02 uses are declared here.

#include "unsorted/tObject_0090be60.h"





extern tObject_0090be60 *gObject_0090be60;

// HYPOTHESIS: a double global at 0x00c7c148 holding the current time.
extern double g_00c7c148;

#include "unsorted/fn_0090bed8_type.h"

// HYPOTHESIS: float constants in the original's read-only data.
extern float g_007959c8;
extern float g_007977c0;
// HYPOTHESIS: a float constant read by 0x004e245e, a margin below the maximum.
extern float g_007a166c;
// HYPOTHESIS: a float constant returned by 0x004e24b0 when the rate is zero.
extern float g_0079d508;

// HYPOTHESIS: a float global at 0x007a7fa8, the initial value of the first per-arc array.
extern float g_007a7fa8;

// HYPOTHESIS: float globals read by 0x004e2243, one per phaser type.
extern float g_007a7fac;
extern float g_007a7fb0;
extern float g_007a7fb4;
extern float g_007a7fb8;
extern float g_007a7fbc;

// HYPOTHESIS: byte tables in the original's data, one row of per-range values for each of
// six columns, read by 0x004e25c0 for each phaser type; 0x00856abc serves two types.
extern unsigned char g_008569fc[6][11];
extern unsigned char g_00856a6c[6][8];
extern unsigned char g_00856abc[6][6];
extern unsigned char g_00856af8[6][13];
extern unsigned char g_00856b7c[6][8];
// HYPOTHESIS: a float constant 0x004e25c0 scales its sum by.
extern float g_0079bc20;
// HYPOTHESIS: a float constant 0x004e28fc compares each recovery timer with; below it a
// bank is ready to fire.
extern float g_007981a8;

// HYPOTHESIS: per-type tables in the original's data, passed with their entry count; read
// as floats, since 0x004e2aaa loads their first entries with fld.
extern const float g_00856a40[];
extern const float g_00856a9c[];
extern const float g_00856ae0[];
extern const float g_00856b48[];
extern const float g_00856bac[];

class tPhaserBank : public tBaseWeapon
{
public:
    tPhaserBank(int param_1, fn_004e1e99_arg* param_2);
    void fn_004e20d1(const char* name, int a2);
    void fn_004e20f4();
    void fn_004e2895();
    float fn_004e2aaa(int param_1, char param_2);
    float fn_004e2243(int i);
    // HYPOTHESIS: a test on the bank that returns a bool; 0x004e2542 calls it.
    bool fn_004e3d9a();
    // HYPOTHESIS: 0x004e28fc calls these two when a bank fires; their roles are not known.
    void fn_004e3b3e(float param_1);
    void fn_004e39c1();
    // HYPOTHESIS: 0x004e2b02 calls 0x004e39c1 or 0x004e340e after the timers, as 0x004e3d9a
    // says, then 0x004e2cfb with the id 0x0042753a returns and 0x004e301e.
    void fn_004e340e();
    void fn_004e2cfb(int param_1);
    void fn_004e301e();
    virtual float vfunction79();
    virtual float vfunction109();
    virtual float vfunction110();
    virtual float vfunction111();
    virtual void vfunction105(int param_1);
    virtual void vfunction102();
    virtual float vfunction74(float param_1, float param_2);
    virtual float vfunction143(float param_1, float param_2);
    virtual float vfunction135(unsigned char param_1);
    virtual int vfunction136(char param_1);
    // HYPOTHESIS: the slot at offset 0x224, a test 0x004e2b02 makes before counting the
    // recovery timers down. Declared here only to give it that offset after the three above.
    virtual bool vfunction138();
    virtual float vfunction114(unsigned char param_1, int param_2, int param_3);
    virtual float vfunction116(unsigned char param_1);
    virtual float vfunction118(unsigned char param_1);
    virtual void vfunction80();
    virtual void vfunction39(float param_1);
    virtual float vfunction70();
    virtual unsigned mStreamOutSystemSpecificData(tSFCStream &s, unsigned aFlags, unsigned mode);

    char pad04[0x0c - 0x04];
    // HYPOTHESIS: a type id that 0x004e2aaa switches on, with values 44 to 49.
    int field_0c;
    char pad10[0x2c - 0x10];
    fn_004e20f4_field2c_type* field_2c;
    char pad30[0x46 - 0x30];
    bool field_46;
    char pad47[0xec - 0x47];
    int field_ec;
    char padf0[0xf8 - 0xf0];
    int field_f8;
    // HYPOTHESIS: a byte that 0x004e2b02 compares with 12.
    char field_fc;
    char padfd[0x100 - 0xfd];
    int field_100;
    int field_104;
    int field_108;
    char pad10c[0x148 - 0x10c];
    fn_004e2895_field148_type field_148;
    char pad149[0x470 - 0x149];
    float field_470[4];
    // HYPOTHESIS: 16 floats; the constructor clears the first four, 0x004e2243 reads every fourth.
    float field_480[16];
    bool field_4c0;
    char pad4c1[0x4c4 - 0x4c1];
    int field_4c4;
    int field_4c8;
    bool field_4cc;
    char pad4cd[0x4d0 - 0x4cd];
    double field_4d0;
    bool field_4d8;
};

// FUNCTION: STARFLEETOP 0x004e1e99
tPhaserBank::tPhaserBank(int param_1, fn_004e1e99_arg* param_2)
    : tBaseWeapon(param_1)
{
    static fn_0040ce21_type marker("tPhaserBank::tPhaserBank");
    field_f8 = param_2->field_00;
    field_100 = param_2->field_04;
    field_104 = param_2->field_08;
    field_108 = param_2->field_0c;
    for (int i = 0; i < 4; i++)
    {
        field_470[i] = g_007a7fa8;
        field_480[i] = 0.0f;
    }
    field_4c8 = 0;
    field_4c0 = false;
    field_4cc = false;
    fn_004e20f4();
    field_4d8 = false;
    field_4d0 = 0.0;
    fn_004e2895();
}

// FUNCTION: STARFLEETOP 0x004e1f61
unsigned tPhaserBank::mStreamOutSystemSpecificData(tSFCStream &s, unsigned aFlags, unsigned mode)
{
    static fn_0040ce21_type marker("tPhaserBank::mStreamOutSystemSpecificData");
    unsigned result;
    int i;

    result = tBaseWeapon::mStreamOutSystemSpecificData(s, aFlags, mode);
    // HYPOTHESIS: the two identical blocks are the original's two cases, written out twice.
    if ((result & 8) > 0)
    {
        result = vfunction24(s, result, mode);
        fn_0040e9df(field_4c4, s, mode, "fNumberofBanks");
        fn_0040e9df(field_4c8, s, mode, "fCurrentBank");
        fn_0040eafc(field_4c0, s, mode, "fBankUsedthisTimeSlice");
        for (i = 0; i < field_4c4; i++)
            fn_0040eb35(field_470[i], s, mode, "fRecoverTimer[i]");
    }
    else if ((result & 0x20) <= 0 && (result & 0x80) > 0)
    {
        result = vfunction24(s, result, mode);
        fn_0040e9df(field_4c4, s, mode, "fNumberofBanks");
        fn_0040e9df(field_4c8, s, mode, "fCurrentBank");
        fn_0040eafc(field_4c0, s, mode, "fBankUsedthisTimeSlice");
        for (i = 0; i < field_4c4; i++)
            fn_0040eb35(field_470[i], s, mode, "fRecoverTimer[i]");
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x004e20d1
void tPhaserBank::fn_004e20d1(const char* name, int a2)
{
    vfunction18(fn_00453c60(name, a2, 0));
}

// FUNCTION: STARFLEETOP 0x004e20f4
void tPhaserBank::fn_004e20f4()
{
    float a = 0.0f;
    float b = 0.0f;

    // HYPOTHESIS: field_f8 is the phaser type; the names and numbers are per-type settings.
    // The name strings are the exe's own: "Phaser1" at 0x00856c30, "Phaser2" 0x00856c38,
    // "Phaser3" 0x00856c40, "Phaser4" 0x00856c48, "PhaserG" 0x00856c50. Cases 12 and 13 are
    // written separately because the compiler's linear compare chain needs six targets; the
    // two identical bodies are merged into one in the output.
    switch (field_f8)
    {
    case 8:
        a = 1.0f;
        b = 1.0f;
        vfunction65(0);
        fn_004e20d1("Phaser1", 0x8480);
        break;
    case 9:
        a = 1.0f;
        b = 1.0f;
        vfunction65(0);
        fn_004e20d1("Phaser2", 0x537a);
        break;
    case 10:
        b = a = g_007959c8;
        vfunction65(12);
        fn_004e20d1("Phaser3", 10000);
        break;
    case 11:
        b = a = g_007977c0;
        vfunction65(0);
        fn_004e20d1("Phaser4", 0x1fe28);
        break;
    case 12:
        a = 1.0f;
        b = 1.0f;
        vfunction65(12);
        fn_004e20d1("PhaserG", 40000);
        break;
    case 13:
        a = 1.0f;
        b = 1.0f;
        vfunction65(12);
        fn_004e20d1("PhaserG", 40000);
        break;
    default:
        break;
    }
    field_4c4 = field_104 + 1;
    vfunction57(field_4c4 * 128);
    vfunction55(field_4c4 * 128);
    vfunction67(a);
    vfunction69(field_4c4 * b);
    field_2c->vfunction118();
}

// FUNCTION: STARFLEETOP 0x004e2243
float tPhaserBank::fn_004e2243(int i)
{
    // HYPOTHESIS: the time left before bank i recovers, per phaser type.
    switch (field_f8)
    {
    case 8:
        return g_007a7fa8 - field_470[i];
    case 9:
        return g_007a7fac - field_470[i];
    case 10:
        return (g_007a7fb0 - field_470[i]) * g_007959c8;
    case 11:
        return (g_007a7fbc - field_470[i]) * 2.0f;
    case 12:
        // HYPOTHESIS: cases 12 and 13 read every fourth float of the array at 0x480.
        return g_007a7fb4 - field_480[i * 4];
    case 13:
        return g_007a7fb8 - field_480[i * 4];
    default:
        return 0.0f;
    }
}

// FUNCTION: STARFLEETOP 0x004e22d5
float tPhaserBank::vfunction79()
{
    // HYPOTHESIS: the recovery time left for the first bank.
    return fn_004e2243(0);
}

// FUNCTION: STARFLEETOP 0x004e22dd
float tPhaserBank::vfunction109()
{
    // HYPOTHESIS: the recovery time left for the second bank; returns a float like vfunction79.
    return fn_004e2243(1);
}

// FUNCTION: STARFLEETOP 0x004e22e5
float tPhaserBank::vfunction110()
{
    // HYPOTHESIS: the recovery time left for the third bank; returns a float like vfunction79.
    return fn_004e2243(2);
}

// FUNCTION: STARFLEETOP 0x004e22ed
float tPhaserBank::vfunction111()
{
    // HYPOTHESIS: the recovery time left for the fourth bank; returns a float like vfunction79.
    return fn_004e2243(3);
}

// FUNCTION: STARFLEETOP 0x004e22f5
void tPhaserBank::vfunction105(int param_1)
{
    tBaseWeapon::vfunction105(param_1);
    fn_004e20f4();
}

// FUNCTION: STARFLEETOP 0x004e230c
void tPhaserBank::vfunction102()
{
    // HYPOTHESIS: resets the recovery timer of every bank from the first one (slot 106) to
    // the bank count (slot 11) to the full time for the phaser type.
    for (int i = vfunction106(); i < vfunction11(); i++)
    {
        // HYPOTHESIS: the slot at 0x480 is named before the switch; the order in which the
        // loop steps its two pointers follows from it.
        float& r = field_480[i * 4];
        switch (field_f8)
        {
        case 8:
            field_470[i] = g_007a7fa8;
            break;
        case 9:
            field_470[i] = g_007a7fac;
            break;
        case 10:
            field_470[i] = g_007a7fb0;
            break;
        case 11:
            field_470[i] = g_007a7fbc;
            break;
        case 12:
            field_470[i] = g_007a7fb4;
            break;
        case 13:
            field_470[i] = g_007a7fb8;
            break;
        }
        if (field_f8 == 12)
            r = g_007a7fb4;
        else if (field_f8 == 13)
            r = g_007a7fb8;
    }
}

// FUNCTION: STARFLEETOP 0x004e23b5
float tPhaserBank::vfunction74(float param_1, float param_2)
{
    // HYPOTHESIS: draws up to param_1 into the object at 0x2c, limited by slot 70 and by the
    // room left below its maximum, and returns the part of param_1 not drawn.
    float max = field_2c->vfunction123();
    float cur = field_2c->vfunction117();
    float limit = vfunction70();
    float amount = limit < param_1 ? limit : param_1;
    if (cur >= max)
        return param_1;
    amount = amount * g_0090bed8.fn_00518603() * param_2;
    if (amount > max - cur)
        amount = max - cur;
    field_2c->vfunction114(amount);
    return param_1 - amount / (g_0090bed8.fn_00518603() * param_2);
}

// FUNCTION: STARFLEETOP 0x004e245e
float tPhaserBank::vfunction143(float param_1, float param_2)
{
    // HYPOTHESIS: the two parameters (ret 8) are unused; their types are guessed from slot 74.
    float max = field_2c->vfunction123();
    float cur = field_2c->vfunction117();
    float rate = field_2c->vfunction112();
    float room = max - g_007a166c;
    if (cur < room)
        return rate * max;
    return 0.0f;
}

// FUNCTION: STARFLEETOP 0x004e24b0
float tPhaserBank::vfunction135(unsigned char param_1)
{
    // HYPOTHESIS: param_1 is a bank index; returns the time until that bank can fire, given
    // the energy held by the object at 0x2c (slot 117) against its rate (slots 112 and 119).
    if (field_2c->vfunction117() > 1.0f)
        return field_470[param_1];
    if (field_2c->vfunction112() == 0.0f)
        return g_0079d508;
    // HYPOTHESIS: the original also stores this pointer to the stack slot that t uses later,
    // which suggests an inline helper not yet identified.
    fn_004e20f4_field2c_type* p = field_2c;
    float t = 1.0f / p->vfunction112();
    t *= p->vfunction119();
    if (t > field_470[param_1])
        return t;
    return field_470[param_1];
}

// FUNCTION: STARFLEETOP 0x004e2542
void tPhaserBank::vfunction80()
{
    // HYPOTHESIS: the call to 0x004d8163 is tBaseWeapon's own slot 80; the loop then sets
    // the recovery timer of all four banks to the full time for the phaser type.
    tBaseWeapon::vfunction80();
    for (int i = 0; i < 4; i++)
    {
        // HYPOTHESIS: as in vfunction102, the slot at 0x480 is named before the switch; the
        // order in which the loop steps its two pointers follows from it.
        float& r = field_480[i * 4];
        switch (field_f8)
        {
        case 8:
            field_470[i] = g_007a7fa8;
            break;
        case 9:
            field_470[i] = g_007a7fac;
            break;
        case 10:
            field_470[i] = g_007a7fb0;
            break;
        case 11:
            field_470[i] = g_007a7fbc;
            break;
        case 12:
            field_470[i] = g_007a7fb4;
            break;
        case 13:
            field_470[i] = g_007a7fb8;
            break;
        }
        if (fn_004e3d9a())
            r = g_007a7fb4;
    }
}

// FUNCTION: STARFLEETOP 0x004e25c0
float tPhaserBank::vfunction114(unsigned char param_1, int param_2, int param_3)
{
    // HYPOTHESIS: sums six table entries for the phaser type, starting at column param_3 and
    // row param_1; a column past the last moves the row on instead. param_2 is unused.
    // HYPOTHESIS: param_3 is an int of which only the low byte is used; a char parameter
    // compiles to the two byte operands of the column sum in the other order.
    float sum = 0.0f;
    for (int i = 1; i <= 6; i++)
    {
        unsigned char row = param_1;
        unsigned char col = i + param_3 - 1;
        if (col >= 6)
        {
            row = col + param_1 - 5;
            col = 5;
        }
        switch (field_f8)
        {
        case 8:
            if (param_1 <11)
                sum += g_008569fc[col][row];
            break;
        case 9:
            if (param_1 <8)
                sum += g_00856a6c[col][row];
            break;
        case 10:
            if (param_1 <6)
                sum += g_00856abc[col][row];
            break;
        case 11:
            if (param_1 <13)
                sum += g_00856af8[col][row];
            break;
        case 12:
            if (param_1 <6)
                sum += g_00856abc[col][row] * 4;
            if (!vfunction134())
                sum = 0.0f;
            break;
        case 13:
            if (param_1 <8)
                sum += g_00856b7c[col][row] * 4;
            if (!vfunction134())
                sum = 0.0f;
            break;
        }
    }
    return sum * g_0079bc20;
}

// FUNCTION: STARFLEETOP 0x004e2711
float tPhaserBank::vfunction116(unsigned char param_1)
{
    // HYPOTHESIS: the value in the last column of the phaser type's table for row param_1,
    // the column that 0x004e25c0 clamps to.
    switch (field_f8)
    {
    case 8:
        return g_008569fc[5][param_1];
    case 9:
        return g_00856a6c[5][param_1];
    case 10:
        return g_00856abc[5][param_1];
    case 11:
        return g_00856af8[5][param_1];
    case 12:
        if (!vfunction134())
            return 0.0f;
        return g_00856abc[5][param_1] * 4;
    case 13:
        if (!vfunction134())
            return 0.0f;
        return g_00856b7c[5][param_1] * 4;
    }
    return 0.0f;
}

// FUNCTION: STARFLEETOP 0x004e27d3
float tPhaserBank::vfunction118(unsigned char param_1)
{
    // HYPOTHESIS: the value in the first column of the phaser type's table for row param_1;
    // for type 13 the original reads the same last-column entry as 0x004e2711.
    switch (field_f8)
    {
    case 8:
        return g_008569fc[0][param_1];
    case 9:
        return g_00856a6c[0][param_1];
    case 10:
        return g_00856abc[0][param_1];
    case 11:
        return g_00856af8[0][param_1];
    case 12:
        if (!vfunction134())
            return 0.0f;
        return g_00856abc[0][param_1] * 4;
    case 13:
        if (!vfunction134())
            return 0.0f;
        return g_00856b7c[5][param_1] * 4;
    }
    return 0.0f;
}

// FUNCTION: STARFLEETOP 0x004e2895
void tPhaserBank::fn_004e2895()
{
    // HYPOTHESIS: clears the object at 0x148 and fills it from the phaser type's table.
    field_148.fn_0050bfaa();
    field_148.fn_004d6fd4();
    switch (field_f8)
    {
    case 8:
        field_148.fn_004d6f22(g_00856a40, 11);
        break;
    case 9:
        field_148.fn_004d6f22(g_00856a9c, 8);
        break;
    case 10:
        field_148.fn_004d6f22(g_00856ae0, 6);
        break;
    case 11:
        field_148.fn_004d6f22(g_00856b48, 13);
        break;
    case 12:
        field_148.fn_004d6f22(g_00856ae0, 6);
        break;
    case 13:
        field_148.fn_004d6f22(g_00856bac, 8);
        break;
    }
}

// FUNCTION: STARFLEETOP 0x004e28fc
int tPhaserBank::vfunction136(char param_1)
{
    // HYPOTHESIS: fires the next ready bank, or with param_1 set up to five banks in a row;
    // returns 0 once a bank has fired, otherwise a reason code.
    bool fired = false;
    int result;
    int count = 0;
    while ((result = fn_004d87e3(0)) == 0x200)
    {
        if (!vfunction134())
        {
            result = 8;
            break;
        }
        if (field_ec == -1)
        {
            result = 0x40;
            break;
        }
        if (!vfunction132(0))
        {
            result = 4;
            break;
        }
        if (!vfunction64())
        {
            result = 0x20;
            break;
        }
        int status = vfunction133();
        if (status != 0)
        {
            result = (status >= 0) + 1;
            break;
        }
        if (fn_004e3d9a())
        {
            int j = 0;
            int n = vfunction106();
            fired = true;
            for (; j < n * 4; j++)
            {
                if (field_480[j] < g_007981a8)
                {
                    field_480[j] = 0.0f;
                    field_4c8 = j / 4;
                    fn_004e3b3e(fn_004d786e());
                    field_4c0 = false;
                    if (!param_1)
                        break;
                    fn_004e39c1();
                }
            }
        }
        else
        {
            int i = 0;
            int n = vfunction106();
            fired = true;
            for (; i < n; i++)
            {
                if (field_470[i] < g_007981a8)
                {
                    field_470[i] = 0.0f;
                    field_4c8 = i;
                    fn_004e3b3e(fn_004d786e());
                    field_4c0 = false;
                    break;
                }
            }
        }
        count++;
        if (!param_1 || count >= 5)
            break;
    }
    if (fired)
    {
        field_46 = true;
        return 0;
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x004e2aaa
float tPhaserBank::fn_004e2aaa(int param_1, char param_2)
{
    // HYPOTHESIS: the first value of the table for the type at 0x0c; for types 46 to 48,
    // param_2 chooses between the table's first and second value. param_1 is not read.
    if (param_2)
    {
        switch (field_0c)
        {
        case 44:
            return g_00856a40[0];
        case 45:
            return g_00856a9c[0];
        case 46:
            return g_00856ae0[0];
        case 47:
            return g_00856ae0[0];
        case 48:
            return g_00856ae0[0];
        case 49:
            return g_00856b48[0];
        }
    }
    else
    {
        switch (field_0c)
        {
        case 44:
            return g_00856a40[0];
        case 45:
            return g_00856a9c[0];
        case 46:
            return g_00856ae0[1];
        case 47:
            return g_00856ae0[1];
        case 48:
            return g_00856ae0[1];
        case 49:
            return g_00856b48[0];
        }
    }
    return g_00856a40[0];
}

// FUNCTION: STARFLEETOP 0x004e2b02
void tPhaserBank::vfunction39(float param_1)
{
    // HYPOTHESIS: the per-frame update: counts every bank's recovery timer down by the
    // elapsed time, then at most once per interval checks the owning ship.
    fn_0043d696(param_1);
    bool& used = field_4c0;
    if (used)
        used = false;
    if (!vfunction138())
        return;
    if (vfunction64() == true)
    {
        int n = vfunction106();
        for (int i = 0; i < n; i++)
        {
            if (field_470[i] > 0.0f)
            {
                field_470[i] -= param_1;
                if (field_470[i] < 0.0f)
                    field_470[i] = 0.0f;
            }
            if (fn_004e3d9a())
            {
                for (int j = 0; j < 4; j++)
                {
                    field_480[i * 4 + j] -= param_1;
                    if (field_480[i * 4 + j] < 0.0f)
                        field_480[i * 4 + j] = 0.0f;
                }
            }
        }
    }
    if (fn_004e3d9a())
        fn_004e39c1();
    else
        fn_004e340e();
    if (!vfunction134())
        return;
    fn_004e2b02_ship_type* ship = vfunction33();
    if (ship != 0 && ship->field_ad4 != 0)
    {
        if (!ship->field_ad4->vfunction161())
            return;
        if (ship->field_ad4->vfunction44() == true)
            return;
    }
    double now = g_00c7c148;
    if (now - field_4d0 < 100000.0 && !field_4d8)
        return;
    fn_0090be9c_type* source = g_0090be9c;
    double ratio = (double)source->vfunction7() / (double)source->field_20;
    field_4d8 = false;
    field_4d0 = ratio * 20000.0 + now;
    if (field_fc == 12 && vfunction134() == true && ship != 0)
    {
        if (ship->vfunction59() == -1 && ship->vfunction32() != true && ship->field_180 != 0)
        {
            int id = gObject_0090be60->fn_0042753a(ship->field_60);
            if (id > 0)
                fn_004e2cfb(id);
            fn_004e301e();
        }
    }
}

// FUNCTION: STARFLEETOP 0x004e568c
float tPhaserBank::vfunction70()
{
    // HYPOTHESIS: the same test as slot 143 without its parameters: the rate the object at
    // 0x2c can take while it is below its maximum by more than the margin, else zero.
    float max = field_2c->vfunction123();
    float cur = field_2c->vfunction117();
    float rate = field_2c->vfunction112();
    float room = max - g_007a166c;
    if (cur < room)
        return rate * max;
    return 0.0f;
}
