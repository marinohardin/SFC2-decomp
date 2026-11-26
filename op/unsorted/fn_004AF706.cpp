// Unit:      unsorted/fn_004AF706.cpp
// Functions: 0x004af706 to 0x004aff50, 19 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

#include "Tiger/tvectormath.h"

// HYPOTHESIS: builds a look-at (view) matrix from an eye position, a target and an up vector.
// FUNCTION: STARFLEETOP 0x004af706
void fn_004af706(const tPoint3 &eye, const tPoint3 &target, const tPoint3 &up, tMatrix &m)
{
	tPoint3 axisZ = eye - target;
	axisZ.mNormalize();
	tPoint3 axisX = up % axisZ;
	axisX.mNormalize();
	tPoint3 axisY = axisZ % axisX;

	tMtx4x4 &mtx = m.mGetMtx4x4();
	mtx[0][0] = axisX.x;
	mtx[0][1] = axisX.y;
	mtx[0][2] = axisX.z;
	mtx[0][3] = 0.0f;
	mtx[1][0] = axisY.x;
	mtx[1][1] = axisY.y;
	mtx[1][2] = axisY.z;
	mtx[1][3] = 0.0f;
	mtx[2][0] = axisZ.x;
	mtx[2][1] = axisZ.y;
	mtx[2][2] = axisZ.z;
	mtx[2][3] = 0.0f;
	mtx[3][0] = 0.0f;
	mtx[3][1] = 0.0f;
	mtx[3][2] = 0.0f;
	mtx[3][3] = 1.0f;
	m.mTranslate(-eye.x, -eye.y, -eye.z);
}
