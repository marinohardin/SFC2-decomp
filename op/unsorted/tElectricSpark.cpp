// HYPOTHESIS: layout and the counter global are inferred from the constructor's stores.
extern int g_00898ca4;
// HYPOTHESIS: the shared buffer holds 12-byte elements of an unnamed type with an empty constructor.
class tSparkSegment
{
public:
	tSparkSegment() {}
	int m00;
	int m04;
	int m08;
};
extern tSparkSegment* g_00898c9c;

class tElectricSpark
{
public:
	tElectricSpark();
	virtual ~tElectricSpark();

	float m04;
	int m08;
	int m0c;
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
	g_00898c9c = new tSparkSegment[n];
	if (!g_00898c9c)
		return false;
	g_00898ca0 = n;
	return true;
}
