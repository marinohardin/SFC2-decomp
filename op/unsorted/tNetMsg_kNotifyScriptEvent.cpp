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
