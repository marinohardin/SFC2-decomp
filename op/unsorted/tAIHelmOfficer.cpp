//	tAIHelmOfficer.cpp: tAIHelmOfficer, the AI officer that steers a ship.
// Evidence:  class (RTTI; original file unknown)

#include <math.h>

#include "tAIHelmOfficer.h"
#include "tObject_0096eca8.h"
#include "tNetControl.h"
#include "Source/network/NetMsgClass.h"

// HYPOTHESIS: the speed message 0x00515db1 builds on the stack (vftable 0x007971a4): one float,
// set to 0 to stop the ship. Its map is at 0x0041978f.
struct tNetMsg_kAdjustSpeed: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	float field_0x4;
};

// HYPOTHESIS: the turn message 0x005169e0 builds on the stack (vftable 0x00796efc): a heading
// and a turn direction (-1, 0 or 1). Its map is at 0x00414b7c.
struct tNetMsg_kMoveInDirection: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	short field_0x4;
	char field_0x6;
};

// HYPOTHESIS: a byte flag at 0x0085922b; 0x005169e0 reports the power request only while set.
extern char g_b0085922b;

// HYPOTHESIS: a double global at 0x00c7c148 holding the current time.
extern double g_00c7c148;

// HYPOTHESIS: free helpers that follow 0x00515f27 in the exe; their meanings are not known.
bool fn_00516670(short param_1, short param_2);
// HYPOTHESIS: 0x005166ed is defined in another unit returning int (it sets all of eax), while
// this unit tests only al, so the declaration seen here says bool; C linkage lets both name it.
extern "C" bool fn_005166ed(short param_1, short param_2, short param_3);
float fn_00516733(float param_1);
float fn_005167ee(float param_1);
short fn_00516825(float param_1);
// HYPOTHESIS: the arc tangent of a point as an angle, as tSfcApp3d.cpp declares it.
short fn_0069afc0(float, float);

// FUNCTION: STARFLEETOP 0x00515daa
tAIHelmOfficer::~tAIHelmOfficer()
{
}

