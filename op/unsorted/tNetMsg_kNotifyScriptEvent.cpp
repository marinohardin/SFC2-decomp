#include "unsorted/tIndividualShipSystem.h"

extern char g_0083bddc[];

class tNetMsg_kNotifyScriptEvent
{
public:
	virtual void vfunction0();
	virtual void* vfunction1();
};

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004101c8
void* tNetMsg_kNotifyScriptEvent::vfunction1()
{
	return g_0083bddc;
}

// FUNCTION: STARFLEETOP 0x0041021f
char vfunction33_result::fn_0041021f()
{
	return field_5c;
}

// FUNCTION: STARFLEETOP 0x00410223
int vfunction33_result::fn_00410223()
{
	return field_64;
}

// FUNCTION: STARFLEETOP 0x00410227
void vfunction33_result::fn_00410227(int value)
{
	field_64 = value;
}
