// Unit:      unsorted/tSensorGroupSystem.cpp
// Functions of tSensorGroupSystem (0x0043dfd4 to 0x0043f6c3); original file unknown.
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tValueBase.h"

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
};

// FUNCTION: STARFLEETOP 0x0043dfd4
// FUNCTION: STARFLEETOP 0x0043dff0
tSensorGroupSystem::~tSensorGroupSystem()
{
    fn_0043e09e();
}
