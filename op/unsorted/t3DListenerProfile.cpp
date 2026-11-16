// Unit:      unsorted/t3DListenerProfile.cpp
// Functions: 0x004355c7 to 0x004356ac, 16 in op/UNITS.tsv
// Evidence:  class; the SDK library's symbols name every member. These bodies are the copies the
//            vtable at 0x00798f44 points to; the SDK library's own copies differ in scheduling.

#include "unsorted/t3DListenerProfile.h"

// FUNCTION: STARFLEETOP 0x004355c7
t3DListenerProfile::~t3DListenerProfile()
{
}

// FUNCTION: STARFLEETOP 0x004355ce
void t3DListenerProfile::mSet3DPosition(tPoint3 *aPoint)
{
    f3DPosition.x = aPoint->x;
    f3DPosition.y = aPoint->y;
    f3DPosition.z = aPoint->z;
}

// FUNCTION: STARFLEETOP 0x004355e6
void t3DListenerProfile::mSet3DPosition(float aX, float aY, float aZ)
{
    f3DPosition.x = aX;
    f3DPosition.y = aY;
    f3DPosition.z = aZ;
}
// Slot 3: the same bytes as slot 2 (0x004355e6), which the linker folded into one body.
void t3DListenerProfile::mSet3DPosition(tPoint3 aPoint) { f3DPosition.x = aPoint.x; f3DPosition.y = aPoint.y; f3DPosition.z = aPoint.z; }

// FUNCTION: STARFLEETOP 0x004355fe
tPoint3 *t3DListenerProfile::mGet3DPosition()
{
    return &f3DPosition;
}

// FUNCTION: STARFLEETOP 0x00435602
void t3DListenerProfile::mSetForwardDirection(tPoint3 *aPoint)
{
    fForwardDirection.x = aPoint->x;
    fForwardDirection.y = aPoint->y;
    fForwardDirection.z = aPoint->z;
}

// FUNCTION: STARFLEETOP 0x0043561a
void t3DListenerProfile::mSetForwardDirection(float aX, float aY, float aZ)
{
    fForwardDirection.x = aX;
    fForwardDirection.y = aY;
    fForwardDirection.z = aZ;
}
// Slot 7: the same bytes as slot 6 (0x0043561a), which the linker folded into one body.
void t3DListenerProfile::mSetForwardDirection(tPoint3 aPoint) { fForwardDirection.x = aPoint.x; fForwardDirection.y = aPoint.y; fForwardDirection.z = aPoint.z; }

// FUNCTION: STARFLEETOP 0x00435632
tPoint3 *t3DListenerProfile::mGetForwardDirection()
{
    return &fForwardDirection;
}

// FUNCTION: STARFLEETOP 0x00435636
void t3DListenerProfile::mSetUpDirection(tPoint3 *aPoint)
{
    fUpDirection.x = aPoint->x;
    fUpDirection.y = aPoint->y;
    fUpDirection.z = aPoint->z;
}

// FUNCTION: STARFLEETOP 0x0043564e
void t3DListenerProfile::mSetUpDirection(float aX, float aY, float aZ)
{
    fUpDirection.x = aX;
    fUpDirection.y = aY;
    fUpDirection.z = aZ;
}
// Slot 11: the same bytes as slot 10 (0x0043564e), which the linker folded into one body.
void t3DListenerProfile::mSetUpDirection(tPoint3 aPoint) { fUpDirection.x = aPoint.x; fUpDirection.y = aPoint.y; fUpDirection.z = aPoint.z; }

// FUNCTION: STARFLEETOP 0x00435666
tPoint3 *t3DListenerProfile::mGetUpDirection()
{
    return &fUpDirection;
}

// FUNCTION: STARFLEETOP 0x0043566a
void t3DListenerProfile::mSetVelocity(tPoint3 *aPoint)
{
    fVelocity.x = aPoint->x;
    fVelocity.y = aPoint->y;
    fVelocity.z = aPoint->z;
}

// FUNCTION: STARFLEETOP 0x00435682
void t3DListenerProfile::mSetVelocity(float aX, float aY, float aZ)
{
    fVelocity.x = aX;
    fVelocity.y = aY;
    fVelocity.z = aZ;
}
// Slot 15: the same bytes as slot 14 (0x00435682), which the linker folded into one body.
void t3DListenerProfile::mSetVelocity(tPoint3 aPoint) { fVelocity.x = aPoint.x; fVelocity.y = aPoint.y; fVelocity.z = aPoint.z; }

// FUNCTION: STARFLEETOP 0x0043569a
tPoint3 *t3DListenerProfile::mGetVelocity()
{
    return &fVelocity;
}

// FUNCTION: STARFLEETOP 0x0043569e
void t3DListenerProfile::mSetUnitScaleInMeters(double aScale)
{
    fUnitScaleInMeters = aScale;
}

// FUNCTION: STARFLEETOP 0x004356a8
double t3DListenerProfile::mGetUnitScaleInMeters()
{
    return fUnitScaleInMeters;
}
