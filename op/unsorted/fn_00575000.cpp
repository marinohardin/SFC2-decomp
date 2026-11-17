// HYPOTHESIS: the 0x48-byte object built here, and the owner holding the value at +0xd0, are unnamed
class fn_0057430c_object
{
public:
	fn_0057430c_object(void* arg);
	char m_pad[0x48];
};

class fn_00575023_owner
{
public:
	void fn_00575023(fn_0057430c_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575023
void fn_00575023_owner::fn_00575023(fn_0057430c_object** out)
{
	*out = new fn_0057430c_object(m_d0);
}