// FUNCTION: STARFLEETOP 0x00515db1
void tAIHelmOfficer::fn_00515db1()
{
	field_0x54 = -1;
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x20));
	if (field_0x140 == 0 && ship && !ship->fn_00419c87() && ship->field_0x874 != 1)
	{
		tNetMsg_kAdjustSpeed msg;
		msg.field_0x4 = 0.0f;
		gNetControl.mQueueMsg(&msg, tNetMsg::kAdjustSpeedAbs, field_0x20, 300.0);
	}
	else if (ship && field_0x140 == 0)
	{
		if (ship->field_0xad4 && ship->field_0xad4->vfunction89() && !ship->field_0x5d)
			ship->field_0xad4->vfunction89()->field_0x4cc = 4;
		field_0x13c = 0xb;
	}
	else if (field_0x18 != 3)
	{
		field_0x18 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00515e79
void tAIHelmOfficer::fn_00515e79()
{
	tEntity *ship = gObject_008e1060.fn_lookup(field_0x20);
	if (ship)
		field_0x28 = ship->fCurrPos;
}

// FUNCTION: STARFLEETOP 0x00515ea9
void tAIHelmOfficer::fn_00515ea9()
{
	tEntity *target = gObject_008e1060.fn_lookup(field_0x54);
	if (target)
	{
		tObject_00528ddb *object = gObject_0096eca8.fn_00528ddb(field_0x54, 0, 4, 0);
		field_0x80 = object->fn_00527c51();
		field_0x8c = target->fCurrPos;
		field_0x58 = target->vfunction14();
	}
}

// FUNCTION: STARFLEETOP 0x00515f27
void tAIHelmOfficer::fn_00515f27()
{
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x20));
	bool turnedOne = false;
	bool turnedOther = false;
	tObject_0042496f *leader = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x140->fn_00525081()));
	field_0x154 = false;
	field_0x155 = false;
	field_0x18 = 4;
	if (leader && ship)
	{
		field_0x80 = leader->field_0x34;
		field_0x8c = leader->field_0x34;
		field_0x58 = 2.0f;
		field_0x140->fn_005252b0(field_0x20, &field_0x80);
		field_0x140->fn_005252b0(field_0x20, &field_0x8c);

		short heading = leader->field_0x90;
		field_0x14e = heading;
		short previous = leader->vfunction66(0);
		if (heading != previous)
		{
			short turn;
			if (heading > previous)
				turn = heading - previous;
			else
				turn = heading - previous + 0x2000;
			if (fn_00516670(heading, turn))
			{
				if (turn > 0x200 && turn < 0x1000)
					turnedOne = true;
				else
					turnedOne = false;
			}
			else
			{
				if (turn > 0x200 && turn < 0x1000)
					turnedOther = true;
				else
					turnedOther = false;
			}
		}

		float speed = leader->vfunction63() * 0.1;
		short shipHeading = ship->field_0x90;
		// HYPOTHESIS: the ship's own speed is read and not used.
		float shipSpeed = ship->vfunction63();
		tPoint3 shipPos = ship->field_0x34;
		tPoint3 toPoint;
		// HYPOTHESIS: computed and not used; the original calls the subtraction for it.
		tPoint3 toLeader;
		toPoint = shipPos - field_0x80;
		toLeader = shipPos - field_0x8c;
		short bearing = fn_0069afc0(toPoint.x, toPoint.y);
		bool b5 = false;
		bool b4 = false;
		float dist = sqrt(toPoint.x * toPoint.x + toPoint.y * toPoint.y);
		field_0x150 = dist;

		if (dist < 90.0f && speed < 0.75)
		{
			short back = (bearing - 0x1000) & 0x1fff;
			if (dist < 0.2)
			{
				field_0x14c = heading;
				field_0x144 = speed;
			}
			else
			{
				if (fn_005166ed(shipHeading, back - 0xa0, back + 0xa0))
					field_0x144 = fn_00516733(dist) + speed;
				else if (dist > 2.0)
					field_0x144 = speed + 0.5;
				else
					field_0x144 = 0;
				field_0x14c = back;
			}
		}

		if (speed > 0.2 && speed < 0.75 && dist < 0.5 || speed >= 0.75 || dist >= 90.0f)
		{
			if (dist < 100.0)
			{
				b5 = fn_005166ed(bearing - shipHeading, 0xec0, 0x1140);
				b4 = fn_005166ed(bearing - shipHeading, -0x140, 0x140);
			}
			bool b6 = fn_005166ed(bearing - shipHeading, 0, 0x1000);
			bool b7 = fn_005166ed(shipHeading - bearing, 0, 0x1000);
			bool b3;
			if (fn_005166ed(bearing - shipHeading, -0x800, 0x800) && speed > 2.0f)
				b3 = true;
			else
				b3 = false;
			bool b8 = fn_005166ed(bearing - shipHeading, 0x800, 0x1800);
			field_0x14c = -1;
			field_0x144 = -1.0f;
			bool aligned = fn_005166ed(heading, shipHeading - 0x3c0, shipHeading + 0x3c0);
			if (dist < 1.0)
			{
				field_0x14c = heading;
				field_0x144 = speed;
				if (dist > 0.3)
				{
					if (b3)
						field_0x144 = speed - 0.1f;
					else if (b8)
						field_0x144 = speed + 0.1f;
					if (b6)
						field_0x14c = heading - 0x10;
					else if (b7)
						field_0x14c = heading + 0x10;
				}
			}
			else if (aligned && b4 && speed > 0.75)
			{
				field_0x144 = speed - fn_005167ee(dist);
				field_0x14c = heading;
			}
			else if (aligned && b5)
			{
				field_0x144 = fn_00516733(dist) + speed;
				field_0x14c = heading;
			}
			else if (b6 && dist < 100.0f && !turnedOne && !turnedOther && (speed > 0.75 || b8))
			{
				if (b3)
					field_0x144 = speed - fn_005167ee(dist);
				else if (b8)
					field_0x144 = fn_00516733(dist) + speed;
				else
					field_0x144 = speed;
				field_0x14c = heading - fn_00516825(dist);
			}
			else if (b7 && dist < 100.0f && !turnedOne && !turnedOther && (speed > 0.75 || b8))
			{
				if (b3)
					field_0x144 = speed - fn_005167ee(dist);
				else if (b8)
					field_0x144 = fn_00516733(dist) + speed;
				else
					field_0x144 = speed;
				field_0x14c = fn_00516825(dist) + heading;
			}
			else
			{
				int mode = turnedOne ? 3 : (turnedOther ? 1 : 4);
				tObject_00528ddb *object =
					gObject_0096eca8.fn_00528ddb(field_0x140->fn_00525081(), 0, mode, 0);
				field_0x80 = object->fn_00527bfd(turnedOne || turnedOther ? 0.99 : 0.2f);
				field_0x140->fn_005252b0(field_0x20, &field_0x80);
				toPoint = shipPos - field_0x80;
				field_0x14c = fn_0069afc0(toPoint.x, toPoint.y) + 0x1000;
				field_0x144 = fn_00516733(dist) + speed;
				field_0x154 = turnedOne && field_0x156;
				field_0x155 = turnedOther && field_0x156;
				if (field_0x154 || field_0x155)
				{
					if (b3)
						field_0x144 = speed - fn_005167ee(dist);
					else if (b8)
						field_0x144 = fn_00516733(dist) + speed;
					else
						field_0x144 = speed;
				}
			}
		}

		if (field_0x14c != -1)
		{
			while (field_0x14c < 0)
				field_0x14c += 0x2000;
			field_0x14c &= 0x1fff;
		}
		if (field_0x144 > field_0x14)
			field_0x144 = field_0x14;
		else if (field_0x144 < 0.0)
			field_0x144 = 0;
		if (!turnedOne && !turnedOther)
			field_0x156 = dist < 15.0f;
	}
}

