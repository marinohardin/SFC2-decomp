// Unit:      unsorted/tSensorGroupSystem.cpp
// Functions of tSensorGroupSystem (0x0043dfd4 to 0x0043f6c3); original file unknown.
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tEntity.h"

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

    char pad80[0x8c - 0x80];
    tMinMaxFloat field_8c;
    tMinMaxFloat field_b0;
    tMinMaxFloat field_d4;
    tMinMaxFloat field_f8;
    tMinMaxFloat field_11c;
    tMinMaxFloat field_140;
    char pad164[0x1e0 - 0x164];
    // HYPOTHESIS: twelve entity IDs; 0x0043e09e looks each up in the entity table.
    int field_1e0[12];
};

// HYPOTHESIS: an entity found by ID whose vftable slot 26 takes one value.
class fn_0043e09e_entity
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
    virtual void vfunction25();
    virtual void vfunction26();
    virtual void vfunction27(int param_1); // slot 26
};

// FUNCTION: STARFLEETOP 0x0043dfd4
// FUNCTION: STARFLEETOP 0x0043dff0
tSensorGroupSystem::~tSensorGroupSystem()
{
    fn_0043e09e();
}

// FUNCTION: STARFLEETOP 0x0043e09e
void tSensorGroupSystem::fn_0043e09e()
{
    for (int i = 0; i < 12; i++)
    {
        // HYPOTHESIS: the entity looked up takes one argument at vftable slot 26, which
        // tPhysicalEntity's declaration does not show; the cast stands for that type.
        fn_0043e09e_entity* entity = (fn_0043e09e_entity*)gObject_008e1060.fn_004249d3(field_1e0[i]);
        if (entity != 0)
            entity->vfunction27(vfunction37(0, 100, 0, 0));
    }
}
