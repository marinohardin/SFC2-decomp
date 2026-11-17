#include "unsorted/t3DVisual.h"
#include "Tiger/tvectormath.h"
#include <string.h>

// HYPOTHESIS: each visual is linked into the list headed at 0x008f5974. Member names
// are not known.
// HYPOTHESIS: m04 is t3DVisual's; the original stores it before the only vtable store,
// which the inlined base constructor explains once its own vtable store is dropped.

// HYPOTHESIS: the object at 0x008cc620 releases a handle through its second slot.
class tMgr_008cc620
{
public:
	virtual void vfunction1();
	virtual void vfunction2(int handle);
};

// HYPOTHESIS: one 0x24-byte entry of the array at +0x158.
struct tEntry_0042f918
{
	unsigned char mFlags;
	char m01[0x13];
	int m14;
	int m18;
	int m1c;
	int m20;
};

// HYPOTHESIS: one 0x10-byte entry of the array at +0x17c: a point and the offset of its
// name in the string block at +0x154.
struct tEntry_0042fa82
{
	tPoint3 mPoint;
	int mName;
};

// HYPOTHESIS: the object 0x0042fd3f is given; slots 55 and 58 return matrices.
class tViewer_0042fd3f
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
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual tMatrix *vfunction55();
	virtual void vfunction56();
	virtual void vfunction57();
	virtual tMatrix *vfunction58();
};

// HYPOTHESIS: the polymorphic objects owned at +0x160.
class tPart_0042f918
{
public:
	virtual ~tPart_0042f918();
	virtual void vfunction1();
	// HYPOTHESIS: slot 2 takes the object, the entry array at +0x158 and the two values
	// 0x0042fd3f passes on.
	virtual void vfunction2(tViewer_0042fd3f *param_1, tEntry_0042f918 *param_2, float param_3, int param_4);
	// HYPOTHESIS: slot 3 takes the object, the entry array at +0x158 and the three values
	// 0x0042fddb passes on.
	virtual void vfunction3(tViewer_0042fd3f *param_1, tEntry_0042f918 *param_2, float param_3, float param_4, int param_5);
	virtual void vfunction4();
	// HYPOTHESIS: slot 5 takes the scale 0x0042fbd4 passes on.
	virtual void vfunction5(float scale);
	// HYPOTHESIS: slot 6 returns a value 0x0042fc61 passes on.
	virtual int vfunction6();
	// HYPOTHESIS: slot 7 takes two values and returns a bool, as 0x0042fc73 passes it on.
	virtual bool vfunction7(int param_1, int param_2);
};

// HYPOTHESIS: the node list at +0x184; its destructor is 0x0042f467.
class tNode_0042f467
{
public:
	~tNode_0042f467();

	tNode_0042f467 *mNext;
	// HYPOTHESIS: a named array of points, as read by 0x0042fb2b.
	char *mName;
	tPoint3 *mPoints;
	unsigned int mCount;
};

class t3DVisual_Model : public t3DVisual
{
public:
	t3DVisual_Model();
	virtual ~t3DVisual_Model();
	void vfunction7(tViewer_0042fd3f *param_1, float param_2, int param_3);
	void vfunction8(tViewer_0042fd3f *param_1, float param_2, float param_3, int param_4);
	// HYPOTHESIS: slot 10 of the vtable; declared non-virtual until the earlier slots are known.
	float vfunction10();
	bool vfunction11(tPoint3 *out, const char *name);
	int vfunction12();
	bool vfunction13(tPoint3 *out, const char *name, unsigned int index);
	unsigned int vfunction14(const char *name);
	void vfunction15(float scale);
	float vfunction16();
	int vfunction17(int index);
	void vfunction18();
	int vfunction19();
	int vfunction20();
	int vfunction21();
	bool vfunction22(int param_1, int param_2);
	int vfunction23(float time);

