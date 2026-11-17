// HYPOTHESIS: the 0x48-byte object built here, and the owner holding the value at +0xd0, are unnamed
class fn_0057430c_object
{
public:
	fn_0057430c_object(void* arg);
	char m_pad[0x48];
};

class fn_00575066_object;

// HYPOTHESIS: the 0x48-byte object built by fn_005750aa is unnamed
class fn_005750aa_object
{
public:
	fn_005750aa_object(void* arg);
	char m_pad[0x48];
};

// HYPOTHESIS: the 0x48-byte object built by fn_005750ed is unnamed
class fn_005750ed_object
{
public:
	fn_005750ed_object(void* arg);
	char m_pad[0x48];
};

class fn_00575023_owner
{
public:
	void fn_005750aa(fn_005750aa_object** out);
	void fn_005750ed(fn_005750ed_object** out);
	void fn_00575023(fn_0057430c_object** out);
	void fn_00575066(fn_00575066_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// HYPOTHESIS: the 0x4c-byte object built by fn_00575066 is unnamed; its constructor takes the value at +0xd0 and the owner
class fn_00575066_object
{
public:
	fn_00575066_object(void* arg, fn_00575023_owner* owner);
	char m_pad[0x4c];
};

// FUNCTION: STARFLEETOP 0x00575023
void fn_00575023_owner::fn_00575023(fn_0057430c_object** out)
{
	*out = new fn_0057430c_object(m_d0);
}

// FUNCTION: STARFLEETOP 0x00575066
void fn_00575023_owner::fn_00575066(fn_00575066_object** out)
{
	*out = new fn_00575066_object(m_d0, this);
}

// FUNCTION: STARFLEETOP 0x005750aa
void fn_00575023_owner::fn_005750aa(fn_005750aa_object** out)
{
	*out = new fn_005750aa_object(m_d0);
}

// FUNCTION: STARFLEETOP 0x005750ed
void fn_00575023_owner::fn_005750ed(fn_005750ed_object** out)
{
	*out = new fn_005750ed_object(m_d0);
}
