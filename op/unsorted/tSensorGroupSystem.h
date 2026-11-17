//	tSensorGroupSystem.h: tSensorGroupSystem, the ship's sensor group.
// Evidence:  class (functions 0x0043dfd4 to 0x0043f6c3; original file unknown)

#ifndef tSensorGroupSystem_h
#define tSensorGroupSystem_h 1

#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tValueBase.h"

class tPhysicalEntity;
class tObject_0042496f;
class fn_0043e366_part;

// HYPOTHESIS: the base is tRepairableShipSystem: the destructor stores vtable 0x00799598 (the
// one after tMinMaxFloat's) before calling tIndividualShipSystem's. Six 0x24-byte value
// objects follow at +0x8c; the fields between +0x80 (where the 8-aligned base ends) and +0x8c
// are not recovered.
class tSensorGroupSystem : public tRepairableShipSystem
{
public:
    virtual ~tSensorGroupSystem();

    // HYPOTHESIS: called first in the destructor; a cleanup step.
    void fn_0043e09e();
    // HYPOTHESIS: adds an entity ID to the list at +0x1e0.
    void fn_0043e0ed(int param_1);
    void fn_0043e201(float param_1);
    // HYPOTHESIS: a test on an entity ID; 0x0043e201 asks it for entities other than its owner.
    // HYPOTHESIS: returns the number of entries of field_1e0 in use.
    int fn_0043e351();
    void fn_0043e359(char param_1);
    // vftable slot 22; saves or loads the sensor fields.
    virtual unsigned int vfunction23(int param_1, unsigned int param_2, int param_3);
    void vfunction51();
    char fn_0043eb5f(int aID);
    void fn_0043e642(int param_1);
    void fn_0043e6ce(int param_1);
    float fn_0043e75a();
    float fn_0043e761();
    float fn_0043e768();
    float fn_0043e76f();
    float fn_0043e776();
    float fn_0043e77d();
    float fn_0043e78a(int param_1);
    void fn_0043e838();
    void fn_0043e853();
    // HYPOTHESIS: param_2 is not read.
    float fn_0043e870(float param_1, int param_2);
    char fn_0043e8da();
    char fn_0043e911(int aID);
    void fn_0043ec81();
    int fn_0043eca6(int param_1);
    void fn_0043f71b(int param_1, int param_2);
    void fn_0043eee8();
    // HYPOTHESIS: the result is an integer 0x0043eee8 converts to float.
    int fn_0043f023(struct vfunction33_result* owner, tObject_0042496f* target);
    // HYPOTHESIS: a value for one map cell; 0x0043f023 sums it along a line of cells.
    int fn_0043f618(int x, int y);
    virtual void vfunction39(float param_1);
    virtual void vfunction48(int param_1);
    virtual void vfunction49(int param_1);
    virtual void vfunction50(int param_1);
    virtual float vfunction70();
    // HYPOTHESIS: overrides slot 72.

    virtual float vfunction73(float param_1, float param_2);
    virtual float vfunction74(float param_1, float param_2);
    // HYPOTHESIS: param_2 is not read; the base call passes 0.
    virtual float vfunction90(float param_1, int param_2);
    virtual void vfunction91(unsigned int param_1, int param_2);
    // HYPOTHESIS: param_2 is not read (the function pops two arguments).
    float fn_0043ed38(int param_1, int param_2);
    int fn_0043edd8();
    void fn_0043ee5a();

    // HYPOTHESIS: three objects 0x0043e366 asks a yes/no question (slot 9) and then calls at
    // slot 50; their type is not recovered.
    fn_0043e366_part* field_80;
    fn_0043e366_part* field_84;
    fn_0043e366_part* field_88;
    tMinMaxFloat field_8c;
    tMinMaxFloat field_b0;
    tMinMaxFloat field_d4;
    tMinMaxFloat field_f8;
    tMinMaxFloat field_11c;
    tMinMaxFloat field_140;
    // HYPOTHESIS: the total ECM and ECCM 0x0043e642 shares between field_184 and field_188.
    int field_164;
    float field_168;
    float field_16c;
    float field_170;
    float field_174;
    float field_178;
    float field_17c;
    float field_180;
    int field_184;
    int field_188;
    // HYPOTHESIS: the answer 0x0043e201 uses for its owner's own entities.
    char field_18c;
    char pad18d[0x190 - 0x18d];
    float field_190;
    char pad194[0x198 - 0x194];
    // HYPOTHESIS: time accumulated by 0x0043f1c1 between sensor updates.
    double field_198;
    // HYPOTHESIS: the entity the owner's slot 11 names, as 0x0043eee8 last found it.
    tPhysicalEntity* field_1a0;
    char pad1a4[0x1d9 - 0x1a4];
    // HYPOTHESIS: the number of entries of field_1e0 in use (read with movsx).
    char field_1d9;
    char pad1da[0x1dc - 0x1da];
    // HYPOTHESIS: time accumulated by 0x0043e201.
    float field_1dc;
    // HYPOTHESIS: twelve entity IDs; 0x0043e09e looks each up in the entity table.
    int field_1e0[12];
};

#endif	// tSensorGroupSystem_h