	int m08;
	t3DVisual_Model *mNext;
	t3DVisual_Model *mPrev;
	char m14;
	char m15[0x118 - 0x15];
	float m118;
	int m11c;
	int m120;
	int m124;
	float m128;
	float m12c[5];
	float m140[5];
	char *m154;
	tEntry_0042f918 *m158;
	int m15c;
	tPart_0042f918 *m160[5];
	int m174;
	float m178;
	tEntry_0042fa82 *m17c;
	int m180;
	tNode_0042f467 *m184;
};

extern t3DVisual_Model *g_008f5974;

// FUNCTION: STARFLEETOP 0x0042f858
t3DVisual_Model::t3DVisual_Model()
{
	m08 = 1;
	mNext = g_008f5974;
	mPrev = 0;
	g_008f5974 = this;
	if (mNext != 0)
		mNext->mPrev = this;
	m118 = 0.0f;
	m120 = -1;
	m128 = 0.0f;
	m14 = 0;
	m11c = 0;
	m124 = 0;
	for (int i = 0; i < 5; i++)
	{
		m12c[i] = 0.0f;
		m140[i] = 0.0f;
		m160[i] = 0;
	}
	m154 = 0;
	m158 = 0;
	m178 = 1.0f;
	m15c = 0;
	m174 = 0;
	m17c = 0;
	m180 = 0;
	m184 = 0;
}

extern tMgr_008cc620 *g_008cc620;

// FUNCTION: STARFLEETOP 0x0042f8fc
// FUNCTION: STARFLEETOP 0x0042f918
t3DVisual_Model::~t3DVisual_Model()
{
	if (m154 != 0)
	{
		for (int i = 0; i < m15c; i++)
		{
			tEntry_0042f918 *entry = &m158[i];
			if (entry->m14 != 0)
				g_008cc620->vfunction2(entry->m14);
			if ((entry->mFlags & 0x20) && entry->m1c != 0)
				g_008cc620->vfunction2(entry->m1c);
		}
	}
	for (int j = 0; j < 5; j++)
	{
		if (m160[j] != 0)
		{
			delete m160[j];
			m160[j] = 0;
		}
	}
	if (m154 != 0)
	{
		delete m154;
		m154 = 0;
	}
	if (m158 != 0)
	{
		delete m158;
		m158 = 0;
		m15c = 0;
	}
	if (m17c != 0)
	{
		delete m17c;
		m17c = 0;
		m180 = 0;
	}
	while (m184 != 0)
	{
		tNode_0042f467 *next = m184->mNext;
		delete m184;
		m184 = next;
	}
	if (mPrev != 0)
		mPrev->mNext = mNext;
	else
		g_008f5974 = mNext;
	if (mNext != 0)
		mNext->mPrev = mPrev;
}

// FUNCTION: STARFLEETOP 0x0042fa75
float t3DVisual_Model::vfunction10()
{
	return m178 * m118;
}

