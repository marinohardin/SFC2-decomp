//	tShuttleBaySystem.h: tShuttleBaySystem, the ship's shuttle bay.
// Evidence:  class (functions 0x0044e554 to 0x0044f16f; original file unknown)

#ifndef tShuttleBaySystem_h
#define tShuttleBaySystem_h 1

#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tValueBase.h"
#include <string>
#include <list>
#include <vector>

// HYPOTHESIS: the objects the lists at +0x84 and +0x4f0 point to; the destructor deletes each
// through slot 0, a virtual destructor. Their class is not known.
class tObject_0044e844_item
{
public:
    virtual ~tObject_0044e844_item();
};

// HYPOTHESIS: the entries of the vector at +0x90; 0x0044ec9a fills field_4 up to field_0 and
// field_c up to field_8, then looks for field_18 in the names of field_2d8 and stores field_14.
// Its destructor is 0x0044edbd. Its class is not known.
class tObject_0044ec9a_item
{
public:
    ~tObject_0044ec9a_item();

    float field_0;
    float field_4;
    float field_8;
    float field_c;
    int field_10;
    int field_14;
    std::string field_18;
};

// HYPOTHESIS: the base is tRepairableShipSystem, as for the other ship systems; the fields
// between +0x7c and +0xa0 are not recovered.
class tShuttleBaySystem : public tRepairableShipSystem
{
public:
    virtual ~tShuttleBaySystem();
    // vftable slot 16; adds four products of member pairs to the base's value.
    virtual int vfunction17();
    virtual void vfunction100();
    virtual void vfunction101();
    // vftable slot 101; returns the address of field_a0.
    virtual tValue<long>* vfunction102();
    // vftable slot 102; returns the address of field_c4.
    virtual tValue<long>* vfunction103();
    // vftable slot 103; returns field_a0's value.
    virtual int vfunction104();
    // vftable slot 104; returns field_bc.
    virtual int vfunction105();
    // vftable slot 105; returns field_c0.
    virtual int vfunction106();
    // vftable slot 106; returns field_c4's value.
    virtual int vfunction107();
    // vftable slot 107; returns field_e8.
    virtual int vfunction108();
    // vftable slot 108; returns field_f4.
    virtual int vfunction109();
    // vftable slot 109; returns element param_1 of field_2c8.
    virtual int vfunction110(int param_1);
    // vftable slot 110; stores param_2 into element param_1 of field_2c8.
    virtual void vfunction111(int param_1, int param_2);
    // Returns a copy of element param_1 of field_2d8.
    virtual std::string vfunction112(short param_1);
    // HYPOTHESIS: vftable slot 113; stores a copy of param_2 into element param_1 of field_2d8.
    virtual void vfunction113(short param_1, std::string param_2);
    virtual void vfunction114();
    virtual void vfunction115();
    // vftable slot 115; returns element param_1 of an int array.
    virtual int vfunction116(int param_1);
    // vftable slot 116; returns element param_1 of an int array.
    virtual int vfunction117(int param_1);
    virtual void vfunction118();
    virtual void vfunction119();
    // HYPOTHESIS: slots 119 and 120 take an index and field_4cc; 0x0044ea7e calls slot 119 for
    // the four entries of field_3a8 and slot 120 for the 32 of field_1c8.
    virtual void vfunction120(int param_1, int param_2);
    virtual void vfunction121(int param_1, int param_2);
    virtual void vfunction122();
    // HYPOTHESIS: the analysis's name; sets field_a0's value and resets the three 32-entry
    // arrays at +0x148, +0x1c8 and +0x248.
    virtual void vfunction123(long param_1);
    // HYPOTHESIS: the analysis's name; stores param_1 into field_bc (vfunction105 returns it).
    virtual void vfunction124(int param_1);
    // HYPOTHESIS: the analysis's name; stores param_1 into field_c0 (vfunction106 returns it).
    virtual void vfunction125(int param_1);
    // HYPOTHESIS: the slots between 117 and 126 are not recovered; vfunction126 is the
    // analysis's name. Stores param_1 into a member.
    virtual void vfunction126(int param_1);
    // vftable slot 127; returns the member vfunction126 sets.
    virtual int vfunction127();
    // vftable slot 128; sets field_c4's value.
    virtual void vfunction128(long param_1);
    // vftable slot 129; stores param_1 into field_e8.
    virtual void vfunction129(int param_1);
    // vftable slot 130; stores param_1 into field_f4.
    virtual void vfunction130(int param_1);
    // vftable slot 131; stores param_2 into element param_1 of an int array.
    virtual void vfunction131(int param_1, int param_2);
    // vftable slot 132; stores param_2 into element param_1 of an int array.
    virtual void vfunction132(int param_1, int param_2);
    // vftable slot 133; stores param_2 into element param_1 of an int array.
    virtual void vfunction133(int param_1, int param_2);
    // vftable slot 134; returns element param_1 of the array vfunction131 stores into.
    virtual int vfunction134(int param_1);
    // vftable slot 135; returns element param_1 of the array vfunction132 stores into.
    virtual int vfunction135(int param_1);
    // vftable slot 136; returns element param_1 of the array vfunction133 stores into.
    virtual int vfunction136(int param_1);
    // HYPOTHESIS: the per-frame update in vftable slot 38. The base declares that slot without
    // the float, so this declaration adds a slot; it sits last so it shifts no known slot.
    virtual void vfunction39(float param_1);
    // HYPOTHESIS: drops the ids in field_80 whose ship still exists and, once it is empty,
    // checks the ships of field_3a8 and field_1c8.
    void fn_0044ea7e();
    // HYPOTHESIS: takes an index into field_3a8 and a state; 0x0044ea7e passes 4.
    void fn_0044fb1f(int param_1, int param_2);
    // HYPOTHESIS: advances two timers of each entry of field_8c by param_1; once both are full,
    // stores the entry's value into the first free slot whose name it finds, then drops it.
    void fn_0044ec9a(float param_1);
    // HYPOTHESIS: tRepairableShipSystem's non-virtual update step, taking the elapsed time.
    void fn_0043d696(float elapsed);
    void fn_0044f071(float param_1);
    void fn_0044f17d(int param_1);
    void fn_0044f222(int param_1);
    void fn_0044ff32();
    void fn_004507b2();

