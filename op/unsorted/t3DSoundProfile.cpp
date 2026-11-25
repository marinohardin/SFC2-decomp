#include "Tiger/tgSound.h"

// FUNCTION: STARFLEETOP 0x00646260
void t3DSoundProfile::mSet3DSoundOrientation(float aX, float aY, float aZ) { f3DSoundOrientation.x = aX; f3DSoundOrientation.y = aY; f3DSoundOrientation.z = aZ; }

// FUNCTION: STARFLEETOP 0x006462d0
void t3DSoundProfile::mSet3DSoundConeAngleForNoAttenuation(float anAngle) { f3DSoundConeAngleForNoAttenuation = anAngle; }