// FUNCTION: STARFLEETOP 0x0042fa82
bool t3DVisual_Model::vfunction11(tPoint3 *out, const char *name)
{
	if (out == 0)
		return false;
	out->x = 0.0f;
	out->y = 0.0f;
	out->z = 0.0f;
	if (name == 0 || m154 == 0)
		return false;
	for (int i = 0; i < m180; i++)
	{
		tEntry_0042fa82 *entry = &m17c[i];
		if (stricmp(m154 + entry->mName, name) == 0)
		{
			*out = entry->mPoint * m178;
			return true;
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0042fb24
int t3DVisual_Model::vfunction12()
{
	return m180;
}

// FUNCTION: STARFLEETOP 0x0042fb2b
bool t3DVisual_Model::vfunction13(tPoint3 *out, const char *name, unsigned int index)
{
	if (out == 0 || name == 0 || *name == 0)
		return false;
	out->x = 0.0f;
	out->y = 0.0f;
	out->z = 0.0f;
	for (tNode_0042f467 *node = m184; node != 0; node = node->mNext)
	{
		if (stricmp(node->mName, name) == 0)
		{
			if (index >= node->mCount)
				return false;
			*out = node->mPoints[index];
			return true;
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0042fb9c
unsigned int t3DVisual_Model::vfunction14(const char *name)
{
	if (name != 0 && *name != 0)
	{
		for (tNode_0042f467 *node = m184; node != 0; node = node->mNext)
		{
			if (stricmp(node->mName, name) == 0)
				return node->mCount;
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x0042fbd4
void t3DVisual_Model::vfunction15(float scale)
{
	m178 = scale;
	for (int i = 0; i < m11c; i++)
	{
		if (m160[i] != 0)
			m160[i]->vfunction5(scale);
	}
}

// FUNCTION: STARFLEETOP 0x0042fc18
float t3DVisual_Model::vfunction16()
{
	return m178;
}

// FUNCTION: STARFLEETOP 0x0042fc1f
int t3DVisual_Model::vfunction17(int index)
{
	if (index < 0 || index >= m11c)
		return m124;
	m120 = index;
	return index;
}

// FUNCTION: STARFLEETOP 0x0042fc40
void t3DVisual_Model::vfunction18()
{
	m120 = -1;
}

// FUNCTION: STARFLEETOP 0x0042fc48
int t3DVisual_Model::vfunction19()
{
	return m11c;
}

// FUNCTION: STARFLEETOP 0x0042fc4f
int t3DVisual_Model::vfunction20()
{
	int result = m120;
	if (result == -1)
		result = m124;
	return result;
}

// FUNCTION: STARFLEETOP 0x0042fc61
int t3DVisual_Model::vfunction21()
{
	if (m160[0] != 0)
		return m160[0]->vfunction6();
	return 0;
}

// FUNCTION: STARFLEETOP 0x0042fc73
bool t3DVisual_Model::vfunction22(int param_1, int param_2)
{
	if (m160[0] != 0)
		return m160[0]->vfunction7(param_1, param_2);
	return false;
}

// FUNCTION: STARFLEETOP 0x0042fc91
int t3DVisual_Model::vfunction23(float time)
{
	if (m11c <= 1)
		return 0;
	if (m120 != -1 && m120 < m11c)
		return m120;
	int result = m124;
	if (time > m128)
	{
		for (int i = result + 1; i < m11c; i++)
		{
			if (time > m12c[i])
				result = i;
			m128 = time;
		}
	}
	else if (time < m128)
	{
		for (int i = result - 1; i >= 0; i--)
		{
			if (time < m140[i])
				result = i;
			m128 = time;
		}
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x0042fd3f
void t3DVisual_Model::vfunction7(tViewer_0042fd3f *param_1, float param_2, int param_3)
{
	int index = 0;
	if (m120 != -1)
		index = m120;
	else if (m11c > 1)
	{
		tMatrix *matrix = param_1->vfunction58();
		tPoint3 point(matrix->mGetItem(0, 3), matrix->mGetItem(1, 3), matrix->mGetItem(2, 3));
		param_1->vfunction55()->mTransform(point);
		// HYPOTHESIS: 0x0069b250 is the point's dot product with itself (the squared length).
		index = vfunction23(point * point);
		m124 = index;
	}
	if (m160[index] != 0)
		m160[index]->vfunction2(param_1, m158, param_2, param_3);
}

// FUNCTION: STARFLEETOP 0x0042fddb
void t3DVisual_Model::vfunction8(tViewer_0042fd3f *param_1, float param_2, float param_3, int param_4)
{
	int index = 0;
	if (m120 != -1)
		index = m120;
	else if (m11c > 1)
	{
		tMatrix *matrix = param_1->vfunction58();
		tPoint3 point(matrix->mGetItem(0, 3), matrix->mGetItem(1, 3), matrix->mGetItem(2, 3));
		param_1->vfunction55()->mTransform(point);
		index = vfunction23(point * point);
		m124 = index;
	}
	if (m160[index] != 0)
		m160[index]->vfunction3(param_1, m158, param_2, param_3, param_4);
}
