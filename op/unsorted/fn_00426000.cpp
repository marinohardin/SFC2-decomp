#include "Tiger/tvectormath.h"

// HYPOTHESIS: the insertion sort of the standard library's sort, instantiated for unsigned int.
// FUNCTION: STARFLEETOP 0x004260a4
void fn_004260a4(unsigned int* first, unsigned int* last)
{
	if (first == last)
		return;
	for (unsigned int* i = first + 1; i != last; ++i)
	{
		unsigned int val = *i;
		if (!(val < *first))
		{
			unsigned int* p = i;
			unsigned int* q = p - 1;
			while (val < *q)
			{
				*p = *q;
				p = q;
				--q;
			}
			*p = val;
		}
		else
		{
			unsigned int* src = i;
			unsigned int* dst = i + 1;
			while (first != src)
				*--dst = *--src;
			*first = val;
		}
	}
}

// HYPOTHESIS: the object held by the global at 0x008cc620 (also declared inside fn_00496000.cpp);
// its third virtual loads a texture by file name and returns a handle.
class fn_00426116_Loader
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual long vfunction2(const char* name, long a, long b, long c);
};

extern fn_00426116_Loader* g_008cc620;
extern char g_008f5230;
// HYPOTHESIS: the background texture handle.
extern long g_008f522c;

// FUNCTION: STARFLEETOP 0x00426116
void fn_00426116()
{
	if (!g_008f5230)
	{
		g_008f522c = g_008cc620->vfunction2("assets/textures/skybgnd-mono.BMP", 1, 0, 0);
	}
	g_008f5230 = 1;
}

// HYPOTHESIS: the game object handed to the constructor of the class at 0x00426141; only its
// virtual at slot 13 (returns a float) and the int at +0x86c are used here.
class fn_00426141_Game
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
	virtual float vfunction13();
	char pad0004[0x30];
	tPoint3 m34;
	char pad0040[0x82c];
	int m86c;
};

// HYPOTHESIS: a 36-byte element of the 17 x 33 grid.
class fn_00426141_Cell
{
public:
	float m00[9];
	fn_00426141_Cell& fn_0042642c(const fn_00426141_Cell& other);
};

// HYPOTHESIS: three floats set together.
class fn_00426141_Color
{
public:
	float r;
	float g;
	float b;
	void Set(float nr, float ng, float nb);
};

inline void fn_00426141_Color::Set(float nr, float ng, float nb)
{
	r = nr;
	g = ng;
	b = nb;
}

// HYPOTHESIS: a vertex: position, normal, then four colour bytes at +0x20 (layout from the accesses
// in 0x00426469 only).
#pragma pack(push)
#pragma pack(1)
struct fn_00426469_Vertex
{
	tPoint3 pos;
	tPoint3 normal;
	char pad18[8];
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
};
#pragma pack(pop)

float fn_0042653f(float);

// HYPOTHESIS: the class whose constructor is at 0x00426141 (name and size unknown).
class fn_00426141_Class
{
public:
	fn_00426141_Class(fn_00426141_Game* game);
	fn_00426141_Cell* fn_0042636b(fn_00426141_Cell* out, int i, int j);
	void fn_0042630b();
	void fn_00426469(int index, tPoint3* p, fn_00426469_Vertex* v);
	void fn_00426886(unsigned int mask);

	fn_00426141_Game* m00;
	float m04[6];
	double m20[6];
	fn_00426141_Color m50[6];
	char m98[8];
	float ma0;
	fn_00426141_Color ma4;
	fn_00426141_Cell mb0[17][33];
};

// FUNCTION: STARFLEETOP 0x00426141
fn_00426141_Class::fn_00426141_Class(fn_00426141_Game* game)
{
	m00 = game;
	fn_00426116();
	int i;
	for (i = 0; i < 6; i++)
	{
		m04[i] = 0.5f;
		m20[i] = 0.0;
		m50[i].Set(0.0f, 0.0f, 0.0f);
		m98[i] = 0;
	}
	switch (game->m86c)
	{
	case 1:
	case 9:
	case 0x18:
		ma4.Set(1.0f, 0.0f, 0.0f);
		break;
	case 0:
	case 8:
		ma4.Set(0.0f, 0.5f, 1.0f);
		break;
	case 2:
	case 10:
		ma4.Set(0.5f, 0.5f, 1.0f);
		break;
	case 3:
	case 0xb:
		ma4.Set(1.0f, 1.0f, 0.0f);
		break;
	case 4:
	case 0xc:
		ma4.Set(0.0f, 0.0f, 1.0f);
		break;
	case 5:
	case 0xd:
		ma4.Set(0.0f, 1.0f, 0.0f);
		break;
	case 6:
	case 0xe:
		ma4.Set(0.81f, 0.37f, 0.9f);
		break;
	case 7:
	case 0xf:
		ma4.Set(0.07f, 0.86f, 0.8f);
		break;
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
		ma4.Set(1.0f, 1.0f, 1.0f);
		break;
	default:
		ma4.Set(1.0f, 1.0f, 1.0f);
		break;
	}
	ma0 = m00->vfunction13();
	for (i = 0; i < 0x11; i++)
	{
		for (int j = 0; j < 0x21; j++)
		{
			fn_00426141_Cell tmp;
			mb0[i][j].fn_0042642c(*fn_0042636b(&tmp, i, j));
		}
	}
}

