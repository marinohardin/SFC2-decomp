//	tBaseWeapon.h: tBaseWeapon, the base of the weapon systems, as its subclasses use it.
// Evidence:  class (original file unknown)

#ifndef tBaseWeapon_h
#define tBaseWeapon_h 1

class tSFCStream;
class fn_004e2b02_ship_type;
class tEntity;

// HYPOTHESIS: the four values the phaser bank constructors (0x004e1e99, 0x004e5325) copy into
// the bank, read as four ints.
struct fn_004e1e99_arg
{
    int field_00;
    int field_04;
    int field_08;
    int field_0c;
};

// HYPOTHESIS: the part at tBaseWeapon offset 0x148; only its method 0x004d7099 is known.
struct fn_004d7099_type
{
    char data[4];
    int fn_004d7099(float param_1);
};

// HYPOTHESIS: the per-bank table vfunction122 fills; only the methods it calls are known.
struct fn_004d77ec_type
{
    unsigned char fn_005da49d();
    void fn_004d6f77(unsigned char param_1, float param_2);
    void fn_004d6f96(unsigned char param_1, float param_2);
    void fn_004d6fb5(unsigned char param_1, float param_2);
};

// HYPOTHESIS: tBaseWeapon's constructor (0x004d70eb) takes the first constructor argument.
// Its bases (tRepairableShipSystem, tIndividualShipSystem, tListMember) are declared only
// inside game/unsorted/tCloakSystem.cpp, so they are not repeated here.
// HYPOTHESIS: slots 1 to 21 are placeholders that only fix the position of slot 22
// (offset 0x58, mStreamOutSystemSpecificData, 0x004d76b3 in tBaseWeapon) and slot 23
// (offset 0x5c, 0x00452f40 in tIndividualShipSystem).
class tBaseWeapon
{
public:
    tBaseWeapon(int param_1);
    virtual ~tBaseWeapon();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    // HYPOTHESIS: the slot at offset 0x24 returns the number of banks 0x004e58ec walks.
    virtual int vfunction10();
    // HYPOTHESIS: the slot at offset 0x28 returns the number of banks 0x004e230c walks.
    virtual int vfunction11();
    virtual void vfunction12();
    virtual void vfunction13();
    virtual void vfunction14();
    virtual void vfunction15();
    virtual void vfunction16();
    virtual int vfunction17();
    // HYPOTHESIS: the slot at offset 0x44 takes the value 0x00453c60 returns.
    virtual void vfunction18(int param_1);
    virtual void vfunction19();
    virtual void vfunction20();
    virtual void vfunction21();
    virtual void vfunction22();
    virtual unsigned mStreamOutSystemSpecificData(tSFCStream &s, unsigned aFlags, unsigned mode);
    virtual unsigned vfunction24(tSFCStream &s, unsigned aFlags, unsigned mode);
    // HYPOTHESIS: the slots from 25 on are placeholders that only fix the positions of the
    // slots 0x004e20f4 calls: offsets 0xd8, 0xe0, 0x100, 0x108 and 0x110.
    virtual void vfunction25();
    virtual void vfunction26();
    virtual void vfunction27();
    virtual void vfunction28();
    virtual void vfunction29();
    virtual void vfunction30();
    virtual void vfunction31();
    virtual void vfunction32();
    // HYPOTHESIS: the slot at offset 0x80 returns the ship that owns the system.
    virtual fn_004e2b02_ship_type* vfunction33();
    // HYPOTHESIS: the slot at offset 0x84 returns the system's target; 0x004e55af passes it
    // with the ship to 0x00424705.
    virtual tEntity* vfunction34();
    // HYPOTHESIS: the slot at offset 0x88 is a test tPhaserXBank's 0x004e5958 makes.
    virtual bool vfunction35();
    virtual void vfunction36();
    virtual void vfunction37();
    virtual void vfunction38();
    virtual void vfunction39(float param_1);
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
    virtual void vfunction54();
    virtual void vfunction55(int param_1);
    virtual void vfunction56();
    virtual void vfunction57(int param_1);
    virtual void vfunction58();
    virtual void vfunction59();
    virtual void vfunction60();
    virtual void vfunction61();
    virtual void vfunction62();
    virtual void vfunction63();
    // HYPOTHESIS: the slot at offset 0xfc is a test 0x004e28fc makes before firing.
    virtual bool vfunction64();
    // HYPOTHESIS: the slot at offset 0x100 sets a mode byte; tPhaserXBank's 0x004e562b reads
    // its argument as a char, and 0x004d813c passes it the byte at +0xfc.
    virtual void vfunction65(char param_1);
    virtual void vfunction66();
    virtual void vfunction67(float param_1);
    virtual void vfunction68();
    virtual void vfunction69(float param_1);
    // HYPOTHESIS: the slot at offset 0x114 returns a float that 0x004e23b5 caps its request at.
    virtual float vfunction70();
    virtual void vfunction71();
    virtual void vfunction72();
    virtual void vfunction73();
    virtual float vfunction74(float param_1, float param_2);
    virtual void vfunction75();
    virtual void vfunction76();
    virtual void vfunction77();
    virtual void vfunction78();
    virtual float vfunction79();
    virtual void vfunction80();
    virtual void vfunction81();
    virtual void vfunction82();
    virtual void vfunction83();
    virtual void vfunction84();
    virtual int vfunction85();
    virtual void vfunction86();
    virtual void vfunction87();
    virtual void vfunction88();
    virtual void vfunction89();
    virtual void vfunction90();
    virtual void vfunction91();
    virtual void vfunction92();
    virtual void vfunction93();
    virtual void vfunction94();
    virtual void vfunction95();
    virtual void vfunction96();
    virtual void vfunction97();
    virtual void vfunction98();
    virtual void vfunction99();
    virtual void vfunction100();
    // HYPOTHESIS: the slot at offset 0x190 takes a damage value and a target ID and returns
    // the damage; tPhaserXBank's 0x004e5958 calls it when the target passes its test.
    virtual int vfunction101(int param_1, int param_2);
    virtual void vfunction102();
    virtual void vfunction103();
    virtual void vfunction104();
    // HYPOTHESIS: tBaseWeapon's slot 104 (0x004d8747), which 0x004e22f5 calls before its own
    // work.
    virtual void vfunction105(int param_1);
    // HYPOTHESIS: the slot at offset 0x1a4 returns the index of the first bank as a char.
    virtual char vfunction106();
    virtual int vfunction107();
    virtual void vfunction108();
    virtual float vfunction109();
    virtual float vfunction110();
    virtual float vfunction111();
    // HYPOTHESIS: slots 112 to 133 are placeholders that only fix the position of slot 134
    // (offset 0x214), a test 0x004e25c0 calls; slot 114 (offset 0x1c4) is the one
    // tPhaserBank overrides at 0x004e25c0.
    virtual void vfunction112(float param_1);
    virtual float vfunction113(float param_1, int param_2, int param_3);
    virtual float vfunction114(unsigned char param_1, int param_2, int param_3);
    virtual float vfunction115(float param_1);
    virtual float vfunction116(unsigned char param_1);
    virtual float vfunction117(float param_1);
    virtual float vfunction118(unsigned char param_1);
    virtual float vfunction119();
    virtual void vfunction120();
    virtual int vfunction121();
    virtual void vfunction122(fn_004d77ec_type* param_1, int param_2, int param_3);
    virtual void vfunction123(float* param_1);
    virtual void vfunction124(int param_1);
    virtual void vfunction125();
    virtual void vfunction126(float param_1);
    virtual bool vfunction127(int param_1);
    // HYPOTHESIS: appends a pair of shorts to a list of at most six at offset 0x114.
    virtual bool vfunction128(short param_1, short param_2);
    virtual void vfunction129();
    virtual bool vfunction130();
    virtual bool vfunction131(short param_1);
    // HYPOTHESIS: 0x004e28fc tests slot 132 (offset 0x20c) with 0 and reads slot 133
    // (offset 0x210) as a signed status, zero when the weapon may fire.
    virtual bool vfunction132(int param_1);
    virtual int vfunction133();
    virtual bool vfunction134();
    virtual float vfunction135(int param_1);
    virtual void vfunction136();
    virtual void vfunction137();
    virtual bool vfunction138();
    // HYPOTHESIS: slots 139 to 143 are placeholders that only fix the positions of slots 144
    // and 145 (offsets 0x240 and 0x244), which slot 103 calls.
    virtual void vfunction139();
    virtual void vfunction140();
    virtual void vfunction141();
    virtual void vfunction142();
    virtual bool vfunction143();
    virtual int vfunction144();
    virtual void vfunction145(int param_1);
    // HYPOTHESIS: members read by vfunction102, placed by offset.
    char pad_04[0x28];
    // HYPOTHESIS: a weapon object at offset 0x2c whose slot 143 (offset 0x238) 0x004d76f8 forwards to.
    tBaseWeapon *field_2c;
    char pad_30[8];
    float field_38;
    float field_3c;
    char pad_40[5];
    char field_45;
    char field_46;
    char pad_47[0x81];
    short field_c8;
    char pad_ca[2];
    int field_cc;
    float field_d0;
    float field_d4;
    float field_d8;
    float field_dc;
    float field_e0;
    float field_e4;
    int field_e8;
    int field_ec;
    char pad_f0[8];
    int field_f8;
    char field_fc;
    char pad_fd[0x17];
    int field_114;
    short field_118[6][2];
    int field_130;
    short field_134;
    char pad_136[0x12];
    // HYPOTHESIS: a part at offset 0x148 whose method 0x004d7099 takes a float.
    fn_004d7099_type field_148;
    // HYPOTHESIS: adds a per-type amount (field_f8 is the type) to field_cc.
    void fn_004d74f6();
    bool fn_004d76f8();
    // HYPOTHESIS: a status that 0x004e28fc compares with 0x200 (ready) before each shot.
    int fn_004d87e3(int param_1);
    // HYPOTHESIS: a float that 0x004e28fc passes to 0x004e3b3e when a bank fires.
    float fn_004d786e();
    // HYPOTHESIS: a base class's own slot 39 (elapsed time), called directly by 0x004e2b02
    // as tCloakSystem's slot 39 calls it.
    void fn_0043d696(float elapsed);
};

#endif // tBaseWeapon_h
