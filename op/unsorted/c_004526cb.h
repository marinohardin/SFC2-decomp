//	c_004526cb.h: the part class whose member 0x004526cb fn_00447000.cpp and fn_00546000.cpp
//	both call, with the base fn_00447000.cpp's 0x00447bbb tests its arguments through.
// Evidence:  calls (original names and file unknown)

#ifndef c_004526cb_h
#define c_004526cb_h 1

// HYPOTHESIS: the three arguments of 0x00447bbb share only a test in slot 3 (offset 0xc).
class c_00447bbb_arg
{
public:
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual bool vfunction4();
};

// HYPOTHESIS: 0x004526cb copies seven 0x28-byte entries, +0x0c up to +0x124, from an object of
// the same layout and returns this; 0x00447c21 calls it with another part as the argument and
// 0x00546787 on the member at +0x30c of 0x00546027's object, which fills the space up to +0x430.
// So the class is 0x124 bytes; the name and the rest of its members are unknown.
class c_004526cb : public c_00447bbb_arg
{
public:
	void fn_004526cb(c_00447bbb_arg *arg);

	char field_4[0x120];
};

#endif	// c_004526cb_h
