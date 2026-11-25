#include "Tiger/tgSound.h"

// FUNCTION: STARFLEETOP 0x00646260
void t3DSoundProfile::mSet3DSoundOrientation(float aX, float aY, float aZ) { f3DSoundOrientation.x = aX; f3DSoundOrientation.y = aY; f3DSoundOrientation.z = aZ; }

// FUNCTION: STARFLEETOP 0x006462d0
void t3DSoundProfile::mSet3DSoundConeAngleForNoAttenuation(float anAngle) { f3DSoundConeAngleForNoAttenuation = anAngle; }

// FUNCTION: STARFLEETOP 0x006462e0
float t3DSoundProfile::vfunction17() { return f3DSoundConeAngleForNoAttenuation; }

// FUNCTION: STARFLEETOP 0x006462f0
void t3DSoundProfile::mSet3DSoundConeAttenuationLevel(long aLevel) { f3DSoundConeAttenuationLevel = aLevel; }

// FUNCTION: STARFLEETOP 0x00646300
long t3DSoundProfile::vfunction19() { return f3DSoundConeAttenuationLevel; }
