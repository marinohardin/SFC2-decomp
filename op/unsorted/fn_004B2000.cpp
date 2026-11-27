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
	bool fn_004b2eb9(int sx, int sy, tPoint2 *out);

	float m00;
	float m04;
	float m08;
	tPoint3 m0c;
	tPoint3 m18;
	tPoint3 m24;
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

// FUNCTION: STARFLEETOP 0x004b2eb9
bool fn_004b2cd3_View::fn_004b2eb9(int sx, int sy, tPoint2 *out)
{
	tPoint3 p;
	p.x = sx - m38;
	p.y = sy - m3c;
	p.z = 1;
	float x = p * m0c;
	float y = p * m18;
	float z = p * m24;
	if (z != 0)
	{
		out->x = 1 / z * x + m00;
		out->y = 1 / z * y + m04;
		return true;
	}
	return false;
}
