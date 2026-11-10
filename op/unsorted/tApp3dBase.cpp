// Unit:      unsorted/tApp3dBase.cpp
// Functions: 0x00401017 to 0x0040135a, 10 in game/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include "unsorted/tApp3dBase.h"

tView3D *fn_0069f020();
tView3D *fn_0069fd20();
void fn_0040165f(tView3D *);
// HYPOTHESIS: takes the same view argument as fn_0040165f; vfunction12 passes 0.
void fn_0040168e(tView3D *);
void fn_0040156f(tView3D *, const char *);
void fn_004014e1(int, const char *);
void fn_00401336();
void fn_00401348();

// HYPOTHESIS: a byte flag at 0x008930a2, beside the application singleton pointer.
extern char g_b008930a2;

// HYPOTHESIS: the application singleton pointer (0x0089309c).
tApp3dBase *g_pApp3dBase;

// FUNCTION: STARFLEETOP 0x00401017
tApp3dBase::tApp3dBase()
{
	g_pApp3dBase = this;
	m_fieldC = 0;
	m_field10 = 0;
	m_field4 = 0;
	m_field8 = 0;
}

// FUNCTION: STARFLEETOP 0x00401057
tApp3dBase::~tApp3dBase()
{
}

// FUNCTION: STARFLEETOP 0x00401033
bool tApp3dBase::vfunction13()
{
	return vfunction20();
}

// FUNCTION: STARFLEETOP 0x00401038
void tApp3dBase::vfunction1(tView3D *)
{
}

// FUNCTION: STARFLEETOP 0x0040105e
bool tApp3dBase::vfunction11(tApplication *param_1, fn_0040105e_Params *param_2)
{
	if (param_2->m_field0C) {
		m_field10 = fn_0069fd20();
	} else {
		m_field10 = fn_0069f020();
	}
	if (m_field10 == 0) {
		return false;
	}
	if (param_2->m_left >= param_2->m_right || param_2->m_top >= param_2->m_bottom) {
		int bottom = param_1->fn_0069a970();
		int right = param_1->fn_0069a940();
		param_2->m_left = 0;
		param_2->m_top = 0;
		param_2->m_right = right;
		param_2->m_bottom = bottom;
	}
	int a = param_1->fn_0069aa00();
	int b = param_1->fn_006fe8a0();
	if (!vfunction7(m_field10, a, b, param_2)) {
		m_field10->vfunction6();
		m_field10->vfunction8();
		m_field10 = 0;
		return false;
	}
	fn_0040165f(m_field10);
	fn_0040156f(m_field10, "");
	fn_004014e1(a, "Loading...");
	if (param_2->m_field0D) {
		m_field10->vfunction62(param_2->m_field10, param_2->m_field14, param_2->m_field18);
	} else {
		m_field10->vfunction64(-1.0f, 1.0f);
	}
	if (param_2->m_field34.m_x != 0.0f || param_2->m_field34.m_y != 0.0f ||
		param_2->m_field34.m_z != 0.0f) {
		fn_0040105e_Vector v(param_2->m_field34.m_x, param_2->m_field34.m_y, param_2->m_field34.m_z);
		m_field10->vfunction35(&v);
	}
	m_field10->vfunction13(fn_00401336, this);
	m_field10->vfunction15(fn_00401348, this);
	vfunction8(m_field10);
	return true;
}

// FUNCTION: STARFLEETOP 0x004011d0
void tApp3dBase::vfunction12()
{
	if (m_field10 != 0) {
		vfunction9(m_field10);
		m_field10->vfunction6();
		m_field10->vfunction8();
	}
	m_fieldC = 0;
	m_field10 = 0;
	fn_0040168e(0);
	m_field4 = 0;
	m_field8 = 0;
}

// FUNCTION: STARFLEETOP 0x00401209
bool tApp3dBase::vfunction7(tView3D *param_1, int param_2, int param_3, fn_0040105e_Params *param_4)
{
	if (!param_4->m_field0C) {
		int driver = g_p008cc580->vfunction5("3D", "driver", 0);
		param_1->vfunction5(driver);
	}
	bool retry;
	if (param_4->m_field00 >= 1) {
		retry = param_4->m_field04 >= 1;
		if (param_1->vfunction4(param_2, param_3, (RECT *)&param_4->m_left, retry) ||
			(retry && param_4->m_field04 != 2 &&
			 param_1->vfunction4(param_2, param_3, (RECT *)&param_4->m_left, false))) {
			return true;
		} else if (param_4->m_field00 == 2) {
			return false;
		}
	}
	retry = param_4->m_field08 >= 1;
	bool ok = param_1->vfunction3(param_2, param_3, param_4->m_field1C, param_4->m_field20,
								  (RECT *)&param_4->m_left, retry);
	if (ok) {
		return true;
	}
	if (retry && param_4->m_field08 != 2) {
		ok = param_1->vfunction3(param_2, param_3, param_4->m_field1C, param_4->m_field20,
								 (RECT *)&param_4->m_left, false);
		if (ok) {
			return true;
		}
	}
	return ok;
}

// FUNCTION: STARFLEETOP 0x004012db
bool tApp3dBase::vfunction20()
{
	if (m_field10 != 0) {
		vfunction1(m_field10);
		if (!m_field14) {
			m_field10->vfunction11();
		}
		m_field8++;
		vfunction3(m_field10);
		if (!m_field14) {
			m_field10->vfunction12();
			if (!g_b008930a2) {
				vfunction5();
				m_field10->vfunction31();
				vfunction6();
			}
		}
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x0040135a
void tApp3dBase::vfunction18()
{
	m_field14 = 0;
}