// FUNCTION: STARFLEETOP 0x00516670
bool fn_00516670(short param_1, short param_2)
{
	if (param_1 > 0x1000)
	{
		if (param_2 < 0x1000)
			param_2 += 0x2000;
		return param_2 - param_1 <= 0x1000;
	}
	else if (param_2 > 0x1000)
	{
		if (param_1 < 0x1000)
			param_1 += 0x2000;
		return param_1 - param_2 <= 0x1000;
	}
	else
	{
		if ((param_2 - param_1 > 0x1000 || param_2 - param_1 < 0) &&
			param_1 - param_2 <= 0x1000 && param_1 - param_2 >= 0)
			return true;
		return false;
	}
}

// FUNCTION: STARFLEETOP 0x00516733
float fn_00516733(float param_1)
{
	// HYPOTHESIS: the speed to add for a distance to the followed point; 0x00515f27 adds it.
	if (param_1 < 0.5)
		return 0.1f;
	if (param_1 < 1.0)
		return 0.3f;
	if (param_1 < 2.0)
		return 0.6f;
	if (param_1 < 4.0)
		return 1.0f;
	if (param_1 < 12.0)
		return param_1 - 3.0;
	if (param_1 < 18.0)
		return 9.0f;
	if (param_1 < 40.0)
		return 10.0f;
	if (param_1 < 70.0)
		return 13.0f;
	if (param_1 < 100.0)
		return 15.0f;
	return 20.0f;
}

// FUNCTION: STARFLEETOP 0x005167ee
// HYPOTHESIS: a piecewise speed for a distance, like 0x00516733: 0.1 below 1, rising linearly
// to 5 at 45, and 5 beyond. The multiplier is 1/9 (5/45) in the original's constant data.
float fn_005167ee(float param_1)
{
	if (param_1 < 1.0f)
		return 0.1f;
	if (param_1 < 45.0f)
		return param_1 * (5.0f / 45.0f);
	return 5.0f;
}

// FUNCTION: STARFLEETOP 0x00516825
// HYPOTHESIS: a heading offset (0x2000 to a full turn) for a distance; 0x00515f27 adds it to
// or subtracts it from the heading to the followed point.
short fn_00516825(float param_1)
{
	if (param_1 < 3.0f)
		return 0x40;
	if (param_1 < 5.0f)
		return 0x60;
	if (param_1 < 10.0f)
		return 0x140;
	if (param_1 < 15.0f)
		return 0x1e0;
	return 0x320;
}

// FUNCTION: STARFLEETOP 0x00516879
// HYPOTHESIS: the officer's update: follow the leader through 0x00515f27 when the ship is in a
// formation it does not lead, otherwise steer on its own.
void tAIHelmOfficer::fn_00516879()
{
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x20));
	if (ship)
	{
		field_0x128 = false;
		field_0x14c = -1;
		field_0x144 = -1.0f;
		if (field_0x140 && field_0x140->fn_00525081() != field_0x20 &&
			field_0x140->fn_00525228(field_0x20))
		{
			field_0x10 = 0;
			field_0x4c = 0;
			fn_00515f27();
		}
		else
		{
			if (ship->fn_00419c87())
				field_0x10 = 20.0f;
			else
				field_0x10 = 8.0f;
			field_0x4c = field_0x48 * field_0x10;
			if (field_0x18 == 1)
				fn_00515ea9();
		}
		fn_00515e79();
		fn_00516e2a();
	}
}

// FUNCTION: STARFLEETOP 0x0051692e
void tAIHelmOfficer::fn_0051692e()
{
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x20));
	if (ship)
	{
		tObject_0043c0f2 *object = ship->field_0xad4->vfunction80();
		if (object)
		{
			object->fn_0043bdb6(1);
			gObject_0096eca8.fn_00528dad(field_0x20);
		}
	}
}

// FUNCTION: STARFLEETOP 0x0051696c
// HYPOTHESIS: sets the highest speed and recomputes the factor 0x00516879 sets: 0 when following,
// otherwise half the speed, at most 12.
void tAIHelmOfficer::fn_0051696c(float aMaxSpeed)
{
	field_0x14 = aMaxSpeed;
	if (field_0x140 && field_0x140->fn_00525081() != field_0x20 &&
		field_0x140->fn_00525228(field_0x20))
		field_0x10 = 0;
	else if (field_0x14 > 24.0f)
		field_0x10 = 12.0f;
	else
		field_0x10 = field_0x14 * 0.5f;
	field_0x4c = field_0x48 * field_0x10;
}

