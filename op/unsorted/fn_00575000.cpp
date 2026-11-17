// HYPOTHESIS: the 0x48-byte object built here, and the owner holding the value at +0xd0, are unnamed
class fn_0057430c_object
{
public:
	fn_0057430c_object(void* arg);
	char m_pad[0x48];
};

class fn_00575066_object;
class fn_00575130_object;
class fn_00575173_object;
class fn_005751b6_object;
class fn_005751f9_object;
class fn_0057523c_object;

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
	void fn_00575130(fn_00575130_object** out);
	void fn_00575023(fn_0057430c_object** out);
	void fn_00575066(fn_00575066_object** out);
	void fn_00575173(fn_00575173_object** out);
	void fn_005751b6(fn_005751b6_object** out);
	void fn_005751f9(fn_005751f9_object** out);
	void fn_0057523c(fn_0057523c_object** out);
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

// HYPOTHESIS: the 0x48-byte object built by fn_00575130 is unnamed
class fn_00575130_object
{
public:
	fn_00575130_object(void* arg);
	char m_pad[0x48];
};

// FUNCTION: STARFLEETOP 0x00575130
void fn_00575023_owner::fn_00575130(fn_00575130_object** out)
{
	*out = new fn_00575130_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575173 is unnamed
class fn_00575173_object
{
public:
	fn_00575173_object(void* arg);
	char m_pad[0x48];
};

// FUNCTION: STARFLEETOP 0x00575173
void fn_00575023_owner::fn_00575173(fn_00575173_object** out)
{
	*out = new fn_00575173_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_005751b6 is unnamed
class fn_005751b6_object
{
public:
	fn_005751b6_object(void* arg);
	char m_pad[0x48];
};

// FUNCTION: STARFLEETOP 0x005751b6
void fn_00575023_owner::fn_005751b6(fn_005751b6_object** out)
{
	*out = new fn_005751b6_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_005751f9 is unnamed
class fn_005751f9_object
{
public:
	fn_005751f9_object(void* arg);
	char m_pad[0x48];
};

// FUNCTION: STARFLEETOP 0x005751f9
void fn_00575023_owner::fn_005751f9(fn_005751f9_object** out)
{
	*out = new fn_005751f9_object(m_d0);
}

// HYPOTHESIS: the 0x98-byte object built by fn_0057523c is unnamed
class fn_0057523c_object
{
public:
	fn_0057523c_object(void* arg);
	char m_pad[0x98];
};

// FUNCTION: STARFLEETOP 0x0057523c
void fn_00575023_owner::fn_0057523c(fn_0057523c_object** out)
{
	*out = new fn_0057523c_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575282 is unnamed
class fn_00575282_object
{
public:
	fn_00575282_object(void* arg);
	char m_pad[0x48];
};

class fn_00575282_owner
{
public:
	void fn_00575282(fn_00575282_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575282
void fn_00575282_owner::fn_00575282(fn_00575282_object** out)
{
	*out = new fn_00575282_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_005752c5 is unnamed
class fn_005752c5_object
{
public:
	fn_005752c5_object(void* arg);
	char m_pad[0x48];
};

class fn_005752c5_owner
{
public:
	void fn_005752c5(fn_005752c5_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005752c5
void fn_005752c5_owner::fn_005752c5(fn_005752c5_object** out)
{
	*out = new fn_005752c5_object(m_d0);
}

// HYPOTHESIS: the 0xa0-byte object built by fn_00575308 is unnamed
class fn_00575308_object
{
public:
	fn_00575308_object(void* arg);
	char m_pad[0xa0];
};

class fn_00575308_owner
{
public:
	void fn_00575308(fn_00575308_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575308
void fn_00575308_owner::fn_00575308(fn_00575308_object** out)
{
	*out = new fn_00575308_object(m_d0);
}

// HYPOTHESIS: the 0x234-byte object built by fn_0057534e is unnamed; its destructor is tBmpSurf's (0x0072eea0) or a related one
class fn_0057534e_object
{
public:
	fn_0057534e_object(void* arg);
	char m_pad[0x234];
};

class fn_0057534e_owner
{
public:
	void fn_0057534e(fn_0057534e_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x0057534e
void fn_0057534e_owner::fn_0057534e(fn_0057534e_object** out)
{
	*out = new fn_0057534e_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575394 is unnamed
class fn_00575394_object
{
public:
	fn_00575394_object(void* arg);
	char m_pad[0x48];
};

class fn_00575394_owner
{
public:
	void fn_00575394(fn_00575394_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575394
void fn_00575394_owner::fn_00575394(fn_00575394_object** out)
{
	*out = new fn_00575394_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_005753d7 is unnamed; it has a destructor, so the new-expression carries an EH frame
class fn_005753d7_object
{
public:
	fn_005753d7_object(void* arg);
	~fn_005753d7_object();
	char m_pad[0x48];
};

class fn_005753d7_owner
{
public:
	void fn_005753d7(fn_005753d7_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005753d7
void fn_005753d7_owner::fn_005753d7(fn_005753d7_object** out)
{
	*out = new fn_005753d7_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_0057541a is unnamed; its constructor is FUN_005642ca
class fn_0057541a_object
{
public:
	fn_0057541a_object(void* arg);
	~fn_0057541a_object();
	char m_pad[0x48];
};

class fn_0057541a_owner
{
public:
	void fn_0057541a(fn_0057541a_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x0057541a
void fn_0057541a_owner::fn_0057541a(fn_0057541a_object** out)
{
	*out = new fn_0057541a_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_0057545d is unnamed; it has a destructor, so the new-expression carries an EH frame
class fn_0057545d_object
{
public:
	fn_0057545d_object(void* arg);
	~fn_0057545d_object();
	char m_pad[0x48];
};

class fn_0057545d_owner
{
public:
	void fn_0057545d(fn_0057545d_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x0057545d
void fn_0057545d_owner::fn_0057545d(fn_0057545d_object** out)
{
	*out = new fn_0057545d_object(m_d0);
}

// HYPOTHESIS: the object built by fn_005754a0 is unnamed
class fn_005754a0_object
{
public:
	fn_005754a0_object(void* arg);
	char m_pad[0x48];
};

class fn_005754a0_owner
{
public:
	void fn_005754a0(fn_005754a0_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005754a0
void fn_005754a0_owner::fn_005754a0(fn_005754a0_object** out)
{
	*out = new fn_005754a0_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_005754e3 is unnamed; its constructor is FUN_0056210d
class fn_005754e3_object
{
public:
	fn_005754e3_object(void* arg);
	~fn_005754e3_object();
	char m_pad[0x48];
};

class fn_005754e3_owner
{
public:
	void fn_005754e3(fn_005754e3_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005754e3
void fn_005754e3_owner::fn_005754e3(fn_005754e3_object** out)
{
	*out = new fn_005754e3_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575526 is unnamed; its constructor is FUN_00561106
class fn_00575526_object
{
public:
	fn_00575526_object(void* arg);
	char m_pad[0x48];
};

class fn_00575526_owner
{
public:
	void fn_00575526(fn_00575526_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575526
void fn_00575526_owner::fn_00575526(fn_00575526_object** out)
{
	*out = new fn_00575526_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575569 is unnamed; it has a destructor, so the new-expression carries an EH frame
class fn_00575569_object
{
public:
	fn_00575569_object(void* arg);
	~fn_00575569_object();
	char m_pad[0x48];
};

class fn_00575569_owner
{
public:
	void fn_00575569(fn_00575569_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575569
void fn_00575569_owner::fn_00575569(fn_00575569_object** out)
{
	*out = new fn_00575569_object(m_d0);
}

// HYPOTHESIS: the object built by fn_005755ac is unnamed (constructor at 0x00562f5f, next to
// nAsyncDatabaseProcedure::tGetLoggedOnCharacterIDs's destructor); it has a destructor, so the new-expression carries an EH frame
class fn_005755ac_object
{
public:
	fn_005755ac_object(void* arg);
	~fn_005755ac_object();
	char m_pad[0x48];
};

class fn_005755ac_owner
{
public:
	void fn_005755ac(fn_005755ac_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005755ac
void fn_005755ac_owner::fn_005755ac(fn_005755ac_object** out)
{
	*out = new fn_005755ac_object(m_d0);
}

// HYPOTHESIS: the object built by fn_005755ef is unnamed; same shape as fn_005755ac_object
class fn_005755ef_object
{
public:
	fn_005755ef_object(void* arg);
	~fn_005755ef_object();
	char m_pad[0x48];
};

class fn_005755ef_owner
{
public:
	void fn_005755ef(fn_005755ef_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005755ef
void fn_005755ef_owner::fn_005755ef(fn_005755ef_object** out)
{
	*out = new fn_005755ef_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575632 is unnamed; constructor FUN_00562429, destructor tBmpSurf's
class fn_00575632_object
{
public:
	fn_00575632_object(void* arg);
	~fn_00575632_object();
	char m_pad[0x48];
};

class fn_00575632_owner
{
public:
	void fn_00575632(fn_00575632_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575632
void fn_00575632_owner::fn_00575632(fn_00575632_object** out)
{
	*out = new fn_00575632_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575675 is unnamed; same shape as fn_00575632_object
class fn_00575675_object
{
public:
	fn_00575675_object(void* arg);
	~fn_00575675_object();
	char m_pad[0x48];
};

class fn_00575675_owner
{
public:
	void fn_00575675(fn_00575675_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575675
void fn_00575675_owner::fn_00575675(fn_00575675_object** out)
{
	*out = new fn_00575675_object(m_d0);
}

// HYPOTHESIS: the object built by fn_005756b8 is unnamed; same shape as fn_00575675_object, 0x58 bytes
class fn_005756b8_object
{
public:
	fn_005756b8_object(void* arg);
	~fn_005756b8_object();
	char m_pad[0x58];
};

class fn_005756b8_owner
{
public:
	void fn_005756b8(fn_005756b8_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005756b8
void fn_005756b8_owner::fn_005756b8(fn_005756b8_object** out)
{
	*out = new fn_005756b8_object(m_d0);
}

// HYPOTHESIS: the object built by fn_005756fb is unnamed; same shape as fn_00575675_object
class fn_005756fb_object
{
public:
	fn_005756fb_object(void* arg);
	~fn_005756fb_object();
	char m_pad[0x48];
};

class fn_005756fb_owner
{
public:
	void fn_005756fb(fn_005756fb_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005756fb
void fn_005756fb_owner::fn_005756fb(fn_005756fb_object** out)
{
	*out = new fn_005756fb_object(m_d0);
}
