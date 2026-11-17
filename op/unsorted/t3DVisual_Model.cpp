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

// HYPOTHESIS: the polymorphic objects owned at +0x160.
class tPart_0042f918
{
public:
	virtual ~tPart_0042f918();
};

// HYPOTHESIS: the node list at +0x184; its destructor is 0x0042f467.
class tNode_0042f467
{
public:
	~tNode_0042f467();

	tNode_0042f467 *mNext;
};

class t3DVisual_Model : public t3DVisual
{
public:
	t3DVisual_Model();
	virtual ~t3DVisual_Model();
	// HYPOTHESIS: slot 10 of the vtable; declared non-virtual until the earlier slots are known.
	float vfunction10();
	bool vfunction11(tPoint3 *out, const char *name);

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
