//	tShipSystemMask.h: tShipSystemMask, the 0xf0-byte mask of a ship's systems.
// Evidence:  class (RTTI names the destructor 0x004459be; original file unknown)

#ifndef tShipSystemMask_h
#define tShipSystemMask_h 1

#include <string>

class tObject_0042496f;
class c_00450e41;
struct tObject_00447415_limits;
struct tObject_0044715d_table;

// The 0xf0-byte object tSFCPlayer keeps in arrays and tSupplyDockItem at +0x74; RTTI names its
// destructor. The methods 0x0044715d..0x00447d55 are written in fn_00447000.cpp.
class tShipSystemMask
{
public:
	// From tSFCPlayer, which keeps the object in arrays: the constructor (0x004454c1), the
	// destructor (0x004459be), the copy (0x00445a14) and three more methods it calls.
	tShipSystemMask();
	~tShipSystemMask();
	tShipSystemMask &operator=(const tShipSystemMask &other);
	void fn_00447af2(char *s, int v);
	char fn_00508f68();
	void fn_0051da3d();
	// HYPOTHESIS: tSupplyDockShipInfo holds one at +0x6c and builds it with 0x004455d7, which
	// sits between the deleting destructor and the destructor.
	tShipSystemMask(long arg1, long arg2, long arg3, long arg4);
	void fn_0044715d(tObject_0042496f *ship);
	void fn_004472bc(tObject_0042496f *ship);
	int fn_00447415(int side, int kind);
	void fn_00447531(int side, int value);
	void fn_004475bc(int kind, int value);
	int fn_004476cf(int level);
	int fn_00447740(int level);
	int fn_004477b1(int index);
	void fn_004477c6(int index, char value, char current);
	int fn_004477f8(int index);
	int fn_0044780d(int index);
	std::string fn_00447822(int index);
	void fn_00447867(int index, std::string value);
	int fn_004478b5(int level);
	void fn_00447926(int kind, int value);
	int fn_004479b4(int kind);
	int fn_00447a54(int kind);
	void fn_00447aa7(tObject_0042496f *ship);
	// HYPOTHESIS: 0x00445a8c and 0x004466ba take the same this and ship as 0x0044715d.
	void fn_00445a8c(tObject_0042496f *ship);
	void fn_004466ba(tObject_0042496f *ship);
	void fn_00447b7a(tObject_0042496f *ship);
	// HYPOTHESIS: 0x00445fe1 and 0x00446d3d take the same this and ship as 0x004472bc.
	void fn_00445fe1(tObject_0042496f *ship);
	void fn_00446d3d(tObject_0042496f *ship);
	bool fn_00447c61(int kind);
	tObject_00447415_limits *fn_00447ce4();
	tObject_0044715d_table *fn_00447ceb();
	c_00450e41 *fn_00447cf2();
	void fn_00447cf9(int index, int value);
	void fn_00447d11(int index, int value);
	void fn_00447d29(int index, int value);
	int fn_00447d41(int index);
	int fn_00447d55(int index);

	char field_0x0[0x4];
	char field_0x4;
	char field_0x5[0x83];
	int field_0x88;
	char field_0x8c[0x58];
	// HYPOTHESIS: the part at 0xe4 is the one 0x00447bbb hands to 0x00450e41.
	c_00450e41 *field_0xe4;
	tObject_00447415_limits *field_0xe8;
	tObject_0044715d_table *field_0xec;
};

#endif	// tShipSystemMask_h
