#include "unsorted/tPriorityPowerRequest.h"

// FUNCTION: STARFLEETOP 0x00524109
float tPriorityPowerRequest::fn_00524109()
{
	float &power = *reinterpret_cast<float *>(fField04);
	float requestedPower = power;
	power = 0.0f;
	return requestedPower;
}
