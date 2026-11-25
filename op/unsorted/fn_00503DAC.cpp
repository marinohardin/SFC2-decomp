#include "unsorted/fn_0040ce21_type.h"

// HYPOTHESIS: tFx_PPDPulse has no header yet; 0x48 bytes from the new below.
class tFx_PPDPulse
{
public:
	tFx_PPDPulse();
	char pad00[0x48];
};

// FUNCTION: STARFLEETOP 0x00503dac
tFx_PPDPulse* fn_00503dac()
{
	static fn_0040ce21_type marker("Let's Paul it!");
	return new tFx_PPDPulse;
}

// HYPOTHESIS: tProbeCell (vftable 0x007aaa54) has no header yet; base tFxCell_Base is not declared.
// HYPOTHESIS: the original base class sets the byte at +4 before the derived vtable store.
class tFxCell_Base
{
public:
	tFxCell_Base() { m04 = 1; }
	char m04;
};

class tProbeCell : public tFxCell_Base
{
public:
	tProbeCell(const float* a, const float* b);
	virtual ~tProbeCell();
	float m08;
	float m0c;
	float m10;
	float m14;
	float m18;
	float m1c;
	float m20;
	float m24;
	float m28;
	float m2c;
	float m30;
	char pad34[8];
	float m3c[3];
	float m48[3];
};

extern float g_0092f61c;
float fn_004baeb1(float x);

// FUNCTION: STARFLEETOP 0x00503e41
tProbeCell::tProbeCell(const float* a, const float* b)
{
	m08 = 0;
	m08 = g_0092f61c;
	m1c = 0.5f;
	m0c = 1.0f;
	m20 = 0.2f;
	m10 = 1.0f;
	m14 = 1.0f;
	m18 = 1.0f;
	m3c[0] = a[0];
	m3c[1] = a[1];
	m3c[2] = a[2];
	m48[0] = b[0];
	m48[1] = b[1];
	m48[2] = b[2];
	m24 = fn_004baeb1(64.0f);
	m28 = fn_004baeb1(125.0f);
	m2c = fn_004baeb1(127.0f);
	m30 = fn_004baeb1(64.0f);
}

// FUNCTION: STARFLEETOP 0x00503ef3
float fn_00503ef3()
{
	return 0.25f;
}
