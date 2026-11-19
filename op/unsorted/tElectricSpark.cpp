// HYPOTHESIS: layout and the counter global are inferred from the constructor's stores.
extern int g_00898ca4;
extern char* g_00898c9c;

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
