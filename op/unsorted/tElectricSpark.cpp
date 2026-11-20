#include <math.h>
#include "Tiger/tvectormath.h"

// HYPOTHESIS: layout and the counter global are inferred from the constructor's stores.
extern int g_00898ca4;
// HYPOTHESIS: the shared buffer holds the spark's points; 0x0040ca18 copies 12-byte tPoint3
// values into it.
extern tPoint3* g_00898c9c;

// HYPOTHESIS: a free function at 0x0069b0e0 returning a float on the FPU stack (a random value).
float fn_0069b0e0();

// HYPOTHESIS: the object 0x0040ca18 draws with; slot 49 takes a flag around the draw. It may
// be the viewer class declared in unsorted/t3DVisual_Model.cpp.
class fn_0040ca18_viewer
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();
	virtual void vfunction41();
	virtual void vfunction42();
	virtual void vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49(int param_1);
};

// HYPOTHESIS: a free function that draws a strip of points with two widths and two colours.
void fn_005090c7(fn_0040ca18_viewer *param_1, tMatrix *param_2, tPoint3 *param_3, int param_4, float param_5, float param_6, float param_7, float param_8, tPoint3 *param_9, tPoint3 *param_10);

class tElectricSpark
{
public:
	tElectricSpark();
	virtual ~tElectricSpark();
	// HYPOTHESIS: a viewer, the camera matrix and the spark's world matrix.
	virtual void vfunction2(fn_0040ca18_viewer *param_1, const tMatrix *param_2, const tMatrix &param_3);
	// HYPOTHESIS: the spark's start and end points, passed by value.
	virtual void vfunction3(tPoint3 param_1, tPoint3 param_2);

	float m04;
	int m08;
	int m0c;
	tPoint3 m10;
	tPoint3 m1c;
	tPoint3 m28;
};

// FUNCTION: STARFLEETOP 0x0040c963
tElectricSpark::tElectricSpark()
{
	m0c = 0;
	m04 = 1.0f;
	g_00898ca4++;
}

// FUNCTION: STARFLEETOP 0x0040c97b
// FUNCTION: STARFLEETOP 0x0040c997
tElectricSpark::~tElectricSpark()
{
	if (g_00898ca4 && --g_00898ca4 == 0 && g_00898c9c)
	{
		delete g_00898c9c;
		g_00898c9c = 0;
	}
}

extern unsigned int g_00898ca0;

// HYPOTHESIS: a free function that grows the shared buffer to n entries of 12 bytes.
// FUNCTION: STARFLEETOP 0x0040c9c6
bool fn_0040c9c6(unsigned int n)
{
	if (g_00898c9c)
	{
		if (g_00898ca0 >= n)
			return true;
		delete g_00898c9c;
	}
	g_00898c9c = new tPoint3[n];
	if (!g_00898c9c)
		return false;
	g_00898ca0 = n;
	return true;
}

// FUNCTION: STARFLEETOP 0x0040ca18
void tElectricSpark::vfunction2(fn_0040ca18_viewer *param_1, const tMatrix *param_2, const tMatrix &param_3)
{
	int i;
	float scale;
	tPoint3 offset;
	float step;

	if (!g_00898c9c && m0c)
		return;

	step = 1.0f / (m0c - 1);
	for (i = 0; i < m0c; i++)
	{
		// MATCH-ONLY: the float casts keep VC6 from reordering the multiplications; the
		// original may have reached the same trees through float temporaries.
		scale = sin((float)(i * 3.14159265359) * step);
		offset.mSet((float)((fn_0069b0e0() - 0.5f) * m04) * scale, (float)((fn_0069b0e0() - 0.5f) * m04) * scale, (float)((fn_0069b0e0() - 0.5f) * m04) * scale);
		g_00898c9c[i] = m10 + m28 * (float)i + offset;
	}

	tMatrix matrix = param_3;
	tMatrix world(tMatrix::kUninitialized);
	param_2->mMult(world, matrix);

	static tPoint3 origin(0.0f, 0.0f, 0.0f);
	tPoint3 position;
	world.mTransform(position, origin);
	// The original computes this squared distance and discards it.
	float distance = position * position;

	param_1->vfunction49(0);
	float width = m04 * 0.25f;
	offset.mSet(0.7f, 0.7f, 1.0f);
	fn_005090c7(param_1, &matrix, g_00898c9c, m0c, width, width, 0.85f, 0.85f, &offset, &offset);
	param_1->vfunction49(1);
}

// HYPOTHESIS: a float constant (points per unit of length) at a fixed exe address.
extern float g_00795bf4;

// FUNCTION: STARFLEETOP 0x0040cc0a
void tElectricSpark::vfunction3(tPoint3 param_1, tPoint3 param_2)
{
	m10 = param_1;
	m1c = param_2;
	m28 = m1c - m10;
	m0c = m28.mLength() * g_00795bf4;
	if (m0c < 2)
	{
		m0c = 0;
	}
	else if (fn_0040c9c6(m0c))
	{
		m28 /= m0c - 1;
		m04 = m28.mLength();
	}
}