// HYPOTHESIS: a double global at 0x00c7c148 holding the current time.
extern double g_00c7c148;

// FUNCTION: STARFLEETOP 0x0042630b
void fn_00426141_Class::fn_0042630b()
{
	for (int i = 0; i < 6; i++)
	{
		if (m98[i])
		{
			double d = (g_00c7c148 - m20[i]) * 5e-07;
			float v = (float)d;
			m04[i] = (float)d;
			if (v > 1.0f)
				m98[i] = 0;
			else
				m04[i] = 1.0f - v;
		}
	}
}

extern "C" double fn_007381c4(double);
extern "C" double fn_00737fa4(double);

// FUNCTION: STARFLEETOP 0x0042636b
fn_00426141_Cell* fn_00426141_Class::fn_0042636b(fn_00426141_Cell* out, int i, int j)
{
	float f = (float)j * 0.03125f;
	float a = f * 3.14159265358979323846;
	a = a + a;
	float g = (float)i * 0.0625f;
	float b = g * 3.14159265358979323846;
	float sa = (float)fn_007381c4(a);
	float ca = (float)fn_00737fa4(a);
	float sb = (float)fn_007381c4(b);
	float cb = (float)fn_00737fa4(b);
	fn_00426141_Cell c;
	c.m00[0] = sb * ma0;
	c.m00[1] = cb * ma0 * ca;
	c.m00[2] = cb * ma0 * sa;
	c.m00[3] = sb;
	c.m00[4] = cb * ca;
	c.m00[5] = cb * sa;
	c.m00[6] = f;
	c.m00[7] = g;
	out->fn_0042642c(c);
	return out;
}

// FUNCTION: STARFLEETOP 0x0042642c
fn_00426141_Cell& fn_00426141_Cell::fn_0042642c(const fn_00426141_Cell& other)
{
	m00[0] = other.m00[0];
	m00[1] = other.m00[1];
	m00[2] = other.m00[2];
	m00[3] = other.m00[3];
	m00[4] = other.m00[4];
	m00[5] = other.m00[5];
	m00[6] = other.m00[6];
	m00[7] = other.m00[7];
	m00[8] = other.m00[8];
	return *this;
}

// FUNCTION: STARFLEETOP 0x00426469
void fn_00426141_Class::fn_00426469(int index, tPoint3* p, fn_00426469_Vertex* v)
{
	if (p)
	{
		tPoint3 d = (*p * (ma0 + 1.0f)) - v->pos;
		float f = (v->normal * d) * 1.5f;
		v->r = (unsigned char)(fn_0042653f(f * ma4.r) * m04[index] * 255.0f);
		v->g = (unsigned char)(fn_0042653f(f * ma4.g) * m04[index] * 255.0f);
		v->b = (unsigned char)(fn_0042653f(f * ma4.b) * m04[index] * 255.0f);
		v->a = (unsigned char)(m04[index] * 255.0f);
	}
}

// FUNCTION: STARFLEETOP 0x0042653f
float fn_0042653f(float x)
{
	if (x > 1.0f)
		x = 1.0f;
	else if (x < 0.0f)
		x = 0.0f;
	return x;
}

#include "unsorted/fn_0040ce21_type.h"

int fn_00454339(unsigned int);

// HYPOTHESIS: two float tables at 0x00c6c004 and 0x00c6a004, indexed by an angle in 8192 steps.
extern float g_00c6c004[];
extern float g_00c6a004[];

// FUNCTION: STARFLEETOP 0x00426886
void fn_00426141_Class::fn_00426886(unsigned int mask)
{
	int index = 0;
	tPoint3 pos = m00->m34;
	int bits = fn_00454339(mask) >> 1;
	if (bits != 0)
	{
		do
		{
			bits >>= 1;
			index++;
		}
		while (bits != 0);
	}
	static fn_0040ce21_type gotcha("Gotcha you SOB!");
	m20[index] = g_00c7c148;
	tPoint3 q;
	q.x = g_00c6c004[mask & 0x1fff] + pos.x;
	q.y = g_00c6a004[mask & 0x1fff] + pos.y;
	q.z = pos.z;
	tPoint3 d = q - pos;
	m50[index].r = d.x;
	m50[index].g = d.y;
	m50[index].b = d.z;
	m98[index] = 1;
}
