// HYPOTHESIS: t3DVisual is the base named by RTTI; its constructor links each visual
// into the list headed at 0x008f5974. Member names are not known.
// HYPOTHESIS: the original stores only one vtable and stores it after m04, so the
// list linking is written in this constructor rather than in an inlined base one.
// HYPOTHESIS: a non-polymorphic base holding m04; the original stores m04 before the
// vtable, which only a base constructor run first explains. Its name is not known.
class tBase_0042f858
{
public:
	tBase_0042f858() { m04 = 1; }
	int m04;
};

class t3DVisual_Model : public tBase_0042f858
{
public:
	t3DVisual_Model();
	virtual ~t3DVisual_Model();

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
	int m154;
	int m158;
	int m15c;
	int m160[5];
	int m174;
	float m178;
	int m17c;
	int m180;
	int m184;
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