    int field_7c;
    // HYPOTHESIS: a list of owned objects; the destructor (0x0044e844) destroys it with the
    // out-of-line list destructor 0x005d6a72, as the one at +0x4f0.
    // 0x0044ea7e passes its elements to the entity lookup: entity ids.
    std::list<int> field_80;
    // HYPOTHESIS: a 0x10-byte vector of owned pointers; its destructor is 0x00571a7a.
    // 0x0044ec9a deletes and erases its entries.
    std::vector<tObject_0044ec9a_item*> field_8c;
    int field_9c;
    // The value object at +0xa4; vfunction104 returns its value.
    tValue<long> field_a0;
    int field_bc;
    int field_c0;
    // HYPOTHESIS: a tValue<long>: vfunction128 sets its value at +0x18 and calls the notify
    // step (0x006712c0) with (3, 0), the shape of tValue's assignment.
    tValue<long> field_c4;
    char pade0[0xe8 - 0xe0];
    int field_e8;
    char padec[0xf4 - 0xec];
    int field_f4;
    // The names above sit 4 bytes below the compiled offsets (field_f4 compiles to +0xf8);
    // from here on names give the compiled offset.
    char padfc[0x104 - 0xfc];
    // Set by vfunction126.
    int field_104;
    // HYPOTHESIS: an int array indexed by vfunction131; its length is not recovered.
    int field_108[1];
    char pad10c[0x118 - 0x10c];
    // HYPOTHESIS: an int array indexed by vfunction132; its length is not recovered.
    int field_118[1];
    char pad11c[0x128 - 0x11c];
    // HYPOTHESIS: an int array indexed by vfunction133; its length is not recovered.
    int field_128[1];
    char pad12c[0x138 - 0x12c];
    // HYPOTHESIS: four ints, one per entry of field_3a8, indexed by vfunction116; 0x0044ea7e
    // stores a count from each ship into it.
    int field_138[4];
    // HYPOTHESIS: 32 flags; vfunction123 sets the first field_a0 of them.
    int field_148[32];
    // HYPOTHESIS: 32 entity IDs; the destructor passes 0 to slot 68 of the ship of each.
    int field_1c8[32];
    // HYPOTHESIS: an int array indexed by vfunction117; its length is not recovered.
    int field_248[1];
    char pad24c[0x2c8 - 0x24c];
    // HYPOTHESIS: an int array indexed by vfunction110; its length is not recovered.
    int field_2c8[1];
    char pad2cc[0x2d8 - 0x2cc];
    // A std::string array indexed by vfunction112; the destructor destroys four.
    std::string field_2d8[4];
    char pad318[0x3a8 - 0x318];
    // HYPOTHESIS: 4 entity IDs, treated by the destructor as field_1c8.
    int field_3a8[4];
    // HYPOTHESIS: four 0x24-byte value objects whose destructors store tValueBase's vtable;
    // tMinMaxFloat has that size.
    tMinMaxFloat field_3b8;
    tMinMaxFloat field_3dc;
    tMinMaxFloat field_400;
    tMinMaxFloat field_424;
    char pad448[0x4cc - 0x448];
    // HYPOTHESIS: passed with an index to slots 119 and 120 by 0x0044ea7e.
    int field_4cc;
    char pad4d0[0x4d4 - 0x4d0];
    // HYPOTHESIS: a flag; 0x0044edc8 calls fn_004507b2 while it is set.
    bool field_4d4;
    char pad4d5[0x4d8 - 0x4d5];
    // HYPOTHESIS: four weights; vfunction17 multiplies field_e8, field_f4, field_a0's value and
    // field_c4's value by them.
    int field_4d8;
    int field_4dc;
    int field_4e0;
    int field_4e4;
    char pad4e8[0x4f0 - 0x4e8];
    // HYPOTHESIS: a list of owned objects; the destructor deletes each and empties it.
    std::list<tObject_0044e844_item*> field_4f0;
};

#endif	// tShuttleBaySystem_h