// FUNCTION: STARFLEETOP 0x005169d6
void tAIHelmOfficer::fn_005169d6(float param_1)
{
	field_0x38 = param_1;
}

// FUNCTION: STARFLEETOP 0x005169e0
// HYPOTHESIS: the analysis named it for the message it builds. Turns the ship: to the heading
// asked for when following, to the target's heading, or along the path for param_1 and param_2,
// then passes the matching entry at 0x98 to slot 3 of the power request.
void tAIHelmOfficer::fn_005169e0(int param_1, int param_2)
{
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x20));
	if (ship && ship->field_0x5c && field_0x18)
	{
		tObject_0042496f *target = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x54));
		short heading = ship->vfunction66(0);
		bool hold = true;
		tNetMsg_kMoveInDirection msg;
		if (field_0x14c == -1)
			hold = true;
		else if (field_0x140)
		{
			if (field_0x12c < 0.0f || field_0x50 || field_0x35 ||
				(field_0x154 || fn_00516670(heading, field_0x14c)) && field_0x138 < 0.0f ||
				(field_0x155 || !fn_00516670(heading, field_0x14c)) && field_0x130 < 0.0f ||
				fn_005166ed(heading, field_0x14c - 0x3c0, field_0x14c + 0x3c0) &&
					field_0x134 < 0.0f)
				hold = true;
			else
				hold = false;
		}
		if (field_0x140 && field_0x140->fn_00525081() != field_0x20 &&
			field_0x140->fn_00525228(field_0x20) && field_0x14c >= 0 && !hold)
		{
			param_2 = 0;
			if (field_0x14c != heading)
			{
				int direction = field_0x154 ? -1 : field_0x155 != 0;
				msg.field_0x4 = field_0x14c;
				msg.field_0x6 = direction;
				((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x20))->fn_00414bfa(&msg);
			}
		}
		else if (field_0x69 && param_2 == field_0x60 && target)
		{
			param_2 = 0;
			msg.field_0x4 = target->vfunction66(0);
			msg.field_0x6 = 0;
			((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x20))->fn_00414bfa(&msg);
		}
		else
		{
			tObject_00528ddb *path = gObject_0096eca8.fn_00528ddb(field_0x20, param_1, param_2, 0);
			if (path->fn_0055e617() < 0)
				msg.field_0x6 = -1;
			else
				msg.field_0x6 = path->fn_0055e617() > 0;
			short angle = (path->fn_00527afe()->field_0x18 + path->fn_00527b02()) & 0x1fff;
			if (angle < 0)
				angle += 0x2000;
			msg.field_0x4 = angle;
			((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x20))->fn_00414bfa(&msg);
		}
		if (g_b0085922b)
		{
			// HYPOTHESIS: the entries at 0x98 are 0x24 bytes, four to a row, like the selector
			// objects tAIWeaponOfficer keeps; that class is declared only in its own unit.
			float power = fn_00524109();
			vfunction3_for_tPriorityPowerRequest(
				(int *)(field_0x98 + (param_1 * 4 + param_2) * 0x24), field_0x54, power);
		}
	}
}
extern const float g_0079707c; // 0.1f: not the pooled literal the other speeds use
// FUNCTION: STARFLEETOP 0x00516c79
float tAIHelmOfficer::vfunction3_for_tPriorityPowerRequest(int *param_1, int param_2,
	float param_3)
{
	tObject_0042496f *ship = ((tObject_0042496f *)gObject_008e1060.fn_0042496f(field_0x20));
	if (ship && ship->field_0x5c && field_0x18 && field_0x18 != 3)
	{
		param_3 = param_3 / field_0x48;
		float speed = ship->vfunction63() * g_0079707c;
		float delta = param_3 - speed;
		if (abs((int)delta) > 2.0f)
			delta *= 0.5f;
		param_3 = delta + speed;
		tNetMsg_kAdjustSpeed msg;
		msg.field_0x4 = param_3;
		double now = g_00c7c148;
		if (now > field_0x158 + 3000000.0f)
		{
			if (msg.field_0x4 > field_0x148 + 1.0 || msg.field_0x4 < field_0x148 - 1.0)
			{
				field_0x158 = now;
				gNetControl.mQueueMsg(&msg, tNetMsg::kAdjustSpeedAbs, field_0x20,
					field_0x140 ? 1 : 300);
				field_0x148 = msg.field_0x4;
			}
		}
		return 0;
	}
	return param_3;
}
