// HYPOTHESIS: layout and the counter global are inferred from the constructor's stores.
extern int g_00898ca4;

class tElectricSpark
{
public:
	tElectricSpark();
	virtual void vfunction0();

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
