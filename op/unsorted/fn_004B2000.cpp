#include <math.h>
#include "Tiger/tvectormath.h"

// HYPOTHESIS: two constants loaded from memory rather than folded into the pi product, so
// declared as const variables: one degree in radians over pi (0x007a13a8) and a half (0x007977c8).
const double gObject_007a13a8 = 1.0 / 180.0;
const double gObject_007977c8 = 0.5;

// HYPOTHESIS: a view projection; the owning class is not identified yet. Only the members
// this function writes are placed.
class fn_004b2cd3_View
{
public:
	void fn_004b2cd3(float fov, float distance, float width, float height);

	float m00;
	float m04;
	float m08;
	char m0c[0x24];
	float m30;
	float m34;
	float m38;
	float m3c;
};

// FUNCTION: STARFLEETOP 0x004b2cd3
void fn_004b2cd3_View::fn_004b2cd3(float fov, float distance, float width, float height)
{
	float halfY = (float)tan(fov * kPi * gObject_007a13a8 * gObject_007977c8) * distance;
	float halfX = width / height * halfY;
	m38 = width * 0.5f;
	m3c = 0.5f * height;
	m30 = -((distance + distance) / (halfX - -halfX) * m38);
	m34 = (distance + distance) / (halfY - -halfY) * m3c;
	m00 = 0;
	m04 = 0;
	m08 = 0;
}
